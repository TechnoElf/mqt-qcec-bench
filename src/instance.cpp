/*
 * Created by janis on 2024-04-28
 */

#include "instance.h"

#include <iostream>

void InstanceResults::print() const {
  std::cout << "InstanceResults {\n"
            << "  name = " << this->displayName << "\n"
            << "  runCount = " << this->runCount << "\n"
            << "  initTime = " << this->initTime << "\n"
            << "  runTime = " << this->runTime << "\n"
            << "  peakUniqueTableSize = " << this->peakUniqueTableSize << "\n"
            << "}\n";
}

InstanceKind InstanceKind::fromStr(const std::string& str) {
  for (const auto& kind : InstanceKind::kinds) {
    if (str == std::get<1>(kind)) {
      return std::get<0>(kind);
    }
  }
  return InstanceKind::IndMp3;
}

InstanceResults Instance::run(const Configuration& conf) const {
  std::cerr << "Generating instance \"" << this->displayName << "\"\n";

  if (std::system(std::format("./get_circuits.py circuits/{} {} {}",
                              this->benchmarkName, this->benchmarkName, this->size)
                      .c_str()) != 0) {
    exit(1);
  }

  std::cerr << "Running instance\n";

  qc::QuantumComputation qc1;
  qc::QuantumComputation qc2;

  switch (*this->kind) {
  case InstanceKind::IndNg0:
  case InstanceKind::IndNg3:
  case InstanceKind::IndMp0:
  case InstanceKind::IndMp3:
    qc1.import(std::format("circuits/{}_ind.qasm", this->benchmarkName));
    break;
  case InstanceKind::Ng0Ng3:
  case InstanceKind::Ng0Mp0:
  case InstanceKind::Ng0Mp3:
    qc1.import(std::format("circuits/{}_ng0.qasm", this->benchmarkName));
    break;
  case InstanceKind::Ng3Mp0:
  case InstanceKind::Ng3Mp3:
    qc1.import(std::format("circuits/{}_ng3.qasm", this->benchmarkName));
    break;
  case InstanceKind::Mp0Mp3:
    qc1.import(std::format("circuits/{}_mp0.qasm", this->benchmarkName));
    break;
  }

  switch (*this->kind) {
  case InstanceKind::IndNg0:
    qc2.import(std::format("circuits/{}_ng0.qasm", this->benchmarkName));
    break;
  case InstanceKind::IndNg3:
  case InstanceKind::Ng0Ng3:
    qc2.import(std::format("circuits/{}_ng3.qasm", this->benchmarkName));
    break;
  case InstanceKind::IndMp0:
  case InstanceKind::Ng0Mp0:
  case InstanceKind::Ng3Mp0:
    qc2.import(std::format("circuits/{}_mp0.qasm", this->benchmarkName));
    break;
  case InstanceKind::IndMp3:
  case InstanceKind::Ng0Mp3:
  case InstanceKind::Ng3Mp3:
  case InstanceKind::Mp0Mp3:
    qc2.import(std::format("circuits/{}_mp3.qasm", this->benchmarkName));
    break;
  }

  bool                                       equivalent = true;
  std::vector<std::chrono::duration<double>> initTime;
  std::vector<std::chrono::duration<double>> runTime;
  std::vector<size_t>                        peakUniqueTableSize;

  for (size_t i = 0; i < runCount; i++) {
    const auto timeBegin = std::chrono::high_resolution_clock::now();
    ec::EquivalenceCheckingManager ecm(qc1, qc2, conf.ecConfig);
    const auto timeAfterInit = std::chrono::high_resolution_clock::now();
    ecm.run();
    const auto timeEnd = std::chrono::high_resolution_clock::now();

    equivalent &= ecm.getResults().consideredEquivalent();
    initTime.emplace_back(timeAfterInit - timeBegin);
    runTime.emplace_back(timeEnd - timeAfterInit);
    peakUniqueTableSize.push_back(ecm.getResults().peakUniqueTableSize);
  }

  std::chrono::duration<double> totalInitTime = {};
  for (const auto& time : initTime) {
    totalInitTime += time;
  }

  std::chrono::duration<double> totalRunTime = {};
  for (const auto& time : runTime) {
    totalRunTime += time;
  }

  size_t totalPeakUniqueTableSize = 0;
  for (const auto size : peakUniqueTableSize) {
    totalPeakUniqueTableSize += size;
  }

  return InstanceResults{.displayName = this->displayName,
                         .runCount    = this->runCount,
                         .timeOut     = this->timeOut,
                         .equivalent  = equivalent,
                         .initTime    = totalInitTime / this->runCount,
                         .runTime     = totalRunTime / this->runCount,
                         .peakUniqueTableSize =
                             totalPeakUniqueTableSize / this->runCount};
}
