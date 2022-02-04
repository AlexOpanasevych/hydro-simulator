#ifndef TORPEDO_GLOBAL_H
#define TORPEDO_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(TORPEDO_LIBRARY)
#  define TORPEDO_EXPORT Q_DECL_EXPORT
#else
#  define TORPEDO_EXPORT Q_DECL_IMPORT
#endif

#endif // TORPEDO_GLOBAL_H
