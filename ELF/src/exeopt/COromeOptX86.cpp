#include "ProcessorEnums.h"

class COromeOptX86
{
public:
    COromeOptX86();

    COromeOptX86             (const COromeOptX86&) = delete;
    COromeOptX86& operator = (const COromeOptX86&) = delete;
    COromeOptX86             (COromeOptX86&&) = default;
    COromeOptX86& operator = (COromeOptX86&&) = default;

private:
    std::vector<SMirkX86Command> x86_instr_vec_;
    std::vector<SMirkOptInfo> opt_info_vec_;
};
