#include <types.h>
#include <imprison.h>
#include <retrieve.h>
#include <xtract.h>
#include <log.h>
#include <jets/k.h>

static void _x_octs(u3_noun octs, u3_atom* p_octs, u3_atom* q_octs) {

  if (c3n == u3r_mean(octs,
             2, p_octs,
             3, q_octs, 0)){
    u3m_bail(c3__exit);
  }

  if (c3n == u3a_is_atom(*p_octs) ||
      c3n == u3a_is_atom(*q_octs)) {
    u3m_bail(c3__exit);
  }
}
static void _x_octs_buffer(u3_atom* p_octs, u3_atom *q_octs, 
                           c3_w* p_octs_w, c3_y** buf_y,
                           c3_w* len_w, c3_w* lead_w)
{
  // XX gracefully handle p.octs exceeding a word
  //
  if (c3n == u3r_safe_word(*p_octs, p_octs_w)) {
    u3m_bail(c3__exit);
  }

  *len_w = u3r_met(3, *q_octs);

  if (c3y == u3a_is_cat(*q_octs)) {
    *buf_y = (c3_y*)q_octs;
  }
  else {
    u3a_atom* ptr_a = u3a_to_ptr(*q_octs);
    *buf_y = (c3_y*)ptr_a->buf_w;
  }

  *lead_w = 0;

  if (*p_octs_w > *len_w) {
    *lead_w = *p_octs_w - *len_w;
  }
  else {
    *len_w = *p_octs_w;
  }

}

u3_noun _qe_bytestream_rip_octs(u3_atom p_octs, u3_atom q_octs) {

  c3_w p_octs_w, len_w, lead_w;
  c3_y* buf_y;

  _x_octs_buffer(&p_octs, &q_octs, &p_octs_w, &buf_y, &len_w, &lead_w);

  if (p_octs_w == 0) {
    return u3_nul;
  }

  buf_y += len_w;

  u3_noun rip = u3_nul;

  while (lead_w--) {
    rip = u3nc(u3i_word(0x0), rip);
  }

  while (len_w--) {
    rip = u3nc(*(--buf_y), rip);
  }

  return rip;
}
u3_noun
u3we_bytestream_rip_octs(u3_noun cor){

  u3_noun sam = u3x_at(u3x_sam, cor);

  u3_atom p_octs, q_octs;
  _x_octs(sam, &p_octs, &q_octs);

  return _qe_bytestream_rip_octs(p_octs, q_octs);

}

u3_noun
_qe_bytestream_cat_octs(u3_noun octs_a, u3_noun octs_b) {

  u3_atom p_octs_a, p_octs_b;
  u3_atom q_octs_a, q_octs_b;

  _x_octs(octs_a, &p_octs_a, &q_octs_a);
  _x_octs(octs_b, &p_octs_b, &q_octs_b);

  c3_w  p_octs_a_w, p_octs_b_w;
  c3_w  len_w, lem_w;
  c3_w  lead_w, leaf_w;

  c3_y* sea_y;
  c3_y* seb_y;

  _x_octs_buffer(&p_octs_a, &q_octs_a, &p_octs_a_w, &sea_y, &len_w, &lead_w);
  _x_octs_buffer(&p_octs_b, &q_octs_b, &p_octs_b_w, &seb_y, &lem_w, &leaf_w);

  if (p_octs_a_w == 0) {
    return u3k(octs_b);
  }
  if (p_octs_b_w == 0) {
    return u3k(octs_a);
  }

  c3_d p_octs_d = p_octs_a_w + p_octs_b_w;

  u3i_slab sab_u;

  u3i_slab_init(&sab_u, 3, (c3_d)p_octs_a_w + lem_w);
  sab_u.buf_w[sab_u.len_w - 1] = 0;

  memcpy(sab_u.buf_y, sea_y, len_w);
  memset(sab_u.buf_y + len_w, 0, lead_w);
  memcpy(sab_u.buf_y + p_octs_a_w, seb_y, lem_w);

  u3_noun q_octs = u3i_slab_moot(&sab_u);
  u3_noun ret = u3nc(u3i_chub(p_octs_d), q_octs);

  return ret;
}

