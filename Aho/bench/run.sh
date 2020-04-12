#!/bin/bash

GENSTRCMD=./pattern_gen.py
SRCSTR=src_str.h

GENFILE_C=src_c/aho_dfa.h
GENFILE_NASM=src_asm/aho_dfa.asm
GENCMD=../bin/main

if [[ $# -lt 3 ]]; then
    echo 'invalid parameters count'
    echo "USAGE: $0 DICT LEN PATTERN [PATTERN ...]"
    exit 1
fi

DICT=$1
LEN=$2

shift 2

echo 'const char* SRC_STR = ' > ${SRCSTR}
${GENSTRCMD} ${DICT} ${LEN} >> ${SRCSTR}
echo ';//SRC_STR' >> ${SRCSTR}

${GENCMD} ${GENFILE_C} ${GENFILE_NASM} $@
(make clean) && make
