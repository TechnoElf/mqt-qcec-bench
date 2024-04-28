/*
 * Created by janis on 2024-04-28
 */

#pragma once

#include <EquivalenceCheckingManager.hpp>
#include <chrono>
#include <string>

struct BenchmarkInstanceResults {
  std::chrono::duration<double> initTime;
  std::chrono::duration<double> runTime;
  size_t                        peakUniqueTableSize;

public:
  void print() const;
};

BenchmarkInstanceResults runBenchmarkInstance(const std::string&       a,
                                              const std::string&       b,
                                              const ec::Configuration& c);