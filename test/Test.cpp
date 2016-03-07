#include <string>
#include <iostream>
#include <sstream>

#include "Test.hpp"
#include "../vm/Context.hpp"
#include "../parser/lexical/LexicalAnalyser.hpp"
#include "../parser/syntaxic/SyntaxicAnalyser.hpp"
#include "../parser/semantic/SemanticAnalyser.hpp"
#include "../parser/semantic/SemanticError.hpp"

Test::Test() {
	total = 0;
	success = 0;
	exeTime = 0;
}

void Test::tests() {

	clock_t begin = clock();
	exeTime = 0;

	/*
	 * General
	 */
	header("General");
	test("", "null");
	test("null", "null");
	test("12", "12");
	test("true", "true");
	test("false", "false");
	test("'toto'", "'toto'");

	/*
	 * Variables
	 */
	header("Variables");
	test("let a = 2", "2");
	test("let a, b, c = 1, 2, 3", "3");
	test("let a", "null");
	test("let a a = 12 a", "12");
	test("let a = 1 let b = (a = 12)", "12");

	/*
	 * Booléens
	 */
	header("Booléens");
	test("true", "true");
	test("false", "false");
	test("!true", "false");
	test("!false", "true");
	test("true and true", "true");
	test("true and false", "false");
	test("false and true", "false");
	test("false and false", "false");
	test("true or true", "true");
	test("true or false", "true");
	test("false or true", "true");
	test("false or false", "false");

	/*
	 * Numbers
	 */
	header("Numbers");
	test("0", "0");
	test("-1", "-1");
	test("-(-1)", "1");
	test("0 + 5", "5");
	test("5 + 5", "10");
	test("10 - 3", "7");
	test("5 * 5", "25");
	test("15 / 3", "5");
	test("15 / 2", "7.5");
	test("12 ^ 2", "144");
	test("2 ^ 5", "32");
	test("2 < 5", "true");
	test("12 < 5", "false");
	test("5 == 12", "false");
	test("12 == 12", "true");
	test("let a = 2 a++", "2");
	test("let a = 2; ++a", "3");
	test("let a = 2 a--", "2");
	test("let a = 2; --a", "1");
	test("let a = 2 a += 5", "7");
	test("let a = 2 a -= 5", "-3");
	test("let a = 2 a *= 5", "10");
	test("let a = 100 a /= 5", "20");
	test("let a = 56 a %= 17", "5");
	test("let a = 15 a ^= 2", "225");
	test("(33 - 2) / 2", "15.5");
	test("12 < (45 / 4)", "false");
	test("12 == (24 / 2)", "true");
	test("2.5 + 4.7", "7.2");
	test("2.5 × 4.7", "11.75");
	// test("12344532132423", "12344532132423");
	test("π", "3.1415926536");

	/*
	 * Opérations
	 */
	header("Operations");
	test("5 * 2 + 3 * 4", "22");
	test("let f = x -> x f(5) + f(7)", "12");
	test("'salut' * (1 + 2)", "'salutsalutsalut'");
	test("('salut' * 1) + 2", "'salut2'");

	/*
	 * Arrays
	 */
	header("Arrays");
	test("[]", "[]");
	test("[1]", "[1]");
	test("[1, 2, 3]", "[1, 2, 3]");
	test("[1, 2, 3] + [4, 5, 6]", "[1, 2, 3, 4, 5, 6]");
	test("[] + 1", "[1]");
	test("[] + 1 + 2 + 3", "[1, 2, 3]");
	test("[1] + [2] + [3]", "[1, 2, 3]");
	test("[1, 2, 3][1]", "2");
	test("let a = [1, 2, 3] a[0]", "1");
	test("let a = [1, 2, 3] a[0] += 5 a[0]", "6");
	test("let v = 12 let a = [v, 2, 3] a[0] += 5 a[0]", "17");
	test("let a = [23, 23, true, '', [], 123] |a|", "6");
	test("let a = [1, 2, 3]; ~a", "[3, 2, 1]");
	test("let a = [1, 2, 3] a[1] = 12 a", "[1, 12, 3]");
	test("[1.2, 321.42, 23.15]", "[1.2, 321.42, 23.15]");
	test("[1, 2, 3, 4, 5][1:3]", "[2, 3, 4]");

	// let a = [1..100]
	// let a = [for let i = 0; i < 100; i++ do i end]
	// a[10:42]
	// a[10:*]
	// a[] = 12 (a += 12)

	/*
	 * Strings
	 */
	header("Strings");
	test("'salut ' + 'ça va ?'", "'salut ça va ?'");
	test("'salut' + 12", "'salut12'");
	test("'salut' + true", "'saluttrue'");
	test("'salut' + null", "'salutnull'");
	test("'salut' * 3", "'salutsalutsalut'");
	test("|'salut'|", "5");
	test("~'bonjour'", "'ruojnob'");
	test("'bonjour'[3]", "'j'");
	test("~('salut' + ' ca va ?')", "'? av ac tulas'");
	test("'bonjour'[2:5]", "'njou'");

	/*
	 * Objects
	 */
	header("Objects");
	test("let a = {}", "{}");
	test("let a = {b: 12, c: 5} a", "{b: 12, c: 5}");
	test("let a = {b: 12, c: 5} a.b", "12");
	test("let a = {b: 12, c: 5} a.b *= 10", "120");
	test("let a = {a: 32, b: 'toto', c: false} |a|", "3");

	/*
	 * Références
	 */
	header("References");
	test("let a = 2 let b = a a = 5 b", "2");
	//test("let a = 2 let b = @a a = 5 b", "5");
	test("let v = 10 let a = [v] a[0]++ v", "10");
	//test("let v = 10 let a = [@v] a[0]++ v", "11");

	/*
	 * Conditions
	 */
	header("Conditions");
	test("if true then 12 else 5 end", "12");
	test("if false then 12 else 5 end", "5");
	test("if (true) { 12 } else { 5 }", "12");
	test("if (false) { 12 } else { 5 }", "5");

	/*
	 * Functions / Lambdas
	 */
	header("Functions / Lambdas");
	test("let f = x -> x f(12)", "12");
	test("let f = x -> x ^ 2 f(12)", "144");
	test("let f = x, y -> x + y f(5, 12)", "17");
	test("let f = -> 12 f()", "12");
	test("(x -> x)(12)", "12");
	test("(x, y -> x + y)(12, 5)", "17");
	test("( -> [])()", "[]");
	test("( -> 12)()", "12");
	test("[-> 12][0]()", "12");
	test("[-> 12, 'toto'][0]()", "12");
	test("(x -> x + 12.12)(1.01)", "13.13");
	test("(x -> x + 12)(1.01)", "13.01");
	test("[x -> x ^ 2][0](12)", "144");
	test("[[x -> x ^ 2]][0][0](12)", "144");
	test("[[[x -> x ^ 2]]][0][0][0](12)", "144");
	test("[[[[[[[x -> x ^ 2]]]]]]][0][0][0][0][0][0][0](12)", "144");

	/*
	test("let a = 5 let f = -> a f()", "5");

	test("let f = x -> x (-> f(12))()", "12");

	test("let f = x -> x let g = x -> f(x) g(12)", "12");

	test("let g = x -> x ^ 2 let f = x, y -> g(x + y) f(6, 2)", "64");
*/

	test("(-> -> 12)()()", "12");
	test("let f = -> -> 12 f()()", "12");
	test("let f = x -> -> 'salut' f()()", "'salut'");
	test("let f = x -> [x, x, x] f(44)", "[44, 44, 44]");
//	test("let fact = x -> if x == 1 { 1 } else { fact(x - 1) * x } fact(10)", "3628800");
//	test("let a = 10 a ~ x -> x ^ 2", "100");
	test("let f = function(x) { let r = x ** 2 return r + 1 } f(10)", "101");
	test("1; 2", "2");
//	test("return 1; 2", "1");
	test("let f = function(x) { if (x < 10) {return true} return 12 } [f(5), f(20)]", "[true, 12]");



	/*
	 * Closures
	 */
	header("Closures");
	test("let f = x -> y -> x + y let g = f(5) g(12)", "17");
	test("let f = x -> y -> x + y f(5)(12)", "17");

	/*
	 * While loops
	 */
	header("While loops");
	test("let i = 0 while (i < 10) { i++ } i", "10");
	test("let i = 0 let s = 0 while (i < 10) { s += i i++ } s", "45");
	test("let i = 0 while (i < 100) { i++ if (i == 50) break } i", "50");
	test("let i = 0 let a = 0 while (i < 10) { i++ if (i < 8) continue a++ } a", "3");

	/*
	 * For loops
	 */
	header("For loops");
	test("for let i = 0; ; i++ {}", "null");
	test("for let i = 0; i < 10; i++ {}", "null");
	test("let s = 0 for let i = 0; i < 5; i++ do s += i end s", "10");
	test("let s = 0 for let i = 0; i < 10; i += 2 do s += i end s", "20");
	test("let i = 0 for i = 0; i < 10; i++ { } i", "10");
	test("let i = 0 for i = 0; i < 10; i++ { if i == 5 { break } } i", "5");
	test("let a = 0 for i = 0; i < 10; i++ { a++ } a", "10");
	test("let a = 0 for i = 0; i < 10; i++ { if i < 5 { continue } a++ } a", "5");

	/*
	 * Foreach loops
	 */
	header("Foreach loops");
	test("let s = 0 for v in [1, 2, 3, 4] { s += v } s", "10");
	test("let s = '' for v in ['salut ', 'ça ', 'va ?'] { s += v } s", "'salut ça va ?'");

	/*
	 * Array operations
	 */
	header("Array operations");
	test("[1, 2, 3, 4, 5] ~~ x -> x ^ 2", "[1, 4, 9, 16, 25]");
	test("[1, 2, 3, 4, 5] ~~ (x -> x ^ 2)", "[1, 4, 9, 16, 25]");
	test("['yo', 'toto', 'salut'] ~~ x -> x + ' !'", "['yo !', 'toto !', 'salut !']");
	test("[1, 2, 3] ~~ x -> [x]", "[[1], [2], [3]]");
	test("[1, 2, 3] ~~ x -> 'yo'", "['yo', 'yo', 'yo']");
	test("let f = x -> x * 10 [1, 2, 3] ~~ f", "[10, 20, 30]");
	test("[1.2, 321.42, 23.15] ~~ x -> x * 1.7", "[2.04, 546.414, 39.355]");

	/*
	 * Swap
	 */
	header("Swap");
	test("let a = 2 let b = 5 a <=> b [a, b]", "[5, 2]");
	test("let a = [1, 2, 3, 4] a[0] <=> a[3] a", "[4, 2, 3, 1]");
	test("let a = 12 let b = 5 let s = a <=> b", "5");


	/*
	 * Classes
	 */
	header("Classes");
	test("null.class", "<class Null>");
	test("true.class", "<class Boolean>");
	test("false.class", "<class Boolean>");
	test("0.class", "<class Number>");
	test("12.class", "<class Number>");
	test("-5.class", "<class Number>");
	test("12.5234.class", "<class Number>");
	test("''.class", "<class String>");
	test("'salut'.class", "<class String>");
	test("[].class", "<class Array>");
	test("[1, 2, 3].class", "<class Array>");
	test("{}.class", "<class Object>");
	test("{a: 12, b: 'yo'}.class", "<class Object>");
	test("(-> 12).class", "<class Function>");
	test("(x, y -> x + y).class", "<class Function>");
	test("12.class.class", "<class Class>");

	test("class A {} let a = new A", "{}");
	test("class A { let b = 2 } let a = new A", "{}");
	test("class A { let b = 2 } let a = new A a.b", "2");
	test("class A { let b = 2 } let a = new A a.class", "<class A>");
	test("class A {} let t = [A] let a = new t[0]", "{}");
	test("class A {} let a = new A let b = new a.class b.class", "<class A>");

	/*
	 * Function operators
	 */
	header("Function operators");
	test("+(1, 2)", "3");
//	test("-(9, 2)", "7");
	test("*(5, 8)", "40");
	test("/(48, 12)", "4");
	test("^(2, 11)", "2048");
	test("%(48, 5)", "3");
	test("let p = + p(1, 2)", "3");
	//test("let p = -\n p(9, 2)", "7");
	test("let p = * p(5, 8)", "40");
	test("let p = / p(48, 12)", "4");
	test("let p = % p(48, 5)", "3");
	test("let p = ^ p(2, 11)", "2048");
	test("+", "<function>");
	test("+.class", "<class Function>");
//	test("let p = +; p.class", "Function");

	/*
	 * Number standard library
	 */
	header("Number standard library");
	test("Number", "<class Number>");
	test("Number()", "0");
	test("new Number", "0");
	test("new Number()", "0");
	test("new Number(12)", "12");
	test("Number.abs", "<function>");
	test("Number.abs(-12)", "12");
	test("Number.floor(5.9)", "5");
	test("var a = 5 Number.floor(a)", "5");
	test("var a = 5.4 Number.floor(a)", "5");
	test("Number.round(5.7)", "6");
	test("Number.round(5.4)", "5");
	test("Number.ceil(5.1)", "6");
	test("Number.max(5, 12)", "12");
	test("Number.cos(0)", "1");
	test("Number.cos(π)", "-1");
	test("Number.cos(π / 2)", "0");
	test("Number.sin(0)", "0");
	test("Number.sin(π)", "0");
	test("Number.sin(π / 2)", "1");
	test("Number.sin(- π / 2)", "-1");

	/*
	 * Number standard library
	 */
	header("String standard library");
	test("String", "<class String>");
	test("String()", "''");
	test("new String", "''");
	test("new String()", "''");
	test("new String('salut')", "'salut'");
	test("String()", "''");
	test("String('yo')", "'yo'");
	test("String.size('salut')", "5");
	test("String.toUpper('salut')", "'SALUT'");
	test("String.length('salut')", "5");
	test("String.reverse('salut')", "'tulas'");
	test("String.replace('bonjour à tous', 'o', '_')", "'b_nj_ur à t_us'");
	test("String.map('salut', x -> '(' + x + ')')", "'(s)(a)(l)(u)(t)'");
	test("String.split('bonjour ça va', ' ')", "['bonjour', 'ça', 'va']");
	test("String.split('bonjour_*_ça_*_va', '_*_')", "['bonjour', 'ça', 'va']");
	test("String.split('salut', '')", "['s', 'a', 'l', 'u', 't']");
	test("String.startsWith('salut ça va', 'salut')", "true");
	test("String.toArray('salut')", "['s', 'a', 'l', 'u', 't']");
	test("Array.average([1, 2, 3, 4, 5, 6])", "3.5");
	test("Array.average([])", "0");

	header("Array standard library");
	test("Array", "<class Array>");
	test("Array()", "[]");
	test("new Array", "[]");
	test("new Array()", "[]");
	test("Array.size([1, 'yo', true])", "3");
	test("[1, 'yo', true].size()", "3");
	test("Array.map([1, 2, 3], x -> x ^ 2)", "[1, 4, 9]");
	test("[3, 4, 5].map(x -> x ^ 2)", "[9, 16, 25]");
	test("Array.map2([1, 'yo ', []], [12, 55, 9], (x, y -> x + y))", "[13, 'yo 55', [9]]");
	test("[321, 213, 121].map(x -> x ^ 2).size()", "3");


	header("Other");

	test("var f = obj -> obj.a [f(12), f({a: 'yo'})]", "[null, 'yo']");
/*
	test("3 ~ x -> x ^ x", "27");
	test("[1, 2, 3] ~ x -> x + 4", "[1, 2, 3, 4]");
	test("[1, 2, 3] ~~ print", "[null, null, null]");
	test("[1, 2, 3] ~~ x -> [x, x]", "[[1, 1], [2, 2], [3, 3]]");
	test("let s = 0 let a = [1, 2, 3, 4] let f = x -> s += x a ~~ f s", "10");
	test("let s = 0 [1, 2, 3, 4, 5].each(x -> s += x)", "15");
	test("[1, 2, 3, 4, 5].fold_left(+,0)", "15");
	//test("let f = x -> x + 10 let g = x -> x ^ 2 [1, 2, 3] ~~ f ~~ g", "[121, 144, 169]");
	//test("[1, 2, 3] ~~ (x -> x * 5) ~~ (x -> x - 1)", "[4, 9, 14]");
*/

	/*
	a ~~ b => (a ~~ b)
	a ~ ~b => a ~ (~b)
	~~a => ~(~a)
	a; ~~b
	*/

	/*
	[1, 2, 3] ~~= (x -> x * 5 + 2)
	[1, 2, 3] ~+= 1   =>   [2, 3, 4]
	[1, 2, 3] ~*= 5
	[1, 2, 3] ~= (x -> x * 5)

	var dist = [...]
	dist ~*= 5
	var newdist = dist ~* 5


	var a
	;[1, 2, 3].map(...)
*/

	double elapsed_secs = double(clock() - begin) / CLOCKS_PER_SEC;

	cout << endl;
	cout << "------------------------------------------------" << endl;
	cout << "Total : " << total << ", succès : " << success << ", erreurs : " << (total - success) << endl;
	cout << "Total time : " << elapsed_secs * 1000 << " ms, execution time : " << (exeTime / CLOCKS_PER_SEC) * 1000 << " ms" << endl;
	cout << "------------------------------------------------" << endl;
}

