#include <ruby.h>
#include <ruby/debug.h>
#include <trace.h>

static VALUE stackTrace;

static rb_event_flag_t trace_events() {
  rb_event_flag_t events = 0;

  VALUE configuration = rb_funcall(stackTrace, rb_intern("configuration"), 0);
  VALUE trace_ruby = rb_funcall(configuration, rb_intern("trace_ruby?"), 0);
  VALUE trace_c = rb_funcall(configuration, rb_intern("trace_c?"), 0);

  if(RTEST(trace_ruby)) events |= RUBY_EVENT_CALL | RUBY_EVENT_RETURN;
  if(RTEST(trace_c)) events |= RUBY_EVENT_C_CALL | RUBY_EVENT_C_RETURN;
  if(events != 0) events |= RUBY_EVENT_RAISE;

  return events;
}

static VALUE rb_trace_point(VALUE self) {
  VALUE tracePoint = rb_iv_get(self, "@trace_point");

  if(NIL_P(tracePoint)) {
    tracePoint = rb_tracepoint_new(Qnil, trace_events(), track, NULL);
    rb_iv_set(self, "@trace_point", tracePoint);
  }

  return tracePoint;
}

void Init_stack_trace_c() {
  stackTrace = rb_const_get(rb_cObject, rb_intern("StackTraceC"));
  VALUE traceClass = rb_define_class_under(stackTrace, "Trace", rb_cObject);

  rb_define_singleton_method(stackTrace, "to_array", rb_to_array, 0);
  rb_define_singleton_method(stackTrace, "to_h", rb_to_hash, 0);
  rb_define_singleton_method(stackTrace, "trace_point", rb_trace_point, 0);
  rb_define_singleton_method(traceClass, "start", rb_start, 0);
}
