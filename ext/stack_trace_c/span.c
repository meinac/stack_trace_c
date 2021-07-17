#include <ruby.h>
#include <span.h>
#include <stack_trace_utils.h>

Span *create_new_span(ID klassName, ID methodName, Span *parent) {
  Span *newSpan = (Span*)malloc(sizeof(Span));

  newSpan->startedAt = monotonic_time();
  newSpan->moduleName = klassName;
  newSpan->methodName = methodName;

  newSpan->parent = parent;
  newSpan->next = NULL;
  newSpan->firstChild = NULL;
  newSpan->lastChild = NULL;

  if(parent != NULL) {
    if(parent->firstChild == NULL) {
      parent->firstChild = newSpan;
      parent->lastChild = newSpan;
    } else {
      parent->lastChild->next = newSpan;
      parent->lastChild = newSpan;
    }
  }

  return newSpan;
}

Span *close_span(Span *span) {
  span->finishedAt = monotonic_time();

  return span->parent;
}
