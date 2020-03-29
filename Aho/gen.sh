#!/bin/bash

GENFILE=aho_dfa.asm
GENCMD=./bin/main

if [[ $# -lt 2 ]]; then
    echo 'invalid parameters count'
    exit 1
fi

${GENCMD} ${GENFILE} $@
make asm
