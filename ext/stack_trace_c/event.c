#include <event.h>

Event *wrap_into_event(VALUE tpVal) {
  Event *event = malloc(sizeof(Event));

  rb_trace_arg_t *traceArg = rb_tracearg_from_tracepoint(tpVal);
  VALUE klass = rb_tracearg_defined_class(traceArg);
  int forSingleton = -1;

  if(FL_TEST(klass, FL_SINGLETON)) {
    klass = rb_ivar_get(klass, rb_intern("__attached__"));
    forSingleton = 1;
  }

  event->tpVal = tpVal;
  event->traceArg = traceArg;
  event->event = rb_tracearg_event_flag(traceArg);
  event->klass = klass;
  event->forSingleton = forSingleton;

  return event;
}
