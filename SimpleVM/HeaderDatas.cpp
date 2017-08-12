#include "HeaderDatas.h"

STypeHeader::STypeHeader():type(EType::OBJECT),derived_from(nullptr)
{

}

STypeHeader::~STypeHeader()
{

}

bool STypeHeader::IsInstanceOf(int type)
{
	auto p = this->derived_from;
	while (p != nullptr)
	{
		if (this->type == type)
			return true;
		p = p->derived_from;
	}
	return false;
}