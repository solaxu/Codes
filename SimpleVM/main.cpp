#include "Assembler.h"
#include "Compiler.h"
#include "RuntimeStructures.h"

int main()
{
	SSVMAssembler assembler;
	assembler.Assemble("load.txt");
	SScript script;
	script.InitEnv(&assembler);
	script.Run();

// 	SCompiler compiler;
// 	compiler.Compile("test_sl.txt");
	return 0;
}