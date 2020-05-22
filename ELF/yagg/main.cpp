#include "src/frontend/CYaggParser.h"
#include "src/backend/CYaggBuilder.h"

int main(int argc, const char* argv[])
{
    test_CYaggParser(argc, argv);
    test_CYaggBuilder(argc, argv);

    return 0;
}