u3_noun
u3we_bytestream_cat_octs(u3_noun cor) {

  u3_noun octs_a, octs_b;

  u3x_mean(cor, u3x_sam_2, &octs_a, u3x_sam_3, &octs_b, 0);

  return _qe_bytestream_cat_octs(octs_a, octs_b);

}

u3_noun
_qe_bytestream_can_octs(u3_noun octs_list) {

  if (u3_nul == octs_list) {
    return u3nc(0, 0);
  }

  if (u3_nul == u3t(octs_list)) {
    u3k(u3h(octs_list));
  }

  /* We can octs in two steps:
   * first loop iteration computes the total required
   * buffer size in bytes, factoring in the leading bytes
   * of the final octs. The second loop iterates over each octs,
   * copying the data to the output buffer.
   */

  // Compute total size
  //
  c3_d tot_d = 0;

  u3_noun octs_list_start = octs_list;
  u3_noun octs = u3_none;

  while (octs_list != u3_nul) {

    octs = u3h(octs_list);

    if (c3n == u3a_is_atom(u3h(octs)) ||
        c3n == u3a_is_atom(u3t(octs))) {
      u3m_bail(c3__exit);
    }
    c3_w p_octs_w;

    if (c3n == u3r_safe_word(u3h(octs), &p_octs_w)) {
      return u3_none;
    }
    // Check for overflow
    //
    if ( p_octs_w > (UINT64_MAX - tot_d)){
      u3m_bail(c3__exit);
    }
    tot_d += p_octs_w;

    octs_list = u3t(octs_list);
  }


  //  Compute leading zeros of final octs -- the buffer
  //  size is decreased by this much.
  //
  //  =leading-zeros (sub p.octs (met 3 q.octs))
  //
  //  p.octs fits into a word -- this has been verified 
  //  in the loop above.
  //
  c3_w last_lead_w = (u3r_word(0, u3h(octs)) - u3r_met(3, u3t(octs)));
  c3_d buf_len_w = tot_d - last_lead_w;

  u3i_slab sab_u;
  u3i_slab_bare(&sab_u, 3, buf_len_w);
  c3_y* buf_y = sab_u.buf_y;

  sab_u.buf_w[sab_u.len_w - 1] = 0;

  c3_y* sea_y;
  u3_atom p_octs, q_octs;
  c3_w p_octs_w, q_octs_w;
  c3_w len_w, lead_w;

  // Bytes written so far
  //
  c3_d wit_d = 0;

  octs_list = octs_list_start;

  while (octs_list != u3_nul) {

    octs = u3h(octs_list);

    _x_octs(octs, &p_octs, &q_octs);
    _x_octs_buffer(&p_octs, &q_octs, &p_octs_w, &sea_y, &len_w, &lead_w);

    if (p_octs_w == 0) {
      octs_list = u3t(octs_list);
      continue;
    }

    memcpy(buf_y, sea_y, len_w);
    buf_y += len_w;
    wit_d += len_w;

    // More bytes to follow, write leading zeros
    //
    if (wit_d < buf_len_w)  {
      memset(buf_y, 0, lead_w);
      buf_y += lead_w;
      wit_d += lead_w;
    }

    octs_list = u3t(octs_list);
  }

  u3_assert((buf_y - sab_u.buf_y) == buf_len_w);

  return u3nc(u3i_chub(tot_d), u3i_slab_mint(&sab_u));
}

