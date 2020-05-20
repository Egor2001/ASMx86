#ifndef DISASM_CELCDISASM
#define DISASM_CELCDISASM

#include <cstdlib>
#include <cstdint>
#include <cstring>
#include <cassert>

#include <vector>

#include "../elc_spec/SMirkElcInstruction.h"
#include "../elc_spec/EMirkElcRegister.h"
#include "../elc_spec/UMirkElcWord.h"

class CElcDisasm
{
public:
    struct SElcData
    {
        SMirkElcInstruction instr;
        const UMirkElcWord* addr;
        size_t len;
    };

    CElcDisasm(size_t word_cnt, const UMirkElcWord* text_buf);

    CElcDisasm             (const CElcDisasm&) = delete;
    CElcDisasm& operator = (const CElcDisasm&) = delete;
    CElcDisasm             (CElcDisasm&&) = default;
    CElcDisasm& operator = (CElcDisasm&&) = default;

    SElcData decode_next();

    size_t word_cnt() const noexcept { return word_cnt_; }
    const UMirkElcWord* text_buf() const noexcept { return text_buf_; }

protected:
    //decode full instruction
    [[nodiscard]]
    int decode_instr(SMirkElcInstruction* elc_instr, 
                     size_t* instr_len) noexcept;
    //decode command info
    [[nodiscard]]
    int decode_cmd(EMirkElcCommand* elc_cmd) noexcept;

    //decode argtype info
    [[nodiscard]]
    int decode_arg(EMirkElcArgType* elc_dst, 
                   EMirkElcArgType* elc_src,
                   size_t* dst_len, size_t* src_len) noexcept;

private:
    size_t word_idx_;

    size_t word_cnt_;
    const UMirkElcWord* text_buf_;
};

#endif //DISASM_CELCDISASM
