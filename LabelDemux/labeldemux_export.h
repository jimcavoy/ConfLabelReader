
#ifndef LABELDEMUX_EXPORT_H
#define LABELDEMUX_EXPORT_H

#ifdef LABELDEMUX_STATIC_DEFINE
#  define LABELDEMUX_EXPORT
#  define LABELDEMUX_NO_EXPORT
#else
#  ifndef LABELDEMUX_EXPORT
#    ifdef LabelDemux_EXPORTS
        /* We are building this library */
#      define LABELDEMUX_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define LABELDEMUX_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef LABELDEMUX_NO_EXPORT
#    define LABELDEMUX_NO_EXPORT 
#  endif
#endif

#ifndef LABELDEMUX_DEPRECATED
#  define LABELDEMUX_DEPRECATED __declspec(deprecated)
#endif

#ifndef LABELDEMUX_DEPRECATED_EXPORT
#  define LABELDEMUX_DEPRECATED_EXPORT LABELDEMUX_EXPORT LABELDEMUX_DEPRECATED
#endif

#ifndef LABELDEMUX_DEPRECATED_NO_EXPORT
#  define LABELDEMUX_DEPRECATED_NO_EXPORT LABELDEMUX_NO_EXPORT LABELDEMUX_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef LABELDEMUX_NO_DEPRECATED
#    define LABELDEMUX_NO_DEPRECATED
#  endif
#endif

#endif /* LABELDEMUX_EXPORT_H */
