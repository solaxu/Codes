#pragma once

enum EType
{
	OBJECT,
	NIL,
	INT_D,
	FLT_D,
	STRING,
	LIST,
	TABLE,
	USER_DEFINE
};

struct STypeHeader
{
	static const int HeaderSize = 8;

	int type;
	STypeHeader* derived_from;

	STypeHeader();
	~STypeHeader();

	bool IsInstanceOf(int type);
};
