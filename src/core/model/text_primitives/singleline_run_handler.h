#ifndef SINGLELINE_RUN_HANDLER_H
#define SINGLELINE_RUN_HANDLER_H

#include <modules/skshaper/include/SkShaper.h>

class SingleLineRunHandler : public SkShaper::RunHandler
{
public:
    SingleLineRunHandler(const char *utf8Text, SkPoint offset)
        : fUtf8Text(utf8Text)
        , fOffset(offset)
    {}

    void reset_text();

    void beginLine() override;
    void runInfo(const RunInfo &) override;
    void commitRunInfo() override;
    Buffer runBuffer(const RunInfo &) override;
    void commitRunBuffer(const RunInfo &) override;
    void commitLine() override;

    sk_sp<SkTextBlob> makeBlob();
    SkPoint endPoint() { return fOffset; }
    const SkRect &bbox() const { return m_rect; }

private:
    SkTextBlobBuilder fBuilder;
    char const *const fUtf8Text;
    uint32_t *fClusters;
    int fClusterOffset;
    int fGlyphCount;
    SkScalar fMaxRunAscent;
    SkScalar fMaxRunDescent;
    SkScalar fMaxRunLeading;
    SkPoint fCurrentPosition;
    SkPoint fOffset;
    SkRect m_rect = {};
    SkScalar m_total_advance = 0;
};

#endif // SINGLELINE_RUN_HANDLER_H
