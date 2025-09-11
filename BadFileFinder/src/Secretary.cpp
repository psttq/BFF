#include "BFF/IChecker.hpp"
#include <BFF/Secretary.hpp>
#include <iostream>
#include <memory>
#include <vector>

namespace BFF {
  Secretary::Secretary(){
    logger = std::make_shared<Logger>();
    logger->log("Started Secretary");
  }

  void Secretary::addChecker(std::unique_ptr<IChecker> checker){
    logger->log("Added checker: " + checker->getName());
    checker->setLogger(logger);
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
          logger->error("Failed to read folder: " + std::string(e.what()));
      }

      return files;
  }

  Secretary::SecretaryResult Secretary::checkFolder(const fs::path &folder_path){
    logger->log("Starting checking folder '" + folder_path.string() + "'");
    auto files = getAllFilesRecursive(folder_path);
    std::vector<IChecker::CheckResult> total_results;
    for(auto &checker : p_checkers){
      std::vector<IChecker::CheckResult> checker_results;
      for(auto file : files){
        auto result = checker->check(file);
        checker_results.push_back(result);
      }
      total_results.insert(total_results.end(), checker_results.begin(), checker_results.end());
    }
    logger->log("Checking finished");
    return total_results;
  }
}
