#!/bin/sh
command -v ffplay > /dev/null || {
	echo >&2 please install ffplay.
	exit 1
}
[ "$#" -ne 2 ] || {
	echo >&2 expected 1 arg.
	exit 1
}
P="$(dirname $(which "$0"))"
export PATH="$P:$P/..:$P/build:$P/../build:$PATH"
awexm_main play "$1" | ffplay -f s16le -ar 48000 -ch_layout stereo -i pipe:0