void Test::header(string text) {
	cout << "----------------" << endl;
	cout << text << endl;
	cout << "----------------" << endl;
}

extern map<string, jit_value_t> internals;
extern map<string, jit_value_t> globals;
extern map<string, jit_value_t> locals;

void Test::test(string code, string result) {

	total++;

	LexicalAnalyser lex;
	vector<Token> tokens = lex.analyse(code);

	SyntaxicAnalyser syn;
	Program* program = syn.analyse(tokens);

	if (syn.getErrors().size() > 0) {
		for (SyntaxicalError* error : syn.getErrors()) {
			cout << error->message << endl;
		}
	} else {

		Context context { "{}" };
		Compiler c;

		try {
			SemanticAnalyser sem;
			sem.analyse(program, &context);
		} catch (SemanticError& e) {
			cout << e.message << endl;
			return;
		}

		internals.clear();
		globals.clear();
		locals.clear();

		jit_init();
		jit_context_t jit_context = jit_context_create();
		jit_context_build_start(jit_context);

		// Create function signature and object. int (*)(int)
		jit_type_t params[0] = {};
		jit_type_t signature = jit_type_create_signature(jit_abi_cdecl, jit_type_int, params, 0, 1);
		jit_function_t F = jit_function_create(jit_context, signature);

		program->compile_jit(c, F, context, false);

		jit_function_compile(F);
		jit_context_build_end(jit_context);

		typedef LSValue* (*FF)();
		FF fun = (FF) jit_function_to_closure(F);

		clock_t begin = clock();
		LSValue* res = fun();
		exeTime += (clock() - begin);

		ostringstream oss;
		res->print(oss);
		string r = oss.str();

		if (result != r) {
			cout << "FAUX : " << code << "  =/=>  " << result << "  got  " << r << endl;
		} else {
			cout << "OK   : " << code << "  ===>  " << result << endl;
			success++;
		}
	}
}

Test::~Test() {

}