u3_noun
u3we_bytestream_can_octs(u3_noun cor)
{

  u3_noun octs_list;

  u3x_mean(cor, u3x_sam_1, &octs_list, 0);

  return _qe_bytestream_can_octs(octs_list);
}
u3_noun _qe_bytestream_skip_line(u3_atom pos, u3_noun octs)
{
  c3_w pos_w;

  if (c3n == u3r_safe_word(pos, &pos_w)) {
    return u3m_bail(c3__exit);
  }

  u3_atom p_octs, q_octs;

  _x_octs(octs, &p_octs, &q_octs);

  c3_w  p_octs_w;
  c3_w  len_w, lead_w;

  c3_y* sea_y;

  _x_octs_buffer(&p_octs, &q_octs, &p_octs_w, &sea_y, &len_w, &lead_w);

  while (pos_w < len_w) {
    if (*(sea_y + pos_w) == '\n') {
      break;
    }
    pos_w++;
  }
  // Newline not found, position at the end
  if (*(sea_y + pos_w) != '\n') {
    pos_w = p_octs;
  }
  else {
    pos_w++;
  }

  return u3nc(u3i_word(pos_w), u3k(octs));
}
u3_noun u3we_bytestream_skip_line(u3_noun cor)
{
  
  u3_atom pos;
  u3_noun octs;

  u3x_mean(cor, u3x_sam_2, &pos, u3x_sam_3, &octs, 0);
  
  return _qe_bytestream_skip_line(pos, octs);

}
u3_noun _qe_bytestream_find_byte(u3_atom bat, u3_atom pos, u3_noun octs)
{
  c3_w bat_w, pos_w;

  if (c3n == u3r_safe_word(bat, &bat_w) || bat_w > 0xff) {
    return u3m_bail(c3__exit);
  }
  if (c3n == u3r_safe_word(pos, &pos_w)) {
    return u3m_bail(c3__exit);
  }

  u3_atom p_octs, q_octs;

  _x_octs(octs, &p_octs, &q_octs);

  c3_w  p_octs_w;
  c3_w  len_w, lead_w;

  c3_y* sea_y;

  _x_octs_buffer(&p_octs, &q_octs, &p_octs_w, &sea_y, &len_w, &lead_w);

  while (pos_w < len_w) {

    if (*(sea_y + pos_w) == bat_w) {
      return u3nc(u3_nul, u3i_word(pos_w));
    }

    pos_w++;
  }
  //  Here we are sure that:
  //  (1) bat_w has not been found
  //  (2) therefore pos_w == len_w
  //  
  //  If bat_w == 0, and there is still input 
  //  in the stream, it means pos_w points at
  //  the first leading zero.
  //
  if (pos_w < p_octs && bat_w == 0) {
    return u3nc(u3_nul, u3i_word(pos_w));
  }

  return u3_nul;
}
u3_noun u3we_bytestream_find_byte(u3_noun cor)
{
  u3_atom bat;
  u3_atom pos;
  u3_noun octs;

  u3x_mean(cor, u3x_sam_2, &bat, 
                u3x_sam_6, &pos, 
                u3x_sam_7, &octs, 0);
  
  return _qe_bytestream_find_byte(bat, pos, octs);
}
u3_noun _qe_bytestream_seek_byte(u3_atom bat, u3_atom pos, u3_noun octs)
{
  c3_w bat_w, pos_w;

  if (c3n == u3r_safe_word(bat, &bat_w) || bat_w > 0xff) {
    return u3m_bail(c3__exit);
  }
  if (c3n == u3r_safe_word(pos, &pos_w)) {
    return u3m_bail(c3__exit);
  }

  u3_atom p_octs, q_octs;

  _x_octs(octs, &p_octs, &q_octs);

  c3_w  p_octs_w;
  c3_w  len_w, lead_w;

  c3_y* sea_y;

  _x_octs_buffer(&p_octs, &q_octs, &p_octs_w, &sea_y, &len_w, &lead_w);

  while (pos_w < len_w) {

    if (*(sea_y + pos_w) == bat_w) {
      u3_noun idx = u3nc(u3_nul, u3i_word(pos_w));
      u3_noun new_octs = u3nc(u3i_word(pos_w), u3k(octs));
      return u3nc(idx, new_octs);
    }

    pos_w++;
  }

  // find leading zero: see comment in *_find_byte
  //
  if (pos_w < p_octs && bat_w == 0) {
      u3_noun idx = u3nc(u3_nul, u3i_word(pos_w));
      u3_noun new_octs = u3nc(u3i_word(pos_w), u3k(octs));
      return u3nc(idx, new_octs);
  }

  return u3nc(u3_nul, u3nc(u3k(pos), u3k(octs)));

}
u3_noun u3we_bytestream_seek_byte(u3_noun cor)
{
  u3_atom bat;
  u3_atom pos;
  u3_noun octs;

  u3x_mean(cor, u3x_sam_2, &bat, 
                u3x_sam_6, &pos, 
                u3x_sam_7, &octs, 0);
  
  return _qe_bytestream_seek_byte(bat, pos, octs);
}

