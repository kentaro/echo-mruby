#include "em.h"
#include "mruby/compile.h"

em *em_new()
{
  em *self;

  self = malloc(sizeof(em));
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

void em_free(em *self)
{
  self->mrb->ud = NULL;
  mrb_close(self->mrb);
  self->mrb = NULL;
  free(self);
}

em_string em_string_from_char(char *val)
{
  return (em_string)val;
}

em_int em_string_from_int(int val)
{
  return (em_int)val;
}

char *em_string_to_string(em_string val)
{
  char *res = malloc(sizeof(em_string));
  sprintf(res, "(string) %s", (char *)val);
  return res;
}

char *em_int_to_string(em_int val)
{
  char *res = malloc(sizeof(em_int));
  sprintf(res, "(int) %d", (int)val);
  return res;
}