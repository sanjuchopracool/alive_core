#include <include/core/SkCanvas.h>
#include <include/core/SkData.h>
#include <include/core/SkFont.h>
#include <include/core/SkFontMgr.h>
#include <include/core/SkImage.h>
#include <include/core/SkStream.h>
#include <include/core/SkSurface.h>
#include <include/core/SkTextBlob.h>
#include <include/core/SkTypeface.h>
#include <include/encode/SkPngEncoder.h>
#ifdef __APPLE__
#include <include/ports/SkFontMgr_mac_ct.h>
#endif
#include <iostream>

void draw(SkCanvas *canvas)
{
    const char *fontFamily = nullptr; // Default system family, if it exists.
    SkFontStyle fontStyle;            // Default is normal weight, normal width,  upright slant.

    sk_sp<SkFontMgr> fontMgr;
#ifdef __APPLE__
    fontMgr.reset(SkFontMgr_New_CoreText(nullptr).release());
#endif
    std::cout << "Family count before: " << fontMgr->countFamilies() << std::endl;
    sk_sp<SkTypeface> typeface
        = fontMgr->makeFromFile("/Users/sanju/Downloads/Roboto_Condensed,Rubik/Roboto_Condensed/"
                                "static/RobotoCondensed-ExtraBoldItalic.ttf",
                                0);
    if (!typeface) {
        std::cout << "Could not create typeface" << std::endl;
        return;
    }
    SkString family_name;
    typeface->getFamilyName(&family_name);
    std::cout << "Family count after: " << fontMgr->countFamilies() << std::endl;
    std::cout << "Family name of typeface: " << family_name.c_str() << std::endl;

    SkFont font1(typeface, 64.0f, 1.0f, 0.0f);
    SkFont font2(typeface, 64.0f, 1.5f, 0.0f);
    font1.setEdging(SkFont::Edging::kAntiAlias);
    font2.setEdging(SkFont::Edging::kAntiAlias);

    // Note: MakeFromString may fail to produce expected results if the typeface
    // does not have glyphs for the characters in the string.  The characters
    // will not be kerned or shaped beyond a simple mapping from one Unicode
    // code point to one glyph with a default advance.
    sk_sp<SkTextBlob> blob1 = SkTextBlob::MakeFromString("Skia", font1);
    sk_sp<SkTextBlob> blob2 = SkTextBlob::MakeFromString("Skia", font2);

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
    canvas->drawTextBlob(blob1.get(), 20.0f, 64.0f, paint1);
    canvas->drawTextBlob(blob1.get(), 20.0f, 144.0f, paint2);
    canvas->drawTextBlob(blob2.get(), 20.0f, 224.0f, paint3);
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
    raster(200, 200, draw, "example_core.png");
    return 0;
}
