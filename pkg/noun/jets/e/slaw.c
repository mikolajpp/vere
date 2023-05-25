/// @fil

#include "jets/k.h"
#include "jets/q.h"
#include "jets/w.h"

#include "noun.h"

#include <ctype.h>

static inline u3_noun
_parse_da(u3_noun a)
{
  u3_weak pro;

  if ( u3_none == (pro = u3s_sift_da(u3x_atom(a))) ) {
    return u3_nul;
  }

  return u3nc(u3_nul, pro);
}

static inline u3_noun
_parse_p(u3_noun a)
{
  u3_weak pro;

  if ( u3_none == (pro = u3s_sift_p(u3x_atom(a))) ) {
    return u3_nul;
  }

  return u3nc(u3_nul, pro);
}

static inline u3_noun
_parse_ud(u3_noun a)
{
  u3_weak pro;

  if ( u3_none == (pro = u3s_sift_ud(u3x_atom(a))) ) {
    return u3_nul;
  }

  return u3nc(u3_nul, pro);
}

static inline u3_noun
_parse_ui(u3_noun a)
{
  u3_weak pro;

  if ( u3_none == (pro = u3s_sift_ui(u3x_atom(a))) ) {
    return u3_nul;
  }

  return u3nc(u3_nul, pro);
}

static inline u3_noun
_parse_ux(u3_noun a)
{
  u3_weak pro;

  if ( u3_none == (pro = u3s_sift_ux(u3x_atom(a))) ) {
    return u3_nul;
  }

  return u3nc(u3_nul, pro);
}

static inline u3_noun
_parse_uv(u3_noun a)
{
  u3_weak pro;

  if ( u3_none == (pro = u3s_sift_uv(u3x_atom(a))) ) {
    return u3_nul;
  }

  return u3nc(u3_nul, pro);
}

static inline u3_noun
_parse_uw(u3_noun a)
{
  u3_weak pro;

  if ( u3_none == (pro = u3s_sift_uw(u3x_atom(a))) ) {
    return u3_nul;
  }

  return u3nc(u3_nul, pro);
}

u3_noun
_parse_tas(u3_noun txt) {
  // For any symbol which matches, txt will return itself as a
  // value. Therefore, this is mostly checking validity.
  c3_c* c = u3a_string(txt);

  // First character must represent a lowercase letter
  c3_c* cur = c;
  if (!islower(cur[0])) {
    u3a_free(c);
    return 0;
  }
  cur++;

  while (cur[0] != 0) {
    if (!(islower(cur[0]) || isdigit(cur[0]) || cur[0] == '-')) {
      u3a_free(c);
      return 0;
    }

    cur++;
  }

  u3a_free(c);
  return u3nc(0, u3k(txt));
}

u3_noun
u3we_slaw(u3_noun cor)
{
  u3_noun mod;
  u3_noun txt;

  if (c3n == u3r_mean(cor, u3x_sam_2, &mod,
                      u3x_sam_3, &txt, 0)) {
    return u3m_bail(c3__exit);
  }

  switch (mod) {
    case c3__da:
      return _parse_da(txt);

    case 'p':
      return _parse_p(txt);

    case c3__ud:
      return _parse_ud(txt);

    case c3__ui:
      return _parse_ui(txt);

    case c3__ux:
      return _parse_ux(txt);

    case c3__uv:
      return _parse_uv(txt);

    case c3__uw:
      return _parse_uw(txt);

      // %ta is used once in link.hoon. don't bother.

    case c3__tas:
      return _parse_tas(txt);

    default:
      return u3_none;
  }
}
