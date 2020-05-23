bool YB_INSTR_FNC(add)(const YB_ARG_TYPE& dst, const YB_ARG_TYPE& src)
{
    YB_DATA_TYPE data = {};
    EYaggArgType dst_type = YB_TYPE_FNC(dst);
    EYaggArgType src_type = YB_TYPE_FNC(src);
    switch ((dst_type << 4u) | src_type)
    {
        case 0x21:
        {
            YB_DATA_SET_REX(data, 0x8);
            YB_DATA_SET_OPC(data, 0x81);
            YB_DATA_SET_EXT(data, 0);
            YB_DATA_SET_ARG(data, r_m, dst);
            YB_DATA_SET_ARG(data, imm, src);
        }
        break;// case
        case 0x41:
        {
            YB_DATA_SET_REX(data, 0x8);
            YB_DATA_SET_OPC(data, 0x81);
            YB_DATA_SET_EXT(data, 0);
            YB_DATA_SET_ARG(data, r_m, dst);
            YB_DATA_SET_ARG(data, imm, src);
        }
        break;// case
        case 0x22:
        {
            YB_DATA_SET_REX(data, 0x8);
            YB_DATA_SET_OPC(data, 0x1);
            YB_DATA_SET_ARG(data, r_m, dst);
            YB_DATA_SET_ARG(data, reg, src);
        }
        break;// case
        case 0x42:
        {
            YB_DATA_SET_REX(data, 0x8);
            YB_DATA_SET_OPC(data, 0x1);
            YB_DATA_SET_ARG(data, r_m, dst);
            YB_DATA_SET_ARG(data, reg, src);
        }
        break;// case
        case 0x24:
        {
            YB_DATA_SET_REX(data, 0x8);
            YB_DATA_SET_OPC(data, 0x3);
            YB_DATA_SET_ARG(data, reg, dst);
            YB_DATA_SET_ARG(data, r_m, src);
        }
        break;// case
        default: return false;
    }// switch
    YB_NEW_DATA(data);
    return true;
}
bool YB_INSTR_FNC(and)(const YB_ARG_TYPE& dst, const YB_ARG_TYPE& src)
{
    YB_DATA_TYPE data = {};
    EYaggArgType dst_type = YB_TYPE_FNC(dst);
    EYaggArgType src_type = YB_TYPE_FNC(src);
    switch ((dst_type << 4u) | src_type)
    {
        case 0x21:
        {
            YB_DATA_SET_REX(data, 0x8);
            YB_DATA_SET_OPC(data, 0x81);
            YB_DATA_SET_EXT(data, 0x4);
            YB_DATA_SET_ARG(data, r_m, dst);
            YB_DATA_SET_ARG(data, imm, src);
        }
        break;// case
        case 0x41:
        {
            YB_DATA_SET_REX(data, 0x8);
            YB_DATA_SET_OPC(data, 0x81);
            YB_DATA_SET_EXT(data, 0x4);
            YB_DATA_SET_ARG(data, r_m, dst);
            YB_DATA_SET_ARG(data, imm, src);
        }
        break;// case
        case 0x22:
        {
            YB_DATA_SET_REX(data, 0x8);
            YB_DATA_SET_OPC(data, 0x21);
            YB_DATA_SET_ARG(data, r_m, dst);
            YB_DATA_SET_ARG(data, reg, src);
        }
        break;// case
        case 0x42:
        {
            YB_DATA_SET_REX(data, 0x8);
            YB_DATA_SET_OPC(data, 0x21);
            YB_DATA_SET_ARG(data, r_m, dst);
            YB_DATA_SET_ARG(data, reg, src);
        }
        break;// case
        case 0x24:
        {
            YB_DATA_SET_REX(data, 0x8);
            YB_DATA_SET_OPC(data, 0x23);
            YB_DATA_SET_ARG(data, reg, dst);
            YB_DATA_SET_ARG(data, r_m, src);
        }
        break;// case
        default: return false;
    }// switch
    YB_NEW_DATA(data);
    return true;
}
bool YB_INSTR_FNC(call)(const YB_ARG_TYPE& dst, const YB_ARG_TYPE& src)
{
    YB_DATA_TYPE data = {};
    EYaggArgType dst_type = YB_TYPE_FNC(dst);
    EYaggArgType src_type = YB_TYPE_FNC(src);
    switch ((dst_type << 4u) | src_type)
    {
        case 0x10:
        {
            YB_DATA_SET_OPC(data, 0xe8);
            YB_DATA_SET_ARG(data, dis, dst);
            YB_NEW_JUMP(data);
        }
        break;// case
        case 0x20:
        {
            YB_DATA_SET_OPC(data, 0xff);
            YB_DATA_SET_EXT(data, 0x2);
            YB_DATA_SET_ARG(data, r_m, dst);
        }
        break;// case
        case 0x40:
        {
            YB_DATA_SET_OPC(data, 0xff);
            YB_DATA_SET_EXT(data, 0x2);
            YB_DATA_SET_ARG(data, r_m, dst);
        }
        break;// case
        default: return false;
    }// switch
    YB_NEW_DATA(data);
    return true;
}
bool YB_INSTR_FNC(cmp)(const YB_ARG_TYPE& dst, const YB_ARG_TYPE& src)
{
    YB_DATA_TYPE data = {};
    EYaggArgType dst_type = YB_TYPE_FNC(dst);
    EYaggArgType src_type = YB_TYPE_FNC(src);
    switch ((dst_type << 4u) | src_type)
    {
        case 0x21:
        {
            YB_DATA_SET_REX(data, 0x8);
            YB_DATA_SET_OPC(data, 0x81);
            YB_DATA_SET_EXT(data, 0x7);
            YB_DATA_SET_ARG(data, r_m, dst);
            YB_DATA_SET_ARG(data, imm, src);
        }
        break;// case
        case 0x41:
        {
            YB_DATA_SET_REX(data, 0x8);
            YB_DATA_SET_OPC(data, 0x81);
            YB_DATA_SET_EXT(data, 0x7);
            YB_DATA_SET_ARG(data, r_m, dst);
            YB_DATA_SET_ARG(data, imm, src);
        }
        break;// case
        case 0x22:
        {
            YB_DATA_SET_REX(data, 0x8);
            YB_DATA_SET_OPC(data, 0x39);
            YB_DATA_SET_ARG(data, r_m, dst);
            YB_DATA_SET_ARG(data, reg, src);
        }
        break;// case
        case 0x42:
        {
            YB_DATA_SET_REX(data, 0x8);
            YB_DATA_SET_OPC(data, 0x39);
            YB_DATA_SET_ARG(data, r_m, dst);
            YB_DATA_SET_ARG(data, reg, src);
        }
        break;// case
        case 0x24:
        {
            YB_DATA_SET_REX(data, 0x8);
            YB_DATA_SET_OPC(data, 0x3b);
            YB_DATA_SET_ARG(data, reg, dst);
            YB_DATA_SET_ARG(data, r_m, src);
        }
        break;// case
        default: return false;
    }// switch
    YB_NEW_DATA(data);
    return true;
}
bool YB_INSTR_FNC(dec)(const YB_ARG_TYPE& dst, const YB_ARG_TYPE& src)
{
    YB_DATA_TYPE data = {};
    EYaggArgType dst_type = YB_TYPE_FNC(dst);
    EYaggArgType src_type = YB_TYPE_FNC(src);
    switch ((dst_type << 4u) | src_type)
    {
        case 0x20:
        {
            YB_DATA_SET_REX(data, 0x8);
            YB_DATA_SET_OPC(data, 0xff);
            YB_DATA_SET_EXT(data, 0x1);
            YB_DATA_SET_ARG(data, r_m, dst);
        }
        break;// case
        case 0x40:
        {
            YB_DATA_SET_REX(data, 0x8);
            YB_DATA_SET_OPC(data, 0xff);
            YB_DATA_SET_EXT(data, 0x1);
            YB_DATA_SET_ARG(data, r_m, dst);
        }
        break;// case
        default: return false;
    }// switch
    YB_NEW_DATA(data);
    return true;
}
bool YB_INSTR_FNC(div)(const YB_ARG_TYPE& dst, const YB_ARG_TYPE& src)
{
    YB_DATA_TYPE data = {};
    EYaggArgType dst_type = YB_TYPE_FNC(dst);
    EYaggArgType src_type = YB_TYPE_FNC(src);
    switch ((dst_type << 4u) | src_type)
    {
        case 0x20:
        {
            YB_DATA_SET_REX(data, 0x8);
            YB_DATA_SET_OPC(data, 0xf7);
            YB_DATA_SET_EXT(data, 0x6);
            YB_DATA_SET_ARG(data, r_m, dst);
        }
        break;// case
        case 0x40:
        {
            YB_DATA_SET_REX(data, 0x8);
            YB_DATA_SET_OPC(data, 0xf7);
            YB_DATA_SET_EXT(data, 0x6);
            YB_DATA_SET_ARG(data, r_m, dst);
        }
        break;// case
        default: return false;
    }// switch
    YB_NEW_DATA(data);
    return true;
}
bool YB_INSTR_FNC(inc)(const YB_ARG_TYPE& dst, const YB_ARG_TYPE& src)
{
    YB_DATA_TYPE data = {};
    EYaggArgType dst_type = YB_TYPE_FNC(dst);
    EYaggArgType src_type = YB_TYPE_FNC(src);
    switch ((dst_type << 4u) | src_type)
    {
        case 0x20:
        {
            YB_DATA_SET_REX(data, 0x8);
            YB_DATA_SET_OPC(data, 0xff);
            YB_DATA_SET_EXT(data, 0);
            YB_DATA_SET_ARG(data, r_m, dst);
        }
        break;// case
        case 0x40:
        {
            YB_DATA_SET_REX(data, 0x8);
            YB_DATA_SET_OPC(data, 0xff);
            YB_DATA_SET_EXT(data, 0);
            YB_DATA_SET_ARG(data, r_m, dst);
        }
        break;// case
        default: return false;
    }// switch
    YB_NEW_DATA(data);
    return true;
}
bool YB_INSTR_FNC(jbe)(const YB_ARG_TYPE& dst, const YB_ARG_TYPE& src)
{
    YB_DATA_TYPE data = {};
    EYaggArgType dst_type = YB_TYPE_FNC(dst);
    EYaggArgType src_type = YB_TYPE_FNC(src);
    switch ((dst_type << 4u) | src_type)
    {
        case 0x10:
        {
            YB_DATA_SET_0FH(data);
            YB_DATA_SET_OPC(data, 0x86);
            YB_DATA_SET_ARG(data, dis, dst);
            YB_NEW_JUMP(data);
        }
        break;// case
        default: return false;
    }// switch
    YB_NEW_DATA(data);
    return true;
}
bool YB_INSTR_FNC(jb)(const YB_ARG_TYPE& dst, const YB_ARG_TYPE& src)
{
    YB_DATA_TYPE data = {};
    EYaggArgType dst_type = YB_TYPE_FNC(dst);
    EYaggArgType src_type = YB_TYPE_FNC(src);
    switch ((dst_type << 4u) | src_type)
    {
        case 0x10:
        {
            YB_DATA_SET_0FH(data);
            YB_DATA_SET_OPC(data, 0x82);
            YB_DATA_SET_ARG(data, dis, dst);
            YB_NEW_JUMP(data);
        }
        break;// case
        default: return false;
    }// switch
    YB_NEW_DATA(data);
    return true;
}
bool YB_INSTR_FNC(jge)(const YB_ARG_TYPE& dst, const YB_ARG_TYPE& src)
{
    YB_DATA_TYPE data = {};
    EYaggArgType dst_type = YB_TYPE_FNC(dst);
    EYaggArgType src_type = YB_TYPE_FNC(src);
    switch ((dst_type << 4u) | src_type)
    {
        case 0x10:
        {
            YB_DATA_SET_0FH(data);
            YB_DATA_SET_OPC(data, 0x8d);
            YB_DATA_SET_ARG(data, dis, dst);
            YB_NEW_JUMP(data);
        }
        break;// case
        default: return false;
    }// switch
    YB_NEW_DATA(data);
    return true;
}
bool YB_INSTR_FNC(jg)(const YB_ARG_TYPE& dst, const YB_ARG_TYPE& src)
{
    YB_DATA_TYPE data = {};
    EYaggArgType dst_type = YB_TYPE_FNC(dst);
    EYaggArgType src_type = YB_TYPE_FNC(src);
    switch ((dst_type << 4u) | src_type)
    {
        case 0x10:
        {
            YB_DATA_SET_0FH(data);
            YB_DATA_SET_OPC(data, 0x8f);
            YB_DATA_SET_ARG(data, dis, dst);
            YB_NEW_JUMP(data);
        }
        break;// case
        default: return false;
    }// switch
    YB_NEW_DATA(data);
    return true;
}
bool YB_INSTR_FNC(jmp)(const YB_ARG_TYPE& dst, const YB_ARG_TYPE& src)
{
    YB_DATA_TYPE data = {};
    EYaggArgType dst_type = YB_TYPE_FNC(dst);
    EYaggArgType src_type = YB_TYPE_FNC(src);
    switch ((dst_type << 4u) | src_type)
    {
        case 0x10:
        {
            YB_DATA_SET_OPC(data, 0xe9);
            YB_DATA_SET_ARG(data, dis, dst);
            YB_NEW_JUMP(data);
        }
        break;// case
        case 0x20:
        {
            YB_DATA_SET_OPC(data, 0xff);
            YB_DATA_SET_EXT(data, 0x4);
            YB_DATA_SET_ARG(data, r_m, dst);
        }
        break;// case
        case 0x40:
        {
            YB_DATA_SET_OPC(data, 0xff);
            YB_DATA_SET_EXT(data, 0x4);
            YB_DATA_SET_ARG(data, r_m, dst);
        }
        break;// case
        default: return false;
    }// switch
    YB_NEW_DATA(data);
    return true;
}
bool YB_INSTR_FNC(jnz)(const YB_ARG_TYPE& dst, const YB_ARG_TYPE& src)
{
    YB_DATA_TYPE data = {};
    EYaggArgType dst_type = YB_TYPE_FNC(dst);
    EYaggArgType src_type = YB_TYPE_FNC(src);
    switch ((dst_type << 4u) | src_type)
    {
        case 0x10:
        {
            YB_DATA_SET_0FH(data);
            YB_DATA_SET_OPC(data, 0x85);
            YB_DATA_SET_ARG(data, dis, dst);
            YB_NEW_JUMP(data);
        }
        break;// case
        default: return false;
    }// switch
    YB_NEW_DATA(data);
    return true;
}
bool YB_INSTR_FNC(jz)(const YB_ARG_TYPE& dst, const YB_ARG_TYPE& src)
{
    YB_DATA_TYPE data = {};
    EYaggArgType dst_type = YB_TYPE_FNC(dst);
    EYaggArgType src_type = YB_TYPE_FNC(src);
    switch ((dst_type << 4u) | src_type)
    {
        case 0x10:
        {
            YB_DATA_SET_0FH(data);
            YB_DATA_SET_OPC(data, 0x84);
            YB_DATA_SET_ARG(data, dis, dst);
            YB_NEW_JUMP(data);
        }
        break;// case
        default: return false;
    }// switch
    YB_NEW_DATA(data);
    return true;
}
bool YB_INSTR_FNC(lea)(const YB_ARG_TYPE& dst, const YB_ARG_TYPE& src)
{
    YB_DATA_TYPE data = {};
    EYaggArgType dst_type = YB_TYPE_FNC(dst);
    EYaggArgType src_type = YB_TYPE_FNC(src);
    switch ((dst_type << 4u) | src_type)
    {
        case 0x24:
        {
            YB_DATA_SET_REX(data, 0x8);
            YB_DATA_SET_OPC(data, 0x8d);
            YB_DATA_SET_ARG(data, reg, dst);
            YB_DATA_SET_ARG(data, r_m, src);
        }
        break;// case
        default: return false;
    }// switch
    YB_NEW_DATA(data);
    return true;
}
bool YB_INSTR_FNC(mov)(const YB_ARG_TYPE& dst, const YB_ARG_TYPE& src)
{
    YB_DATA_TYPE data = {};
    EYaggArgType dst_type = YB_TYPE_FNC(dst);
    EYaggArgType src_type = YB_TYPE_FNC(src);
    switch ((dst_type << 4u) | src_type)
    {
        case 0x21:
        {
            YB_DATA_SET_REX(data, 0x8);
            YB_DATA_SET_OPC(data, 0xc7);
            YB_DATA_SET_EXT(data, 0);
            YB_DATA_SET_ARG(data, r_m, dst);
            YB_DATA_SET_ARG(data, imm, src);
        }
        break;// case
        case 0x41:
        {
            YB_DATA_SET_REX(data, 0x8);
            YB_DATA_SET_OPC(data, 0xc7);
            YB_DATA_SET_EXT(data, 0);
            YB_DATA_SET_ARG(data, r_m, dst);
            YB_DATA_SET_ARG(data, imm, src);
        }
        break;// case
        case 0x22:
        {
            YB_DATA_SET_REX(data, 0x8);
            YB_DATA_SET_OPC(data, 0x89);
            YB_DATA_SET_ARG(data, r_m, dst);
            YB_DATA_SET_ARG(data, reg, src);
        }
        break;// case
        case 0x42:
        {
            YB_DATA_SET_REX(data, 0x8);
            YB_DATA_SET_OPC(data, 0x89);
            YB_DATA_SET_ARG(data, r_m, dst);
            YB_DATA_SET_ARG(data, reg, src);
        }
        break;// case
        case 0x24:
        {
            YB_DATA_SET_REX(data, 0x8);
            YB_DATA_SET_OPC(data, 0x8b);
            YB_DATA_SET_ARG(data, reg, dst);
            YB_DATA_SET_ARG(data, r_m, src);
        }
        break;// case
        default: return false;
    }// switch
    YB_NEW_DATA(data);
    return true;
}
bool YB_INSTR_FNC(mul)(const YB_ARG_TYPE& dst, const YB_ARG_TYPE& src)
{
    YB_DATA_TYPE data = {};
    EYaggArgType dst_type = YB_TYPE_FNC(dst);
    EYaggArgType src_type = YB_TYPE_FNC(src);
    switch ((dst_type << 4u) | src_type)
    {
        case 0x20:
        {
            YB_DATA_SET_REX(data, 0x8);
            YB_DATA_SET_OPC(data, 0xf7);
            YB_DATA_SET_EXT(data, 0x4);
            YB_DATA_SET_ARG(data, r_m, dst);
        }
        break;// case
        case 0x40:
        {
            YB_DATA_SET_REX(data, 0x8);
            YB_DATA_SET_OPC(data, 0xf7);
            YB_DATA_SET_EXT(data, 0x4);
            YB_DATA_SET_ARG(data, r_m, dst);
        }
        break;// case
        default: return false;
    }// switch
    YB_NEW_DATA(data);
    return true;
}
bool YB_INSTR_FNC(neg)(const YB_ARG_TYPE& dst, const YB_ARG_TYPE& src)
{
    YB_DATA_TYPE data = {};
    EYaggArgType dst_type = YB_TYPE_FNC(dst);
    EYaggArgType src_type = YB_TYPE_FNC(src);
    switch ((dst_type << 4u) | src_type)
    {
        case 0x20:
        {
            YB_DATA_SET_REX(data, 0x8);
            YB_DATA_SET_OPC(data, 0xf7);
            YB_DATA_SET_EXT(data, 0x3);
            YB_DATA_SET_ARG(data, r_m, dst);
        }
        break;// case
        case 0x40:
        {
            YB_DATA_SET_REX(data, 0x8);
            YB_DATA_SET_OPC(data, 0xf7);
            YB_DATA_SET_EXT(data, 0x3);
            YB_DATA_SET_ARG(data, r_m, dst);
        }
        break;// case
        default: return false;
    }// switch
    YB_NEW_DATA(data);
    return true;
}
bool YB_INSTR_FNC(not)(const YB_ARG_TYPE& dst, const YB_ARG_TYPE& src)
{
    YB_DATA_TYPE data = {};
    EYaggArgType dst_type = YB_TYPE_FNC(dst);
    EYaggArgType src_type = YB_TYPE_FNC(src);
    switch ((dst_type << 4u) | src_type)
    {
        case 0x20:
        {
            YB_DATA_SET_REX(data, 0x8);
            YB_DATA_SET_OPC(data, 0xf7);
            YB_DATA_SET_EXT(data, 0x2);
            YB_DATA_SET_ARG(data, r_m, dst);
        }
        break;// case
        case 0x40:
        {
            YB_DATA_SET_REX(data, 0x8);
            YB_DATA_SET_OPC(data, 0xf7);
            YB_DATA_SET_EXT(data, 0x2);
            YB_DATA_SET_ARG(data, r_m, dst);
        }
        break;// case
        default: return false;
    }// switch
    YB_NEW_DATA(data);
    return true;
}
bool YB_INSTR_FNC(or)(const YB_ARG_TYPE& dst, const YB_ARG_TYPE& src)
{
    YB_DATA_TYPE data = {};
    EYaggArgType dst_type = YB_TYPE_FNC(dst);
    EYaggArgType src_type = YB_TYPE_FNC(src);
    switch ((dst_type << 4u) | src_type)
    {
        case 0x21:
        {
            YB_DATA_SET_REX(data, 0x8);
            YB_DATA_SET_OPC(data, 0x81);
            YB_DATA_SET_EXT(data, 0x1);
            YB_DATA_SET_ARG(data, r_m, dst);
            YB_DATA_SET_ARG(data, imm, src);
        }
        break;// case
        case 0x41:
        {
            YB_DATA_SET_REX(data, 0x8);
            YB_DATA_SET_OPC(data, 0x81);
            YB_DATA_SET_EXT(data, 0x1);
            YB_DATA_SET_ARG(data, r_m, dst);
            YB_DATA_SET_ARG(data, imm, src);
        }
        break;// case
        case 0x22:
        {
            YB_DATA_SET_REX(data, 0x8);
            YB_DATA_SET_OPC(data, 0x9);
            YB_DATA_SET_ARG(data, r_m, dst);
            YB_DATA_SET_ARG(data, reg, src);
        }
        break;// case
        case 0x42:
        {
            YB_DATA_SET_REX(data, 0x8);
            YB_DATA_SET_OPC(data, 0x9);
            YB_DATA_SET_ARG(data, r_m, dst);
            YB_DATA_SET_ARG(data, reg, src);
        }
        break;// case
        case 0x24:
        {
            YB_DATA_SET_REX(data, 0x8);
            YB_DATA_SET_OPC(data, 0xb);
            YB_DATA_SET_ARG(data, reg, dst);
            YB_DATA_SET_ARG(data, r_m, src);
        }
        break;// case
        default: return false;
    }// switch
    YB_NEW_DATA(data);
    return true;
}
bool YB_INSTR_FNC(pop)(const YB_ARG_TYPE& dst, const YB_ARG_TYPE& src)
{
    YB_DATA_TYPE data = {};
    EYaggArgType dst_type = YB_TYPE_FNC(dst);
    EYaggArgType src_type = YB_TYPE_FNC(src);
    switch ((dst_type << 4u) | src_type)
    {
        case 0x20:
        {
            YB_DATA_SET_OPC(data, 0x8f);
            YB_DATA_SET_EXT(data, 0);
            YB_DATA_SET_ARG(data, r_m, dst);
        }
        break;// case
        case 0x40:
        {
            YB_DATA_SET_OPC(data, 0x8f);
            YB_DATA_SET_EXT(data, 0);
            YB_DATA_SET_ARG(data, r_m, dst);
        }
        break;// case
        default: return false;
    }// switch
    YB_NEW_DATA(data);
    return true;
}
bool YB_INSTR_FNC(push)(const YB_ARG_TYPE& dst, const YB_ARG_TYPE& src)
{
    YB_DATA_TYPE data = {};
    EYaggArgType dst_type = YB_TYPE_FNC(dst);
    EYaggArgType src_type = YB_TYPE_FNC(src);
    switch ((dst_type << 4u) | src_type)
    {
        case 0x10:
        {
            YB_DATA_SET_OPC(data, 0x68);
            YB_DATA_SET_ARG(data, imm, dst);
        }
        break;// case
        case 0x20:
        {
            YB_DATA_SET_OPC(data, 0xff);
            YB_DATA_SET_EXT(data, 0x6);
            YB_DATA_SET_ARG(data, r_m, dst);
        }
        break;// case
        case 0x40:
        {
            YB_DATA_SET_OPC(data, 0xff);
            YB_DATA_SET_EXT(data, 0x6);
            YB_DATA_SET_ARG(data, r_m, dst);
        }
        break;// case
        default: return false;
    }// switch
    YB_NEW_DATA(data);
    return true;
}
bool YB_INSTR_FNC(ret)(const YB_ARG_TYPE& dst, const YB_ARG_TYPE& src)
{
    YB_DATA_TYPE data = {};
    EYaggArgType dst_type = YB_TYPE_FNC(dst);
    EYaggArgType src_type = YB_TYPE_FNC(src);
    switch ((dst_type << 4u) | src_type)
    {
        case 0:
        {
            YB_DATA_SET_OPC(data, 0xc3);
        }
        break;// case
        default: return false;
    }// switch
    YB_NEW_DATA(data);
    return true;
}
bool YB_INSTR_FNC(sub)(const YB_ARG_TYPE& dst, const YB_ARG_TYPE& src)
{
    YB_DATA_TYPE data = {};
    EYaggArgType dst_type = YB_TYPE_FNC(dst);
    EYaggArgType src_type = YB_TYPE_FNC(src);
    switch ((dst_type << 4u) | src_type)
    {
        case 0x21:
        {
            YB_DATA_SET_REX(data, 0x8);
            YB_DATA_SET_OPC(data, 0x81);
            YB_DATA_SET_EXT(data, 0x5);
            YB_DATA_SET_ARG(data, r_m, dst);
            YB_DATA_SET_ARG(data, imm, src);
        }
        break;// case
        case 0x41:
        {
            YB_DATA_SET_REX(data, 0x8);
            YB_DATA_SET_OPC(data, 0x81);
            YB_DATA_SET_EXT(data, 0x5);
            YB_DATA_SET_ARG(data, r_m, dst);
            YB_DATA_SET_ARG(data, imm, src);
        }
        break;// case
        case 0x22:
        {
            YB_DATA_SET_REX(data, 0x8);
            YB_DATA_SET_OPC(data, 0x29);
            YB_DATA_SET_ARG(data, r_m, dst);
            YB_DATA_SET_ARG(data, reg, src);
        }
        break;// case
        case 0x42:
        {
            YB_DATA_SET_REX(data, 0x8);
            YB_DATA_SET_OPC(data, 0x29);
            YB_DATA_SET_ARG(data, r_m, dst);
            YB_DATA_SET_ARG(data, reg, src);
        }
        break;// case
        case 0x24:
        {
            YB_DATA_SET_REX(data, 0x8);
            YB_DATA_SET_OPC(data, 0x2b);
            YB_DATA_SET_ARG(data, reg, dst);
            YB_DATA_SET_ARG(data, r_m, src);
        }
        break;// case
        default: return false;
    }// switch
    YB_NEW_DATA(data);
    return true;
}
bool YB_INSTR_FNC(syscall)(const YB_ARG_TYPE& dst, const YB_ARG_TYPE& src)
{
    YB_DATA_TYPE data = {};
    EYaggArgType dst_type = YB_TYPE_FNC(dst);
    EYaggArgType src_type = YB_TYPE_FNC(src);
    switch ((dst_type << 4u) | src_type)
    {
        case 0:
        {
            YB_DATA_SET_0FH(data);
            YB_DATA_SET_OPC(data, 0x5);
        }
        break;// case
        default: return false;
    }// switch
    YB_NEW_DATA(data);
    return true;
}
bool YB_INSTR_FNC(xor)(const YB_ARG_TYPE& dst, const YB_ARG_TYPE& src)
{
    YB_DATA_TYPE data = {};
    EYaggArgType dst_type = YB_TYPE_FNC(dst);
    EYaggArgType src_type = YB_TYPE_FNC(src);
    switch ((dst_type << 4u) | src_type)
    {
        case 0x21:
        {
            YB_DATA_SET_REX(data, 0x8);
            YB_DATA_SET_OPC(data, 0x81);
            YB_DATA_SET_EXT(data, 0x6);
            YB_DATA_SET_ARG(data, r_m, dst);
            YB_DATA_SET_ARG(data, imm, src);
        }
        break;// case
        case 0x41:
        {
            YB_DATA_SET_REX(data, 0x8);
            YB_DATA_SET_OPC(data, 0x81);
            YB_DATA_SET_EXT(data, 0x6);
            YB_DATA_SET_ARG(data, r_m, dst);
            YB_DATA_SET_ARG(data, imm, src);
        }
        break;// case
        case 0x22:
        {
            YB_DATA_SET_REX(data, 0x8);
            YB_DATA_SET_OPC(data, 0x31);
            YB_DATA_SET_ARG(data, r_m, dst);
            YB_DATA_SET_ARG(data, reg, src);
        }
        break;// case
        case 0x42:
        {
            YB_DATA_SET_REX(data, 0x8);
            YB_DATA_SET_OPC(data, 0x31);
            YB_DATA_SET_ARG(data, r_m, dst);
            YB_DATA_SET_ARG(data, reg, src);
        }
        break;// case
        case 0x24:
        {
            YB_DATA_SET_REX(data, 0x8);
            YB_DATA_SET_OPC(data, 0x33);
            YB_DATA_SET_ARG(data, reg, dst);
            YB_DATA_SET_ARG(data, r_m, src);
        }
        break;// case
        default: return false;
    }// switch
    YB_NEW_DATA(data);
    return true;
}
