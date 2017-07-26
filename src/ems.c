#include "ems.h"
#include "mruby/compile.h"

ems *ems_new()
{
  ems *self;

  self = malloc(sizeof(ems));
  if (self == NULL) {
    return NULL;
  }

  self->mrb = mrb_open();

  if (self->mrb == NULL) {
    free(self);
    return NULL;
  }

  self->mrb->ud = self;

  return self;
}

void ems_free(ems *self)
{
  self->mrb->ud = NULL;
  mrb_close(self->mrb);
  self->mrb = NULL;
  free(self);
}

ems_string ems_string_from_char(char *val)
{
  return (ems_string)val;
}

ems_int ems_string_from_int(int val)
{
  return (ems_int)val;
}

char *ems_string_to_string(ems_string val)
{
  char *res = malloc(sizeof(ems_string));
  sprintf(res, "(string) %s", (char *)val);
  return res;
}

char *ems_int_to_string(ems_int val)
{
  char *res = malloc(sizeof(ems_int));
  sprintf(res, "(int) %d", (int)val);
  return res;
}