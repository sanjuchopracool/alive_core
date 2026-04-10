#ifndef UPDATE_LISTENER_H
#define UPDATE_LISTENER_H

#include "property_update_listener.h"
#include <set>

namespace alive::model {

class SimpleObserver : public IObserver
{
public:
    ~SimpleObserver()
    {
        for (ISubject *subject : m_subjects) {
            subject->on_observer_destroyed(this);
        }
        m_subjects.clear();
    }

    void observe(ISubject *subject) override
    {
        if (subject) {
            subject->add_observer(this);
            m_subjects.emplace(subject);
        }
    }

    void stop_observing(ISubject *subject) override
    {
        m_subjects.erase(subject);
        subject->remove_observer(this);
    }

private:
    void on_subject_destroyed(ISubject *subject) override { m_subjects.erase(subject); }
    std::set<ISubject *> m_subjects;
};

class SimpleSubject : public ISubject
{
public:
    SimpleSubject(IObserver *listener = nullptr)
    {
        if (listener)
            m_observers.emplace(listener);
    }

    ~SimpleSubject()
    {
        for (IObserver *observer : m_observers) {
            observer->on_subject_destroyed(this);
        }
        m_observers.clear();
    }

    void notify_observers() override
    {
        for (auto &observer : m_observers)
            observer->on_update(this);
    }

    void mark_dirty(bool dirty) { m_dirty = dirty; }
    bool is_dirty() const { return m_dirty; }

private:
    void add_observer(IObserver *observer) override
    {
        if (observer) {
            m_observers.emplace(observer);
        }
    }

    void remove_observer(IObserver *observer) override
    {
        if (observer) {
            auto it = m_observers.find(observer);
            m_observers.erase(it);
        }
    }

    void on_observer_destroyed(IObserver *observer) override { m_observers.erase(observer); }

protected:
    bool m_dirty = true;

private:
    mutable std::set<IObserver *> m_observers;
    friend class CascadeUpdateListener;
};

class CascadeUpdateListener : public SimpleObserver, public SimpleSubject
{
public:
    CascadeUpdateListener(IObserver *listener = nullptr) { add_observer(listener); }
    void on_update(ISubject *subject) override
    {
        mark_dirty(true);
        for (auto &observer : m_observers)
            observer->on_update(subject);
    }
    ~CascadeUpdateListener() {}
};

} // namespace alive::model
#endif // UPDATE_LISTENER_H
