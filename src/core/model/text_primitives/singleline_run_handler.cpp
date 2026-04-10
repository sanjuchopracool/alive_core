#include <core/model/text_primitives/singleline_run_handler.h>
#include <include/core/SkFontMetrics.h>

void SingleLineRunHandler::beginLine()
{
    // qDebug() << Q_FUNC_INFO;
    fCurrentPosition = fOffset;
    fMaxRunAscent = 0;
    fMaxRunDescent = 0;
    fMaxRunLeading = 0;
    m_total_advance = 0;
    m_rect = {};
}

void SingleLineRunHandler::runInfo(const RunInfo &info)
{
    // qDebug() << Q_FUNC_INFO;
    SkFontMetrics metrics;
    info.fFont.getMetrics(&metrics);
    fMaxRunAscent = std::min(fMaxRunAscent, metrics.fAscent);
    fMaxRunDescent = std::max(fMaxRunDescent, metrics.fDescent);
    fMaxRunLeading = std::max(fMaxRunLeading, metrics.fLeading);
    // qDebug() << info.fAdvance.x() << info.fAdvance.y();
    // qDebug() << fMaxRunAscent;
}

void SingleLineRunHandler::commitRunInfo()
{
    // qDebug() << Q_FUNC_INFO;
    fCurrentPosition.fY -= fMaxRunAscent;
    // qDebug() << fCurrentPosition.fY;
}

void SingleLineRunHandler::commitRunBuffer(const RunInfo &info)
{
    // qDebug() << Q_FUNC_INFO;
    SkASSERT(0 <= fClusterOffset);
    for (int i = 0; i < fGlyphCount; ++i) {
        SkASSERT(fClusters[i] >= (unsigned) fClusterOffset);
        fClusters[i] -= fClusterOffset;
    }
    fCurrentPosition += info.fAdvance;
}

void SingleLineRunHandler::commitLine()
{
    // qDebug() << Q_FUNC_INFO;
    fOffset += {0, fMaxRunDescent + fMaxRunLeading - fMaxRunAscent};
}

sk_sp<SkTextBlob> SingleLineRunHandler::makeBlob()
{
    return fBuilder.make();
}

SkShaper::RunHandler::Buffer SingleLineRunHandler::runBuffer(const RunInfo &info)
{
    // qDebug() << Q_FUNC_INFO;
    int glyphCount = SkTFitsIn<int>(info.glyphCount) ? info.glyphCount : INT_MAX;
    int utf8RangeSize = SkTFitsIn<int>(info.utf8Range.size()) ? info.utf8Range.size() : INT_MAX;

    const auto &runBuffer = fBuilder.allocRunTextPos(info.fFont, glyphCount, utf8RangeSize);
    if (runBuffer.utf8text && fUtf8Text) {
        memcpy(runBuffer.utf8text, fUtf8Text + info.utf8Range.begin(), utf8RangeSize);
    }
    fClusters = runBuffer.clusters;
    fGlyphCount = glyphCount;
    fClusterOffset = info.utf8Range.begin();
    SkRect rect;
    info.fFont.measureText(runBuffer.utf8text, utf8RangeSize, SkTextEncoding::kUTF8, &rect);
    m_rect.join(rect);
    // qDebug() << "RECT for Text " << runBuffer.utf8text << m_rect.left() << m_rect.top()
    //          << m_rect.width() << m_rect.height();
    if (m_rect.left() < 0) {
        m_rect.offset(-m_rect.left(), 0);
    }

    m_total_advance += info.fAdvance.x();
    m_rect = SkRect::MakeXYWH(0, m_rect.y(), m_total_advance, m_rect.height());
    return {runBuffer.glyphs, runBuffer.points(), nullptr, runBuffer.clusters, fCurrentPosition};
}
