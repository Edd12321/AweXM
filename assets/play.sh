#!/bin/sh
die () {
	echo >&2 "$@"
	exit 1
}

command -v ffmpeg > /dev/null || die please install ffmpeg.
command -v ffplay > /dev/null || die please install ffplay.
[ "$#" -eq 1 ] || die expected module filename.
P="$(dirname "$(realpath "$(which "$0")")")"
export PATH="$P:$P/..:$P/build:$P/../build:$PATH"
[ -f "$1" ] || die "$1": no such file or directory.

awexm_main dump "$1" > /dev/null && {
	awexm_main nice "$1" | {
		read n
		T="$((n + 2))"
		i=0
		str=""
		while [ "$i" -lt "$n" ]; do
			str="$str|c$i=c$((i + 2))"
			i=$((i + 1))
		done
		ffmpeg 2>/dev/null \
			-f s16le -ar 48000 -ch_layout "${T} channels" -i pipe:0 \
			-filter_complex "
				[0:a]pan=stereo|c0=c0|c1=c1[out1];
				[0:a]pan=${n}C${str},showwaves=mode=p2p:split_channels=1:r=120:s=1200x480:colors=lime[out0]
			" \
			-map "[out0]" -map "[out1]" \
			-c:v rawvideo -pix_fmt yuv420p \
			-c:a pcm_s16le -f nut - | ffplay >/dev/null 2>/dev/null -autoexit -i pipe:0 -x 600 -y 240
	}
}