u3_noun _qe_peek_octs(c3_w n_w, c3_w pos_w, c3_w p_octs_w, c3_y* sea_y, 
                      c3_w len_w)
{
  if (n_w == 0) {
    return u3nc(0, 0);
  }

  if (pos_w + n_w > p_octs_w) {
    return u3m_bail(c3__exit);
  }

  // Read leading zeros only
  //
  if (pos_w >= len_w) {
    return u3nc(u3i_word(n_w), 0);
  }
  // Number of remaining buffer bytes
  c3_w reb_w = len_w - pos_w;

  u3i_slab sab_u;
  c3_w my_len_w;

  if (n_w < reb_w) {
    my_len_w = n_w;
  }
  else {
    my_len_w = reb_w;
  }
  u3i_slab_bare(&sab_u, 3, my_len_w);
  sab_u.buf_w[sab_u.len_w - 1] = 0;
  memcpy(sab_u.buf_y, sea_y + pos_w, my_len_w);

  return u3nc(u3i_word(n_w), u3i_slab_moot(&sab_u));
}
u3_noun _qe_bytestream_chunk(u3_atom size, u3_noun pos, u3_noun octs)
{
  c3_w size_w, pos_w;

  if (c3n == u3r_safe_word(size, &size_w)) {
    return u3m_bail(c3__exit);
  }

  if (size_w == 0) {
    return u3_nul;
  }

  if (c3n == u3r_safe_word(pos, &pos_w)) {
    return u3m_bail(c3__exit);
  }

  u3_atom p_octs, q_octs;

  _x_octs(octs, &p_octs, &q_octs);

  c3_w  p_octs_w;
  c3_w  len_w, lead_w;

  c3_y* sea_y;

  _x_octs_buffer(&p_octs, &q_octs, &p_octs_w, &sea_y, &len_w, &lead_w);

  u3_noun hun = u3_nul;

  while (pos_w < p_octs) {
    // Remaining bytes
    //
    c3_w rem = (p_octs - pos_w);

    if (rem < size) {
      u3_noun octs = _qe_peek_octs(rem, pos_w, p_octs_w, sea_y,
                                   len_w);
      hun = u3nc(octs, hun);
      pos_w += rem;
    }
    else {
      u3_noun octs = _qe_peek_octs(size, pos_w, p_octs_w, sea_y, 
                                   len_w);
      hun = u3nc(octs, hun);
      pos_w += size;
    }
  }

  return u3kb_flop(hun);
}

u3_noun u3we_bytestream_chunk(u3_noun cor)
{
  u3_atom size;
  u3_atom pos;
  u3_noun octs;

  u3x_mean(cor, u3x_sam_2, &size, 
                u3x_sam_6, &pos, 
                u3x_sam_7, &octs, 0);
  
  return _qe_bytestream_chunk(size, pos, octs);
}
