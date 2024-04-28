#include "bench.h"

#include <iostream>

int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv) {
  std::cout << "dj ng0 / ng3: ";
  runBenchmark("./circuits/dj_nativegates_ibm_qiskit_opt0_8.qasm",
               "./circuits/dj_nativegates_ibm_qiskit_opt3_8.qasm",
               10, true, true)
      .print();
  std::cout << "\n";
  
  std::cout << "grover ng0 / ng3: ";
  runBenchmark("./circuits/grover-noancilla_nativegates_ibm_qiskit_opt0_8.qasm",
               "./circuits/grover-noancilla_nativegates_ibm_qiskit_opt3_8.qasm",
               10, true, true)
      .print();
  std::cout << "\n";

  std::cout << "shor_15_4 ng0 / ng2: ";
  runBenchmark("./circuits/shor_15_4_nativegates_ibm_qiskit_opt0_18.qasm",
               "./circuits/shor_15_4_nativegates_ibm_qiskit_opt2_18.qasm", 5,
               true, false)
      .print();
  std::cout << "\n";
}