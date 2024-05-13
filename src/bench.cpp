/*
 * Created by janis on 2024-04-28
 */

#include "bench.h"

#include "config.h"
#include "instance.h"

#include <iostream>

void BenchmarkResults::print() const {
  std::cout << "BenchmarkResults {\n"
            << "  name = " << this->name << "\n"
            << "  runCount = " << this->runCount << "\n"
            << "  runB = " << this->runB << "\n"
            << "  runA = " << this->runA << "\n"
            << "  avgAInitTime = " << this->avgAInitTime << "\n"
            << "  avgARunTime = " << this->avgARunTime << "\n"
            << "  avgBInitTime = " << this->avgBInitTime << "\n"
            << "  avgBRunTime = " << this->avgBRunTime << "\n"
            << "  peakAUniqueTableSize = " << this->peakAUniqueTableSize << "\n"
            << "  peakBUniqueTableSize = " << this->peakBUniqueTableSize << "\n"
            << "}\n";
}

BenchmarkResults runBenchmark(const std::string& name, const std::string& a,
                              const std::string& b, const Configuration& confA,
                              const Configuration& confB, size_t runCount,
                              bool runA, bool runB) {
  std::vector<BenchmarkInstanceResults> resultsA;
  std::vector<BenchmarkInstanceResults> resultsB;

  for (size_t i = 0; i < runCount; i++) {
    if (runA) {
      resultsA.push_back(runBenchmarkInstance(a, b, confA));
    }

    if (runB) {
      resultsB.push_back(runBenchmarkInstance(a, b, confB));
    }
  }

  std::chrono::duration<double> totalDiffInitTime{};
  std::chrono::duration<double> totalDiffRunTime{};

  for (const auto& res : resultsA) {
    totalDiffInitTime += res.initTime;
    totalDiffRunTime += res.runTime;
  }

  std::chrono::duration<double> totalPropInitTime{};
  std::chrono::duration<double> totalPropRunTime{};

  for (const auto& res : resultsB) {
    totalPropInitTime += res.initTime;
    totalPropRunTime += res.runTime;
  }

  return BenchmarkResults{
      .name         = name,
      .runCount     = runCount,
      .runA         = runA,
      .runB         = runB,
      .avgAInitTime = totalDiffInitTime / runCount,
      .avgARunTime  = totalDiffRunTime / runCount,
      .avgBInitTime = totalPropInitTime / runCount,
      .avgBRunTime  = totalPropRunTime / runCount,
      .peakAUniqueTableSize =
          resultsA.empty() ? 0 : resultsA[0].peakUniqueTableSize,
      .peakBUniqueTableSize =
          resultsB.empty() ? 0 : resultsB[0].peakUniqueTableSize};
}
