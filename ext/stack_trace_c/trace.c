#include <ruby.h>
#include <ruby/debug.h>
#include <span.h>
#include <event.h>

#define CALL_EVENTS (RUBY_EVENT_CALL | RUBY_EVENT_C_CALL)
#define RETURN_EVENTS (RUBY_EVENT_RETURN | RUBY_EVENT_C_RETURN)

static Span *current, *start;

static int trackable(Event *event) {
  return current != NULL && event->klass;
}

static void track_call_event(Event *event) {
  if(!trackable(event)) return;

  VALUE klass_name = rb_funcall(event->klass, rb_intern("name"), 0);
  VALUE meth = rb_tracearg_method_id(event->traceArg);

  current = create_new_span(rb_intern_str(klass_name), SYM2ID(meth), current);
}

static void track_return_event(Event *event) {
  if(!trackable(event)) return;

  current = close_span(current);
}

static void track_raise_event(Event *event) {
  if(current == NULL) return;
}

void track(VALUE tpVal, void *_data) {
  Event *event = wrap_into_event(tpVal);

  if(event->event & CALL_EVENTS) {
    track_call_event(event);
  } else if(event->event & RETURN_EVENTS) {
    track_return_event(event);
  } else if(event->event & RUBY_EVENT_RAISE){
    track_raise_event(event);
  }

  free(event);
}

static VALUE span_to_ary(Span *span, VALUE env) {
  if(span->methodName == 0) return Qnil;

  VALUE tmp = rb_ary_new2(4);
  VALUE children = rb_ary_new();
  rb_ary_push(tmp, ID2SYM(span->moduleName));
  rb_ary_push(tmp, ID2SYM(span->methodName));
  rb_ary_push(tmp, LONG2NUM((span->finishedAt - span->startedAt)));
  rb_ary_push(tmp, children);
  rb_ary_push(env, tmp);

  return children;
}

static VALUE span_to_hash(Span *span, VALUE env) {
  if(span->methodName == 0) return Qnil;

  VALUE tmp = rb_hash_new();
  VALUE children = rb_ary_new();
  rb_hash_aset(tmp, rb_str_new2("module_name"), ID2SYM(span->moduleName));
  rb_hash_aset(tmp, rb_str_new2("method_name"), ID2SYM(span->methodName));
  rb_hash_aset(tmp, rb_str_new2("duration"), LONG2NUM((span->finishedAt - span->startedAt)));
  rb_hash_aset(tmp, rb_str_new2("spans"), children);
  rb_ary_push(env, tmp);

  return children;
}

static void traverse(Span *startSpan, VALUE (*encoder)(Span *span, VALUE env), VALUE ret) {
  if(startSpan == NULL) return;

  VALUE childrenEnv = encoder(startSpan, ret);
  childrenEnv = RTEST(childrenEnv) ? childrenEnv : ret;

  traverse(startSpan->firstChild, encoder, childrenEnv);
  traverse(startSpan->next, encoder, ret);
}

VALUE rb_to_array(VALUE self) {
  VALUE ret = rb_ary_new();

  traverse(start, &span_to_ary, ret);

  return ret;
}

VALUE rb_to_hash(VALUE self) {
  VALUE ret = rb_ary_new();

  traverse(start, &span_to_hash, ret);

  return ret;
}

VALUE rb_start(VALUE self) {
  start = current = create_new_span(0, 0, NULL);

  return Qtrue;
}
