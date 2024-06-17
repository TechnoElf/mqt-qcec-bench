/*
 * Created by janis on 2024-05-09
 */

#include "trial.h"

#include <format>
#include <iomanip>
#include <iostream>

void TrialResults::print() const {
  std::cout << "TrialResults:\n";
  std::cout << std::left << std::setw(20) << "Benchmark"
            << " " << std::setw(20) << "A"
            << " " << std::setw(20) << "B"
            << "\n";
  for (const BenchmarkResults& bench : rawResults) {
    std::cout << std::setw(20) << bench.name << " " << std::setw(20)
              << bench.avgARunTime << " " << std::setw(20) << bench.avgBRunTime
              << "\n";
  }
}

void TrialResults::save(std::fstream& out, const std::string& sep) const {
  out << "name" << sep << "time a" << sep << "time b" << sep << "nodes a" << sep << "nodes b\n";
  for (const BenchmarkResults& bench : rawResults) {
    std::string avgARunTime = std::format("{}", bench.avgARunTime);
    avgARunTime.pop_back();
    std::string avgBRunTime = std::format("{}", bench.avgBRunTime);
    avgBRunTime.pop_back();
    out << bench.name << sep << avgARunTime << sep << avgBRunTime << sep << bench.peakAUniqueTableSize << sep << bench.peakBUniqueTableSize << "\n";
  }
}

TrialResults runTrial(const Configuration& confA, const Configuration& confB) {
  std::vector<BenchmarkResults> rawResults;

  rawResults.push_back(
      runBenchmark("groundstate_small ng0 / ng3",
             "./circuits/groundstate_small_nativegates_ibm_qiskit_opt0_4.qasm",
             "./circuits/groundstate_small_nativegates_ibm_qiskit_opt3_4.qasm",
             confA, confB, 1, true, true));

  rawResults.push_back(
      runBenchmark("groundstate_small ind / mp3",
                   "./circuits/groundstate_small_indep_qiskit_4.qasm",
                   "./circuits/groundstate_small_mapped_ibm_washington_qiskit_opt3_4.qasm",
                   confA, confB, 1, true, true));

  rawResults.push_back(
      runBenchmark("tsp_4 ng0 / ng3",
                   "./circuits/tsp_nativegates_ibm_qiskit_opt0_4.qasm",
                   "./circuits/tsp_nativegates_ibm_qiskit_opt3_4.qasm",
                   confA, confB, 1, true, true));

  rawResults.push_back(
      runBenchmark("tsp_4 ind / mp3",
                   "./circuits/tsp_indep_qiskit_4.qasm",
                   "./circuits/tsp_mapped_ibm_washington_qiskit_opt3_4.qasm",
                   confA, confB, 1, true, true));

  rawResults.push_back(
      runBenchmark("tsp_9 ng0 / ng3",
                   "./circuits/tsp_nativegates_ibm_qiskit_opt0_9.qasm",
                   "./circuits/tsp_nativegates_ibm_qiskit_opt3_9.qasm",
                   confA, confB, 1, true, true));

  rawResults.push_back(
      runBenchmark("tsp_16 ng0 / ng3",
                   "./circuits/tsp_nativegates_ibm_qiskit_opt0_16.qasm",
                   "./circuits/tsp_nativegates_ibm_qiskit_opt3_16.qasm",
                   confA, confB, 1, true, true));

  rawResults.push_back(runBenchmark(
      "grover ng0 / ng3",
      "./circuits/grover-noancilla_nativegates_ibm_qiskit_opt0_8.qasm",
      "./circuits/grover-noancilla_nativegates_ibm_qiskit_opt3_8.qasm", confA,
      confB, 1, true, true));

  rawResults.push_back(runBenchmark(
      "grover mp0 / mp3",
      "./circuits/grover-noancilla_mapped_ibm_washington_qiskit_opt0_8.qasm",
      "./circuits/grover-noancilla_mapped_ibm_washington_qiskit_opt3_8.qasm",
      confA, confB, 1, true, true));

  rawResults.push_back(runBenchmark(
      "grover ind / ng0", "./circuits/grover-noancilla_indep_qiskit_8.qasm",
      "./circuits/grover-noancilla_nativegates_ibm_qiskit_opt0_8.qasm", confA,
      confB, 1, true, true));

  rawResults.push_back(runBenchmark(
      "grover ind / ng3", "./circuits/grover-noancilla_indep_qiskit_8.qasm",
      "./circuits/grover-noancilla_nativegates_ibm_qiskit_opt3_8.qasm", confA,
      confB, 1, true, true));

  rawResults.push_back(runBenchmark(
      "grover ng0 / mp0",
      "./circuits/grover-noancilla_nativegates_ibm_qiskit_opt0_8.qasm",
      "./circuits/grover-noancilla_mapped_ibm_washington_qiskit_opt3_8.qasm",
      confA, confB, 1, true, true));

  rawResults.push_back(runBenchmark(
      "grover ind / mp0", "./circuits/grover-noancilla_indep_qiskit_8.qasm",
      "./circuits/grover-noancilla_mapped_ibm_washington_qiskit_opt0_8.qasm",
      confA, confB, 1, true, true));

  rawResults.push_back(runBenchmark(
      "dj ng0 / ng3", "./circuits/dj_nativegates_ibm_qiskit_opt0_8.qasm",
      "./circuits/dj_nativegates_ibm_qiskit_opt3_8.qasm", confA, confB, 1,
      true, true));

  rawResults.push_back(
      runBenchmark("shor_15_4 ng0 / ng2",
                   "./circuits/shor_15_4_nativegates_ibm_qiskit_opt0_18.qasm",
                   "./circuits/shor_15_4_nativegates_ibm_qiskit_opt2_18.qasm",
                   confA, confB, 1, true, false));

  return TrialResults{.rawResults = rawResults};
}