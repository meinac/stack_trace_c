typedef struct SpanS Span;
typedef struct SpanGroupS SpanGroup;

/*
* This data model is optimized for
* traversing the whole trace from
* beginning to end.
*
* Map of spans looks something like;
*
* S(1) - S(2) - S(3) - S(4)
*         |
*        S(5) - S(6)
*         |
*        S(7)
*         |
*        S(8) - S(9)
*                |
*               S(10)
*/
struct SpanS {
  ID moduleName;
  ID methodName;

  long startedAt;
  long finishedAt;
  Span *parent;     // holds the reference to parent span
  Span *next;       // next sibling of this span
  Span *firstChild; // holds the first child span of this span
  Span *lastChild;  // holds the last child span of this span to prevent traversing all
};

Span *create_new_span(ID, ID, Span *);
Span *close_span(Span *);
