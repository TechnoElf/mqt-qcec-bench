/*
 * Created by janis on 2024-04-28
 */

#include "bench.h"

#include "config.h"
#include "instance.h"

#include <iostream>

void BenchmarkResults::print() const {
  std::cout << "BenchmarkResults:\n";
  std::cout << std::left << std::setw(20) << "Benchmark" << " " << std::setw(20)
            << "Run Time" << " " << std::setw(20) << "Equivalent" << "\n";
  for (const InstanceResults& bench : rawResults) {
    std::cout << std::setw(20) << bench.displayName << " " << std::setw(20)
              << bench.runTime << " " << std::setw(20)
              << (bench.equivalent ? "true" : "false") << "\n";
  }
}

void BenchmarkResults::save(std::fstream& out, const std::string& sep) const {
  out << "name" << sep << "runCount" << sep << "timeOut" << sep << "equivalent"
      << sep << "deterministic" << sep << "numQubits1" << sep << "numQubits2"
      << sep << "numGates1" << sep << "numGates2" << sep
      << "diffEquivalenceCount" << sep << "initTime" << sep << "runTime" << sep
      << "maxActiveNodes\n";
  for (const InstanceResults& bench : rawResults) {
    std::string runTime = std::format("{}", bench.runTime);
    runTime.pop_back();
    std::string initTime = std::format("{}", bench.initTime);
    initTime.pop_back();
    std::string timeOut = std::format("{}", bench.timeOut);
    timeOut.pop_back();
    out << bench.displayName << sep << bench.runCount << sep << timeOut << sep
        << bench.equivalent << sep << bench.deterministic << sep
        << bench.numQubits1 << sep << bench.numQubits2 << sep << bench.numGates1
        << sep << bench.numGates2 << sep << bench.diffEquivalenceCount << sep
        << initTime << sep << runTime << sep << bench.maxActiveNodes << "\n";
  }
}

void Benchmark::add(const Instance&& instance) {
  this->instances.push_back(instance);
}

BenchmarkResults Benchmark::run(const Configuration& conf) const {
  std::vector<InstanceResults> rawResults;

  for (const auto& inst : this->instances) {
    const InstanceResults res = inst.run(conf);
    rawResults.push_back(res);
  }

  return BenchmarkResults{.rawResults = rawResults};
}
