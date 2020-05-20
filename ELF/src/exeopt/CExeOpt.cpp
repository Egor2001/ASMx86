#include "ProcessorEnums.h"

class CExeOpt
{
public:
    CExeOpt();

    CExeOpt             (const CExeOpt&) = delete;
    CExeOpt& operator = (const CExeOpt&) = delete;
    CExeOpt             (CExeOpt&&) = default;
    CExeOpt& operator = (CExeOpt&&) = default;

private:
    std::vector<SMirkX86Command> x86_instr_vec_;
    std::vector<SMirkOptInfo> opt_info_vec_;
};
