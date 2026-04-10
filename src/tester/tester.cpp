#include "tester.h"

#include "alivetest_fixture.h"
#include <gtest/gtest.h>
namespace alive {

void add_test(const fs::path &test_dir,
              const fs::path &test_file,
              const fs::path &golden_dir,
              bool generate_golden_files)
{
    testing::RegisterTest(test_dir.string().c_str(),
                          test_file.string().c_str(),
                          nullptr,
                          test_file.string().c_str(),
                          __FILE__,
                          __LINE__,
                          // Important to use the fixture type as the return type here.
                          [=]() -> AliveTest * {
                              return new AliveTest(test_dir,
                                                   test_file,
                                                   golden_dir,
                                                   generate_golden_files);
                          });
}

Tester::Tester(const fs::path &test_files_dir,
               const fs::path &golden_files_dir,
               bool generate_golden_files)
{
    using recursive_directory_iterator = std::filesystem::recursive_directory_iterator;
    for (const auto &dir_entry : recursive_directory_iterator(test_files_dir)) {
        auto path = dir_entry.path();
        if ((path.extension() == ".json" || path.extension() == ".alive")
            && std::filesystem::is_regular_file(path)) {
            add_test(test_files_dir,
                     std::filesystem::relative(path, test_files_dir),
                     golden_files_dir,
                     generate_golden_files);
        }
    }
}

Tester::~Tester() {}
} // namespace alive
