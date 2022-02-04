#ifndef VDEBUG_GLOBAL_H
#define VDEBUG_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(VDEBUG_LIBRARY)
#  define VDEBUG_EXPORT Q_DECL_EXPORT
#else
#  define VDEBUG_EXPORT Q_DECL_IMPORT
#endif

#endif // VDEBUG_GLOBAL_H
