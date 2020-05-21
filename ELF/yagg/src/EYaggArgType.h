#ifndef YAGG_ARG_TYPE_H
#define YAGG_ARG_TYPE_H

enum EYaggArgType
{
#define YAGG_ARG_TYPE(ARG_ENUM, ARG_CODE, ARG_NAME) \
    YAGG_ARG_TYPE_##ARG_ENUM = ARG_CODE,

    #include "YaggArgTypes.h"

#undef YAGG_ARG_TYPE
};

#endif //YAGG_ARG_TYPE_H
