#ifndef TRAJECTORY_GLOBAL_H
#define TRAJECTORY_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(TRAJECTORY_LIBRARY)
#  define TRAJECTORY_EXPORT Q_DECL_EXPORT
#else
#  define TRAJECTORY_EXPORT Q_DECL_IMPORT
#endif

#endif // TRAJECTORY_GLOBAL_H
