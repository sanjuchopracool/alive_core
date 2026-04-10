#include "windows_exporter.h"

// clang-format off
#include <Windows.h>
#include <mfapi.h>
#include <mfidl.h>
#include <Mfreadwrite.h>
#include <mferror.h>
// clang-format on

#include <iostream>

#pragma comment(lib, "mfreadwrite")
#pragma comment(lib, "mfplat")
#pragma comment(lib, "mfuuid")

template<class T>
void SafeRelease(T **ppT)
{
    if (*ppT) {
        (*ppT)->Release();
        *ppT = NULL;
    }
}

HRESULT InitializeSinkWriter(IMFSinkWriter **ppWriter,
                             DWORD *pStreamIndex,
                             UINT32 VIDEO_WDITH,
                             UINT32 VIDEO_HEIGHT,
                             UINT32 VIDEO_FPS,
                             GUID VIDEO_INPUT_FORMAT,
                             GUID VIDEO_ENCODING_FORMAT,
                             UINT32 VIDEO_BIT_RATE,
                             const std::string &path)
{
    *ppWriter = NULL;
    *pStreamIndex = NULL;

    IMFSinkWriter *pSinkWriter = NULL;
    IMFMediaType *pMediaTypeOut = NULL;
    IMFMediaType *pMediaTypeIn = NULL;
    DWORD streamIndex;

    std::wstring temp = std::wstring(path.begin(), path.end());
    HRESULT hr = MFCreateSinkWriterFromURL(temp.c_str(), NULL, NULL, &pSinkWriter);

    // Set the output media type.
    if (SUCCEEDED(hr)) {
        hr = MFCreateMediaType(&pMediaTypeOut);
    }
    if (SUCCEEDED(hr)) {
        hr = pMediaTypeOut->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Video);
    }
    if (SUCCEEDED(hr)) {
        hr = pMediaTypeOut->SetGUID(MF_MT_SUBTYPE, VIDEO_ENCODING_FORMAT);
    }
    if (SUCCEEDED(hr)) {
        hr = pMediaTypeOut->SetUINT32(MF_MT_AVG_BITRATE, VIDEO_BIT_RATE);
    }
    if (SUCCEEDED(hr)) {
        hr = pMediaTypeOut->SetUINT32(MF_MT_INTERLACE_MODE, MFVideoInterlace_Progressive);
    }
    if (SUCCEEDED(hr)) {
        hr = MFSetAttributeSize(pMediaTypeOut, MF_MT_FRAME_SIZE, VIDEO_WDITH, VIDEO_HEIGHT);
    }
    if (SUCCEEDED(hr)) {
        hr = MFSetAttributeRatio(pMediaTypeOut, MF_MT_FRAME_RATE, VIDEO_FPS, 1);
    }
    if (SUCCEEDED(hr)) {
        hr = MFSetAttributeRatio(pMediaTypeOut, MF_MT_PIXEL_ASPECT_RATIO, 1, 1);
    }
    if (SUCCEEDED(hr)) {
        hr = pSinkWriter->AddStream(pMediaTypeOut, &streamIndex);
    }

    // Set the input media type.
    if (SUCCEEDED(hr)) {
        hr = MFCreateMediaType(&pMediaTypeIn);
    }
    if (SUCCEEDED(hr)) {
        hr = pMediaTypeIn->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Video);
    }
    if (SUCCEEDED(hr)) {
        hr = pMediaTypeIn->SetGUID(MF_MT_SUBTYPE, VIDEO_INPUT_FORMAT);
    }
    if (SUCCEEDED(hr)) {
        hr = pMediaTypeIn->SetUINT32(MF_MT_INTERLACE_MODE, MFVideoInterlace_Progressive);
    }
    if (SUCCEEDED(hr)) {
        hr = MFSetAttributeSize(pMediaTypeIn, MF_MT_FRAME_SIZE, VIDEO_WDITH, VIDEO_HEIGHT);
    }
    if (SUCCEEDED(hr)) {
        hr = MFSetAttributeRatio(pMediaTypeIn, MF_MT_FRAME_RATE, VIDEO_FPS, 1);
    }
    if (SUCCEEDED(hr)) {
        hr = MFSetAttributeRatio(pMediaTypeIn, MF_MT_PIXEL_ASPECT_RATIO, 1, 1);
    }

    if (SUCCEEDED(hr)) {
        GUID subtype = GUID_NULL;
        UINT32 width = 0;
        UINT32 height = 0;
        LONG lStride = 0;
        // Get the subtype and the image size.
        hr = pMediaTypeIn->GetGUID(MF_MT_SUBTYPE, &subtype);
        if (SUCCEEDED(hr)) {
            hr = MFGetAttributeSize(pMediaTypeIn, MF_MT_FRAME_SIZE, &width, &height);
        }
        if (SUCCEEDED(hr)) {
            hr = MFGetStrideForBitmapInfoHeader(subtype.Data1, width, &lStride);
        }

        // Set the attribute for later reference.
        if (SUCCEEDED(hr)) {
            (void) pMediaTypeIn->SetUINT32(MF_MT_DEFAULT_STRIDE, -lStride);
        }
    }

    if (SUCCEEDED(hr)) {
        hr = pSinkWriter->SetInputMediaType(streamIndex, pMediaTypeIn, NULL);
    }

    // Tell the sink writer to start accepting data.
    if (SUCCEEDED(hr)) {
        hr = pSinkWriter->BeginWriting();
    }

    // Return the pointer to the caller.
    if (SUCCEEDED(hr)) {
        *ppWriter = pSinkWriter;
        (*ppWriter)->AddRef();
        *pStreamIndex = streamIndex;
    }

    SafeRelease(&pSinkWriter);
    SafeRelease(&pMediaTypeOut);
    SafeRelease(&pMediaTypeIn);
    return hr;
}

