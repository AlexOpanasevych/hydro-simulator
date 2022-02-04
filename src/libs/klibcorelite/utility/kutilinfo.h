#ifndef KUTILINFO_H
#define KUTILINFO_H

#include <QMetaType>

template<typename T>
struct is_registered
{
    enum
    {
        value =
#if QT_VERSION >= 0x050000 // Qt 5.0.0
            QMetaTypeId2<T>::Defined
#elif QT_VERSION >= 0x040000 // Qt 4.0.0
            QMetaTypeId2<T>::Defined
#endif
    };
};


#endif // KUTILINFO_H

