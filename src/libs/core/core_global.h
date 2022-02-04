#ifndef LIB_CORE_PROJECT_GLOBAL_H
#define LIB_CORE_PROJECT_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(CORE_LIBRARY)
#  define CORE_LIBRARY_EXPORT Q_DECL_EXPORT
#else
#  define CORE_LIBRARY_EXPORT Q_DECL_IMPORT
#endif

#endif // LIB_PROJECT_GLOBAL_H
