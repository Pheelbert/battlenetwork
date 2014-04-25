#pragma once
#include <memory>
using std::shared_ptr;

template <class C> void FreeClear(C& cntr)
{
    for (typename C::iterator it = cntr.begin();
        it != cntr.end(); ++it)
    {
        delete *it;
    }
    cntr.clear();
}