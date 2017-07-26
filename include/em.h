#ifndef EM_H
#define EM_H

#include <stdlib.h>

#include "mruby.h"
#include "mruby/compile.h"
#include "mruby/string.h"

typedef struct em_t {
  mrb_state *mrb;
} em;

typedef int em_int;
typedef char* em_string;

em *em_new();
void em_free(em *self);

em_string em_string_from_char(char *val);
em_int em_string_from_int(int val);
em_int em_string_from_null(int val);

char *em_string_to_string(em_string val);
char *em_int_to_string(em_int val);

#endif // EM_H