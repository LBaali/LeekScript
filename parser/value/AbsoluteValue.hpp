#ifndef ABSOLUTEVALUE_HPP
#define ABSOLUTEVALUE_HPP

#include "Expression.hpp"
#include "../../vm/LeekCode.hpp"

class AbsoluteValue : public Value {
public:

	Value* expression;

	AbsoluteValue();
	virtual ~AbsoluteValue();

	virtual void print(ostream&) const override;

	virtual void analyse(SemanticAnalyser*, const Type) override;

	virtual jit_value_t compile_jit(Compiler&, jit_function_t&, Type) const override;
};

#endif