#include "bench.h"
#include "config.h"

#include <fstream>
#include <iostream>
#include <optional>
#include <tclap/CmdLine.h>
#include <toml++/toml.hpp>

std::optional<std::fstream>
parseOutputArg(const TCLAP::ValueArg<std::string>&& arg) {
  if (!arg.getValue().empty()) {
    std::fstream maybeOutput = std::fstream(arg.getValue(), std::ios::out);
    if (maybeOutput.is_open()) {
      return maybeOutput;
    } else {
      std::cerr << "error: couldn't open \"" << arg.getValue() << "\"\n";
      std::exit(1);
    }
  } else {
    return std::nullopt;
  }
}

Benchmark parseBenchmarkArg(const TCLAP::ValueArg<std::string>&& arg) {
  Benchmark benchmark;

  toml::table table = toml::parse_file(arg.getValue());
  for (const auto& node : table) {
    if (toml::table* subTablePtr = node.second.as_table()) {
      toml::table subTable = *subTablePtr;
      auto        name     = subTable["benchmark"].value<std::string>();
      if (!name) {
        std::cerr << "error: key benchmark missing in " << node.first << "\n";
        std::exit(1);
      }

      std::string kind = subTable["kind"].value_or("");
      size_t      size = subTable["size"].value_or(8UL);

      benchmark.add({.displayName   = std::string(node.first.str()),
                     .benchmarkName = name.value(),
                     .kind          = InstanceKind::fromStr(kind),
                     .size          = size});
    } else {
      std::cerr << "error: field " << node.first
                << " is not part of a benchmark table\n";
      std::exit(1);
    }
  }

  return benchmark;
}

Configuration parseCheckerArg(const TCLAP::ValueArg<std::string>&& arg) {
  Configuration config;

  toml::table table = toml::parse_file(arg.getValue());
  std::string kind  = table["kind"].value_or("");

  if (kind == "diff") {
    return Configuration::makeDiff();
  }

  if (kind == "proportional") {
    return Configuration::makeProportional();
  }

  std::cerr << "error: unknown checker \"" << kind << "\"\n";
  exit(1);
}

int main(int argc, char** argv) {
  std::optional<std::fstream> output;
  Benchmark                   benchmark;
  Configuration               config;

  try {
    TCLAP::CmdLine cmd("MQT QCEC Benchmarking Tool", ' ', "0.1.0");
    const TCLAP::ValueArg<std::string> outputArg(
        "o", "output", "File path to write results to in CSV format.", false,
        "", "path", cmd);
    const TCLAP::ValueArg<std::string> benchmarkArg(
        "b", "benchmark", "Benchmark description in TOML format.", true, "",
        "path", cmd);
    const TCLAP::ValueArg<std::string> checkerArg(
        "c", "checker", "MQT-QCEC configuration in TOML format.", true, "",
        "path", cmd);
    cmd.parse(argc, argv);

    output    = parseOutputArg(std::move(outputArg));
    benchmark = parseBenchmarkArg(std::move(benchmarkArg));
    config    = parseCheckerArg(std::move(checkerArg));
  } catch (const TCLAP::ArgException& e) {
    std::cerr << "error: " << e.error() << " for arg " << e.argId() << "\n";
    std::exit(1);
  } catch (const toml::parse_error& e) {
    std::cerr << "error: " << e << "\n";
    std::exit(1);
  }

  const BenchmarkResults res = benchmark.run(config);
  res.print();

  if (output) {
    res.save(*output, ",");
  }
}
