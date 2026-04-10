#include <include/core/SkCanvas.h>
#include <include/core/SkData.h>
#include <include/core/SkFont.h>
#include <include/core/SkFontMetrics.h>
#include <include/core/SkFontMgr.h>
#include <include/core/SkImage.h>
#include <include/core/SkMatrix.h>
#include <include/core/SkRSXform.h>
#include <include/core/SkStream.h>
#include <include/core/SkSurface.h>
#include <include/core/SkTextBlob.h>
#include <include/core/SkTypeface.h>
#include <include/encode/SkPngEncoder.h>
#include <modules/skshaper/include/SkShaper.h>
#include <modules/skshaper/include/SkShaper_harfbuzz.h>
#include <modules/skunicode/include/SkUnicode.h>
#include <modules/skunicode/include/SkUnicode_icu.h>
#include <third_party/externals/harfbuzz/src/hb-aat.h>
#include <third_party/externals/harfbuzz/src/hb-ot.h>
#include <third_party/externals/harfbuzz/src/hb.h>
#ifdef _WINDOWS
#include <include/ports/SkTypeface_win.h>
#elif __APPLE__
#include <include/ports/SkFontMgr_mac_ct.h>
#else
#include <include/ports/SkFontMgr_directory.h>
#endif

#include <iostream>

namespace {
sk_sp<SkUnicode> get_unicode()
{
    auto unicode = SkUnicodes::ICU::Make();
    if (unicode) {
        return unicode;
    } else {
        std::cout << "Invalid unicode,  not available" << std::endl;
        return nullptr;
    }
}

} // namespace

bool DrawGlyphs(SkCanvas *canvas, const SkFont &font, SkPaint &paint)
{
    std::unique_ptr<SkShaper> shaper = SkShapers::HB::ShaperDrivenWrapper(get_unicode(), nullptr);
    if (!shaper) {
        std::cout << "Shaper not available" << std::endl;
    }

    // \n is treated as space
    std::string name("अधिकारी Chopra\n\n\nKuku");
    const char *text = name.c_str();
    size_t textBytes = name.size();

    SkFontMetrics fm;
    font.getMetrics(&fm);

    std::cout << "fAscent =" << fm.fAscent << "\nfDescent = " << fm.fDescent
              << "\nfTop = " << fm.fTop << "\nfBottom = " << fm.fBottom << "\nfXMin = " << fm.fXMin
              << "\nfXMin = " << fm.fXMax << "\nfXHeight = " << fm.fXHeight
              << "\nfCapHeight = " << fm.fCapHeight << "\nfLeading = " << fm.fLeading
              << "\nfAvgCharWidth = " << fm.fAvgCharWidth << "\n\n";

    // bound height
    std::cout << "(-fm.fAscent + fm.fDescent) = " << (-fm.fAscent + fm.fDescent) << std::endl;
    // bound left , x min

    SkTextBlobBuilderRunHandler textBlobBuilder(text, {0, 0});

    const SkBidiIterator::Level defaultLevel = SkBidiIterator::kLTR;
    std::unique_ptr<SkShaper::BiDiRunIterator> bidi = SkShaper::MakeBiDiRunIterator(text,
                                                                                    textBytes,
                                                                                    defaultLevel);
    SkASSERT(bidi);

    std::unique_ptr<SkShaper::LanguageRunIterator> language
        = SkShaper::MakeStdLanguageRunIterator(text, textBytes);
    SkASSERT(language);

    std::unique_ptr<SkShaper::ScriptRunIterator> script
        = SkShapers::HB::ScriptRunIterator(text, textBytes);
    SkASSERT(script);

    std::unique_ptr<SkShaper::FontRunIterator> fontRuns
        = SkShaper::MakeFontMgrRunIterator(text, textBytes, font, SkFontMgr::RefEmpty());
    SkASSERT(fontRuns);

    shaper->shape(
        text, textBytes, *fontRuns, *bidi, *script, *language, nullptr, 0, 400, &textBlobBuilder);
    SkPoint endPoint = textBlobBuilder.endPoint();
    std::cout << "End Point: " << endPoint.x() << " " << endPoint.y() << std::endl;
    sk_sp<const SkTextBlob> blob = textBlobBuilder.makeBlob();
    canvas->drawTextBlob(blob.get(), 20, 0, paint);
    paint.setStyle(SkPaint::kStroke_Style);
    SkRect bounds = blob->bounds();
    std::cout << "Blob rect :" << bounds.left() << " " << bounds.top() << " " << bounds.right()
              << " " << bounds.bottom() << std::endl;
    canvas->drawRect(bounds, paint);

    return true;
} // end of DrawGlyphs

