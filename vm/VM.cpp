#include "VM.hpp"
#include "Context.hpp"
#include "../parser/lexical/LexicalAnalyser.hpp"
#include "../parser/syntaxic/SyntaxicAnalyser.hpp"
#include "../parser/semantic/SemanticAnalyser.hpp"
#include "../parser/semantic/SemanticError.hpp"
#include <sstream>
#include <chrono>

VM::VM() {}

VM::~VM() {}

map<string, jit_value_t> internals;
map<string, jit_value_t> globals;
map<string, Type> globals_types;
map<string, jit_value_t> locals;

string VM::execute(const string code, string ctx, ExecMode mode) {

	clock_t begin = clock();

	// Do lexical analysis
	LexicalAnalyser lex;
	vector<Token> tokens = lex.analyse(code);

//	cout << endl << "Tokens:" << endl;
//	for (Token token : tokens) {
//		cout << token << " ";
//	}
//	cout << endl << endl;

	// Syntaxical analysis
	SyntaxicAnalyser syn;
	Program* program = syn.analyse(tokens);


	if (syn.getErrors().size() > 0) {
		if (mode == ExecMode::COMMAND_JSON) {

			cout << "{\"success\":false,\"errors\":[";
			for (auto error : syn.getErrors()) {
				cout << "{\"line\":" << error->token->line << ",\"message\":\"" << error->message << "\"}";
			}
			cout << "]}" << endl;
			return ctx;

		} else {
			for (auto error : syn.getErrors()) {
				cout << error->token->line << " : " <<  error->message << endl;
			}
			return ctx;
		}
	}

	Compiler c;
	Context context { ctx };

	try {
		SemanticAnalyser sem;
		sem.analyse(program, &context);
	} catch (SemanticError& e) {

		if (mode == ExecMode::COMMAND_JSON) {
			cout << "{\"success\":false,\"errors\":[{\"line\":" << e.line << ",\"message\":\"" << e.message << "\"}]}" << endl;
		} else {
			cout << e.message << endl;
		}
		return ctx;
	}

	// program->print(cout);
//	cout << "return type : " << (int) program->type << endl;

	internals.clear();
	globals.clear();
	globals_types.clear();
	locals.clear();

	jit_init();
	jit_context_t jit_context = jit_context_create();
	jit_context_build_start(jit_context);

	jit_type_t params[0] = {};
	jit_type_t signature = jit_type_create_signature(jit_abi_cdecl, jit_type_int, params, 0, 1);
	jit_function_t F = jit_function_create(jit_context, signature);

	program->compile_jit(c, F, context, mode != ExecMode::NORMAL);

	jit_function_compile(F);
	jit_context_build_end(jit_context);

	typedef LSValue* (*FF)();
	FF fun = (FF) jit_function_to_closure(F);

	/*
	 * Execute
	 */
	auto start = chrono::high_resolution_clock::now();
	LSValue* res = fun();
	auto end = chrono::high_resolution_clock::now();

	long exe_time_ns = chrono::duration_cast<chrono::nanoseconds>(end - start).count();
	double exe_time_ms = (((double) exe_time_ns / 1000) / 1000);
	double comp_time = (double(clock() - begin) / CLOCKS_PER_SEC) * 1000;

	/*
	 * Return results
	 */
	string res_string;

	if (mode == ExecMode::COMMAND_JSON || mode == ExecMode::TOP_LEVEL) {

		LSArray* res_array = (LSArray*) res;

		ostringstream oss;
		res_array->at(LSNumber::get(0))->print(oss);
		res_string = oss.str();

		string ctx = "{";

		unsigned i = 0;
		for (auto g : globals) {

			LSValue* v = res_array->at(LSNumber::get(i + 1));
			ctx += "\"" + g.first + "\":" + v->to_json();
			if (i < globals.size() - 1) ctx += ",";
			i++;
		}

		ctx += "}";

		if (mode == ExecMode::TOP_LEVEL) {
			cout << res_string << endl;
			// cout << "ctx: " << ctx << endl;
			// cout << "(" << comp_time << "ms + " << exe_time_ms << " ms)" << endl;
			return ctx;
		} else {
			cout << "{\"success\":true,\"time\":" << exe_time_ns << ",\"ctx\":" << ctx << ",\"res\":\""
					<< res_string << "\"}" << endl;
			return ctx;
		}

	} else {

		ostringstream oss;
		res->print(oss);
		res_string = oss.str();

		cout << res_string << endl;
		cout << "(" << comp_time << "ms + " << exe_time_ms << " ms)" << endl;

		return ctx;
	}
}

