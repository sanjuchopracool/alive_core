#ifndef TAG_MANAGER_H
#define TAG_MANAGER_H

#include <Corrade/Containers/Pointer.h>
#include <core/model/property/update_listener.h>
#include <core/tagging/tag.h>
#include <core/alive_types/class_helpers.h>
#include <string>
#include <vector>

namespace alive {
class TagManager : model::CascadeUpdateListener
{
    NON_COPYABLE(TagManager)
public:
    using TagPtr = Corrade::Containers::Pointer<Tag>;
    TagManager(IObserver *listener = nullptr);
    ~TagManager();

    Tag *tag(const std::string &name);
    Tag *tag(const Tag::TagId id);
    int index(const std::string &name) const;
    int index(Tag::TagId id) const;

    Tag *create_tag(const std::string &name);
    Tag *create_tag(const std::string &name, Tag::TagId id);
    bool delete_tag(Tag::TagId id);

    Tag *at(int index);
    size_t count() const { return m_tags.size(); }
    int index(Tag *tag);

private:
    typedef std::vector<TagPtr> Container;

public:
    typedef Container::iterator iterator;
    typedef Container::const_iterator const_iterator;

    iterator begin() { return m_tags.begin(); }
    iterator end() { return m_tags.end(); }

    const_iterator begin() const { return m_tags.begin(); }
    const_iterator end() const { return m_tags.end(); }

private:
    Container m_tags;
    int m_counter = 0;
};

} // namespace alive

#endif // TAG_MANAGER_H
