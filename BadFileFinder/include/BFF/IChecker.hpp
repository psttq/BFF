#pragma once
#include <string>
#include "Logger.hpp"
#include <filesystem>
#include <memory>

namespace BFF {
  namespace fs = std::filesystem;
  class IChecker{
    protected:
    std::string p_name;
    std::shared_ptr<Logger> p_logger;

    public:
    struct CheckResult {
      fs::path file_path;
      std::string vulnerability;
      std::string checker_name;
      bool isOk;
      bool isChecked;
    };

    public:
    IChecker(const std::string name) : p_name(name) {}
    virtual CheckResult check(const fs::path &file_path) = 0;
    virtual ~IChecker() = default;
    
    void setLogger(std::shared_ptr<Logger> logger){
      p_logger = logger;
    }

    std::string getName() const {
      return p_name;
    }
  };
}