LSValue* create_null_object(int n) {
	return LSNull::null_var;
}
LSValue* create_number_object(int n) {
	return LSNumber::get(n);
}
LSValue* create_bool_object(bool n) {
	return new LSBoolean(n);
}
LSValue* create_func_object(void* f) {
	return new LSFunction(f);
}
LSValue* create_float_object(double n) {
	return LSNumber::get(n);
}

void* get_conv_fun(Type type) {
	switch (type.raw_type) {
		case RawType::NULLL: return (void*) &create_null_object;
		case RawType::INTEGER: return (void*) &create_number_object;
		case RawType::FLOAT: return (void*) &create_float_object;
		case RawType::BOOLEAN: return (void*) &create_bool_object;
		case RawType::FUNCTION: return (void*) &create_func_object;
		default: {
			return (void*) &create_number_object;
		}
	}
}

jit_value_t VM::value_to_pointer(jit_function_t& F, jit_value_t& v, Type type) {

	void* fun = get_conv_fun(type);

	bool floatt = jit_type_get_kind(jit_value_get_type(v)) == JIT_TYPE_FLOAT64;
	if (floatt) {
		fun = (void*) &create_float_object;
	}

	jit_type_t args_types[1] = {
		(type.raw_type == RawType::FUNCTION or type.raw_type == RawType::LONG)
		? JIT_INTEGER_LONG :
		(type.raw_type == RawType::FLOAT) ?
		JIT_FLOAT :
		JIT_INTEGER
	};
	jit_type_t sig = jit_type_create_signature(jit_abi_cdecl, JIT_INTEGER, args_types, 1, 0);

	return jit_insn_call_native(F, "convert", (void*) fun, sig, &v, 1, JIT_CALL_NOTHROW);
}
/*
bool VM::get_number(jit_function_t& F, jit_value_t& val) {

	// x & (1 << 31) == 0

	jit_value_t is_int = jit_insn_eq(F,
		jit_insn_and(F, val, jit_value_create_nint_constant(F, jit_type_int, 2147483648)),
		jit_value_create_nint_constant(F, jit_type_int, 0)
	);

	jit_value_t res;

	jit_label_t label_else;
	jit_insn_branch_if_not(F, is_int, &label_else);

	return false;
}*/

map<int, void*> VM::globals_vars;

void VM::add_global_var(int index, void* value) {
	cout << "add_global_var " << index << " : " << value << endl;
	globals_vars.insert(pair<int, void*>(index, value));
}

LSArray* new_array() {
	cout << "lol" << endl;
	return new LSArray();
}

void push_array_value(LSArray* array, int value) {
	array->push(LSNumber::get(value));
}

void push_array_pointer(LSArray* array, LSValue* value) {
	array->push(value);
}

jit_value_t VM::new_array(jit_function_t& F) {
	jit_type_t args_t[0] = {};
	jit_value_t args[0] = {};
	jit_type_t sig = jit_type_create_signature(jit_abi_cdecl, jit_type_int, args_t, 0, 0);
	return jit_insn_call_native(F, "new", (void*) new_array, sig, args, 0, JIT_CALL_NOTHROW);
}

void VM::push_array_value(jit_function_t& F, jit_value_t& array, jit_value_t& value) {
	jit_type_t args[2] = {JIT_INTEGER, JIT_INTEGER};
	jit_type_t sig = jit_type_create_signature(jit_abi_cdecl, jit_type_void, args, 2, 0);
	jit_value_t args_v[] = {array, value};
	jit_insn_call_native(F, "push", (void*) push_array_value, sig, args_v, 2, JIT_CALL_NOTHROW);
}

void VM::push_array_pointer(jit_function_t& F, jit_value_t& array, jit_value_t& value) {
	jit_type_t args[2] = {JIT_INTEGER, JIT_INTEGER};
	jit_type_t sig = jit_type_create_signature(jit_abi_cdecl, jit_type_void, args, 2, 0);
	jit_value_t args_v[] = {array, value};
	jit_insn_call_native(F, "push", (void*) push_array_pointer, sig, args_v, 2, JIT_CALL_NOTHROW);
}