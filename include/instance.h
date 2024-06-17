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

struct InstanceKind {
  enum class Kind : uint8_t {
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
  } kind;

  using Kind::IndNg0;
  using Kind::IndNg3;
  using Kind::IndMp0;
  using Kind::IndMp3;
  using Kind::Ng0Ng3;
  using Kind::Ng0Mp0;
  using Kind::Ng0Mp3;
  using Kind::Ng3Mp0;
  using Kind::Ng3Mp3;
  using Kind::Mp0Mp3;

  static constexpr std::array<std::tuple<Kind, const char*>, 10> kinds = {
      std::tuple(IndNg0, "IndNg0"),
      std::tuple(IndNg3, "IndNg3"),
      std::tuple(IndMp0, "IndMp0"),
      std::tuple(IndMp3, "IndMp3"),
      std::tuple(Ng0Ng3, "Ng0Ng3"),
      std::tuple(Ng0Mp0, "Ng0Mp0"),
      std::tuple(Ng0Mp3, "Ng0Mp3"),
      std::tuple(Ng3Mp0, "Ng3Mp0"),
      std::tuple(Ng3Mp3, "Ng3Mp3"),
      std::tuple(Mp0Mp3, "Mp0Mp3")
  };

public:
  InstanceKind(Kind kind) : kind(kind) {}

  Kind operator*() const { return kind; }

  static InstanceKind fromStr(const std::string& str);
};

struct Instance {
  std::string displayName;

  std::string  benchmarkName;
  InstanceKind kind = InstanceKind::IndMp3;
  size_t       size = 8;

  size_t                        runCount = 1;
  std::chrono::duration<double> timeOut  = std::chrono::seconds(10);

public:
  InstanceResults run(const Configuration& conf) const;
};