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
              << bench.runTimeMean << " " << std::setw(20)
              << (bench.equivalent ? "true" : "false") << "\n";
  }
}

void BenchmarkResults::save(std::fstream& out, const std::string& sep) const {
  out << "name" << sep << "runCount" << sep << "timeOut" << sep << "equivalent"
      << sep << "deterministic" << sep << "finished" << sep << "numQubits1"
      << sep << "numQubits2" << sep << "numGates1" << sep << "numGates2" << sep
      << "diffEquivalenceCount" << sep << "initTimeMean" << sep
      << "initTimeVariance" << sep << "runTimeMean" << sep << "runTimeVariance"
      << sep << "maxActiveNodes\n";
  for (const InstanceResults& bench : rawResults) {
    std::string equivalent    = bench.equivalent ? "true" : "false";
    std::string deterministic = bench.deterministic ? "true" : "false";
    std::string finished =
        (bench.runTimeMean != bench.timeOut.count()) ? "true" : "false";
    out << bench.displayName << sep << bench.runCount << sep
        << bench.timeOut.count() << sep << equivalent << sep << deterministic
        << sep << finished << sep << bench.numQubits1 << sep << bench.numQubits2
        << sep << bench.numGates1 << sep << bench.numGates2 << sep
        << bench.diffEquivalenceCount << sep << bench.initTimeMean << sep
        << bench.initTimeVariance << sep << bench.runTimeMean << sep
        << bench.runTimeVariance << sep << bench.maxActiveNodes << "\n";
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
