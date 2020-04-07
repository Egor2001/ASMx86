#!/bin/bash

GENFILE_C=aho_dfa.h
GENFILE_NASM=aho_dfa.asm
GENCMD=./bin/main

if [[ $# -lt 3 ]]; then
    echo 'invalid parameters count'
    exit 1
fi

${GENCMD} ${GENFILE_C} ${GENFILE_NASM} $@
make asm
