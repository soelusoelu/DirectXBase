#include "Singleton.h"

const int kMaxFinalizerSize = 256;
static int gNumFinalizerSize = 0;
static SingletonFinalizer::FinalizerFunc gFinalizers[kMaxFinalizerSize];

void SingletonFinalizer::addFinalizer(FinalizerFunc func) {
    assert(gNumFinalizerSize < kMaxFinalizerSize);
    gFinalizers[gNumFinalizerSize++] = func;
}

void SingletonFinalizer::finalize() {
    for (int i = gNumFinalizerSize - 1; i >= 0; --i) {
        (*gFinalizers[i])();
    }
    gNumFinalizerSize = 0;
}
