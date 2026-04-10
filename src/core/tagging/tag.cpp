#include <algorithm>
#include <core/tagging/tag.h>
#include <core/tagging/taggable.h>

namespace alive {

Tag::~Tag()
{
    for (auto *entity : m_entities) {
        entity->on_tag_delete(this);
    }
}

Taggable *Tag::at(int index)
{
    if (index >= 0 && index < m_entities.size()) {
        return m_entities.at(index);
    }

    return nullptr;
}

bool Tag::add_taggable(Taggable *entity)
{
    if (std::find(m_entities.cbegin(), m_entities.cend(), entity) == m_entities.cend()) {
        m_entities.emplace_back(entity);
        notify_observers();
        return true;
    }

    return false;
}

bool Tag::remove_taggable(Taggable *entity)
{
    auto it = std::remove(m_entities.begin(), m_entities.end(), entity);
    bool removed = it != m_entities.end();
    m_entities.erase(it, m_entities.end());
    if (removed) {
        notify_observers();
    }
    return removed;
}

} // namespace alive
