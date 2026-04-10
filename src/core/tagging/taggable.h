#ifndef TAGGABLE_H
#define TAGGABLE_H

#include <set>
#include <string>
#include <core/alive_types/class_helpers.h>
namespace alive {

class Tag;

class Taggable
{
    ONLY_CLONABLE(Taggable)
public:
    using itr = std::set<Tag *>::iterator;
    Taggable() = default;
    ~Taggable();

    virtual const std::string &name() const;

    bool add_tag(Tag *tag);
    bool remove_tag(Tag *tag);
    bool has_tag(const Tag *tag);

    itr begin() const { return m_tags.cbegin(); }
    itr end() const { return m_tags.cend(); }

protected:
    Taggable(const Taggable& other);

private:
    void on_tag_delete(Tag *tag);

private:
    std::set<Tag *> m_tags;
    friend class Tag;
};

} // namespace alive
#endif // TAGGABLE_H
