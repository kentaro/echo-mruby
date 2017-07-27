#include "em.h"

void em_mrb_init(em *self);
char *em_mrb_value_to_str(em *self, mrb_value value);

mrb_value em_mrb_method_reply_count(mrb_state *mrb, mrb_value value);
mrb_value em_mrb_method_set_reply_prefix(mrb_state *mrb, mrb_value value);

em *em_new()
{
  em *self;

  self = malloc(sizeof(em));
  if (self == NULL) {
    return NULL;
  }

  // init members of em struct
  self->reply_count = 0;
  self->reply_prefix = "em>";

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

void em_reply_count_incr(em *self)
{
  self->reply_count++;
}

char *em_mrb_eval(em *self, char *code)
{
  char *res;

  mrb_value value;
  mrbc_context *cxt;

  mrb_int ai = mrb_gc_arena_save(self->mrb);

  // to use `self` in mruby script
  self->mrb->ud = self;

  cxt = mrbc_context_new(self->mrb);
  value = mrb_load_string_cxt(self->mrb, code, cxt);

  // undef: parse error
  // nil:   runtime error
  if (self->mrb->exc != 0 && (mrb_nil_p(value) || mrb_undef_p(value))) {
    value = mrb_obj_value(self->mrb->exc);
  }

  res = em_mrb_value_to_str(self, value);

  self->mrb->exc = 0;
  mrbc_context_free(self->mrb, cxt);
  mrb_gc_arena_restore(self->mrb, ai);

  em_reply_count_incr(self);

  return res;
}

void em_string_free(em *self, char *str)
{
  if (str != NULL)
    free(str);
}

char *em_mrb_value_to_str(em *self, mrb_value value)
{
  char *str;
  enum mrb_vtype type = mrb_type(value);

  if (mrb_undef_p(value) || mrb_nil_p(value)) {
    asprintf(&str, "(nil)");
    return str;
  }

  switch (type) {
    case MRB_TT_FLOAT: {
      asprintf(&str, "%s (float) %f\n", self->reply_prefix, mrb_float(value));
      break;
    }
    case MRB_TT_FIXNUM: {
      asprintf(&str, "%s (integer) %d\n", self->reply_prefix, mrb_fixnum(value));
      break;
    }
    case MRB_TT_STRING: {
      asprintf(&str, "%s (string) %s\n", self->reply_prefix, mrb_str_to_cstr(self->mrb, value));
      break;
    }
    case MRB_TT_ARRAY: {
      char *inspect = mrb_str_to_cstr(self->mrb, mrb_inspect(self->mrb, value));
      asprintf(&str, "%s (array) %s\n", self->reply_prefix, inspect);
      break;
    }
    case MRB_TT_HASH: {
      char *inspect = mrb_str_to_cstr(self->mrb, mrb_inspect(self->mrb, value));
      asprintf(&str, "%s (hash) %s\n", self->reply_prefix, inspect);
      break;
    }
    case MRB_TT_EXCEPTION: {
      asprintf(&str, "%s (error) %s\n", self->reply_prefix, mrb_str_to_cstr(self->mrb, mrb_inspect(self->mrb, value)));
      break;
    }
    default: {
      char *inspect = mrb_str_to_cstr(self->mrb, mrb_inspect(self->mrb, value));
      asprintf(&str, "%s (other) %s\n", self->reply_prefix, inspect);
    }
  }

  return str;
}

mrb_value em_mrb_method_reply_count(mrb_state *mrb, mrb_value self)
{
  em *em_obj = (em *)mrb->ud;
  int count = em_obj->reply_count;

  return mrb_fixnum_value(count);
}

mrb_value em_mrb_method_set_reply_prefix(mrb_state *mrb, mrb_value self)
{
  mrb_value prefix;
  em *em_obj = (em *)mrb->ud;

  mrb_get_args(mrb, "S", &prefix);
  em_obj->reply_prefix = mrb_str_to_cstr(mrb, prefix);

  return prefix;
}

void em_mrb_init(em *self)
{
  mrb_int ai = mrb_gc_arena_save(self->mrb);

  struct RClass *class;
  class = mrb_define_class(self->mrb, "Em", self->mrb->object_class);

  mrb_define_class_method(self->mrb, class, "reply_count", em_mrb_method_reply_count, MRB_ARGS_NONE());
  mrb_define_class_method(self->mrb, class, "set_reply_prefix", em_mrb_method_set_reply_prefix, MRB_ARGS_REQ(1));

  mrb_gc_arena_restore(self->mrb, ai);
}