#pragma once


struct SExp
{
	SExp();
	virtual ~SExp();
	virtual void eval();
};

struct SUnaryExp : public SExp
{
	SExp* exp = nullptr;
	SUnaryExp();
	virtual ~SUnaryExp();
	virtual void eval();
};

struct SConstInt : public SUnaryExp
{
	SConstInt();
	virtual ~SConstInt();
	virtual void eval();
};

struct SConstFloat : public SUnaryExp
{
	SConstFloat();
	virtual ~SConstFloat();
	virtual void eval();
};

struct SBinaryExp : public SUnaryExp
{
	SExp* left_exp = nullptr;
	SExp* right_exp = nullptr;
	SBinaryExp();
	virtual ~SBinaryExp();
	virtual void eval();
};

struct SStatement
{

};