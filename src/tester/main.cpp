#include "tester.h"
#include <core/initializer.h>
#include <cxxopts.hpp>
#include <filesystem>
#include <format>
#include <gtest/gtest.h>

const std::string k_resources_dir_key{"resources_dir"};
const std::string k_test_files_dir_key{"test_files_dir"};
const std::string k_golden_output_dir_key{"golden_output_dir"};
const std::string k_override_golden_files_key{"override_golden_files"};

float g_rmse_threshold = 0.005;
float g_ssim_threshold = 0.995;

void return_invalid_arg(std::string_view message)
{
    std::cout << message << std::endl;
    exit(-1);
}

void return_runtime_error(std::string_view message)
{
    std::cout << message << std::endl;
    exit(1);
}

void return_invalid_arg(std::string_view message, std::string_view help)
{
    std::cout << message << std::endl;
    std::cout << help << std::endl;
    exit(-1);
}

bool validate_dir(const std::filesystem::path &dir)
{
    return (std::filesystem::exists(dir.parent_path())
            && std::filesystem::is_directory(dir.parent_path()));
}

int main(int argc, char **argv)
{
    ::testing::InitGoogleTest(&argc, argv);

    cxxopts::Options
        options("\nalive_tester",
                "Utility to to test alive for test directory and golden output directoty.");

    // clang-format off
    options.add_options()
            ("resources_dir","Resources directory containing alive resource directory for test files", cxxopts::value<std::string>())
            ("test_files_dir","directory containg alive test files",cxxopts::value<std::string>())
            ("golden_output_dir","Output directory containing golden output for test files",cxxopts::value<std::string>())
            ("override_golden_files","override/create golden files for specified tests")
            ("h,help", "Print usage");
    // clang-format on

    cxxopts::ParseResult result;
    try {
        result = options.parse(argc, argv);
    } catch (const cxxopts::exceptions::exception &e) {
        return_invalid_arg(e.what(), options.help());
    } catch (...) {
        return_invalid_arg(options.help());
    }

    if (result.count("help")) {
        return_invalid_arg(options.help());
    }

    std::string error;

    namespace fs = std::filesystem;
    fs::path resources_dir;
    if (result.count(k_resources_dir_key) == 0) {
        error += std::format("\nERROR: missing {} argument\n", k_resources_dir_key);
    } else {
        resources_dir = result[k_resources_dir_key].as<std::string>();
        if (!validate_dir(resources_dir)) {
            error += std::format("\nERROR: {} : {} does not exist!\n",
                                 k_resources_dir_key,
                                 resources_dir.string());
        }
    }

    fs::path test_directory;
    if (result.count(k_test_files_dir_key) == 0) {
        error += std::format("\nERROR: missing {} argument\n", k_test_files_dir_key);
    } else {
        test_directory = result[k_test_files_dir_key].as<std::string>();
        if (!validate_dir(test_directory)) {
            error += std::format("\nERROR: {} : {} does not exist!\n",
                                 k_test_files_dir_key,
                                 test_directory.string());
        }
    }

    fs::path golden_files_dir;
    if (result.count(k_golden_output_dir_key) == 0) {
        error += std::format("\nERROR: missing {} argument\n", k_golden_output_dir_key);
    } else {
        golden_files_dir = result[k_golden_output_dir_key].as<std::string>();
        if (!validate_dir(golden_files_dir)) {
            error += std::format("\nERROR: {} : {} does not exist!\n",
                                 k_golden_output_dir_key,
                                 golden_files_dir.string());
        }
    }

    if (!error.empty()) {
        return_invalid_arg(error);
    }

    alive::Tester tester(test_directory,
                         golden_files_dir,
                         result.count(k_override_golden_files_key));
    alive::core::Initializer::Params param{spdlog::level::level_enum::debug, resources_dir.string().c_str()};
    alive::core::Initializer core_init(param);

    return RUN_ALL_TESTS();
}
