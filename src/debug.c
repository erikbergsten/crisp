#include <crisp/debug.h>
#include <stdlib.h>
#include <stdio.h>

FILE *log_files[3];

void cr_debug_init(FILE * info, FILE * warning, FILE * fatal){
  log_files[cr_level_fatal] = fatal;
  log_files[cr_level_warning] = warning;
  log_files[cr_level_info] = info;
}

char * _prefix(cr_debug_lvl lvl){
  switch(lvl){
    case cr_level_fatal:
      return "FATAL";
    case cr_level_warning:
      return "WARNING";
    case cr_level_info:
      return "INFO";
  }
}
void cr_debug_finish(){
  for(int i = 0; i < 3; i++){
    if(log_files[i] && log_files[i] != stdout){
      fclose(log_files[i]);
    }
  }
}
void cr_debug_log_(cr_debug_lvl lvl, char * format, ...){
  FILE * file = log_files[lvl];
  if(file != NULL){
    va_list args;
    va_start(args, format);
    fprintf(file, "[%s]: ", _prefix(lvl));
    vfprintf(file, format, args);
    fprintf(file, "\n");
    va_end(args);
  }
  if(lvl == cr_level_fatal){
    exit(-1);
  }
}

