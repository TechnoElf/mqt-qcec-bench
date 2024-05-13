/*
 * Created by janis on 2024-04-28
 */

#pragma once

#include <EquivalenceCheckingManager.hpp>

struct Configuration {
  ec::Configuration ecConfig;

public:
  static Configuration makeDiff();
  static Configuration makeProportional();
};
