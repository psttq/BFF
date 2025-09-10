#pragma once
#include <string>
#include <filesystem>

namespace BFF {
  namespace fs = std::filesystem;
  class IChecker{
    protected:
    std::string p_name;

    public:
    struct CheckResult {
      fs::path file_path;
      std::string valnerability;
      std::string checker_name;
      bool isOk;
    };

    public:
    IChecker(const std::string name) : p_name(name) {}
    virtual CheckResult check(const fs::path &file_path) = 0;
    virtual ~IChecker() = default;
  };
}
