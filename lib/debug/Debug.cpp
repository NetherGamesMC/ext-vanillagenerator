#include "Debug.h"

#include <iostream>
#include <fstream>

namespace DebugLogger {
std::ofstream *logger = nullptr;

void initLogger() {
  logger = new std::ofstream;
  logger->open(R"(E:\extnoise\debug\coordinates.txt)");
}

void writeLogger(const std::string &value) {
  if (logger == nullptr || !logger->is_open()) initLogger();

  (*logger) << value << std::endl;
}

}