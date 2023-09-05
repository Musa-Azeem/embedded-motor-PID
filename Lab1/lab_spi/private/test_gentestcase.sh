#!/bin/sh

# Copyright 2021 Jason Bakos, Philip Conrad, Charles Daniels
#
# Part of the University of South Carolina CSCE317 course materials. Used by
# instructors for test case generators. Do not redistribute.

# This program exhaustively validates all possible one-exchange SPI
# transactions that gentestcase.py could potentially validate.

cd "$(dirname "$0")"
. ../.venv/bin/activate

for CPOL in 0 1 ; do
	for CPHA in 0 1 ; do
# for CPOL in 1 ; do
#         for CPHA in 0 ; do
		for val in $(seq 0 255) ; do
			printf "CPOL=%d CPHA=%d val=%d " $CPOL $CPHA $val

			res=$(python3 ./gentestcase.py --input /dev/stdout --output /dev/null --cpol $CPOL --cpha $CPHA --oneoff $val 0 | \
				sh ../grade.sh --text2vcd /dev/stdin /dev/stdout | \
				sigrok-cli -P spi -I vcd -i /dev/stdin -A spi=mosi-data -P spi:cpha=$CPHA:cpol=$CPOL:bitorder=msb-first:miso=miso:mosi=mosi:cs=ss:clk=sclk 2>&1 | \
				grep "spi-2" | cut -d: -f2 | tr -d ' ')

			res_dec=$(printf '%d\n' 0x$res)

			if [ $res_dec != $val ] ; then
				printf "ERROR: master2slave res=%d " $res_dec
			fi

			res=$(python3 ./gentestcase.py --input /dev/stdout --output /dev/null --cpol $CPOL --cpha $CPHA --oneoff 0 $val | \
				sh ../grade.sh --text2vcd /dev/stdin /dev/stdout | \
				sigrok-cli -P spi -I vcd -i /dev/stdin -A spi=mosi-data -P spi:cpha=$CPHA:cpol=$CPOL:bitorder=msb-first:miso=miso:mosi=mosi:cs=ss:clk=sclk 2>&1 | \
				grep "spi-1" | cut -d: -f2 | tr -d ' ')

			res_dec=$(printf '%d\n' 0x$res)

			if [ $res_dec != $val ] ; then
				printf "ERROR: slave2master res=%d " $res_dec
			fi

			echo ""

		done
	done
done
