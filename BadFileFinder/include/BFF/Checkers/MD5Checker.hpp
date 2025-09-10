#pragma once

#include "BFF/IChecker.hpp"
#include <filesystem>
#include <memory>
#include <string>
#include <unordered_map>

namespace BFF{
  class MD5Checker : public IChecker{
    fs::path p_path_to_base;
    std::unordered_map<std::string, std::string> MD5_base;

    private:

    MD5Checker(const fs::path &path_to_base);

    std::string getFileMD5Hash(const fs::path &file_path);
    void readBase();

    public:
    static std::unique_ptr<MD5Checker> create(const fs::path &path_to_base);
    ~MD5Checker() override = default;

    CheckResult check(const fs::path &file_path) override;
  };
}
