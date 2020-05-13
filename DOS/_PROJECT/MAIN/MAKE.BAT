@echo off

tasm hint.asm
tlink /t init.obj file.obj ring.obj hint.obj
