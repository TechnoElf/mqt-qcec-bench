/*
 * Created by janis on 2024-04-28
 */

#pragma once

#include "config.h"

#include <chrono>
#include <string>

struct BenchmarkResults {
  std::string name;

  size_t runCount;
  bool   runA;
  bool   runB;

  std::chrono::duration<double> avgAInitTime;
  std::chrono::duration<double> avgARunTime;
  std::chrono::duration<double> avgBInitTime;
  std::chrono::duration<double> avgBRunTime;

  size_t peakAUniqueTableSize;
  size_t peakBUniqueTableSize;

public:
  void print() const;
};

BenchmarkResults runBenchmark(const std::string& name, const std::string& a,
                              const std::string& b, const Configuration& confA,
                              const Configuration& confB, size_t runCount,
                              bool runA, bool runB);