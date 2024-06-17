/*
 * Created by janis on 2024-04-28
 */

#pragma once

#include "config.h"
#include "instance.h"

#include <chrono>
#include <string>

struct BenchmarkResults {
  std::vector<InstanceResults> rawResults;

public:
  void print() const;
  void save(std::fstream& out, const std::string& sep) const;
};

struct Benchmark {
  std::vector<Instance> instances;

public:
  void                           add(const Instance&& instance);
  [[nodiscard]] BenchmarkResults run(const Configuration& conf) const;
};