#include "bench.h"
#include "config.h"

#include <fstream>
#include <iostream>
#include <optional>
#include <tclap/CmdLine.h>

int main(int argc, char** argv) {
  std::optional<std::fstream> output = {};

  try {
    TCLAP::CmdLine cmd("MQT QCEC Benchmarking Tool", ' ', "0.1.0");
    const TCLAP::ValueArg<std::string> outputArg(
        "o", "output", "File path to write results to in CSV format.", false,
        "", "path", cmd);
    cmd.parse(argc, argv);

    if (!outputArg.getValue().empty()) {
      std::fstream maybeOutput =
          std::fstream(outputArg.getValue(), std::ios::out);
      if (maybeOutput.is_open()) {
        output = std::move(maybeOutput);
      } else {
        std::cerr << "error: couldn't open \"" << outputArg.getValue()
                  << "\"\n";
        std::exit(1);
      }
    }
  } catch (const TCLAP::ArgException& e) {
    std::cerr << "error: " << e.error() << " for arg " << e.argId() << "\n";
    std::exit(1);
  }

  Benchmark benchmark;
  benchmark.add({.displayName   = "dj ind/mp0",
                 .benchmarkName = "dj",
                 .kind          = InstanceKind::IndMp0,
                 .size          = 12});
  benchmark.add({.displayName   = "tsp ind/mp0",
                 .benchmarkName = "tsp",
                 .kind          = InstanceKind::IndMp0,
                 .size          = 2});

  const BenchmarkResults res = benchmark.run(Configuration::makeDiff());
  res.print();

  if (output) {
    res.save(*output, ",");
  }
}
