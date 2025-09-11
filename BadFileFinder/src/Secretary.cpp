#include "BFF/IChecker.hpp"
#include <BFF/Secretary.hpp>
#include <iostream>
#include <memory>
#include <vector>
#include <mutex>
#include <future>

namespace BFF
{
  Secretary::Secretary()
  {
    logger = std::make_shared<Logger>();
    logger->log("Started Secretary");
  }

  void Secretary::addChecker(std::unique_ptr<IChecker> checker)
  {
    logger->log("Added checker: " + checker->getName());
    checker->setLogger(logger);
    p_checkers.push_back(std::move(checker));
  }

  std::vector<fs::path> Secretary::getAllFilesRecursive(const fs::path &directory)
  {
    std::vector<fs::path> all_files;
    std::mutex files_mutex;

    // First, get all directories recursively (single-threaded)
    std::vector<fs::path> all_directories = {directory};

    try
    {
      for (const auto &entry : fs::recursive_directory_iterator(directory))
      {
        if (entry.is_directory())
        {
          all_directories.push_back(entry.path());
        }
      }
    }
    catch (const fs::filesystem_error &e)
    {
      logger->error("Failed to read folder: " + std::string(e.what()));
      return all_files;
    }

    // Process each directory in parallel
    std::vector<std::future<std::vector<fs::path>>> futures;

    auto processDirectory = [](const fs::path &dir) -> std::vector<fs::path>
    {
      std::vector<fs::path> dir_files;
      try
      {
        for (const auto &entry : fs::directory_iterator(dir))
        {
          if (entry.is_regular_file())
          {
            dir_files.push_back(entry.path());
          }
        }
      }
      catch (const fs::filesystem_error &e)
      {
        // Error handling would need to be handled differently here
      }
      return dir_files;
    };

    // Launch async tasks for each directory
    for (const auto &dir : all_directories)
    {
      futures.push_back(std::async(std::launch::async, processDirectory, dir));
    }

    // Collect results
    for (auto &future : futures)
    {
      auto dir_files = future.get();
      std::lock_guard<std::mutex> lock(files_mutex);
      all_files.insert(all_files.end(), dir_files.begin(), dir_files.end());
    }

    return all_files;
  }

  Secretary::SecretaryResult Secretary::checkFolder(const fs::path &folder_path)
  {
    logger->log("Starting checking folder '" + folder_path.string() + "'");
    auto files = getAllFilesRecursive(folder_path);
    logger->log("Found " + std::to_string(files.size()) + " files");

    std::vector<IChecker::CheckResult> total_results;
    std::mutex results_mutex;

    unsigned int num_threads = std::thread::hardware_concurrency();
    if (num_threads == 0)
      num_threads = 4;
    num_threads = std::min(num_threads, static_cast<unsigned int>(files.size()));

    logger->log("Using" + std::to_string(num_threads) + " threads");



    size_t files_per_thread = files.size() / num_threads;

    for (auto &checker : p_checkers)
    {
      std::vector<std::thread> threads;
      std::vector<IChecker::CheckResult> checker_results;

      auto processFilesChunk = [&](size_t start_idx, size_t end_idx)
      {
        std::vector<IChecker::CheckResult> thread_results;
        for (size_t i = start_idx; i < end_idx; ++i)
        {
          auto result = checker->check(files[i]);
          thread_results.push_back(result);
        }
        std::lock_guard<std::mutex> lock(results_mutex);
        checker_results.insert(checker_results.end(),
                               thread_results.begin(),
                               thread_results.end());
      };

      for (unsigned int i = 0; i < num_threads; ++i)
      {
        size_t start = i * files_per_thread;
        size_t end = (i == num_threads - 1) ? files.size() : start + files_per_thread;

        threads.emplace_back(processFilesChunk, start, end);
      }

      for (auto &thread : threads)
      {
        if (thread.joinable())
        {
          thread.join();
        }
      }

      total_results.insert(total_results.end(), checker_results.begin(), checker_results.end());
    }
    
    logger->log("Checking finished");
    return total_results;
  }
}