void draw(SkCanvas *canvas)
{
    const char *fontFamily = nullptr; // Default system family, if it exists.
    SkFontStyle fontStyle;            // Default is normal weight, normal width,  upright slant.

    sk_sp<SkFontMgr> fontMgr;
#ifdef _WINDOWS
    fontMgr.reset(SkFontMgr_New_DirectWrite().release());
    sk_sp<SkTypeface> typeface
        = fontMgr->makeFromFile("C:/Users/sanju/alive_resources/fonts/hindi/Hind/Hind-Bold.ttf", 0);
#elif defined(__APPLE__)
    fontMgr = SkFontMgr_New_CoreText(nullptr);
    sk_sp<SkTypeface> typeface
        = fontMgr->makeFromFile("/Users/sanju/alive_resources/fonts/hindi/Hind/Hind-Bold.ttf", 0);
#else
    fontMgr.reset(SkFontMgr_New_Custom_Directory("/usr/share/fonts/").release());
    sk_sp<SkTypeface> typeface
        = fontMgr->makeFromFile("/Users/sanju/alive_resources/fonts/hindi/Hind/Hind-Bold.ttf", 0);
#endif

    if (!typeface) {
        std::cout << "Could not create typeface" << std::endl;
        return;
    }
    SkString family_name;
    typeface->getFamilyName(&family_name);
    std::cout << "Family count after: " << fontMgr->countFamilies() << std::endl;
    std::cout << "Family name of typeface: " << family_name.c_str() << std::endl;

    SkFont font1(typeface, 20.0f, 1.0f, 0.0f);
    SkFont font2(typeface, 30.0f, 1.5f, 0.0f);
    font1.setEdging(SkFont::Edging::kAntiAlias);
    font2.setEdging(SkFont::Edging::kAntiAlias);

    SkPaint paint1, paint2, paint3;

    paint1.setAntiAlias(true);
    paint1.setColor(SK_ColorWHITE);

    paint2.setAntiAlias(true);
    paint2.setColor(SkColorSetARGB(0xFF, 0xDB, 0x44, 0x37));
    paint2.setStyle(SkPaint::kStroke_Style);
    paint2.setStrokeWidth(3.0f);

    paint3.setAntiAlias(true);
    paint3.setColor(SkColorSetARGB(0xFF, 0x0F, 0x9D, 0x58));

    canvas->clear(SK_ColorBLUE);

    canvas->save();
    DrawGlyphs(canvas, font1, paint1);
    canvas->restore();

    //
    canvas->translate(20, 50);

    const char text[] = "Hello";
    int glyphCount = 20;
    SkGlyphID glyphs[20] = {};
    std::vector<SkRSXform> xforms(20, SkRSXform::Make(1, 0, 0, 0));
    font1.textToGlyphs(text, strlen(text), SkTextEncoding::kUTF8, glyphs, glyphCount);
    std::cout << "glyphCount = " << glyphCount << std::endl;
    auto xform = SkRSXform::Make(1, 0, 0, 0);
    for (int i = 0; i < 20; ++i) {
        std::cout << "glayphs id at " << i << " = " << glyphs[i] << std::endl;
        xforms[i] = SkRSXform::Make(1, 0, i * 20, 0);
    }

    canvas->drawGlyphs(5, glyphs, xforms.data(), SkPoint::Make(0, 0), font1, paint1);

    paint1.setColor(SkColors::kRed);
    SkRect rect = SkRect::MakeXYWH(0, 50, 200, 50);
    canvas->drawRect(rect, paint1);
}

//void draw(SkCanvas *canvas)
//{
//    const char *font_family = "Arial"; // Default system family, if it exists.
//    SkFontStyle font_style;            // Default is normal weight, normal width,  upright slant.

//    auto mgr = SkFontMgr::RefEmpty();
//    if (mgr) {
//        sk_sp<SkTypeface> typeface = mgr->makeFromFile("Arial");
//        // clear canvas, transparent
//        if (typeface) {
//            SkFont font1(typeface, 100);
//            //        SkFont font1;
//            //        font1.setSize(100);
//            //        font1.setEdging(SkFont::Edging::kAntiAlias);
//            sk_sp<SkTextBlob> blob1 = SkTextBlob::MakeFromString("Skia", font1);
//            SkPaint paint1;
//            paint1.setAntiAlias(true);
//            paint1.setColor(SkColorSetARGB(0xFF, 0x42, 0x85, 0xF4));
//            // Text draw
//            canvas->drawTextBlob(blob1.get(), 0, 100, paint1);
//        } else {
//            std::cout << "Cannot create typeface" << std::endl;
//        }
//    } else {
//        std::cout << "Cannot create Font Manager" << std::endl;
//    }
//}
void raster(int width, int height, void (*sk_draw)(SkCanvas *), const char *path)
{
    sk_sp<SkSurface> raster_surface = SkSurfaces::Raster(SkImageInfo::MakeN32Premul(width, height));
    SkCanvas *raster_canvas = raster_surface->getCanvas();
    draw(raster_canvas);
    sk_sp<SkImage> img(raster_surface->makeImageSnapshot());
    if (!img) {
        return;
    }
    sk_sp<SkData> png = SkPngEncoder::Encode(nullptr, img.get(), SkPngEncoder::Options());
    if (!png) {
        return;
    }
    SkFILEWStream out(path);
    (void) out.write(png->data(), png->size());
}

using namespace std;
int main()
{
    raster(1920, 1080, draw, "example_core.png");
    return 0;
}
