#include "src/frontend/CYaggParser.hpp"
#include "src/backend/CYaggBuilder.hpp"

int main(int argc, const char* argv[])
{
    test_CYaggParser(argc, argv);
    test_CYaggBuilder(argc, argv);

    return 0;
}
