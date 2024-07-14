#!/usr/bin/env python3

import sys
import os

def main(argc, argv):
    if (argc < 4):
        print("Usage: get_circuits.py <file prefix> <benchmark> <size>")
        return 1

    file_prefix = argv[1]
    benchmark = argv[2]
    try:
        size = int(argv[3])
    except ValueError:
        print("<size> should be an integer")
        return 1

    get_circuits(file_prefix, benchmark, size)

    return 0


configurations = [
    ("_ind", "indep", 0, ""),
    ("_ng0", "nativegates", 0, "ibm"),
    ("_ng3", "nativegates", 3, "ibm"),
    ("_mp0", "mapped", 0, "ibm_washington"),
    ("_mp3", "mapped", 3, "ibm_washington")
]


def get_circuits(file_prefix, benchmark, size):
    exists = True
    for config in configurations:
        if not os.path.isfile(file_prefix + config[0] + ".qasm"):
            exists = False
            break

    if not exists:
        from mqt.bench import CompilerSettings, QiskitSettings, get_benchmark
        from qiskit.qasm2 import dump as qasm2_dump

        for config in configurations:
            settings = CompilerSettings(qiskit=QiskitSettings(optimization_level=config[2]))

            if config[1] == "indep":
                circuit = get_benchmark(benchmark_name=benchmark, level=config[1], circuit_size=size, compiler="qiskit")
                with open(file_prefix + config[0] + ".qasm", "w+") as file:
                    qasm2_dump(circuit, file)

            if config[1] == "nativegates":
                circuit = get_benchmark(benchmark_name=benchmark, level=config[1], circuit_size=size, compiler="qiskit", compiler_settings=settings, provider_name=config[3])
                with open(file_prefix + config[0] + ".qasm", "w+") as file:
                    qasm2_dump(circuit, file)

            if config[1] == "mapped":
                circuit = get_benchmark(benchmark_name=benchmark, level=config[1], circuit_size=size, compiler="qiskit", compiler_settings=settings, device_name=config[3])
                with open(file_prefix + config[0] + ".qasm", "w+") as file:
                    qasm2_dump(circuit, file)


if __name__ == "__main__":
    try:
        exit(main(len(sys.argv), sys.argv))
    except KeyboardInterrupt:
        exit(1)
