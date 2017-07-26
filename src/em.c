#include "em.h"

void em_mrb_init(em *self);
mrb_value em_mrb_method_reply_count(mrb_state *mrb, mrb_value value);
char *em_mrb_value_to_str(em *self, mrb_value *value);

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

  em_mrb_init(self);

  return self;
}

void em_free(em *self)
{
  self->mrb->ud = NULL;
  mrb_close(self->mrb);
  self->mrb = NULL;
  free(self);
}

char *em_mrb_eval(em *self, char *code)
{
  char *res;
  mrb_value value;
  mrb_int ai = mrb_gc_arena_save(self->mrb);

  value = mrb_load_string(self->mrb, code);

  if (self->mrb->exc) {
    value = mrb_obj_value(self->mrb->exc);
  }

  res = em_mrb_value_to_str(self, &value);

  self->mrb->exc = 0;
  mrb_gc_arena_restore(self->mrb, ai);

  return res;
}

void em_string_free(em *self, char *str)
{
  if (str != NULL)
    free(str);
}

char *em_mrb_value_to_str(em *self, mrb_value *value)
{
  char *str;
  enum mrb_vtype type = mrb_type(*value);

  switch (type) {
    case MRB_TT_FIXNUM: {
      asprintf(&str, "(integer) %d\n", mrb_fixnum(*value));
      break;
    }
    case MRB_TT_STRING: {
      asprintf(&str, "(string) %s\n", mrb_str_to_cstr(self->mrb, *value));
      break;
    }
    case MRB_TT_EXCEPTION: {
      asprintf(&str, "(error) %s\n", mrb_str_to_cstr(self->mrb, mrb_inspect(self->mrb, *value)));
      break;
    }
    default: {
      char *inspect = mrb_str_to_cstr(self->mrb, mrb_inspect(self->mrb, *value));
      asprintf(&str, "(other) %s\n", inspect);
    }
  }

  return str;
}

mrb_value em_mrb_method_reply_count(mrb_state *mrb, mrb_value value)
{
  // em *self = (em *)mrb->ud;
  // int count = self->count;

  return mrb_fixnum_value(111);
  // return mrb_fixnum_value(count);
}

void em_mrb_init(em *self)
{
  self->mrb->ud = self;

  struct RClass *class;
  class = mrb_define_class(self->mrb, "Em", self->mrb->object_class);

  mrb_define_class_method(self->mrb, class, "reply_count", em_mrb_method_reply_count, MRB_ARGS_NONE());
}