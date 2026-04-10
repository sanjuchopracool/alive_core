#include <algorithm>
#include <core/tagging/tag.h>
#include <core/tagging/taggable.h>
#include <core/alive_types/common_types.h>

namespace alive {

Taggable::~Taggable()
{
    for (auto *tag : m_tags) {
        tag->remove_taggable(this);
    }
}

const std::string &Taggable::name() const {
    return alive::empty_string();
}

bool Taggable::add_tag(Tag *tag)
{
    return tag->add_taggable(this) && m_tags.emplace(tag).second;
}

bool Taggable::remove_tag(Tag *tag)
{
    return tag->remove_taggable(this) && m_tags.erase(tag);
}

bool Taggable::has_tag(const Tag *tag)
{
    return std::find(m_tags.begin(), m_tags.end(), tag) != m_tags.end();
}

Taggable::Taggable(const Taggable &other) : m_tags(other.m_tags) {}

void Taggable::on_tag_delete(Tag *tag)
{
    m_tags.erase(tag);
}

} // namespace alive
