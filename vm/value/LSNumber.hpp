#ifndef LSNUMBER_H_
#define LSNUMBER_H_

#include <iostream>
#include "../LSValue.hpp"
#include "LSString.hpp"
#include "LSClass.hpp"
#include "../../lib/gason.h"
#include "../Type.hpp"

#define NUMBER_TYPE double
#define USE_CACHE 0
#define CACHE_LOW -128
#define CACHE_HIGH 1000

class LSNumber : public LSValue {
public:

#if USE_CACHE
	const NUMBER_TYPE value;
#else
	NUMBER_TYPE value;
#endif

	static LSClass* number_class;
	static LSNumber* cache[CACHE_HIGH - CACHE_LOW + 1];
	static void build_cache();
	static LSNumber* get(NUMBER_TYPE);

	LSNumber();
	LSNumber(NUMBER_TYPE value);
	LSNumber(JsonValue& data);

	virtual ~LSNumber();

	bool isTrue() const override;

	LSValue* operator - () const override;
	LSValue* operator ! () const override;
	LSValue* operator ~ () const override;

	LSValue* operator ++ () override;
	LSValue* operator ++ (int) override;

	LSValue* operator -- () override;
	LSValue* operator -- (int) override;

	LSValue* operator + (const LSValue*) const override;
	LSValue* operator + (const LSNull*) const override;
	LSValue* operator + (const LSBoolean*) const override;
	LSValue* operator + (const LSNumber*) const override;
	LSValue* operator + (const LSString*) const override;
	LSValue* operator + (const LSArray*) const override;
	LSValue* operator + (const LSObject*) const override;
	LSValue* operator + (const LSFunction*) const override;
	LSValue* operator + (const LSClass*) const override;

	LSValue* operator += (LSValue*) const override;
	LSValue* operator += (const LSNull*) override;
	LSValue* operator += (const LSBoolean*) override;
	LSValue* operator += (const LSNumber*) override;
	LSValue* operator += (const LSString*) override;
	LSValue* operator += (const LSArray*) override;
	LSValue* operator += (const LSObject*) override;
	LSValue* operator += (const LSFunction*) override;
	LSValue* operator += (const LSClass*) override;

	LSValue* operator - (const LSValue*) const override;
	LSValue* operator - (const LSNull*) const override;
	LSValue* operator - (const LSBoolean*) const override;
	LSValue* operator - (const LSNumber*) const override;
	LSValue* operator - (const LSString*) const override;
	LSValue* operator - (const LSArray*) const override;
	LSValue* operator - (const LSObject*) const override;
	LSValue* operator - (const LSFunction*) const override;
	LSValue* operator - (const LSClass*) const override;

	LSValue* operator -= (LSValue*) const override;
	LSValue* operator -= (const LSNull*) override;
	LSValue* operator -= (const LSBoolean*) override;
	LSValue* operator -= (const LSNumber*) override;
	LSValue* operator -= (const LSString*) override;
	LSValue* operator -= (const LSArray*) override;
	LSValue* operator -= (const LSObject*) override;
	LSValue* operator -= (const LSFunction*) override;
	LSValue* operator -= (const LSClass*) override;

	LSValue* operator * (const LSValue*) const override;
	LSValue* operator * (const LSNull*) const override;
	LSValue* operator * (const LSBoolean*) const override;
	LSValue* operator * (const LSNumber*) const override;
	LSValue* operator * (const LSString*) const override;
	LSValue* operator * (const LSArray*) const override;
	LSValue* operator * (const LSObject*) const override;
	LSValue* operator * (const LSFunction*) const override;
	LSValue* operator * (const LSClass*) const override;

	LSValue* operator *= (LSValue*) const override;
	LSValue* operator *= (const LSNull*) override;
	LSValue* operator *= (const LSBoolean*) override;
	LSValue* operator *= (const LSNumber*) override;
	LSValue* operator *= (const LSString*) override;
	LSValue* operator *= (const LSArray*) override;
	LSValue* operator *= (const LSObject*) override;
	LSValue* operator *= (const LSFunction*) override;
	LSValue* operator *= (const LSClass*) override;

	LSValue* operator / (const LSValue*) const override;
	LSValue* operator / (const LSNull*) const override;
	LSValue* operator / (const LSBoolean*) const override;
	LSValue* operator / (const LSNumber*) const override;
	LSValue* operator / (const LSString*) const override;
	LSValue* operator / (const LSArray*) const override;
	LSValue* operator / (const LSObject*) const override;
	LSValue* operator / (const LSFunction*) const override;
	LSValue* operator / (const LSClass*) const override;

	LSValue* operator /= (LSValue*) const override;
	LSValue* operator /= (const LSNull*) override;
	LSValue* operator /= (const LSBoolean*) override;
	LSValue* operator /= (const LSNumber*) override;
	LSValue* operator /= (const LSString*) override;
	LSValue* operator /= (const LSArray*) override;
	LSValue* operator /= (const LSObject*) override;
	LSValue* operator /= (const LSFunction*) override;
	LSValue* operator /= (const LSClass*) override;

