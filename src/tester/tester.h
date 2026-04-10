#ifndef TESTER_H
#define TESTER_H

#include <filesystem>
namespace fs = std::filesystem;

namespace alive {
class Tester
{
public:
    Tester(const fs::path &test_files_dir,
           const fs::path &golden_files_dir,
           bool generate_golden_files = false);
    ~Tester();
};
} // namespace alive
#endif // TESTER_H
