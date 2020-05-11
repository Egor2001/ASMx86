#include "src/mirk.h"

int main(int argc, char* argv[])
{
    assert(argc == 2);

    FILE* fout = fopen(argv[1u], "w");

    struct SMirkContext context = {};
    mirk_gen_elf(&context, fout);

    fclose(fout);
    fout = NULL;

    return 0;
}
