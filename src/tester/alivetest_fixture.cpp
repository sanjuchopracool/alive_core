#include "alivetest_fixture.h"
#include "directory_comparator.h"
#include <core/exporter/exporter.h>
#include <core/model/composition.h>
#include <core/serializer/editor_support.h>
#include <core/serializer/lottie_parser.h>
#include <cstdio>
#include <format>
#include <fstream>

extern float g_rmse_threshold;
extern float g_ssim_threshold;

namespace alive {

enum class TestInfoIndex { e_Scale, e_RMSEThreshold, e_SSIMThreshold, e_Last };

void recursive_delete_dir(const fs::path &dir)
{
    for (auto &p : fs::recursive_directory_iterator(dir)) {
        if (fs::is_directory(p)) {
            recursive_delete_dir(p);
        } else if (fs::is_regular_file(p)) {
            std::filesystem::remove(p);
        }
    }
}
void AliveTest::TestBody()
{
    auto full_path = m_test_dir / m_test_file;
    std::ifstream ifs(full_path.c_str());
    ifs.seekg(0, std::ios::end);
    size_t size = ifs.tellg();
    std::string buffer(size, ' ');
    ifs.seekg(0);
    ifs.read(&buffer[0], size);
    alive::AliveProjectInformation setting;
    alive::Messages messages;
    auto comp = alive::LottieParser::parse_alive(buffer.c_str(), setting, messages);
    bool result = false;
    std::string error;
    if (comp) {
        auto golden_path = m_golden_dir / m_test_file;
        golden_path = golden_path.replace_extension();
        if (!fs::exists(golden_path)) {
            if (m_generate_golden_files) {
                if (!fs::create_directories(golden_path)) {
                    error = std::format("Unable to create golden directory {}\n",
                                        golden_path.string());
                }
            } else {
                error = std::format("golden directory {} does not exist!\n", golden_path.string());
            }
        } else if (fs::is_regular_file(golden_path)) {
            error = std::format(
                "golden directory {} exists as a regular file, it must be a directory!\n",
                golden_path.string());
        }

        if (error.empty()) {
            fs::path export_path;
            if (m_generate_golden_files) {
                export_path = golden_path;
            } else {
                // create a temp directorty , if not exist
                export_path = fs::temp_directory_path() / m_test_file;
                export_path = export_path.replace_extension();

                if (!fs::exists(export_path)) {
                    if (!fs::create_directories(export_path)) {
                        error = std::format("Unable to create export directory {}\n",
                                            export_path.string());
                    }
                } else {
                    if (fs::is_regular_file(export_path)) {
                        error = std::format("export directory {} exists as a regular file, it must "
                                            "be a directory!\n",
                                            export_path.string());
                    }
                }
            }

            if (error.empty()) {
                // read info in golden dir if present
                float rmse_threshold = g_rmse_threshold;
                float ssim_threshold = g_ssim_threshold;
                float scale = 1.0f;

                fs::path threshold_path(golden_path / "test.info");
                if (fs::exists(threshold_path)) {
                    std::ifstream infile(threshold_path);
                    int count = 0;
                    if (infile.is_open()) {
                        std::string line;
                        while (std::getline(infile, line)) {
                            if (line.empty()) {
                                continue;
                            }

                            std::istringstream iss(line);
                            TestInfoIndex index = static_cast<TestInfoIndex>(count);
                            switch (index) {
                            case TestInfoIndex::e_Scale:
                                iss >> scale;
                                break;
                            case TestInfoIndex::e_RMSEThreshold:
                                iss >> rmse_threshold;
                                break;
                            case TestInfoIndex::e_SSIMThreshold:
                                iss >> ssim_threshold;
                                break;
                            default:
                                break;
                            }
                            ++count;
                        }
                    } else {
                        error = std::format("could not open file {}, please check permissions!\n",
                                            threshold_path.string());
                        EXPECT_TRUE(false);
                    }
                }

                alive::exporter::Exporter image_exporter = alive::exporter::Exporter(comp.get());
                result = image_exporter.export_as(export_path.string(),
                                                  alive::exporter::Exporter::e_PNGSequence,
                                                  scale);

                if (!m_generate_golden_files) {
                    if (result) {
                        result = compare_directories(golden_path,
                                                     export_path,
                                                     error,
                                                     rmse_threshold,
                                                     ssim_threshold);
                    }

                    // delete only in case of success
                    if (result) {
                        recursive_delete_dir(export_path);
                    }
                }
            }
        }
    } else {
        error = std::format("Unable to create composition from {}\n", full_path.string());
    }

    EXPECT_TRUE(result) << error;
}

} // namespace alive
