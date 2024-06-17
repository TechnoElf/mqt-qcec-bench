/*
 * Created by janis on 2024-04-28
 */

#pragma once

#include "config.h"

#include <EquivalenceCheckingManager.hpp>
#include <chrono>
#include <string>

struct InstanceResults {
  const std::string& displayName;

  size_t                        runCount;
  std::chrono::duration<double> timeOut;
  bool                          equivalent;

  std::chrono::duration<double> initTime;
  std::chrono::duration<double> runTime;
  size_t                        peakUniqueTableSize;

public:
  void print() const;
};

enum class InstanceKind : uint8_t {
  IndNg0,
  IndNg3,
  IndMp0,
  IndMp3,
  Ng0Ng3,
  Ng0Mp0,
  Ng0Mp3,
  Ng3Mp0,
  Ng3Mp3,
  Mp0Mp3
};

struct Instance {
  const std::string& displayName;

  const std::string& benchmarkName;
  InstanceKind       kind = InstanceKind::IndMp3;
  size_t             size = 8;

  size_t                        runCount = 1;
  std::chrono::duration<double> timeOut  = std::chrono::seconds(10);

public:
  InstanceResults run(const Configuration& conf) const;
};