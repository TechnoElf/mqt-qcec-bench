#!/bin/sh

./cmake-build-release/src/mqt-qcec-bench -b configs/benchmark_big.toml -c configs/checker_proportional.toml -o results_prop.csv
./cmake-build-release/src/mqt-qcec-bench -b configs/benchmark_big.toml -c configs/checker_diff.toml -o results_diff.csv
