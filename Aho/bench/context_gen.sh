#!/bin/bash

GENSTRCMD=./pattern_gen.py
SRCSTR=src_str.h

GENFILE_C=src_c/aho_dfa.h
GENFILE_NASM=src_asm/aho_dfa.asm
GENCMD=../bin/main

#if [[ $# -lt 3 ]]; then
#    echo 'invalid parameters count'
#    exit 1
#fi

echo 'const char* SRC_STR = ' > ${SRCSTR}
${GENSTRCMD} abc 1024 >> ${SRCSTR}
echo ';//SRC_STR' >> ${SRCSTR}

${GENCMD} ${GENFILE_C} ${GENFILE_NASM} $@
(make clean) && make
