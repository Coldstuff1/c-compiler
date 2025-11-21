#include "../lexer/scanner.hpp"
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>

int main(int argc, char **argv) {
  std::filesystem::path defaultPath = "examples/hello-world.c";
  if (const char *workspace = std::getenv("BUILD_WORKSPACE_DIRECTORY")) {
    defaultPath = std::filesystem::path(workspace) / defaultPath;
  }

  std::filesystem::path inputPath =
      argc > 1 ? std::filesystem::path(argv[1]) : defaultPath;
  std::ifstream inputFile(inputPath);
  if (!inputFile.is_open()) {
    std::cout << "File not found!" << std::endl;
    return -1;
  }
  lexer::Scanner scanner(inputFile);

  while (scanner.hasNext()) {
    std::cout << scanner.next();
  }
  std::cout << std::endl;
  return 0;
}
