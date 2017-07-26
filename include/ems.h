#ifndef EMS_H
#define EMS_H

#include <stdlib.h>

#include "mruby.h"
#include "mruby/compile.h"
#include "mruby/string.h"

typedef struct ems_t {
  mrb_state *mrb;
} ems;

typedef int ems_int;
typedef char* ems_string;

ems *ems_new();
void ems_free(ems *self);

ems_string ems_string_from_char(char *val);
ems_int ems_string_from_int(int val);
ems_int ems_string_from_null(int val);

char *ems_string_to_string(ems_string val);
char *ems_int_to_string(ems_int val);

#endif // EMS_H