HRESULT WriteFrame(IMFSinkWriter *pWriter,
                   DWORD streamIndex,
                   const LONGLONG &rtStart, // Time stamp.
                   UINT32 VIDEO_WIDTH,
                   UINT32 VIDEO_HEIGHT,
                   UINT64 VIDEO_FRAME_DURATION,
                   const unsigned char *videoFrameBuffer)
{
    IMFSample *pSample = NULL;
    IMFMediaBuffer *pBuffer = NULL;

    const LONG cbWidth = 4 * VIDEO_WIDTH;
    const DWORD cbBuffer = cbWidth * VIDEO_HEIGHT;

    BYTE *pData = NULL;

    // Create a new memory buffer.
    HRESULT hr = MFCreateMemoryBuffer(cbBuffer, &pBuffer);

    // Lock the buffer and copy the video frame to the buffer.
    if (SUCCEEDED(hr)) {
        hr = pBuffer->Lock(&pData, NULL, NULL);
    }
    if (SUCCEEDED(hr)) {
        hr = MFCopyImage(pData,            // Destination buffer.
                         cbWidth,          // Destination stride.
                         videoFrameBuffer, // First row in source image.
                         cbWidth,          // Source stride.
                         cbWidth,          // Image width in bytes.
                         VIDEO_HEIGHT      // Image height in pixels.
        );
    }
    if (pBuffer) {
        pBuffer->Unlock();
    }

    // Set the data length of the buffer.
    if (SUCCEEDED(hr)) {
        hr = pBuffer->SetCurrentLength(cbBuffer);
    }

    // Create a media sample and add the buffer to the sample.
    if (SUCCEEDED(hr)) {
        hr = MFCreateSample(&pSample);
    }
    if (SUCCEEDED(hr)) {
        hr = pSample->AddBuffer(pBuffer);
    }

    // Set the time stamp and the duration.
    if (SUCCEEDED(hr)) {
        hr = pSample->SetSampleTime(rtStart);
    }
    if (SUCCEEDED(hr)) {
        hr = pSample->SetSampleDuration(VIDEO_FRAME_DURATION);
    }

    // Send the sample to the Sink Writer.
    if (SUCCEEDED(hr)) {
        hr = pWriter->WriteSample(streamIndex, pSample);
    }

    SafeRelease(&pSample);
    SafeRelease(&pBuffer);
    return hr;
}

namespace inae::exporter {

struct WindowsExporterData
{
    WindowsExporterData(int width,
                        int height,
                        float frame_rate,
                        int total_frames,
                        WindowsExporter::InputFormat format,
                        const std::string &file_path)
        : m_width(width)
        , m_height(height)
        , m_frame_rate(frame_rate)
        , m_total_frames(total_frames)
        , m_format(format)
        , m_file_path(file_path)
    {
        m_frame_duration = 10 * 1000 * 1000 / (int) m_frame_rate;
        m_bit_rate = m_width * m_height * 0.5;
    }
    const int m_width;
    const int m_height;
    const float m_frame_rate;
    const int m_total_frames;
    WindowsExporter::InputFormat m_format;
    IMFSinkWriter *m_sink_writer = nullptr;
    DWORD m_sink_stream;
    UINT64 m_frame_duration = 0;
    HRESULT m_hr;
    LONGLONG rtStart = 0;
    const std::string m_file_path;
    uint32_t m_bit_rate = 3000000;
};

WindowsExporter::WindowsExporter(int width,
                                 int height,
                                 float frame_rate,
                                 int total_frames,
                                 InputFormat format,
                                 const std::string &file_path)
    : m_data(std::make_unique<WindowsExporterData>(width,
                                                   height,
                                                   frame_rate,
                                                   total_frames,
                                                   format,
                                                   file_path))
{
    init();
}

bool WindowsExporter::write_frame(const unsigned char *data)
{
    bool success = false;
    m_data->m_hr = WriteFrame(m_data->m_sink_writer,
                              m_data->m_sink_stream,
                              m_data->rtStart,
                              m_data->m_width,
                              m_data->m_height,
                              m_data->m_frame_duration,
                              data);
    if (FAILED(m_data->m_hr)) {
    } else {
        m_data->rtStart += m_data->m_frame_duration;
        success = true;
    }
    return success;
}

WindowsExporter::~WindowsExporter()
{
    deinit();
}

void WindowsExporter::init()
{
    m_data->m_hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
    if (SUCCEEDED(m_data->m_hr)) {
        m_data->m_hr = MFStartup(MF_VERSION);
        if (SUCCEEDED(m_data->m_hr)) {
            GUID input_format = MFVideoFormat_ARGB32;
            m_data->m_hr = InitializeSinkWriter(&m_data->m_sink_writer,
                                                &m_data->m_sink_stream,
                                                m_data->m_width,
                                                m_data->m_height,
                                                m_data->m_frame_rate,
                                                input_format,
                                                MFVideoFormat_WMV3,
                                                3000000,
                                                m_data->m_file_path);
            if (SUCCEEDED(m_data->m_hr)) {
            }
        }
    }
}

void WindowsExporter::deinit()
{
    if (m_data->m_sink_stream) {
        if (SUCCEEDED(m_data->m_hr)) {
            m_data->m_hr = m_data->m_sink_writer->Finalize();
        }

        SafeRelease(&m_data->m_sink_writer);
        MFShutdown();
    }
    CoUninitialize();
}

} // namespace inae::exporter
