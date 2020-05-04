#include "src/naive.h"
#include "src/sse_blend.h"

int main(int argc, char* argv[])
{
    //test_naive(argc, argv);
    test_sse_blend(argc, argv);

    return 0;
}
