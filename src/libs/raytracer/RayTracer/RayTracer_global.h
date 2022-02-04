#ifndef RAYTRACER_GLOBAL_H
#define RAYTRACER_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(RAYTRACER_LIBRARY)
#  define RAYTRACER_EXPORT Q_DECL_EXPORT
#else
#  define RAYTRACER_EXPORT Q_DECL_IMPORT
#endif

#endif // RAYTRACER_GLOBAL_H
