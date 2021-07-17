#include <ruby.h>
#include <ruby/debug.h>

typedef struct EventS Event;

struct EventS{
  VALUE tpVal;
  rb_event_flag_t event;
  rb_trace_arg_t *traceArg;
  VALUE klass;
  int forSingleton;
};

Event *wrap_into_event(VALUE tpVal);
