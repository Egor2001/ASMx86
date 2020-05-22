#ifndef YAGG_TARGET_H
#define YAGG_TARGET_H

enum EYaggTarget
{
#define YAGG_TARGET(TGT_ENUM, TGT_NAME) \
    YAGG_TGT_##TGT_ENUM,

    #include "../lists/YaggTargets.h"

#undef YAGG_TARGET
};

#endif //YAGG_TARGET_H
