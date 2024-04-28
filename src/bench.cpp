/*
 * Created by janis on 2024-04-28
 */

#include "bench.h"

#include "config.h"
#include "instance.h"

#include <iostream>

void BenchmarkResults::print() const {
  std::cout << "BenchmarkResults {\n"
            << "  runCount = " << this->runCount << "\n"
            << "  runProp = " << this->runProp << "\n"
            << "  runDiff = " << this->runDiff << "\n"
            << "  avgDiffInitTime = " << this->avgDiffInitTime << "\n"
            << "  avgDiffRunTime = " << this->avgDiffRunTime << "\n"
            << "  avgPropInitTime = " << this->avgPropInitTime << "\n"
            << "  avgPropRunTime = " << this->avgPropRunTime << "\n"
            << "  peakDiffUniqueTableSize = " << this->peakDiffUniqueTableSize
            << "\n"
            << "  peakPropUniqueTableSize = " << this->peakPropUniqueTableSize
            << "\n"
            << "}\n";
}

BenchmarkResults runBenchmark(const std::string& a, const std::string& b,
                              size_t runCount, bool runDiff, bool runProp) {
  std::vector<BenchmarkInstanceResults> diffResults;
  std::vector<BenchmarkInstanceResults> propResults;

  const ec::Configuration diffConfig = makeDiffConfig();
  const ec::Configuration propConfig = makeProportionalConfig();

  for (size_t i = 0; i < runCount; i++) {
    if (runDiff) {
      diffResults.push_back(runBenchmarkInstance(a, b, diffConfig));
    }

    if (runProp) {
      propResults.push_back(runBenchmarkInstance(a, b, propConfig));
    }
  }

  std::chrono::duration<double> totalDiffInitTime{};
  std::chrono::duration<double> totalDiffRunTime{};

  for (const auto& res : diffResults) {
    totalDiffInitTime += res.initTime;
    totalDiffRunTime += res.runTime;
  }

  std::chrono::duration<double> totalPropInitTime{};
  std::chrono::duration<double> totalPropRunTime{};

  for (const auto& res : propResults) {
    totalPropInitTime += res.initTime;
    totalPropRunTime += res.runTime;
  }

  return BenchmarkResults{
      .runCount        = runCount,
      .runDiff         = runDiff,
      .runProp         = runProp,
      .avgDiffInitTime = totalDiffInitTime / runCount,
      .avgDiffRunTime  = totalDiffRunTime / runCount,
      .avgPropInitTime = totalPropInitTime / runCount,
      .avgPropRunTime  = totalPropRunTime / runCount,
      .peakDiffUniqueTableSize =
          diffResults.empty() ? 0 : diffResults[0].peakUniqueTableSize,
      .peakPropUniqueTableSize =
          propResults.empty() ? 0 : propResults[0].peakUniqueTableSize};
}
