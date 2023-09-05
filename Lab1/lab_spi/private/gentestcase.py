# Copyright 2021 Jason Bakos, Philip Conrad, Charles Daniels
#
# Part of the University of South Carolina CSCE317 course materials. Used by
# instructors for test case generators. Do not redistribute.

# example of validation using sigrok-cli
#
#
# python3 private/gentestcase.py -i /dev/stdout -o /dev/null  | sh grade.sh --text2vcd /dev/stdin cpol0cpha0.vcd
# sigrok-cli -P spi -I vcd -i cpol0cpha0.vcd -A spi=mosi-data -P spi:cpha=0:cpol=0:bitorder=msb-first:miso=miso:mosi=mosi:cs=ss:clk=sclk


import pathlib
import sys
import os
import argparse
import random
import copy

# find where the python utils directory is and add it to path so we can import
# it
script_dir = os.path.split(os.path.abspath(sys.argv[0]))[0]
parent_dir = os.path.split(script_dir)[0]
python_utils_dir = os.path.join(parent_dir, "utils", "python_utils")
sys.path.append(python_utils_dir)
from waves import Waves

parser = argparse.ArgumentParser("test case generator for CSCE317 SPI decoding lab")

parser.add_argument("--length", "-l", type=int, default=64, help="Number of SPI transactions (default: 64)")

parser.add_argument("--max_stream_length", "-s", type=int, default=32, help="Maximum length of an SPI stream. (default: 32)")

parser.add_argument("--stream_probability", "-p", type=float, default=0, help="Probability that a transaction will be streaming in 0.0...1.0. (default: 0.0)")

parser.add_argument("--input", "-i", type=pathlib.Path, required=True, help="Path to which the test case input should be written.")

parser.add_argument("--output", "-o", type=pathlib.Path, required=True, help="Path to which the test case output should be written.")

parser.add_argument("--cpol", "-c", type=int, default=0, choices=[1,0], help="CPOL value for SPI bpus (default:0)")

parser.add_argument("--cpha", "-a", type=int, default=0, choices=[1,0], help="CPHA value for SPI bpus (default:0)")

parser.add_argument("--max_sep", "-S", type=int, default=1000, help="Maximum separation between transactions in time steps. (default: 100)")

parser.add_argument("--resolution", "-r", type=int, default=100, help="Time steps per clock cycle. (default: 100)")

parser.add_argument("--oneoff", "-O", nargs=2, type=int, default=None, help="Rather than generating a test case normally, just generate one single exchange and write the output. This is used for testing that the test-case generator is correct.", metavar=("MASTER2SLAVE", "SLAVE2MASTER"))

args = parser.parse_args()

w = Waves()
w.sizes = {
        "sclk": 1,
        "mosi": 1,
        "miso": 1,
        "ss": 1,
        "cpol": 1,
        "cpha": 1,
}

w.data.append((0.0, {
        "sclk": args.cpol,
        "mosi": 0,
        "miso": 0,
        "ss": 1,
        "cpol": args.cpol,
        "cpha": args.cpha,
}))

w.data.append((args.max_sep, {
        "sclk": args.cpol,
        "mosi": 0,
        "miso": 0,
        "ss": 1,
        "cpol": args.cpol,
        "cpha": args.cpha,
}))

ofile = open(args.output, "w")
ifile = open(args.input, "w")

def exchange(mts, stm, desel):
    # Given the settings of args.cpol and args.cpha, write the given data bytes
    # to miso and mosi, where mts is the "master-to-slave" data, and stm is
    # the "slave-to-master" data.
    #
    # Data is transmitted MSB-first.
    #
    # If desel is True, ss is deasserted and the clock advanced after the
    # exchange.

    miso = 0
    mosi = 0

    # start time
    st = w.data[-1][0]

    # idle value
    sclk = args.cpol

    t = st

    for i in reversed(range(8)):
        mask = 1 << i

        sclk = args.cpol

        t = t + args.resolution / 2
        if args.cpha == 0:
            mosi = (mts & mask) >> i
            miso = (stm & mask) >> i

        w.data.append((t, {
                "sclk": sclk,
                "mosi": mosi,
                "miso": miso,
                "ss": 0,
                "cpol": args.cpol,
                "cpha": args.cpha,
        }))

        sclk = 1 - args.cpol

        if args.cpha == 1:
            mosi = (mts & mask) >> i
            miso = (stm & mask) >> i

        t = t + args.resolution / 2

        w.data.append((t, {
                "sclk": sclk,
                "mosi": mosi,
                "miso": miso,
                "ss": 0,
                "cpol": args.cpol,
                "cpha": args.cpha,
        }))

    if desel:
        t = t + args.resolution / 2
        sclk = args.cpol
        w.data.append((t, {
                "sclk": sclk,
                "mosi": mosi,
                "miso": miso,
                "ss": 0,
                "cpol": args.cpol,
                "cpha": args.cpha,
        }))

        t = t + args.resolution / 2
        w.data.append((t, {
                "sclk": sclk,
                "mosi": 0,
                "miso": 0,
                "ss": 1,
                "cpol": args.cpol,
                "cpha": args.cpha,
        }))

        t = t + args.resolution / 2
        w.data.append((t, {
                "sclk": sclk,
                "mosi": 0,
                "miso": 0,
                "ss": 1,
                "cpol": args.cpol,
                "cpha": args.cpha,
        }))

if args.oneoff is not None:
    exchange(args.oneoff[0], args.oneoff[1], True)

else:
    # guarantee at least one stream if stream_probability > 0
    had_stream = False
    for transno in range(args.length):
        if (random.random() < args.stream_probability) or ((args.stream_probability > 0.0) and (not had_stream)):
            # stream transaction
            addr = random.randrange(0, 64)
            write = random.choice([0,1])
            l = random.randrange(1, args.max_stream_length)
            data = [random.randrange(0, 256) for x in range(l)]
            command = (addr << 2) | (write << 1)| 1
            exchange(command, 0, False)

            #  data length
            exchange(len(data), 0, False)

            for i in range(len(data)):
                done=False
                if i >= len(data)-1:
                    done=True
                d = data[i]
                if write == 0:
                    exchange(0, d, done)
                else:
                    exchange(d, 0, done)

            if write == 0:
                ofile.write("RD STREAM ")
            else:
                ofile.write("WR STREAM ")

            ofile.write("{:02x} {}\n".format(addr, " ".join(["{:02x}".format(d) for d in data ])))

            had_stream = True


        else:
            # single mode transaction
            addr = random.randrange(0, 64)
            write = random.choice([0,1])
            data = random.randrange(0, 256)
            command = (addr << 2) | (write << 1)| 0
            exchange(command, 0, False)

            if write == 0:
                exchange(0, data, True)
            else:
                exchange(data, 0, True)

            if write == 0:
                ofile.write("RD ")
            else:
                ofile.write("WR ")

            ofile.write("{:02x} {:02x}\n".format(addr, data))

    sep = random.randrange(10, args.max_sep)
    w.data.append((w.data[-1][0] + sep, copy.deepcopy(w.data[-1][1])))

# write out the generated waves
ifile.write(w.toText())

ofile.close()
ifile.close()
