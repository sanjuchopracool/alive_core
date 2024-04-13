#ifndef INAE_CORE_API_H
#define INAE_CORE_API_H

#include <stdint.h>

#if _WIN32
#include <windows.h>
#else
#include <pthread.h>
#include <unistd.h>
#endif

#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) \
    || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#define INAE_CORE_DLL_EXPORT __declspec(dllexport)
#define Q_DECL_IMPORT __declspec(dllimport)
#else
#define INAE_CORE_DLL_EXPORT __attribute__((visibility("default")))
#define Q_DECL_IMPORT __attribute__((visibility("default")))
#endif

#if defined(INAE_CORE_SHARED_LIBRARY)
#define INAE_CORE_SHARED_EXPORT INAE_CORE_DLL_EXPORT
#else
#define INAE_CORE_SHARED_EXPORT Q_DECL_IMPORT
#endif

#ifdef __cplusplus
extern "C" {
#endif

INAE_CORE_SHARED_EXPORT uint8_t inae_init(char *error);
INAE_CORE_SHARED_EXPORT uint8_t inae_de_init(char *error);
INAE_CORE_SHARED_EXPORT int add(int a, int b);

#ifdef __cplusplus
}
#endif

#endif // INAE_CORE_API_H
