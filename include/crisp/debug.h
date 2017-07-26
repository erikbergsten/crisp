#ifndef __cr_debug_h__
#define __cr_debug_h__

#include <stdio.h>
#include <stdarg.h>

typedef enum{
  cr_level_fatal,
  cr_level_warning,
  cr_level_info
} cr_debug_lvl;

void cr_debug_init(FILE * info, FILE * warning, FILE * fatal);
void cr_debug_finish();
#define cr_debug_init_std() cr_debug_init(stdout, stdout, stdout)
void cr_debug_log_(cr_debug_lvl lvl, char * format, ...);
#ifdef CR_DEBUG
  #define cr_debug_log(lvl, format, ...) cr_debug_log_(lvl, format, ##__VA_ARGS__)
  #define cr_debug_info(format, ...) cr_debug_log_(cr_level_info, format, ##__VA_ARGS__)
  #define cr_debug_warn(format, ...) cr_debug_log_(cr_level_warning, format, ##__VA_ARGS__)
  #define cr_debug_fail(format, ...) cr_debug_log_(cr_level_fatal, format, ##__VA_ARGS__)
#else
  #define cr_debug_log(lvl, format, ...)
  #define cr_debug_info(format, ...)
  #define cr_debug_warn(format, ...)
  #define cr_debug_fail(format, ...)
#endif

#endif /* __cr_debug_h__ */
