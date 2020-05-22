#include "CYaggParser.hpp"

bool CYaggParser::parse_instr(SYaggInstr* instr, 
                              const std::string_view& text) try
{
    assert(instr);

    static const char INSTR_STR[] = "INSTR";

    text_ = text;
    text_.remove_prefix(offset_skip());

    parse_list_begin();

    size_t pref = offset_word();
    if (text_.compare(0u, sizeof(INSTR_STR) - 1u, INSTR_STR)) 
        throw CYaggException("instr expected", text_);

    text_.remove_prefix(pref);
    text_.remove_prefix(offset_skip());

    parse_instr_name(instr);
    while (!text_.empty() && text_.front() != ']')
    {
        instr->entry_vec.emplace_back();
        parse_entry(&instr->entry_vec.back(), text_);
    }

    parse_list_end();

    return true;
}
catch (const CYaggException& exc)
{
    *instr = {};
    throw;//it must be implicit, but I have no idea why it's not
}

bool CYaggParser::parse_entry(SYaggEntry* entry, 
                              const std::string_view& text) try
{
    assert(entry);

    static const char ENTRY_STR[] = "ENTRY";

    text_ = text;
    text_.remove_prefix(offset_skip());

    parse_list_begin();
    parse_list_begin();

    size_t pref = offset_word();
    if (text_.compare(0u, sizeof(ENTRY_STR) - 1u, ENTRY_STR)) 
        throw CYaggException("entry expected", text_);

    text_.remove_prefix(pref);
    text_.remove_prefix(offset_skip());

    parse_entry_dst(entry);
    parse_entry_src(entry);

    parse_list_end();

    parse_data(&entry->data, text_);

    parse_list_end();

    return true;
}
catch (const CYaggException& exc)
{
    *entry = {};
    throw;//it must be implicit, but I have no idea why it's not
}

