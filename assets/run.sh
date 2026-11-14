#!/bin/sh
PATH="./build:../build:.:$PATH"
awexm_main play "$1" | ffplay -nodisp -f s16le -ar 48000 -ch_layout stereo -i pipe:0
