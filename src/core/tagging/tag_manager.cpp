#include <algorithm>
#include <core/tagging/tag_manager.h>
#include <core/tagging/taggable.h>

namespace alive {

TagManager::TagManager(IObserver *listener)
    : model::CascadeUpdateListener(listener)
{
}

TagManager::~TagManager() {}

Tag *TagManager::create_tag(const std::string &name)
{
    if (!tag(name)) {
        m_tags.emplace_back(new Tag(name, ++m_counter, this));
        notify_observers();
        return m_tags.back().get();
    }

    return nullptr;
};

Tag *TagManager::create_tag(const std::string &name, Tag::TagId id)
{
    if (std::find_if(m_tags.cbegin(),
                     m_tags.cend(),
                     [name, id](const TagPtr &ptr) {
                         return ptr->name() == name || ptr->id() == id;
                     })
        == m_tags.end()) {
        m_tags.emplace_back(new Tag(name, id, this));
        m_counter = std::max(m_counter, id);
        notify_observers();
        return m_tags.back().get();
    }

    return nullptr;
}

bool TagManager::delete_tag(Tag::TagId id)
{
    auto it = std::remove_if(m_tags.begin(), m_tags.end(), [id](const TagPtr &ptr) {
        return ptr->id() == id;
    });

    if (it != m_tags.end()) {
        m_tags.erase(it, m_tags.end());
        notify_observers();
        return true;
    }

    return false;
}

Tag *TagManager::at(int index)
{
    if (index >= 0 && index < m_tags.size()) {
        return m_tags.at(index).get();
    }

    return nullptr;
}

int TagManager::index(Tag *tag)
{
    auto it = std::find_if(m_tags.begin(), m_tags.end(), [tag](const TagPtr &p) {
        return p.get() == tag;
    });

    if (it != m_tags.end()) {
        return (it - m_tags.begin());
    }

    return -1;
}

Tag *alive::TagManager::tag(const std::string &name)
{
    auto it = std::find_if(m_tags.begin(), m_tags.end(), [name](const TagPtr &ptr) {
        return ptr->name() == name;
    });

    if (it != m_tags.end()) {
        return it->get();
    }
    return nullptr;
}

Tag *alive::TagManager::tag(Tag::TagId id)
{
    auto it = std::find_if(m_tags.begin(), m_tags.end(), [id](const TagPtr &ptr) {
        return ptr->id() == id;
    });

    if (it != m_tags.end()) {
        return it->get();
    }
    return nullptr;
}

int TagManager::index(const std::string &name) const
{
    auto it = std::find_if(m_tags.cbegin(), m_tags.cend(), [name](const TagPtr &ptr) {
        return ptr->name() == name;
    });

    if (it != m_tags.end()) {
        return (it - m_tags.begin());
    }

    return -1;
}

int TagManager::index(Tag::TagId id) const
{
    auto it = std::find_if(m_tags.cbegin(), m_tags.cend(), [id](const TagPtr &ptr) {
        return ptr->id() == id;
    });

    if (it != m_tags.end()) {
        return (it - m_tags.begin());
    }

    return -1;
}

} // namespace alive
