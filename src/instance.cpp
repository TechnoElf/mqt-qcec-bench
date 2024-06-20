/*
 * Created by janis on 2024-04-28
 */

#include "instance.h"

#include <iostream>

void InstanceResults::print() const {
  std::cout << "InstanceResults {\n"
            << "  name = " << this->displayName << "\n"
            << "  runCount = " << this->runCount << "\n"
            << "  initTime = " << this->initTimeMean << "\n"
            << "  runTime = " << this->runTimeMean << "\n"
            << "  maxActiveNodes = " << this->maxActiveNodes << "\n"
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
  if (this->runCount == 0) {
    return InstanceResults{.displayName  = this->displayName,
                           .runCount     = this->runCount,
                           .timeOut      = this->timeOut,
                           .initTimeMean = this->timeOut.count(),
                           .runTimeMean  = this->timeOut.count()};
  }

  std::cerr << "Generating instance \"" << this->displayName << "\"\n";

  const std::string fullBenchmarkName =
      std::format("{}_{}", this->benchmarkName, this->size);

  if (std::system(std::format("./get_circuits.py circuits/{} {} {}",
                              fullBenchmarkName, this->benchmarkName,
                              this->size)
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
    qc1.import(std::format("circuits/{}_ind.qasm", fullBenchmarkName));
    break;
  case InstanceKind::Ng0Ng3:
  case InstanceKind::Ng0Mp0:
  case InstanceKind::Ng0Mp3:
    qc1.import(std::format("circuits/{}_ng0.qasm", fullBenchmarkName));
    break;
  case InstanceKind::Ng3Mp0:
  case InstanceKind::Ng3Mp3:
    qc1.import(std::format("circuits/{}_ng3.qasm", fullBenchmarkName));
    break;
  case InstanceKind::Mp0Mp3:
    qc1.import(std::format("circuits/{}_mp0.qasm", fullBenchmarkName));
    break;
  }

  switch (*this->kind) {
  case InstanceKind::IndNg0:
    qc2.import(std::format("circuits/{}_ng0.qasm", fullBenchmarkName));
    break;
  case InstanceKind::IndNg3:
  case InstanceKind::Ng0Ng3:
    qc2.import(std::format("circuits/{}_ng3.qasm", fullBenchmarkName));
    break;
  case InstanceKind::IndMp0:
  case InstanceKind::Ng0Mp0:
  case InstanceKind::Ng3Mp0:
    qc2.import(std::format("circuits/{}_mp0.qasm", fullBenchmarkName));
    break;
  case InstanceKind::IndMp3:
  case InstanceKind::Ng0Mp3:
  case InstanceKind::Ng3Mp3:
  case InstanceKind::Mp0Mp3:
    qc2.import(std::format("circuits/{}_mp3.qasm", fullBenchmarkName));
    break;
  }

  std::vector<bool>   equivalent;
  std::vector<size_t> numQubits1;
  std::vector<size_t> numQubits2;
  std::vector<size_t> numGates1;
  std::vector<size_t> numGates2;
  std::vector<size_t> diffEquivalenceCount;
  std::vector<double> initTime;
  std::vector<double> runTime;
  std::vector<size_t> maxActiveNodes;

  {
    const auto timeBegin = std::chrono::high_resolution_clock::now();
    ec::EquivalenceCheckingManager ecm(qc1, qc2, conf.ecConfig);
    const auto timeAfterInit = std::chrono::high_resolution_clock::now();
    ecm.run();
    const auto timeEnd = std::chrono::high_resolution_clock::now();

    equivalent.push_back(ecm.getResults().consideredEquivalent());
    numQubits1.push_back(ecm.getResults().numQubits1);
    numQubits2.push_back(ecm.getResults().numQubits2);
    numGates1.push_back(ecm.getResults().numGates1);
    numGates2.push_back(ecm.getResults().numGates2);
    diffEquivalenceCount.push_back(ecm.getResults().diffEquivalenceCount);
    initTime.emplace_back(
        std::chrono::duration<double>(timeAfterInit - timeBegin).count());
    runTime.emplace_back(
        std::chrono::duration<double>(timeEnd - timeAfterInit).count());
    maxActiveNodes.push_back(ecm.getResults().maxActiveNodes);
  }

  size_t realRunCount = std::max(
      static_cast<size_t>(std::floor(1.0 / (initTime[0] + runTime[0]))),
      this->runCount);

  for (size_t i = 0; i < realRunCount - 1; i++) {
    const auto timeBegin = std::chrono::high_resolution_clock::now();
    ec::EquivalenceCheckingManager ecm(qc1, qc2, conf.ecConfig);
    const auto timeAfterInit = std::chrono::high_resolution_clock::now();
    ecm.run();
    const auto timeEnd = std::chrono::high_resolution_clock::now();

    equivalent.push_back(ecm.getResults().consideredEquivalent());
    numQubits1.push_back(ecm.getResults().numQubits1);
    numQubits2.push_back(ecm.getResults().numQubits2);
    numGates1.push_back(ecm.getResults().numGates1);
    numGates2.push_back(ecm.getResults().numGates2);
    diffEquivalenceCount.push_back(ecm.getResults().diffEquivalenceCount);
    initTime.emplace_back(
        std::chrono::duration<double>(timeAfterInit - timeBegin).count());
    runTime.emplace_back(
        std::chrono::duration<double>(timeEnd - timeAfterInit).count());
    maxActiveNodes.push_back(ecm.getResults().maxActiveNodes);
  }

  bool   deterministic       = true;
  double totalInitTime       = 0.0;
  double totalRunTime        = 0.0;
  size_t totalMaxActiveNodes = 0;

  for (size_t i = 0; i < realRunCount; i++) {
    totalInitTime += initTime[i];
    totalRunTime += runTime[i];
    totalMaxActiveNodes += maxActiveNodes[i];

    if (i > 0) {
      deterministic &= equivalent[i] == equivalent[i - 1];
      deterministic &= numQubits1[i] == numQubits1[i - 1];
      deterministic &= numQubits2[i] == numQubits2[i - 1];
      deterministic &= numGates1[i] == numGates1[i - 1];
      deterministic &= numGates2[i] == numGates2[i - 1];
      deterministic &= diffEquivalenceCount[i] == diffEquivalenceCount[i - 1];
    }
  }

  double initTimeMean = totalInitTime / static_cast<double>(realRunCount);
  double runTimeMean  = totalRunTime / static_cast<double>(realRunCount);
  totalInitTime       = 0.0;
  totalRunTime        = 0.0;

  for (size_t i = 0; i < realRunCount; i++) {
    totalInitTime +=
        (initTime[i] - initTimeMean) * (initTime[i] - initTimeMean);
    totalRunTime += (runTime[i] - runTimeMean) * (runTime[i] - runTimeMean);
  }

  return InstanceResults{
      .displayName          = this->displayName,
      .runCount             = realRunCount,
      .timeOut              = this->timeOut,
      .equivalent           = equivalent[0],
      .deterministic        = deterministic,
      .numQubits1           = numQubits1[0],
      .numQubits2           = numQubits2[0],
      .numGates1            = numGates1[0],
      .numGates2            = numGates2[0],
      .diffEquivalenceCount = diffEquivalenceCount[0],
      .initTimeMean         = initTimeMean,
      .initTimeVariance     = totalInitTime / static_cast<double>(realRunCount),
      .runTimeMean          = runTimeMean,
      .runTimeVariance      = totalRunTime / static_cast<double>(realRunCount),
      .maxActiveNodes       = totalMaxActiveNodes / realRunCount};
}
