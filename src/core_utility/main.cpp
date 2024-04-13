#include <core/exporter/exporter.h>
#include <core/inae_version.h>
#include <core/initializer.h>
#include <core/model/composition.h>
#include <core/serializer/inae_parser.h>
#include <cxxopts.hpp>
#include <filesystem>
#include <fmt/format.h>
#include <fstream>
#include <iostream>

using namespace inae;
const std::string k_resources_key{"resources"};
const std::string k_file_key{"file"};
const std::string k_scale_key{"scale"};
const std::string k_output_dir_key{"outputdir"};
const std::string k_output_file_key{"outputfile"};

void return_invalid_arg(std::string_view message) {
    std::cout << message << std::endl;
    exit(-1);
}

void return_runtime_error(std::string_view message) {
    std::cout << message << std::endl;
    exit(1);
}

void return_invalid_arg(std::string_view message, std::string_view help) {
    std::cout << message << std::endl;
    std::cout << help << std::endl;
    exit(-1);
}

int main(int argc, char *argv[])
{
    cxxopts::Options options("core_utility", "Utility to render inae projects");

    options.add_options()
        ("r,resources", "Resources directory containing inae_resources", cxxopts::value<std::string>())
        ("s,scale", "Scale", cxxopts::value<float>()->default_value("1.0"))
        ("f,file", "Input json file absolute path", cxxopts::value<std::string>())
        ("w,outputfile", "Output file path for exported webp file", cxxopts::value<std::string>())
        ("o,outputdir", "Output directory for exported png", cxxopts::value<std::string>())
        ("h,help", "Print usage");

    cxxopts::ParseResult result;
    try {
        result = options.parse(argc, argv);
    } catch(const cxxopts::exceptions::exception& e) {
        return_invalid_arg(e.what(), options.help());
    } catch(...) {
        return_invalid_arg(options.help());
    }

    if (result.count("help"))
    {
        return_invalid_arg(options.help());
    }

    std::string input_file_path;
    if (result.count(k_file_key) == 0)
    {
        return_invalid_arg("missing file argument\n", options.help());
    } else {
        input_file_path = result[k_file_key].as<std::string>();
        if (!std::filesystem::exists(input_file_path) || !std::filesystem::is_regular_file(input_file_path)) {
         return_invalid_arg(fmt::format("input file {} does not exist\n", input_file_path), options.help());
        }
    }

    std::string output_dir_path;
    if (result.count(k_output_dir_key))
    {
        output_dir_path = result[k_output_dir_key].as<std::string>();
        if (!std::filesystem::exists(output_dir_path) || !std::filesystem::is_directory(output_dir_path)) {
            return_invalid_arg(fmt::format("output directory {} does not exist\n", output_dir_path), options.help());
        }
    }

    std::filesystem::path output_file_path;
    if (result.count(k_output_file_key))
    {
         output_file_path = result[k_output_file_key].as<std::string>();
        if (!std::filesystem::exists(output_file_path.parent_path()) || !std::filesystem::is_directory(output_file_path.parent_path())) {
            return_invalid_arg(fmt::format("output file directory does not exist for path {} \n", output_file_path.string()), options.help());
        }
    }

    if (output_file_path.empty() && output_dir_path.empty()) {
        return_invalid_arg("one of outputdir or outputfile argument is required\n", options.help());
    }

    std::string resources_dir;
    if (result.count(k_resources_key)) {
        resources_dir = result[k_resources_key].as<std::string>();
    }

    float scale = 1.0;
    if (result.count(k_scale_key)) {
        scale = result[k_scale_key].as<float>();
    }

    if (scale < 0.1) {
        return_invalid_arg(fmt::format("invalid scale {}, scale should be greater than 0.1\n", scale), options.help());
    }

    if (resources_dir.empty()) {
        // default path
    }

    core::Initializer core_init(resources_dir);
    std::ifstream ifs(input_file_path.c_str());
    ifs.seekg(0, std::ios::end);
    size_t size = ifs.tellg();
    std::string buffer(size, ' ');
    ifs.seekg(0);
    ifs.read(&buffer[0], size);
    inae::core::parser::InaeProjectInformation setting;
    inae::core::parser::ParserMessages messages;
    auto comp = inae::core::parser::parse_inae(buffer.c_str(), setting, messages);
    if (comp) {
        exporter::Exporter image_exporter = exporter::Exporter(comp.get());
        if (!output_dir_path.empty())
        {
            image_exporter.export_as(output_dir_path);
        }

        if (!output_file_path.empty())
        {
            image_exporter.export_as(output_file_path.string(), exporter::Exporter::e_WebM);
        }

    } else {
        return_runtime_error(fmt::format("Unable to create composition from {}\n", input_file_path));
    }
    return 0;
}
