#pragma once
#include "IChecker.hpp"
#include <vector>
#include <memory>
#include <filesystem>
#include "Logger.hpp"
namespace BFF {
  namespace fs = std::filesystem;

  class Secretary{
    std::vector<std::unique_ptr<IChecker>> p_checkers;
    std::shared_ptr<Logger> logger;

    private:
    std::vector<fs::path> getAllFilesRecursive(const fs::path& directory);

    public:
    using SecretaryResult = std::vector<IChecker::CheckResult>;

    Secretary();

    void addChecker(std::unique_ptr<IChecker> checker);

    SecretaryResult checkFolder(const fs::path &folder_path);


  };
}
