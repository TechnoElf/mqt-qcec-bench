/*
 * Created by janis on 2024-04-28
 */

#pragma once

#include <chrono>
#include <string>

struct BenchmarkResults {
  std::string name;

  size_t runCount;
  bool   runDiff;
  bool   runProp;

  std::chrono::duration<double> avgDiffInitTime;
  std::chrono::duration<double> avgDiffRunTime;
  std::chrono::duration<double> avgPropInitTime;
  std::chrono::duration<double> avgPropRunTime;

  size_t peakDiffUniqueTableSize;
  size_t peakPropUniqueTableSize;

public:
  void print() const;
};

BenchmarkResults runBenchmark(const std::string& name, const std::string& a,
                              const std::string& b, size_t runCount,
                              bool runDiff, bool runProp);