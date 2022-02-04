#ifndef DATABASE_GLOBAL_H
#define DATABASE_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(DATABASE_LIBRARY)
#  define DATABASE_EXPORT Q_DECL_EXPORT
#else
#  define DATABASE_EXPORT Q_DECL_IMPORT
#endif

#endif // DATABASE_GLOBAL_H
