/*
 * Created by janis on 2024-04-28
 */

#include "instance.h"

#include <iostream>

void BenchmarkInstanceResults::print() const {
  std::cout << "BenchmarkInstanceResults {\n"
            << "  initTime = " << this->initTime << "\n"
            << "  runTime = " << this->runTime << "\n"
            << "  peakUniqueTableSize = " << this->peakUniqueTableSize << "\n"
            << "}\n";
}

BenchmarkInstanceResults runBenchmarkInstance(const std::string&   a,
                                              const std::string&   b,
                                              const Configuration& c) {
  qc::QuantumComputation qc1;
  qc::QuantumComputation qc2;
  qc1.import(a);
  qc2.import(b);

  const auto timeBegin = std::chrono::high_resolution_clock::now();
  ec::EquivalenceCheckingManager ecm(qc1, qc2, c.ecConfig);
  const auto timeAfterInit = std::chrono::high_resolution_clock::now();
  ecm.run();
  const auto timeEnd = std::chrono::high_resolution_clock::now();

  const size_t peakUniqueTableSize = ecm.getResults().peakUniqueTableSize;

  return BenchmarkInstanceResults{.initTime = timeAfterInit - timeBegin,
                                  .runTime  = timeEnd - timeAfterInit,
                                  .peakUniqueTableSize = peakUniqueTableSize};
}