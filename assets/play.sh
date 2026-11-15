#!/bin/sh
die () {
	echo >&2 "$@"
	exit 1
}

command -v ffplay > /dev/null || die please install ffplay.
[ "$#" -eq 1 ] || die expected module filename.
P="$(dirname "$(realpath "$(which "$0")")")"
export PATH="$P:$P/..:$P/build:$P/../build:$PATH"
[ -f "$1" ] || die "$1": no such file or directory.
awexm_main dump "$1" > /dev/null && {
	awexm_main play "$1" | ffplay \
		-f s16le -ar 48000 -ch_layout stereo -i pipe:0 -autoexit
}
