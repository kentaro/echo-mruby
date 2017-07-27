#ifndef EM_H
#define EM_H

#include <stdlib.h>

#include "mruby.h"
#include "mruby/compile.h"
#include "mruby/string.h"

typedef struct em_t {
  int reply_count;
  char *reply_prefix;
  mrb_state *mrb;
} em;

em *em_new();
void em_free(em *self);
char *em_mrb_eval(em *self, char *code);
void em_string_free(em *self, char *str);

#endif // EM_H