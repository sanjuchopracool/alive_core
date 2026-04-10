#ifndef TAG_H
#define TAG_H

#include <core/alive_types/class_helpers.h>
#include <core/model/property/update_listener.h>
#include <string>
#include <vector>

namespace alive {

class Taggable;
class TagManager;

class Tag : public model::SimpleSubject
{
    ONLY_CLONABLE(Tag)
public:
    using TagId = int;

    ~Tag();
    const std::string &name() const { return m_name; }
    void set_name(const std::string &name) { m_name = name; }
    TagId id() const { return m_id; };

    Taggable *at(int index);
    size_t count() const { return m_entities.size(); }
    const std::vector<Taggable *> &entities() const { return m_entities; };
    Tag(const Tag&) = delete;

private:
    Tag(const std::string &name, TagId id, model::IObserver *listener = nullptr)
        : model::SimpleSubject(listener)
        , m_name(name)
        , m_id(id)
    {}

    bool add_taggable(Taggable *entity);
    bool remove_taggable(Taggable *entity);

private:
    std::string m_name;
    TagId m_id;
    std::vector<Taggable *> m_entities;
    friend class Taggable;
    friend class TagManager;
};

} // namespace alive

#endif // TAG_H
