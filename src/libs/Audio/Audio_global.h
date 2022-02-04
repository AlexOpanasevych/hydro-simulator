#ifndef AUDIO_GLOBAL_H
#define AUDIO_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(AUDIO_LIBRARY)
#  define AUDIO_EXPORT Q_DECL_EXPORT
#else
#  define AUDIO_EXPORT Q_DECL_IMPORT
#endif

#endif // AUDIO_GLOBAL_H
