#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <ctype.h>

#include "SDL.h"

#define sexp_isalpha(x) (isalpha(x))
#define sexp_isxdigit(x) (isxdigit(x))
#define sexp_isdigit(x) (isdigit(x))
#define sexp_tolower(x) (tolower(x))
#define sexp_toupper(x) (toupper(x))

// dont use something like poll
// TODO: remove it
#define SEXP_USE_POLL_PORT 0
#define sexp_poll_input(ctx, port) SDL_Delay(SEXP_POLL_SLEEP_TIME)
#define sexp_poll_output(ctx, port) SDL_Delay(SEXP_POLL_SLEEP_TIME)

#define sexp_default_module_path "."
#define sexp_so_extension "so"
#define sexp_platform "any"
#define sexp_version "1"
#define sexp_release_name "finally"

#define sdl_assert() *((int*)0) = 0;
#ifdef __cplusplus
} /* extern "C" */
#endif