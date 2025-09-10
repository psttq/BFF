#include "BFF/IChecker.hpp"
#include <BFF/Secretary.hpp>
#include <iostream>
#include <memory>
#include <vector>

namespace BFF {
  Secretary::Secretary(){}

  void Secretary::addChecker(std::unique_ptr<IChecker> checker){
    p_checkers.push_back(std::move(checker));
  }

  std::vector<fs::path> Secretary::getAllFilesRecursive(const fs::path& directory) {
      std::vector<fs::path> files;

      try {
          for (const auto& entry : fs::recursive_directory_iterator(directory)) {
              if (entry.is_regular_file()) {
                  files.push_back(entry.path());
              }
          }
      } catch (const fs::filesystem_error& e) {
          std::cerr << "Error accessing directory: " << e.what() << std::endl; //TODO: Change to logger
      }

      return files;
  }

  void Secretary::checkFolder(const fs::path &folder_path){
    auto files = getAllFilesRecursive(folder_path);
    std::vector<IChecker::CheckResult> total_results;
    for(auto &checker : p_checkers){
      std::vector<IChecker::CheckResult> checker_results;
      //DO MULTITHREADING
      for(auto file : files){
        auto result = checker->check(file);
        checker_results.push_back(result);
      }
      total_results.(checker_results.begin(), checker_results.end());
    }
  }
}
