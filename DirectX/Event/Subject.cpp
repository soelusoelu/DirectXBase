#include "Subject.h"
#include "IObserver.h"

Subject::Subject() = default;

Subject::~Subject() = default;

void Subject::addObserver(const ObserverPtr& observer) {
    mObservers.emplace_back(observer);
}

void Subject::removeObserver(const ObserverPtr& observer) {
    for (auto itr = mObservers.begin(); itr != mObservers.end(); ++itr) {
        if (*itr == observer) {
            mObservers.erase(itr);
            return;
        }
    }
}

void Subject::notify() {
    for (auto&& observer : mObservers) {
        observer->onNotify();
    }
}
