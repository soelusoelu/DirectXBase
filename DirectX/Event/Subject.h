#pragma once

#include <list>
#include <memory>

class IObserver;

class Subject {
    using ObserverPtr = std::shared_ptr<IObserver>;
    using ObserverPtrList = std::list<ObserverPtr>;

public:
    Subject();
    virtual ~Subject();
    void addObserver(const ObserverPtr& observer);
    void removeObserver(const ObserverPtr& observer);

protected:
    void notify();

private:
    ObserverPtrList mObservers;
};
