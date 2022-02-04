#ifndef SCENEITEMS_GLOBAL_H
#define SCENEITEMS_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(VESSEL_LIBRARY)
#   define VESSEL_EXPORT Q_DECL_EXPORT
#else
#  define VESSEL_EXPORT Q_DECL_IMPORT
#endif

#endif // SCENEITEMS_GLOBAL_H
