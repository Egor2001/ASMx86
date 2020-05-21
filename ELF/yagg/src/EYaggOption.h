#ifndef YAGG_OPTION_H
#define YAGG_OPTION_H

enum EYaggOption
{
#define YAGG_OPTION(OPT_ENUM, OPT_NAME, OPT_LEN) \
    YAGG_OPT_##OPT_ENUM,

    #include "YaggOptions.h"

#undef YAGG_OPTION
};

#endif //YAGG_OPTION_H
