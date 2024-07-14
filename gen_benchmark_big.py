#!/usr/bin/env python3

import sys

def main(argc, argv):
    kinds = ["portfolioqaoa", "portfoliovqe", "qnn", "qft", "dj"]
    levels = [("ind_ng0", "IndNg0"), ("ind_ng3", "IndNg3"), ("ind_mp0", "IndMp0"), ("ind_mp3", "IndMp3"), ("ng0_ng3", "Ng0Ng3"), ("ng0_mp0", "Ng0Mp0"), ("ng0_mp3", "Ng0Mp3"), ("ng3_mp0", "Ng3Mp0"), ("ng3_mp3", "Ng3Mp3"), ("mp0_mp3", "Mp0Mp3")]

    with open("configs/benchmark_big.toml", "w+") as f:
        for kind in kinds:
            for level in levels:
                f.write(f"[{kind}_04_{level[0]}]\n")
                f.write(f"benchmark = \"{kind}\"\n")
                f.write(f"kind = \"{level[1]}\"\n")
                f.write(f"size = 4\n")
                f.write(f"\n")

                f.write(f"[{kind}_08_{level[0]}]\n")
                f.write(f"benchmark = \"{kind}\"\n")
                f.write(f"kind = \"{level[1]}\"\n")
                f.write(f"size = 8\n")
                f.write(f"\n")

                f.write(f"[{kind}_16_{level[0]}]\n")
                f.write(f"benchmark = \"{kind}\"\n")
                f.write(f"kind = \"{level[1]}\"\n")
                f.write(f"size = 16\n")
                f.write(f"\n")


    return 0


if __name__ == "__main__":
    try:
        exit(main(len(sys.argv), sys.argv))
    except KeyboardInterrupt:
        exit(1)
