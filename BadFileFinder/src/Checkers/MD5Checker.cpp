#include "BFF/Checkers/MD5Checker.hpp"
#include "BFF/IChecker.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <memory>
#include <openssl/md5.h>
#include <ranges>
#include <vector>

namespace BFF {
MD5Checker::MD5Checker(const fs::path &path_to_base)
    : IChecker("MD5Checker"),
      p_path_to_base(path_to_base) {
  readBase();
  p_name = std::string("MD5Checker (") + path_to_base.string() + std::string(")");
}

std::unique_ptr<MD5Checker> MD5Checker::create(const fs::path &path_to_base) {
  return std::unique_ptr<MD5Checker>(new MD5Checker(path_to_base));
}

std::vector<std::string> splitString(const std::string &str, char delimiter) {
  auto tokens = str | std::ranges::views::split(delimiter) |
                std::ranges::views::transform([](auto &&range) {
                  return std::string(range.begin(), range.end());
                });

  return std::vector<std::string>(tokens.begin(), tokens.end());
}

void MD5Checker::readBase() {
  std::vector<std::vector<std::string>> result;

  std::ifstream file(p_path_to_base);
  if (!file.is_open()) {
    throw std::runtime_error("Cannot open file: " +
                             (p_path_to_base.string()));
  }

  std::string line;
  while (std::getline(file, line)) {
    auto split_result = splitString(line, ';');
    if (split_result.size() != 2) {
      throw std::runtime_error("Error while reading MD5 Base");
    }

    MD5_base[split_result[0]] = split_result[1];
  }
}

std::string MD5Checker::getFileMD5Hash(const fs::path &file_path) {
  std::ifstream file(file_path, std::ios::binary);
  if (!file) {
    throw std::runtime_error("Cannot open file: " + file_path.string());
  }

  MD5_CTX md5Context;
  MD5_Init(&md5Context);

  char buffer[1024];
  while (file.read(buffer, sizeof(buffer)) || file.gcount() > 0) {
    MD5_Update(&md5Context, buffer, file.gcount());
  }

  unsigned char hash[MD5_DIGEST_LENGTH];
  MD5_Final(hash, &md5Context);

  std::stringstream ss;
  for (int i = 0; i < MD5_DIGEST_LENGTH; ++i) {
    ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
  }

  return ss.str();
}

IChecker::CheckResult MD5Checker::check(const fs::path &file_path) {
  CheckResult result;
  result.file_path = file_path;
  result.checker_name = p_name;
  result.isOk = true;
  result.isChecked = true;
  result.vulnerability = "None";
  
  std::string file_hash = "None";

  try {
    file_hash = getFileMD5Hash(file_path);

    auto hash_info = MD5_base.find(file_hash);

    if (hash_info != MD5_base.end()) {
      result.isOk = false;
      result.vulnerability = hash_info->second;
    }

  } catch (const std::exception &e) {
    result.isOk = false;
    result.isChecked = false;
    result.vulnerability = std::string("Error checking MD5: ") + e.what();
  }

  p_logger->log("Checked file '" + file_path.string() +"'. Hash: " + file_hash + ". Checked: " + (result.isChecked ? "+" : "-") + ", Ok: " + (result.isOk ? "+" : "-") + ", Vulnerability: " + result.vulnerability);
  
  return result;
}

} // namespace BFF
