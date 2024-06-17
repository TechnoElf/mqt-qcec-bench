#include "config.h"
#include "trial.h"

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

    std::fstream maybeOutput =
        std::fstream(outputArg.getValue(), std::ios::out);
    if (maybeOutput.is_open()) {
      output = std::move(maybeOutput);
    } else {
      std::cerr << "error: couldn't open " << outputArg.getValue() << "\n";
      std::exit(1);
    }
  } catch (const TCLAP::ArgException& e) {
    std::cerr << "error: " << e.error() << " for arg " << e.argId() << "\n";
    std::exit(1);
  }

  const TrialResults res =
      runTrial(Configuration::makeDiff(), Configuration::makeProportional());
  res.print();

  if (output) {
    res.save(*output, ",");
  }
}
