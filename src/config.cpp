/*
 * Created by janis on 2024-04-28
 */

#include "config.h"

Configuration Configuration::makeDiff() {
  return {
      .ecConfig = {
          .execution     = {.numericalTolerance = dd::RealNumber::eps,
                            .parallel           = true,
                            .nthreads =
                                std::max(2U, std::thread::hardware_concurrency()),
                            .timeout                = 0.0,
                            .runConstructionChecker = false,
                            .runSimulationChecker   = false,
                            .runAlternatingChecker  = true,
                            .runZXChecker           = false},
          .optimizations = {.fixOutputPermutationMismatch     = false,
                            .fuseSingleQubitGates             = false,
                            .reconstructSWAPs                 = false,
                            .removeDiagonalGatesBeforeMeasure = false,
                            .transformDynamicCircuit          = false,
                            .reorderOperations                = false,
                            .backpropagateOutputPermutation   = false,
                            .elidePermutations                = false},
          .application = {.alternatingScheme = ec::ApplicationSchemeType::Diff},
          .functionality = {},
          .simulation    = {},
          .parameterized = {}}};
}

Configuration Configuration::makeProportional() {
  return {
      .ecConfig = {.execution     = {.numericalTolerance = dd::RealNumber::eps,
                                     .parallel           = true,
                                     .nthreads           = std::max(
                                     2U, std::thread::hardware_concurrency()),
                                     .timeout                = 0.0,
                                     .runConstructionChecker = false,
                                     .runSimulationChecker   = false,
                                     .runAlternatingChecker  = true,
                                     .runZXChecker           = false},
                   .optimizations = {.fixOutputPermutationMismatch     = false,
                                     .fuseSingleQubitGates             = false,
                                     .reconstructSWAPs                 = false,
                                     .removeDiagonalGatesBeforeMeasure = false,
                                     .transformDynamicCircuit          = false,
                                     .reorderOperations                = false,
                                     .backpropagateOutputPermutation   = false,
                                     .elidePermutations                = false},
                   .application   = {.alternatingScheme =
                                         ec::ApplicationSchemeType::Proportional},
                   .functionality = {},
                   .simulation    = {},
                   .parameterized = {}}};
}
