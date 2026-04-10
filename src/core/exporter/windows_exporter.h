#ifndef WINDOWSEXPORTER_H
#define WINDOWSEXPORTER_H

#include <memory>
#include <string>
namespace inae::exporter {

struct WindowsExporterData;
class WindowsExporter
{
public:
    enum InputFormat { e_RGBA, e_ARGB };
    WindowsExporter(int width,
                    int height,
                    float frame_rate,
                    int total_frames,
                    InputFormat format,
                    const std::string &file_path);

    bool write_frame(const unsigned char *data);
    ~WindowsExporter();

private:
    void init();
    void deinit();

private:
    std::unique_ptr<WindowsExporterData> m_data;
};

} // namespace inae::exporter
#endif // WINDOWSEXPORTER_H
