#include <chrono>
#include <iostream>

#include <EquivalenceCheckingManager.hpp>

const ec::Configuration CONFIG = {
    {dd::RealNumber::eps, true,
     std::max(2U, std::thread::hardware_concurrency()), 0.0, false, false, true,
     false},
    {false, false, false, false, false, false},
    {},
    {},
    {},
    {}};

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
  ec::Configuration c             = CONFIG;
  c.application.alternatingScheme = ec::ApplicationSchemeType::Diff;
  qc::QuantumComputation qc1;
  qc::QuantumComputation qc2;
  qc1.import("./circuits/shor_15_4_nativegates_ibm_qiskit_opt0_18.qasm");
  qc2.import("./circuits/shor_15_4_nativegates_ibm_qiskit_opt2_18.qasm");

  const auto begin = std::chrono::high_resolution_clock::now();

  ec::EquivalenceCheckingManager ecm(qc1, qc2, c);
  ecm.run();
  const size_t totalPeakUniqueTableSize = ecm.getResults().peakUniqueTableSize;

  const auto end = std::chrono::high_resolution_clock::now();
  const std::chrono::duration<double> runTime = end - begin;

  std::cout << runTime << "\n";
  std::cout << "peak unique node count = " << totalPeakUniqueTableSize << "\n";
}