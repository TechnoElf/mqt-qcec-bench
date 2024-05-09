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
            << " " << std::setw(20) << "Diff"
            << " " << std::setw(20) << "Proportional"
            << "\n";
  for (const BenchmarkResults& bench : rawResults) {
    std::cout << std::setw(20) << bench.name << " " << std::setw(20)
              << bench.avgDiffRunTime << " " << std::setw(20)
              << bench.avgPropRunTime << "\n";
  }
}

void TrialResults::save(std::fstream& out, const std::string& sep) const {
  out << "name" << sep << "diff" << sep << "proportional\n";
  for (const BenchmarkResults& bench : rawResults) {
    std::string avgDiffRunTime = std::format("{}", bench.avgDiffRunTime);
    avgDiffRunTime.pop_back();
    std::string avgPropRunTime = std::format("{}", bench.avgPropRunTime);
    avgPropRunTime.pop_back();
    out << bench.name << sep << avgDiffRunTime << sep << avgPropRunTime << "\n";
  }
}

TrialResults runTrial() {
  std::vector<BenchmarkResults> rawResults;

  rawResults.push_back(runBenchmark(
      "grover ng0 / ng3",
      "./circuits/grover-noancilla_nativegates_ibm_qiskit_opt0_8.qasm",
      "./circuits/grover-noancilla_nativegates_ibm_qiskit_opt3_8.qasm", 1, true,
      true));

  rawResults.push_back(runBenchmark(
      "grover mp0 / mp3",
      "./circuits/grover-noancilla_mapped_ibm_washington_qiskit_opt0_8.qasm",
      "./circuits/grover-noancilla_mapped_ibm_washington_qiskit_opt3_8.qasm", 1,
      true, true));

  rawResults.push_back(runBenchmark(
      "grover ind / ng0", "./circuits/grover-noancilla_indep_qiskit_8.qasm",
      "./circuits/grover-noancilla_nativegates_ibm_qiskit_opt0_8.qasm", 1, true,
      true));

  rawResults.push_back(runBenchmark(
      "grover ind / ng3", "./circuits/grover-noancilla_indep_qiskit_8.qasm",
      "./circuits/grover-noancilla_nativegates_ibm_qiskit_opt3_8.qasm", 1, true,
      true));

  rawResults.push_back(runBenchmark(
      "grover ng0 / mp0",
      "./circuits/grover-noancilla_nativegates_ibm_qiskit_opt0_8.qasm",
      "./circuits/grover-noancilla_mapped_ibm_washington_qiskit_opt3_8.qasm", 1,
      true, true));

  rawResults.push_back(runBenchmark(
      "grover ind / mp0", "./circuits/grover-noancilla_indep_qiskit_8.qasm",
      "./circuits/grover-noancilla_mapped_ibm_washington_qiskit_opt0_8.qasm", 1,
      true, true));

  rawResults.push_back(runBenchmark(
      "dj ng0 / ng3", "./circuits/dj_nativegates_ibm_qiskit_opt0_8.qasm",
      "./circuits/dj_nativegates_ibm_qiskit_opt3_8.qasm", 1, true, true));

  rawResults.push_back(
      runBenchmark("shor_15_4 ng0 / ng2",
                   "./circuits/shor_15_4_nativegates_ibm_qiskit_opt0_18.qasm",
                   "./circuits/shor_15_4_nativegates_ibm_qiskit_opt2_18.qasm",
                   1, true, false));

  return TrialResults{.rawResults = rawResults};
}