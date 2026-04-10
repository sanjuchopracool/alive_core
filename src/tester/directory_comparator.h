#ifndef DIRECTORY_COMPARATOR_H
#define DIRECTORY_COMPARATOR_H

#include <filesystem>

namespace alive {
namespace fs = std::filesystem;

bool compare_directories(const fs::path &golden_dir,
                         const fs::path &test_dir,
                         std::string &error,
                         float rmse_threshold,
                         float ssim_threshold);
} // namespace alive
#endif // DIRECTORY_COMPARATOR_H
