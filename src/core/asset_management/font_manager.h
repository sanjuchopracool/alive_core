#ifndef FONT_MANAGER_H
#define FONT_MANAGER_H

#include "include/core/SkRefCnt.h"
#include <boost/noncopyable.hpp>
#include <memory>

class SkTypeface;

namespace inae::core {
struct FontManagerPrivateData;
struct DatabaseAssetId;
using FontImpl = SkTypeface;
class FontManager : boost::noncopyable
{
public:
    ~FontManager();

    static void init();
    static FontManager &instance();

    void load_system_fonts();
    std::size_t families_count() const;
    int family_index(const std::string &name) const;
    std::pair<int, int> default_index() const;
    const std::string& family_name(size_t family_id) const;
    bool family_contains_system_fonts(size_t family_id) const;
    bool family_contains_db_fonts(size_t family_id) const;

    std::size_t style_count(size_t family_id) const;
    std::string style_name(size_t family_id, size_t font_id) const;
    int style_index(size_t family_id, const std::string &name) const;
    bool is_system(size_t family_id, size_t font_id) const;
    FontImpl *font_impl(size_t family_id, size_t font_id) const;
    void add_typeface(sk_sp<SkTypeface> typeface, const DatabaseAssetId &db_id);

private:
    FontManager();

private:
    std::unique_ptr<FontManagerPrivateData> m_d;
};
} // namespace inae::core
#endif // FONT_MANAGER_H
