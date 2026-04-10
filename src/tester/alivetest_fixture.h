#ifndef ALIVETEST_FIXTURE_H
#define ALIVETEST_FIXTURE_H

#include <filesystem>
#include <gtest/gtest.h>

namespace fs = std::filesystem;

namespace alive {
class AliveTest : public testing::Test
{
public:
    explicit AliveTest(const fs::path &test_dir,
                       const fs::path &test_file,
                       const fs::path &golden_dir,
                       bool generate_golden_files)
        : m_test_dir(test_dir)
        , m_test_file(test_file)
        , m_golden_dir(golden_dir)
        , m_generate_golden_files(generate_golden_files)
    {}
    void TestBody() override;

private:
    const fs::path &m_test_dir;
    const fs::path m_test_file;
    const fs::path m_golden_dir;
    bool m_generate_golden_files;
};
} // namespace alive

#endif // ALIVETEST_FIXTURE_H
