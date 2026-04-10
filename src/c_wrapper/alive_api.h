#ifndef ALIVE_CORE_API_H
#define ALIVE_CORE_API_H

#include <stdint.h>

#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) \
    || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#define ALIVE_CORE_DLL_EXPORT __declspec(dllexport)
#define Q_DECL_IMPORT __declspec(dllimport)
#else
#define ALIVE_CORE_DLL_EXPORT __attribute__((visibility("default")))
#define Q_DECL_IMPORT __attribute__((visibility("default")))
#endif

#if defined(ALIVE_CORE_SHARED_LIBRARY)
#define ALIVE_CORE_SHARED_EXPORT ALIVE_CORE_DLL_EXPORT
#else
#define ALIVE_CORE_SHARED_EXPORT Q_DECL_IMPORT
#endif

#ifdef __cplusplus
extern "C" {
#endif

// NOLINTBEGIN
enum log_level { trace, debug, info, warn, err, critical, off };
// NOLINTEND

typedef struct
{
    uint8_t r; /**< red */
    uint8_t g; /**< green */
    uint8_t b; /**< blue */
    uint8_t a; /**< alpha */
} alive_color;

/**
 * @brief ALIVE_init
 * @param resources_path
 * @param error         : user should allocate this memory, should be 1024 bytes
 * @param level
 * @return
 */

ALIVE_CORE_SHARED_EXPORT uint8_t alive_init(const char *resources_path,
                                           char *error,
                                           enum log_level level);
ALIVE_CORE_SHARED_EXPORT uint8_t alive_de_init(char *error);

ALIVE_CORE_SHARED_EXPORT void *alive_open_composition(const char *path, char *error);
ALIVE_CORE_SHARED_EXPORT void alive_close_composition(void *comp, char *error);

ALIVE_CORE_SHARED_EXPORT int32_t alive_compoistion_width(void *comp);
ALIVE_CORE_SHARED_EXPORT int32_t alive_compoistion_height(void *comp);
ALIVE_CORE_SHARED_EXPORT int32_t alive_composition_first_frame(void *comp);
ALIVE_CORE_SHARED_EXPORT int32_t alive_composition_last_frame(void *comp);
ALIVE_CORE_SHARED_EXPORT float alive_composition_fps(void *comp);

ALIVE_CORE_SHARED_EXPORT const uint8_t *alive_composition_draw(
    void *comp, float time, int32_t width, int32_t height, alive_color color);

#ifdef __cplusplus
}
#endif

#endif // ALIVE_CORE_API_H
