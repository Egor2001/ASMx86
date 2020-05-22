#ifndef YAGG_NAME_HOLDER_H
#define YAGG_NAME_HOLDER_H

struct SYaggNameHolder
{
    constexpr static size_t NAME_BUF_LEN = 16;

    char class_name[NAME_BUF_LEN];
    char fnc_prefix[NAME_BUF_LEN];
    char arg_type_name[NAME_BUF_LEN];
    char arg_data_name[NAME_BUF_LEN];
};

#endif //YAGG_NAME_HOLDER_H
