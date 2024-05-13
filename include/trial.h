/*
 * Created by janis on 2024-05-09
 */

#pragma once

#include "bench.h"
#include "config.h"

#include <fstream>
#include <vector>

struct TrialResults {
  std::vector<BenchmarkResults> rawResults;

public:
  void print() const;
  void save(std::fstream& out, const std::string& sep) const;
};

TrialResults runTrial(const Configuration& confA, const Configuration& confB);
