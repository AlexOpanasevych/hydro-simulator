#ifndef SCANABLE_H
#define SCANABLE_H

#include <map>
#include "core_global.h"

class CORE_LIBRARY_EXPORT Scanable
{
public:
    virtual void scan(const std::map<int, double> & objects, int itemId) = 0;
    bool isScanable() {return true;}
    Scanable();
};

#endif // SCANABLE_H
