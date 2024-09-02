#!/usr/bin/env python3

import sys
import csv
from dataclasses import dataclass

import matplotlib.pyplot as plt
import numpy as np


@dataclass
class Instance:
    name: str
    run_count: int
    time_out: float
    equivalent: bool
    deterministic: bool
    finished: bool
    num_qubits_1: int
    num_qubits_2: int
    num_gates_1: int
    num_gates_2: int
    diff_equivalence_count: int
    init_time_mean: float
    init_time_variance: float
    run_time_mean: float
    run_time_variance: float
    max_active_nodes: int

    def __init__(self, row: dict):
        self.name = row["name"]
        self.run_count = int(row["runCount"])
        self.time_out = int(row["timeOut"])
        self.equivalent = bool(row["equivalent"])
        self.deterministic = bool(row["deterministic"])
        self.finished = bool(row["finished"])
        self.num_qubits_1 = int(row["numQubits1"])
        self.num_qubits_2 = int(row["numQubits2"])
        self.num_gates_1 = int(row["numGates1"])
        self.num_gates_2 = int(row["numGates2"])
        self.diff_equivalence_count = int(row["diffEquivalenceCount"])
        self.init_time_mean = float(row["initTimeMean"])
        self.init_time_variance = float(row["initTimeVariance"])
        self.run_time_mean = float(row["runTimeMean"])
        self.run_time_variance = float(row["runTimeVariance"])
        self.max_active_nodes = int(row["maxActiveNodes"])


def main(argc, argv):
    data_diff = []
    with open("results_diff.csv", "r") as f:
        r = csv.DictReader(f)
        data_diff = [Instance(row) for row in r][1:]

    data_prop = []
    with open("results_prop.csv", "r") as f:
        r = csv.DictReader(f)
        data_prop = [Instance(row) for row in r][1:]

    data = []
    for i in range(len(data_diff)):
        data += [(data_diff[i], data_prop[i])]

    data = [r for r in data if r[0].finished and r[1].finished]

    run_time_improvement = [-(r[0].run_time_mean / r[1].run_time_mean * 100 - 100) for r in data]
    max_active_nodes_improvement = [0 if r[1].max_active_nodes == 0 else -(r[0].max_active_nodes / r[1].max_active_nodes * 100 - 100) for r in data]

    fig, ax = plt.subplots()
    ax.hist(run_time_improvement, bins=10)

    fig, ax = plt.subplots()
    ax.bar([r[0].name for r in data], [r[0].run_time_mean for r in data], -0.35, align='edge', color = 'red')
    ax.bar([r[1].name for r in data], [r[1].run_time_mean for r in data], 0.35, align='edge', color = 'blue')
    ax.set_title("Run Time Dependent on Benchmark")

    fig, ax = plt.subplots()
    ax.bar([r[0].name for r in data], run_time_improvement)
    ax.set_title("Run Time Improvement Dependent on Benchmark")

    fig, ax = plt.subplots()
    ax.bar([r[0].name for r in data], max_active_nodes_improvement)
    ax.set_title("Maximum Active DD-Nodes Improvement Dependent on Benchmark")

    fig, ax = plt.subplots()
    ax.scatter(max_active_nodes_improvement, run_time_improvement)
    ax.set_title("Run Time Improvement Dependent on Maximum Active DD-Nodes Improvement")

    fig, ax = plt.subplots()
    ax.scatter([r[0].diff_equivalence_count / (r[0].num_gates_1 + r[0].num_gates_2) for r in data], run_time_improvement)
    ax.grid(True)
    ax.set_title("Run Time Improvement Dependent on Gate Equivalence Rate")

    fig, ax = plt.subplots()
    ax.scatter([abs(r[0].num_gates_1 - r[0].num_gates_2) for r in data], run_time_improvement)
    ax.grid(True)
    ax.set_title("Run Time Improvement Dependent on Gate Count Difference")

    fig, ax = plt.subplots()
    ax.scatter([r[0].num_gates_1 + r[0].num_gates_2 for r in data], run_time_improvement)
    ax.grid(True)
    ax.set_title("Run Time Improvement Dependent on Total Gate Count")

    fig, ax = plt.subplots()
    ax.scatter([r[0].num_qubits_1 + r[0].num_qubits_2 for r in data], run_time_improvement)
    ax.grid(True)
    ax.set_title("Run Time Improvement Dependent on Total Qubit Count")

    data_filtered = data
    #data_filtered = [r for r in data_filtered if abs(r[0].num_gates_1 - r[0].num_gates_2) < 100]
    data_filtered = [r for r in data_filtered if r[0].diff_equivalence_count / (r[0].num_gates_1 + r[0].num_gates_2) > 0.4]
    run_time_improvement_filtered = [-(r[0].run_time_mean / r[1].run_time_mean * 100 - 100) for r in data_filtered]

    fig, ax = plt.subplots()
    ax.bar([r[0].name for r in data_filtered], run_time_improvement_filtered)
    ax.set_title("Run Time Improvement Dependent on Benchmark with Filter")

    fig, ax = plt.subplots()
    ax.scatter([r[0].diff_equivalence_count / (r[0].num_gates_1 + r[0].num_gates_2) for r in data_filtered], run_time_improvement_filtered)
    ax.grid(True)
    ax.set_title("Run Time Improvement Dependent on Gate Equivalence Rate with Filter")

    plt.show()

    return 0


if __name__ == "__main__":
    try:
        exit(main(len(sys.argv), sys.argv))
    except KeyboardInterrupt:
        exit(1)
