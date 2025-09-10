#pragma once
#include "IChecker.hpp"
#include <vector>
#include <memory>
#include <filesystem>

namespace BFF {
  namespace fs = std::filesystem;

  class Secretary{
    std::vector<std::unique_ptr<IChecker>> p_checkers;

    private:
    std::vector<fs::path> getAllFilesRecursive(const fs::path& directory);

    public:
    Secretary();

    void addChecker(std::unique_ptr<IChecker> checker);

    void checkFolder(const fs::path &folder_path);
  };
}
