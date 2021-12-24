#include "LLVM_exp19_VivadoHLSIREval.h"
#include <iostream>

using namespace llvm;
using namespace polly;

std::string clock_period_str;
std::string HLS_lib_path;


using namespace clang;
using namespace clang::driver;
using namespace clang::tooling;

static llvm::cl::OptionCategory StatSampleCategory("Stat Sample");

int main(int argc, const char **argv)
{

    SMDiagnostic Err;
    LLVMContext Context;

    std::unique_ptr<llvm::Module> Mod(parseIRFile("top.bc", Err, Context));
    if (!Mod)
    {
        Err.print(argv[0], errs());
        return 1;
    }

    // Create a pass manager and fill it with the passes we want to run.
    legacy::PassManager PM, PM1, PM11, PM2, PM_pre, PM3, PM4;
    LLVMTargetRef T;
    ModulePassManager MPM;

    char *Error;

    if (LLVMGetTargetFromTriple((Mod->getTargetTriple()).c_str(), &T, &Error))
    {
        std::cout<<"Error: " << Error <<"\n";
    }
    else
    {
        std::string targetname = LLVMGetTargetName(T);
        targetname = "The target machine is: " + targetname;
        std::cout<<targetname.c_str()<<"\n";
    }

    return 0;
}
