#pragma once


class CShaderVM
{
public:
	CShaderVM();
	~CShaderVM();




	void RunScript(void* instructions);
};