bool CYaggParser::parse_data(SYaggData* data, 
                             const std::string_view& text) try
{
    assert(data);

    text_ = text;
    text_.remove_prefix(offset_skip());

    parse_list_begin();

    size_t pref = 0u;
    while (!text_.empty() && text_.front() != ']')
    {
        parse_list_begin();

        pref = offset_word();
        #define YAGG_OPTION(OPT_ENUM, OPT_NAME, OPT_SIZE) \
            else if (!text_.compare(0u, sizeof(#OPT_NAME) - 1u, #OPT_NAME)) \
            { \
                text_.remove_prefix(pref); \
                text_.remove_prefix(offset_skip()); \
                parse_opt_##OPT_NAME(data); \
            }

        if (false) {}
        #include "../lists/YaggOptions.h"
        else throw CYaggException("unrecognized word", text_);

        #undef YAGG_OPTION

        parse_list_end();
    }

    parse_list_end();

    return true;
}
catch (const CYaggException& exc)
{
    *data = {};
    throw;//it must be implicit, but I have no idea why it's not
}

bool CYaggParser::parse_instr_name(SYaggInstr* instr)
{
    assert(instr);

    parse_list_begin();

    size_t pref = offset_word();
    if (pref >= SYaggInstr::NAME_BUF_LEN)
        throw CYaggException("name is too long", text_);

    std::memcpy(instr->name_buf, text_.data(), pref);

    text_.remove_prefix(pref);
    text_.remove_prefix(offset_skip());

    parse_list_end();

    return true;
}

bool CYaggParser::parse_entry_dst(SYaggEntry* entry)
{
    assert(entry);

    parse_list_begin();

    size_t pref = offset_word();

    #define YAGG_ARG_TYPE(ARG_ENUM, ARG_CODE, ARG_NAME) \
        else if (!text_.compare(0u, sizeof(#ARG_NAME) - 1u, #ARG_NAME)) \
        { \
            entry->dst = YAGG_ARG_TYPE_##ARG_ENUM; \
            \
            text_.remove_prefix(pref); \
            text_.remove_prefix(offset_skip()); \
        }

    if (false) {}
    #include "../lists/YaggArgTypes.h"
    else throw CYaggException("unrecognized arg type", text_);

    #undef YAGG_ARG_TYPE

    parse_list_end();

    return true;
}

bool CYaggParser::parse_entry_src(SYaggEntry* entry)
{
    assert(entry);

    if (text_.empty() || text_.front() == ']')
    {
        entry->src = YAGG_ARG_TYPE_NUL;
        return true;
    }

    parse_list_begin();

    size_t pref = offset_word();

    #define YAGG_ARG_TYPE(ARG_ENUM, ARG_CODE, ARG_NAME) \
        else if (!text_.compare(0u, sizeof(#ARG_NAME) - 1u, #ARG_NAME)) \
        { \
            entry->src = YAGG_ARG_TYPE_##ARG_ENUM; \
            \
            text_.remove_prefix(pref); \
            text_.remove_prefix(offset_skip()); \
        }

    if (false) {}
    #include "../lists/YaggArgTypes.h"
    else throw CYaggException("unrecognized arg type", text_);

    #undef YAGG_ARG_TYPE

    parse_list_end();

    return true;
}

bool CYaggParser::parse_opt_rex(SYaggData* data)
{
    assert(data);

    if (data->mask & SYaggData::PREF_REX)
        throw CYaggException("REX duplication", text_);
    else
        data->mask |= SYaggData::PREF_REX;

    char* num_end = nullptr;
    data->rex = std::strtoul(text_.data(), &num_end, 0);

    if (num_end == text_.data())
        throw CYaggException("expected immediate", text_);
    else if (!num_end)
        throw CYaggException("invalid immediate", text_);

    text_.remove_prefix(num_end - text_.data());
    text_.remove_prefix(offset_skip());

    return true;
}

bool CYaggParser::parse_opt_0fh(SYaggData* data)
{
    assert(data);

    if (data->mask & SYaggData::PREF_0FH)
        throw CYaggException("0FH duplication", text_);
    else
        data->mask |= SYaggData::PREF_0FH;

    return true;
}

bool CYaggParser::parse_opt_opc(SYaggData* data)
{
    assert(data);

    if (data->mask & SYaggData::BYTE_OPC)
        throw CYaggException("0FH duplication", text_);
    else
        data->mask |= SYaggData::BYTE_OPC;

    char* num_end = nullptr;
    data->opc = std::strtoul(text_.data(), &num_end, 0);

    if (num_end == text_.data())
        throw CYaggException("expected immediate", text_);
    else if (!num_end)
        throw CYaggException("invalid immediate", text_);

    text_.remove_prefix(num_end - text_.data());
    text_.remove_prefix(offset_skip());

    return true;
}

bool CYaggParser::parse_opt_ext(SYaggData* data)
{
    assert(data);

    if (data->mask & SYaggData::SUFF_EXT)
        throw CYaggException("EXT duplication", text_);
    else
        data->mask |= SYaggData::SUFF_EXT;

    char* num_end = nullptr;
    data->ext = std::strtoul(text_.data(), &num_end, 0);

    if (num_end == text_.data())
        throw CYaggException("expected immediate", text_);
    else if (!num_end)
        throw CYaggException("invalid immediate", text_);

    text_.remove_prefix(num_end - text_.data());
    text_.remove_prefix(offset_skip());

    return true;
}

bool CYaggParser::parse_opt_dst(SYaggData* data)
{
    assert(data);

    if (data->mask & SYaggData::DATA_DST)
        throw CYaggException("DST duplication", text_);
    else
        data->mask |= SYaggData::DATA_DST;

    size_t pref = offset_word();

    #define YAGG_TARGET(TGT_ENUM, TGT_NAME) \
        else if (!text_.compare(0u, sizeof(#TGT_NAME) - 1u, #TGT_NAME)) \
        { \
            data->dst = YAGG_TGT_##TGT_ENUM; \
            \
            text_.remove_prefix(pref); \
            text_.remove_prefix(offset_skip()); \
        }

    if (false) {}
    #include "../lists/YaggTargets.h"
    else throw CYaggException("unrecognized target", text_);

    #undef YAGG_TARGET

    return true;
}

bool CYaggParser::parse_opt_src(SYaggData* data)
{
    assert(data);

    if (data->mask & SYaggData::DATA_SRC)
        throw CYaggException("SRC duplication", text_);
    else
        data->mask |= SYaggData::DATA_SRC;

    size_t pref = offset_word();

    #define YAGG_TARGET(TGT_ENUM, TGT_NAME) \
        else if (!text_.compare(0u, sizeof(#TGT_NAME) - 1u, #TGT_NAME)) \
        { \
            data->src = YAGG_TGT_##TGT_ENUM; \
            \
            text_.remove_prefix(pref); \
            text_.remove_prefix(offset_skip()); \
        }

    if (false) {}
    #include "../lists/YaggTargets.h"
    else throw CYaggException("unrecognized target", text_);

    #undef YAGG_TARGET

    return true;
}

bool CYaggParser::parse_opt_jmp(SYaggData* data)
{
    assert(data);

    if (data->mask & SYaggData::FLAG_JMP)
        throw CYaggException("JMP duplication", text_);
    else
        data->mask |= SYaggData::FLAG_JMP;

    return true;
}

bool CYaggParser::parse_list_begin()
{
    if (text_.front() != '[')
        throw CYaggException("'[' expected", text_);

    text_.remove_prefix(1u);
    text_.remove_prefix(offset_skip());

    return true;
}

bool CYaggParser::parse_list_end()
{
    if (text_.front() != ']')
        throw CYaggException("']' expected", text_);

    text_.remove_prefix(1u);
    text_.remove_prefix(offset_skip());

    return true;
}

size_t CYaggParser::offset_skip() const noexcept
{
    size_t skip = text_.find_first_not_of(" \t\r\n");
    skip = std::min(skip, std::string_view::npos);

    return skip;
}

size_t CYaggParser::offset_word() const noexcept
{
    size_t skip = text_.find_first_of(" \t\r\n[]");
    skip = std::min(skip, std::string_view::npos);

    return skip;
}

void CYaggParser::dump(FILE* fout, size_t tab) const noexcept
{
    assert(fout);

    fprintf(fout, YAGG_TAB("SInstr::dump(%p)\n", tab), this);
    fprintf(fout, YAGG_TAB("{\n", tab));

    fprintf(fout, YAGG_TAB("text_ = { \n", tab + 4u));
    fprintf(fout, YAGG_TAB(".size = %zu, \n", tab + 8u), text_.size());
    fprintf(fout, YAGG_TAB(".data = %p   \n", tab + 8u), text_.data());
    fprintf(fout, YAGG_TAB("};// text_ \n", tab + 4u));

    fprintf(fout, YAGG_TAB("}\n", tab));
}

int test_CYaggParser(int argc, const char* argv[])
{
    constexpr size_t BUF_SIZE = 0x100;
    char text_buf[BUF_SIZE] = "";

    FILE* fin = stdin;
    fscanf(fin, "%255[^\r\n]", text_buf);

    std::string_view text{ text_buf };
    printf("received %.*s\n", static_cast<int>(text.size()), text.data());

    CYaggParser parser; 
    SYaggInstr instr = {};
    try 
    {
        parser.parse_instr(&instr, text);
    }
    catch (const CYaggException& exc)
    {
        const auto& where = exc.where();
        printf("error: %s\n", exc.what());
        printf("at %zu \"%*s...\"\n", text.size() - where.size(), 
                static_cast<int>(std::min(where.size(), 16ul)), where.data());

        return 0;
    }

    instr.dump(stdout);

    return 0;
}
