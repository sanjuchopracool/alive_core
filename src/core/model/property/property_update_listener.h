#ifndef PROPERTY_UPDATE_LISTENER_H
#define PROPERTY_UPDATE_LISTENER_H

namespace alive::model {

class ISubject;
class IObserver
{
public:
    virtual ~IObserver() {}
    virtual void on_update(ISubject *subject) = 0;
    virtual void observe(ISubject *subject) = 0;
    virtual void stop_observing(ISubject *subject) = 0;

private:
    virtual void on_subject_destroyed(ISubject *subject) = 0;
    friend class SimpleSubject;
};

class ISubject
{
public:
    virtual ~ISubject() {}
    virtual void notify_observers() = 0;

private:
    virtual void on_observer_destroyed(IObserver *observer) = 0;
    virtual void add_observer(IObserver *observer) = 0;
    virtual void remove_observer(IObserver *observer) = 0;
    friend class SimpleObserver;
};

} // namespace alive::model
#endif // PROPERTY_UPDATE_LISTENER_H