	LSValue* poww(const LSValue*) const override;
	LSValue* poww(const LSNull*) const override;
	LSValue* poww(const LSBoolean*) const override;
	LSValue* poww(const LSNumber*) const override;
	LSValue* poww(const LSString*) const override;
	LSValue* poww(const LSArray*) const override;
	LSValue* poww(const LSObject*) const override;
	LSValue* poww(const LSFunction*) const override;
	LSValue* poww(const LSClass*) const override;

	LSValue* pow_eq(LSValue*) const override;
	LSValue* pow_eq(const LSNull*) override;
	LSValue* pow_eq(const LSBoolean*) override;
	LSValue* pow_eq(const LSNumber*) override;
	LSValue* pow_eq(const LSString*) override;
	LSValue* pow_eq(const LSArray*) override;
	LSValue* pow_eq(const LSObject*) override;
	LSValue* pow_eq(const LSFunction*) override;
	LSValue* pow_eq(const LSClass*) override;

	LSValue* operator % (const LSValue*) const override;
	LSValue* operator % (const LSNull*) const override;
	LSValue* operator % (const LSBoolean*) const override;
	LSValue* operator % (const LSNumber*) const override;
	LSValue* operator % (const LSString*) const override;
	LSValue* operator % (const LSArray*) const override;
	LSValue* operator % (const LSObject*) const override;
	LSValue* operator % (const LSFunction*) const override;
	LSValue* operator % (const LSClass*) const override;

	LSValue* operator %= (LSValue*) const override;
	LSValue* operator %= (const LSNull*) override;
	LSValue* operator %= (const LSBoolean*) override;
	LSValue* operator %= (const LSNumber*) override;
	LSValue* operator %= (const LSString*) override;
	LSValue* operator %= (const LSArray*) override;
	LSValue* operator %= (const LSObject*) override;
	LSValue* operator %= (const LSFunction*) override;
	LSValue* operator %= (const LSClass*) override;

	bool operator == (const LSValue*) const override;
	bool operator == (const LSNull*) const override;
	bool operator == (const LSBoolean*) const override;
	bool operator == (const LSNumber*) const override;
	bool operator == (const LSString*) const override;
	bool operator == (const LSArray*) const override;
	bool operator == (const LSFunction*) const override;
	bool operator == (const LSObject*) const override;
	bool operator == (const LSClass*) const override;

	bool operator < (const LSValue*) const override;
	bool operator < (const LSNull*) const override;
	bool operator < (const LSBoolean*) const override;
	bool operator < (const LSNumber*) const override;
	bool operator < (const LSString*) const override;
	bool operator < (const LSArray*) const override;
	bool operator < (const LSFunction*) const override;
	bool operator < (const LSObject*) const override;
	bool operator < (const LSClass*) const override;

	bool operator > (const LSValue*) const override;
	bool operator > (const LSNull*) const override;
	bool operator > (const LSBoolean*) const override;
	bool operator > (const LSNumber*) const override;
	bool operator > (const LSString*) const override;
	bool operator > (const LSArray*) const override;
	bool operator > (const LSFunction*) const override;
	bool operator > (const LSObject*) const override;
	bool operator > (const LSClass*) const override;

	bool operator <= (const LSValue*) const override;
	bool operator <= (const LSNull*) const override;
	bool operator <= (const LSBoolean*) const override;
	bool operator <= (const LSNumber*) const override;
	bool operator <= (const LSString*) const override;
	bool operator <= (const LSArray*) const override;
	bool operator <= (const LSFunction*) const override;
	bool operator <= (const LSObject*) const override;
	bool operator <= (const LSClass*) const override;

	bool operator >= (const LSValue*) const override;
	bool operator >= (const LSNull*) const override;
	bool operator >= (const LSBoolean*) const override;
	bool operator >= (const LSNumber*) const override;
	bool operator >= (const LSString*) const override;
	bool operator >= (const LSArray*) const override;
	bool operator >= (const LSFunction*) const override;
	bool operator >= (const LSObject*) const override;
	bool operator >= (const LSClass*) const override;

	bool in(const LSValue*) const override;

	LSValue* at (const LSValue* value) const override;
	LSValue** atL (const LSValue* value) override;

	LSValue* range(const LSValue* start, const LSValue* end) const override;
	LSValue* rangeL(const LSValue* start, const LSValue* end) override;

	LSValue* attr(const LSValue* key) const override;
	LSValue** attrL(const LSValue* key) override;

	LSValue* abso() const override;

	LSValue* clone() const override;

	std::ostream& print(std::ostream& os) const override;
	std::string json() const override;
	std::string toString() const;

	LSValue* getClass() const override;

	bool isInteger() const;

	int typeID() const override;

	virtual RawType getRawType() const override;
};

#endif
