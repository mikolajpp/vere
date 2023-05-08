/// @file

#include "noun.h"

static inline c3_i
_da_etch(mpz_t a_mp, const c3_c* dat_c)
{
  u3_atom dat = u3i_mp(a_mp);
  c3_c*  out_c;
  c3_i   ret_i = 1;
  size_t len_i;

  len_i = u3s_etch_da_c(dat, &out_c);

  if ( 0 != strcmp(dat_c, out_c) ) {
    fprintf(stderr, "etch_da: 0x");
    mpz_out_str(stderr, 16, a_mp);
    fprintf(stderr, " fail; expected %s, got '%s'\r\n",
                    dat_c, out_c);
    ret_i = 0;
  }

  else {
    u3_noun out = u3s_etch_da(dat);
    u3_noun tou = u3i_bytes(len_i, (c3_y*)out_c);

    if ( c3n == u3r_sing(tou, out) ) {
      fprintf(stderr, "etch_da: 0x");
      mpz_out_str(stderr, 16, a_mp);
      fprintf(stderr, " mismatch; expected %s\r\n", dat_c);
      u3m_p("out", out);
      ret_i = 0;
    }

    u3z(out);
    u3z(tou);
  }

  c3_free(out_c);
  u3z(dat);

  return ret_i;
}

#define _init_date_atom(mp, hi, lo) \
  { \
    mpz_init(mp); \
    mpz_set_ui(dat_mp, hi); \
    mpz_mul_2exp(dat_mp, dat_mp, 64); \
    mpz_add_ui(dat_mp, dat_mp, lo); \
  } \

static c3_i
_test_etch_da(void)
{
  mpz_t dat_mp;
  c3_i ret_i = 1;

  // In the beginning was the Word
  _init_date_atom(dat_mp, 0x0, 0x0);
  ret_i &= _da_etch(dat_mp, "~292277024401-.1.1");

  // the Word was with God
  _init_date_atom(dat_mp, 
      0x7ffffffe58e40f80, 
      0xbabe000000000000);
  ret_i &= _da_etch(dat_mp, "~1.12.25..00.00.00..babe");

  // and the Word was God - John 1:1
  _init_date_atom(dat_mp, 
      0x7ffffffe93b72f70,
      0x3300000000000000)
  ret_i &= _da_etch(dat_mp, "~33.4.3..15.00.00..3300");

  // Test fractional seconds
  //
  _init_date_atom(dat_mp,
      0x8000000d32bb462f,
      0xcafe000000000000);
  ret_i &= _da_etch(dat_mp, "~2023.3.24..05.44.15..cafe");

  _init_date_atom(dat_mp,
      0x8000000d32bb462f,
      0x0000cafe00000000);
  ret_i &= _da_etch(dat_mp, "~2023.3.24..05.44.15..0000.cafe");

  _init_date_atom(dat_mp, 
      0x8000000d32bb462f,
      0x00000000cafe0000);
  ret_i &= _da_etch(dat_mp, "~2023.3.24..05.44.15..0000.0000.cafe");

  _init_date_atom(dat_mp, 
      0x8000000d32bb462f,
      0x000000000000cafe);
  ret_i &= _da_etch(dat_mp, "~2023.3.24..05.44.15..0000.0000.0000.cafe");

  // General tests 
  //
  _init_date_atom(dat_mp, 
      0x8000000d329d6f76,
      0xadef000000000000);
  ret_i &= _da_etch(dat_mp, "~2023.3.1..14.32.22..adef");

  _init_date_atom(dat_mp, 
      0x8000000d32c33b88, 
      0x2d00000000000000);
  ret_i &= _da_etch(dat_mp, "~2023.3.30..06.36.56..2d00");

  _init_date_atom(dat_mp, 
      0x8000000d32c51c00,
      0x2d00000000000000);
  ret_i &= _da_etch(dat_mp, "~2023.3.31..16.46.56..2d00");

  _init_date_atom(dat_mp, 
      0x8000000d3a19f0c0,
      0x2d00000000000000);
  ret_i &= _da_etch(dat_mp, "~2027.2.22..07.26.56..2d00");

  _init_date_atom(dat_mp, 
      0x80000029dd78fec0,
      0x2d00000000000000);
  ret_i &= _da_etch(dat_mp, "~5924.11.10..10.06.56..2d00");

  _init_date_atom(dat_mp, 
      0x8000700808c7aec0,
      0x2d00000000000000);
  ret_i &= _da_etch(dat_mp, "~3903639.9.11..12.46.56..2d00");

  return ret_i;
}


/* _setup(): prepare for tests.
*/
static void
_setup(void)
{
  u3m_init(1 << 20);
  u3m_pave(c3y);
}

static inline c3_i
_ud_etch(c3_d num_d, const c3_c* num_c)
{
  u3_atom  num = u3i_chub(num_d);
  c3_c*  out_c;
  size_t len_i = u3s_etch_ud_c(num, &out_c);
  c3_i   ret_i = 1;

  if ( 0 != strcmp(num_c, out_c) ) {
    fprintf(stderr, "etch_ud: %" PRIu64 " fail; expected %s, got '%s'\r\n",
                    num_d, num_c, out_c);
    ret_i = 0;
  }
  else {
    u3_noun out = u3s_etch_ud(num);
    u3_noun tou = u3i_bytes(len_i, (c3_y*)out_c);

    if ( c3n == u3r_sing(tou, out) ) {
      fprintf(stderr, "etch_ud: %" PRIu64 " mismatch; expected %s\r\n", num_d, num_c);
      u3m_p("out", out);
      ret_i = 0;
    }

    u3z(out);
    u3z(tou);
  }

  c3_free(out_c);
  u3z(num);

  return ret_i;
}

static c3_i
_test_etch_ud(void)
{
  c3_i ret_i = 1;

  ret_i &= _ud_etch(0, "0");
  ret_i &= _ud_etch(1, "1");
  ret_i &= _ud_etch(12, "12");
  ret_i &= _ud_etch(123, "123");
  ret_i &= _ud_etch(1234, "1.234");
  ret_i &= _ud_etch(12345, "12.345");
  ret_i &= _ud_etch(123456, "123.456");
  ret_i &= _ud_etch(1234567, "1.234.567");
  ret_i &= _ud_etch(12345678, "12.345.678");
  ret_i &= _ud_etch(123456789, "123.456.789");
  ret_i &= _ud_etch(100000000, "100.000.000");
  ret_i &= _ud_etch(101101101, "101.101.101");
  ret_i &= _ud_etch(201201201, "201.201.201");
  ret_i &= _ud_etch(302201100, "302.201.100");

  ret_i &= _ud_etch(8589934592ULL, "8.589.934.592");
  ret_i &= _ud_etch(2305843009213693952ULL, "2.305.843.009.213.693.952");
  ret_i &= _ud_etch(18446744073709551615ULL, "18.446.744.073.709.551.615");

  {
    c3_c* num_c = "340.282.366.920.938.463.463.374.607.431.768.211.456";
    u3_atom num = u3qc_bex(128);
    c3_c*  out_c;
    size_t len_i = u3s_etch_ud_c(num, &out_c);

    if ( 0 != strncmp(num_c, out_c, len_i) ) {
      fprintf(stderr, "etch_ud: (bex 128) fail; expected %s, got '%s'\r\n",
                      num_c, out_c);
      ret_i = 0;
    }
    else {
      u3_noun out = u3s_etch_ud(num);
      u3_noun tou = u3i_bytes(len_i, (c3_y*)out_c);

      if ( c3n == u3r_sing(tou, out) ) {
        fprintf(stderr, "etch_ud: (bex 128) mismatch; expected %s\r\n", num_c);
        u3m_p("out", out);
        ret_i = 0;
      }

      u3z(out);
      u3z(tou);
    }

    c3_free(out_c);
    u3z(num);
  }

  return ret_i;
}


static inline c3_i
_ux_etch(c3_d num_d, const c3_c* num_c)
{
  u3_atom  num = u3i_chub(num_d);
  c3_c*  out_c;
  size_t len_i = u3s_etch_ux_c(num, &out_c);
  c3_i   ret_i = 1;

  if ( 0 != strcmp(num_c, out_c) ) {
    fprintf(stderr, "etch_ux: 0x%" PRIx64 " fail; expected %s, got '%s'\r\n",
                    num_d, num_c, out_c);
    ret_i = 0;
  }
  else {
    u3_noun out = u3s_etch_ux(num);
    u3_noun tou = u3i_bytes(len_i, (c3_y*)out_c);

    if ( c3n == u3r_sing(tou, out) ) {
      fprintf(stderr, "etch_ux: 0x%" PRIx64 " mismatch; expected %s\r\n", num_d, num_c);
      u3m_p("out", out);
      ret_i = 0;
    }

    u3z(out);
    u3z(tou);
  }

  c3_free(out_c);
  u3z(num);

  return ret_i;
}

static c3_i
_test_etch_ux(void)
{
  c3_i ret_i = 1;

  ret_i &= _ux_etch(0x0, "0x0");
  ret_i &= _ux_etch(0x1, "0x1");
  ret_i &= _ux_etch(0x12, "0x12");
  ret_i &= _ux_etch(0x123, "0x123");
  ret_i &= _ux_etch(0x1234, "0x1234");
  ret_i &= _ux_etch(0x12345, "0x1.2345");
  ret_i &= _ux_etch(0x123456, "0x12.3456");
  ret_i &= _ux_etch(0x1234567, "0x123.4567");
  ret_i &= _ux_etch(0x12345678, "0x1234.5678");
  ret_i &= _ux_etch(0x123456789, "0x1.2345.6789");
  ret_i &= _ux_etch(0x100000000, "0x1.0000.0000");
  ret_i &= _ux_etch(0x101101101, "0x1.0110.1101");
  ret_i &= _ux_etch(0x201201201, "0x2.0120.1201");
  ret_i &= _ux_etch(0x302201100, "0x3.0220.1100");

  ret_i &= _ux_etch(0x123456789abcdefULL, "0x123.4567.89ab.cdef");
  ret_i &= _ux_etch(0x8589934592ULL, "0x85.8993.4592");
  ret_i &= _ux_etch(0x5843009213693952ULL, "0x5843.0092.1369.3952");
  ret_i &= _ux_etch(0x6744073709551615ULL, "0x6744.0737.0955.1615");

  {
    c3_c* num_c = "0x1.0000.0000.0000.0000.0000.0000.0000.0000";
    u3_atom num = u3qc_bex(128);
    c3_c*  out_c;
    size_t len_i = u3s_etch_ux_c(num, &out_c);

    if ( 0 != strncmp(num_c, out_c, len_i) ) {
      fprintf(stderr, "etch_ux: (bex 128) fail; expected %s, got '%s'\r\n",
                      num_c, out_c);
      ret_i = 0;
    }
    else {
      u3_noun out = u3s_etch_ux(num);
      u3_noun tou = u3i_bytes(len_i, (c3_y*)out_c);

      if ( c3n == u3r_sing(tou, out) ) {
        fprintf(stderr, "etch_ux: (bex 128) mismatch; expected %s\r\n", num_c);
        u3m_p("out", out);
        ret_i = 0;
      }

      u3z(out);
      u3z(tou);
    }

    c3_free(out_c);
    u3z(num);
  }

  return ret_i;
}

static inline c3_i
_uv_etch(c3_d num_d, const c3_c* num_c)
{
  u3_atom  num = u3i_chub(num_d);
  c3_c*  out_c;
  size_t len_i = u3s_etch_uv_c(num, &out_c);
  c3_i   ret_i = 1;

  if ( 0 != strcmp(num_c, out_c) ) {
    fprintf(stderr, "etch_uv: 0x%" PRIx64 " fail; expected %s, got '%s'\r\n",
                    num_d, num_c, out_c);
    ret_i = 0;
  }
  else {
    u3_noun out = u3s_etch_uv(num);
    u3_noun tou = u3i_bytes(len_i, (c3_y*)out_c);

    if ( c3n == u3r_sing(tou, out) ) {
      fprintf(stderr, "etch_uv: 0x%" PRIx64 " mismatch; expected %s\r\n", num_d, num_c);
      u3m_p("out", out);
      ret_i = 0;
    }

    u3z(out);
    u3z(tou);
  }

  c3_free(out_c);
  u3z(num);

  return ret_i;
}

static c3_i
_test_etch_uv(void)
{
  c3_i ret_i = 1;

  ret_i &= _uv_etch(0x0, "0v0");
  ret_i &= _uv_etch(0x1, "0v1");
  ret_i &= _uv_etch(0x10, "0vg");
  ret_i &= _uv_etch(0x12, "0vi");
  ret_i &= _uv_etch(0x123, "0v93");
  ret_i &= _uv_etch(0x1234, "0v4hk");
  ret_i &= _uv_etch(0x12345, "0v28q5");
  ret_i &= _uv_etch(0x123456, "0v14d2m");
  ret_i &= _uv_etch(0x1234567, "0vi6hb7");
  ret_i &= _uv_etch(0x12345678, "0v9.38ljo");
  ret_i &= _uv_etch(0x123456789, "0v4h.kaps9");
  ret_i &= _uv_etch(0x100000000, "0v40.00000");
  ret_i &= _uv_etch(0x101101101, "0v40.h0481");
  ret_i &= _uv_etch(0x201201201, "0v80.i04g1");
  ret_i &= _uv_etch(0x302201100, "0vc1.20480");

  ret_i &= _uv_etch(0x123456789abcdefULL, "0v28.q5cu4.qnjff");
  ret_i &= _uv_etch(0x8589934592ULL, "0vgm4.p6hci");
  ret_i &= _uv_etch(0x5843009213693952ULL, "0v5gg.o0i89.mieai");
  ret_i &= _uv_etch(0x6744073709551615ULL, "0v6eh.076s4.la5gl");

  {
    c3_c* num_c = "0v8.00000.00000.00000.00000.00000";
    u3_atom num = u3qc_bex(128);
    c3_c*  out_c;
    size_t len_i = u3s_etch_uv_c(num, &out_c);

    if ( 0 != strncmp(num_c, out_c, len_i) ) {
      fprintf(stderr, "etch_uv: (bex 128) fail; expected %s, got '%s'\r\n",
                      num_c, out_c);
      ret_i = 0;
    }
    else {
      u3_noun out = u3s_etch_uv(num);
      u3_noun tou = u3i_bytes(len_i, (c3_y*)out_c);

      if ( c3n == u3r_sing(tou, out) ) {
      //   fprintf(stderr, "etch_uv: (bex 128) mismatch; expected %s\r\n", num_c);
        u3m_p("out", out);
        ret_i = 0;
      }

      u3z(out);
      u3z(tou);
    }

    c3_free(out_c);
    u3z(num);
  }

  return ret_i;
}

static inline c3_i
_uw_etch(c3_d num_d, const c3_c* num_c)
{
  u3_atom  num = u3i_chub(num_d);
  c3_c*  out_c;
  size_t len_i = u3s_etch_uw_c(num, &out_c);
  c3_i   ret_i = 1;

  if ( 0 != strcmp(num_c, out_c) ) {
    fprintf(stderr, "etch_uw: 0x%" PRIx64 " fail; expected %s, got '%s'\r\n",
                    num_d, num_c, out_c);
    ret_i = 0;
  }
  else {
    u3_noun out = u3s_etch_uw(num);
    u3_noun tou = u3i_bytes(len_i, (c3_y*)out_c);

    if ( c3n == u3r_sing(tou, out) ) {
      fprintf(stderr, "etch_uw: 0x%" PRIx64 " mismatch; expected %s\r\n", num_d, num_c);
      u3m_p("out", out);
      ret_i = 0;
    }

    u3z(out);
    u3z(tou);
  }

  c3_free(out_c);
  u3z(num);

  return ret_i;
}

static c3_i
_test_etch_uw(void)
{
  c3_i ret_i = 1;

  ret_i &= _uw_etch(0x0, "0w0");
  ret_i &= _uw_etch(0x1, "0w1");
  ret_i &= _uw_etch(0x10, "0wg");
  ret_i &= _uw_etch(0x12, "0wi");
  ret_i &= _uw_etch(0x123, "0w4z");
  ret_i &= _uw_etch(0x1234, "0w18Q");
  ret_i &= _uw_etch(0x12345, "0wid5");
  ret_i &= _uw_etch(0x123456, "0w4zhm");
  ret_i &= _uw_etch(0x1234567, "0w18QlD");
  ret_i &= _uw_etch(0x12345678, "0wid5pU");
  ret_i &= _uw_etch(0x123456789, "0w4.zhmu9");
  ret_i &= _uw_etch(0x100000000, "0w4.00000");
  ret_i &= _uw_etch(0x101101101, "0w4.14141");
  ret_i &= _uw_etch(0x201201201, "0w8.18181");
  ret_i &= _uw_etch(0x302201100, "0wc.28140");

  ret_i &= _uw_etch(0x123456789abcdefULL, "0w4zhmu.9GYTL");
  ret_i &= _uw_etch(0x8589934592ULL, "0w8m.9AQmi");
  ret_i &= _uw_etch(0x5843009213693952ULL, "0w5.x3098.jqjBi");
  ret_i &= _uw_etch(0x6744073709551615ULL, "0w6.t41Ps.9lhol");

  {
    c3_c* num_c = "0w40.00000.00000.00000.00000";
    u3_atom num = u3qc_bex(128);
    c3_c*  out_c;
    size_t len_i = u3s_etch_uw_c(num, &out_c);

    if ( 0 != strncmp(num_c, out_c, len_i) ) {
      fprintf(stderr, "etch_uw: (bex 128) fail; expected %s, got '%s'\r\n",
                      num_c, out_c);
      ret_i = 0;
    }
    else {
      u3_noun out = u3s_etch_uw(num);
      u3_noun tou = u3i_bytes(len_i, (c3_y*)out_c);

      if ( c3n == u3r_sing(tou, out) ) {
        fprintf(stderr, "etch_uw: (bex 128) mismatch; expected %s\r\n", num_c);
        u3m_p("out", out);
        ret_i = 0;
      }

      u3z(out);
      u3z(tou);
    }

    c3_free(out_c);
    u3z(num);
  }

  return ret_i;
}
static inline c3_i
_da_good(c3_d hi, c3_d lo, const c3_c* dat_c)
{
  u3_weak out;

  out = u3s_sift_da_bytes(strlen(dat_c), (c3_y*)dat_c);

  if ( u3_none == out) {
    fprintf(stderr, "sift_da: %s fail; expected hi: 0x%llx, lo: 0x%llx\r\n", 
        dat_c, hi, lo);

    return 0;
  }

  c3_d out_lo = u3r_chub(0, out);

  // Careful, works only on 128-bit dates
  c3_d out_hi = u3r_chub(1, out);

  if ( out_hi != hi || out_lo != lo ) {
    fprintf(stderr, "sift_da: %s fail; expected 0x%llx,0x%llx: actual 0x%llx,0x%llx\r\n",dat_c, hi, lo, out_hi, out_lo);

    u3z(out);

    return 0;
  }

  u3z(out);

  return 1;
}

static inline c3_i
_da_fail(const c3_c* dat_c)
{
  u3_weak out;

  if ( u3_none != (out = u3s_sift_da_bytes(strlen(dat_c), (c3_y*)dat_c)) ) {
    u3m_p("out", out);
    fprintf(stderr, "sift_da: %s expected fail\r\n", dat_c);

    u3z(out);

    return 0;
  }

  u3z(out);

  return 1;
}

static c3_i 
_test_sift_da(void)
{
  c3_i ret_i = 1;

  ret_i &= _da_good(0x0, 0x0, "~292277024401-.1.1");
  ret_i &= _da_good(0x7ffffffe58e40f80, 
                    0xbabe000000000000, 
                    "~1.12.25..00.00.00..babe");
  ret_i &= _da_good(0x7ffffffe93b72f70, 
                    0x3300000000000000, 
                    "~33.4.3..15.00.00..3300");
  ret_i &= _da_good(0x7ffffffe93b72f70, 
                    0x3300000000000000, 
                    "~33.4.3..15.00.00..3300");
  ret_i &= _da_good(0x8000000d32bb462f, 
                    0xcafe000000000000, 
                    "~2023.3.24..05.44.15..cafe");
  ret_i &= _da_good(0x8000000d32bb462f, 
                    0xcafe00000000, 
                    "~2023.3.24..05.44.15..0000.cafe");
  ret_i &= _da_good(0x8000000d32bb462f, 
                    0xcafe0000, 
                    "~2023.3.24..05.44.15..0000.0000.cafe");
  ret_i &= _da_good(0x8000000d32bb462f, 
                    0xcafe, 
                    "~2023.3.24..05.44.15..0000.0000.0000.cafe");
  ret_i &= _da_good(0x8000000d329d6f76, 
                    0xadef000000000000, 
                    "~2023.3.1..14.32.22..adef");
  ret_i &= _da_good(0x8000000d32c33b88, 
                    0x2d00000000000000, 
                    "~2023.3.30..06.36.56..2d00");
  ret_i &= _da_good(0x8000000d32c51c00, 
                    0x2d00000000000000, 
                    "~2023.3.31..16.46.56..2d00");
  ret_i &= _da_good(0x8000000d3a19f0c0, 
                    0x2d00000000000000, 
                    "~2027.2.22..07.26.56..2d00");
  ret_i &= _da_good(0x80000029dd78fec0, 
                    0x2d00000000000000, 
                    "~5924.11.10..10.06.56..2d00");
  ret_i &= _da_good(0x8000700808c7aec0, 
                    0x2d00000000000000, 
                    "~3903639.9.11..12.46.56..2d00");

  ret_i &= _da_fail("~2023--.1.1");
  ret_i &= _da_fail("~2.023.1.1");
  ret_i &= _da_fail("~2023.01.1");
  ret_i &= _da_fail("~2023.1.01");
  ret_i &= _da_fail("~2023.13.1");
  ret_i &= _da_fail("~2023.12.32");
  ret_i &= _da_fail("~2023.2.31");
  ret_i &= _da_fail("~2023.2.29");

  ret_i &= _da_fail("~2023.3.3..24.00.00");
  ret_i &= _da_fail("~2023.3.3..24.00.00..ca");
  ret_i &= _da_fail("~2023.3.3..24.00.00..cAFE");
  ret_i &= _da_fail("~2023.3.3..24.00.00..cAFE");
  ret_i &= _da_fail("~2023.3.3..24.00.00..cafe.cafe.");

  return ret_i;
}


static inline c3_i
_ud_good(c3_w num_w, const c3_c* num_c)
{
  u3_weak out;
  if ( num_w != (out = u3s_sift_ud_bytes(strlen(num_c), (c3_y*)num_c)) ) {
    if ( u3_none == out ) {
      fprintf(stderr, "sift_ud: %s fail; expected %u\r\n", num_c, num_w);
    }
    else {
      fprintf(stderr, "sift_ud: %s wrong; expected %u: actual %u\r\n", num_c, num_w, out);
    }
    return 0;
  }

  return 1;
}

static inline c3_i
_ud_fail(const c3_c* num_c)
{
  u3_weak out;
  if ( u3_none != (out = u3s_sift_ud_bytes(strlen(num_c), (c3_y*)num_c)) ) {
    u3m_p("out", out);
    fprintf(stderr, "sift_ud: %s expected fail\r\n", num_c);
    return 0;
  }

  return 1;
}

static c3_i
_test_sift_ud(void)
{
  c3_i ret_i = 1;

  ret_i &= _ud_good(0, "0");
  ret_i &= _ud_good(1, "1");
  ret_i &= _ud_good(12, "12");
  ret_i &= _ud_good(123, "123");
  ret_i &= _ud_good(1234, "1.234");
  ret_i &= _ud_good(12345, "12.345");
  ret_i &= _ud_good(123456, "123.456");
  ret_i &= _ud_good(1234567, "1.234.567");
  ret_i &= _ud_good(12345678, "12.345.678");
  ret_i &= _ud_good(123456789, "123.456.789");
  ret_i &= _ud_good(100000000, "100.000.000");
  ret_i &= _ud_good(101101101, "101.101.101");
  ret_i &= _ud_good(201201201, "201.201.201");
  ret_i &= _ud_good(302201100, "302.201.100");

  ret_i &= _ud_fail("01");
  ret_i &= _ud_fail("02");
  ret_i &= _ud_fail("003");
  ret_i &= _ud_fail("1234");
  ret_i &= _ud_fail("1234.5");
  ret_i &= _ud_fail("1234.567.8");
  ret_i &= _ud_fail("1234.56..78.");
  ret_i &= _ud_fail("123.45a");
  ret_i &= _ud_fail(".123.456");

  {
    c3_c* num_c = "4.294.967.296";
    u3_weak out = u3s_sift_ud_bytes(strlen(num_c), (c3_y*)num_c);
    u3_atom pro = u3qc_bex(32);

    if ( u3_none == out ) {
      fprintf(stderr, "sift_ud: (bex 32) fail\r\n");
      ret_i = 0;
    }
    else { 
      if ( c3n == u3r_sing(pro, out) ) {
        u3m_p("out", out);
        fprintf(stderr, "sift_ud: (bex 32) wrong\r\n");
        ret_i = 0;
      }
    }

    u3z(out); u3z(pro);
  }


  {
    c3_c* num_c = "340.282.366.920.938.463.463.374.607.431.768.211.456";
    u3_weak out = u3s_sift_ud_bytes(strlen(num_c), (c3_y*)num_c);
    u3_atom pro = u3qc_bex(128);

    if ( u3_none == out ) {
      fprintf(stderr, "sift_ud: (bex 128) fail\r\n");
      ret_i = 0;
    }

    else {
      if ( c3n == u3r_sing(pro, out) ) {
        u3m_p("out", out);
        fprintf(stderr, "sift_ud: (bex 128) wrong\r\n");
        ret_i = 0;
      }
    }

    u3z(out); u3z(pro);
  }

  return ret_i;
}

static inline c3_i
_ui_good(c3_w num_w, const c3_c* num_c)
{
  u3_weak out;
  if ( num_w != (out = u3s_sift_ui_bytes(strlen(num_c), (c3_y*)num_c)) ) {
    if ( u3_none == out ) {
      fprintf(stderr, "sift_ui: %s fail; expected %u\r\n", num_c, num_w);
    }
    else {
      fprintf(stderr, "sift_ui: %s wrong; expected %u: actual %u\r\n", num_c, num_w, out);
    }
    return 0;
  }

  return 1;
}

static inline c3_i
_ui_fail(const c3_c* num_c)
{
  u3_weak out;
  if ( u3_none != (out = u3s_sift_ui_bytes(strlen(num_c), (c3_y*)num_c)) ) {
    u3m_p("out", out);
    fprintf(stderr, "sift_ui: %s expected fail\r\n", num_c);
    return 0;
  }

  return 1;
}

static c3_i
_test_sift_ui(void)
{
  c3_i ret_i = 1;

  ret_i &= _ui_good(0, "0i0");
  ret_i &= _ui_good(1, "0i1");
  ret_i &= _ui_good(12, "0i12");
  ret_i &= _ui_good(123, "0i123");
  ret_i &= _ui_good(1234, "0i1234");
  ret_i &= _ui_good(12345, "0i12345");
  ret_i &= _ui_good(123456, "0i123456");
  ret_i &= _ui_good(1234567, "0i1234567");
  ret_i &= _ui_good(12345678, "0i12345678");
  ret_i &= _ui_good(123456789, "0i123456789");
  ret_i &= _ui_good(100000000, "0i100000000");
  ret_i &= _ui_good(101101101, "0i101101101");
  ret_i &= _ui_good(201201201, "0i201201201");
  ret_i &= _ui_good(302201100, "0i302201100");

  ret_i &= _ui_fail("0i");
  ret_i &= _ui_fail("i0");
  ret_i &= _ui_fail("0i01");

  {
    c3_c* num_c = "0i4294967296";
    u3_weak out = u3s_sift_ui_bytes(strlen(num_c), (c3_y*)num_c);
    u3_atom pro = u3qc_bex(32);

    if ( u3_none == out ) {
      fprintf(stderr, "sift_ui: (bex 32) fail\r\n");
      ret_i = 0;
    }

    else {
      if ( c3n == u3r_sing(pro, out) ) {
        u3m_p("out", out);
        fprintf(stderr, "sift_ui: (bex 32) wrong\r\n");
        ret_i = 0;
      }
    }

    u3z(out); u3z(pro);
  }


  {
    c3_c* num_c = "0i340282366920938463463374607431768211456";
    u3_weak out = u3s_sift_ui_bytes(strlen(num_c), (c3_y*)num_c);
    u3_atom pro = u3qc_bex(128);

    if ( u3_none == out ) {
      fprintf(stderr, "sift_ui: (bex 128) fail\r\n");
      ret_i = 0;
    }

    else {
      if ( c3n == u3r_sing(pro, out) ) {
        u3m_p("out", out);
        fprintf(stderr, "sift_ui: (bex 128) wrong\r\n");
        ret_i = 0;
      }
    }

    u3z(out); u3z(pro);
  }

  return ret_i;
}

static inline c3_i
_uv_good(c3_d num_d, const c3_c* num_c)
{
  u3_weak out;

  out = u3s_sift_uv_bytes(strlen(num_c), (c3_y*)num_c);

  if ( c3y == u3a_is_cat(out) ) {

    if ( num_d != (out = u3s_sift_uv_bytes(strlen(num_c), (c3_y*)num_c)) ) {
      if ( u3_none == out ) {
        fprintf(stderr, "sift_uv: %s fail; expected 0x%llx\r\n", num_c, num_d);
      }
      else {
        fprintf(stderr, "sift_uv: %s wrong; expected 0x%llx: actual 0x%x\r\n", num_c, num_d, out);
      }

      return 0;
    }
  }
  
  else {
    c3_d out_d = u3r_chub(0, out);

    if ( num_d != out_d ) {
        fprintf(stderr, "sift_uv: %s wrong; expected 0x%llx: actual 0x%llx\r\n", num_c, num_d, out_d);

        u3z(out);
        return 0;
    }
    u3z(out);
  }


  return 1;

}

static inline c3_i
_uv_fail(const c3_c* num_c)
{
  u3_weak out;
  if ( u3_none != (out = u3s_sift_uv_bytes(strlen(num_c), (c3_y*)num_c)) ) {
    u3m_p("out", out);
    fprintf(stderr, "sift_uv: %s expected fail\r\n", num_c);
    u3z(out);

    return 0;
  }


  return 1;
}

static c3_i
_test_sift_uv(void)
{
  c3_i ret_i = 1;


  ret_i &= _uv_good(0x0, "0v0");
  ret_i &= _uv_good(0x1, "0v1");
  ret_i &= _uv_good(0x110c85, "0v12345");
  ret_i &= _uv_good(0x63a12a, "0v6789a");
  ret_i &= _uv_good(0xb635cf, "0vbcdef");
  ret_i &= _uv_good(0x108ca74, "0vghijk");
  ret_i &= _uv_good(0x15b5f19, "0vlmnop");
  ret_i &= _uv_good(0x1adf3be, "0vqrstu");
  ret_i &= _uv_good(0xa5b1bf, "0vabcdv");
  ret_i &= _uv_good(0x886110c85, "0v123.12345");
  ret_i &= _uv_good(0x88663a12a, "0v123.6789a");
  ret_i &= _uv_good(0x886b635cf, "0v123.bcdef");
  ret_i &= _uv_good(0x88708ca74, "0v123.ghijk");
  ret_i &= _uv_good(0x8875b5f19, "0v123.lmnop");
  ret_i &= _uv_good(0x887adf3be, "0v123.qrstu");
  ret_i &= _uv_good(0x887f0887f, "0v123.v123v");
  ret_i &= _uv_good(0xfffffffffffffff, "0vvv.vvvvv.vvvvv");

  ret_i &= _uv_fail("0v");
  ret_i &= _uv_fail("v0");
  ret_i &= _uv_fail("0v01");
  ret_i &= _uv_fail("0v12.345");
  ret_i &= _uv_fail("0v12.f3456.v789");
  ret_i &= _uv_fail("0v1.3456v.v789vv");

  {
    c3_c* num_c = "0v40.00000";
    u3_weak out = u3s_sift_uv_bytes(strlen(num_c), (c3_y*)num_c);
    u3_atom pro = u3qc_bex(32);

    if ( u3_none == out ) {
      fprintf(stderr, "sift_uv: (bex 32) fail\r\n");
      ret_i = 0;
    }

    else {
      if ( c3n == u3r_sing(pro, out) ) {
        u3m_p("out", out);
        fprintf(stderr, "sift_uv: (bex 32) wrong\r\n");
        ret_i = 0;
      }
    }

    u3z(out); u3z(pro);
  }

  {
    c3_c* num_hex_c = "0x1.1234.5678.9abc.def0.1234.5678.9abc.def0";
    c3_c* num_c = "0v8.i6hb7.h6lsr.ro14d.2mf2d.bpnng";

    u3_weak out_hex = u3s_sift_ux_bytes(strlen(num_hex_c), (c3_y*)num_hex_c);
    u3_atom out = u3s_sift_uv_bytes(strlen(num_c), (c3_y*)num_c);

    if ( u3_none == out) {
      fprintf(stderr, "sift_uv: big viz fail\r\n");
      ret_i = 0;
    }

    if ( u3_none == out_hex ) {
      fprintf(stderr, "sift_uv: big hex fail during big viz test\r\n");
      ret_i = 0;
    }

    else {
      if ( c3n == u3r_sing(out_hex, out) ) {
        u3m_p("out", out);
        u3m_p("out_hex", out_hex);
        fprintf(stderr, "sift_uv: big viz wrong\r\n");
        ret_i = 0;
      }
    }

    u3z(out_hex);
    u3z(out);
  }

  return ret_i;
}

static inline c3_i
_uw_good(c3_d num_d, const c3_c* num_c)
{
  u3_weak out;

  out = u3s_sift_uw_bytes(strlen(num_c), (c3_y*)num_c);

  if ( c3y == u3a_is_cat(out) ) {

    if ( num_d != (out = u3s_sift_uw_bytes(strlen(num_c), (c3_y*)num_c)) ) {
      if ( u3_none == out ) {
        fprintf(stderr, "sift_uw: %s fail; expected 0x%llx\r\n", num_c, num_d);
      }
      else {
        fprintf(stderr, "sift_uw: %s wrong; expected 0x%llx: actual 0x%x\r\n", num_c, num_d, out);
      }

      return 0;
    }
  }
  
  else {
    c3_d out_d = u3r_chub(0, out);

    if ( num_d != out_d ) {
        fprintf(stderr, "sift_uw: %s wrong; expected 0x%llx: actual 0x%llx\r\n", num_c, num_d, out_d);

        u3z(out);
        return 0;
    }
    u3z(out);
  }


  return 1;

}

static inline c3_i
_uw_fail(const c3_c* num_c)
{
  u3_weak out;
  if ( u3_none != (out = u3s_sift_uw_bytes(strlen(num_c), (c3_y*)num_c)) ) {
    u3m_p("out", out);
    fprintf(stderr, "sift_uw: %s expected fail\r\n", num_c);
    u3z(out);

    return 0;
  }


  return 1;
}

static c3_i
_test_sift_uw(void)
{
  c3_i ret_i = 1;

  
  ret_i &= _uw_good(0x0, "0w0");
  ret_i &= _uw_good(0x1, "0w1");
  ret_i &= _uw_good(0x1083105, "0w12345");
  ret_i &= _uw_good(0x61c824a, "0w6789a");
  ret_i &= _uw_good(0xb30d38f, "0wbcdef");
  ret_i &= _uw_good(0x104524d4, "0wghijk");
  ret_i &= _uw_good(0x15597619, "0wlmnop");
  ret_i &= _uw_good(0x1a6dc75e, "0wqrstu");
  ret_i &= _uw_good(0x1f8218a3, "0wvwxyz");
  ret_i &= _uw_good(0x249669e8, "0wABCDE");
  ret_i &= _uw_good(0x29aabb2d, "0wFGHIJ");
  ret_i &= _uw_good(0x2ebf0c72, "0wKLMNO");
  ret_i &= _uw_good(0x33d35db7, "0wPQRST");
  ret_i &= _uw_good(0x38e7aefc, "0wUVWXY");
  ret_i &= _uw_good(0x3dffe24a, "0wZ~-9a");
  ret_i &= _uw_good(0x3efbefbe, "0w-----");
  ret_i &= _uw_good(0x3fffffff, "0w~~~~~");
  ret_i &= _uw_good(0x3effeffe, "0w-~-~-");
  ret_i &= _uw_good(0x820820820820820, "0wwwwww.wwwwww");

  ret_i &= _uw_fail("w0");
  ret_i &= _uw_fail("0w01");
  ret_i &= _uw_fail("0w12.345");
  ret_i &= _uw_fail("0w1~.f3456.-789");
  ret_i &= _uw_fail("0w1.3456-.-789~-");

  {
    c3_c* num_c = "0w4.00000";
    u3_weak out = u3s_sift_uw_bytes(strlen(num_c), (c3_y*)num_c);
    u3_atom pro = u3qc_bex(32);

    if ( u3_none == out ) {
      fprintf(stderr, "sift_uw: (bex 32) fail\r\n");
      ret_i = 0;
    }

    else {
      if ( c3n == u3r_sing(pro, out) ) {
        u3m_p("out", out);
        fprintf(stderr, "sift_uw: (bex 32) wrong\r\n");
        ret_i = 0;
      }
    }

    u3z(out); u3z(pro);
  }

  {
    
    c3_c* num_hex_c = "0w9.37a8e.elucg.lcgpl.~--38.alllz.-----.~~~~~";
    c3_c* num_c = "0x24.31ca.20e3.9578.c415.3106.55ff.ef83.20a5.5556.3fbe.fbef.bfff.ffff";

    u3_weak out_hex = u3s_sift_ux_bytes(strlen(num_hex_c), (c3_y*)num_hex_c);
    u3_atom out = u3s_sift_uw_bytes(strlen(num_c), (c3_y*)num_c);

    if ( u3_none == out) {
      fprintf(stderr, "sift_uw: big wiz fail\r\n");
      ret_i = 0;
    }

    if ( u3_none == out_hex ) {
      fprintf(stderr, "sift_uw: big hex fail during big wiz test\r\n");
      ret_i = 0;
    }

    else {
      if ( c3n == u3r_sing(out_hex, out) ) {
        u3m_p("out", out);
        u3m_p("out_hex", out_hex);
        fprintf(stderr, "sift_uw: big wiz wrong\r\n");
        ret_i = 0;
      }
    }

    u3z(out_hex);
    u3z(out);
  }

  return ret_i;
}

static inline c3_i
_ux_good(c3_d num_d, const c3_c* num_c)
{
  u3_weak out;

  out = u3s_sift_ux_bytes(strlen(num_c), (c3_y*)num_c);

  if ( c3y == u3a_is_cat(out) ) {

    if ( num_d != (out = u3s_sift_ux_bytes(strlen(num_c), (c3_y*)num_c)) ) {
      if ( u3_none == out ) {
        fprintf(stderr, "sift_ux: %s fail; expected 0x%llx\r\n", num_c, num_d);
      }
      else {
        fprintf(stderr, "sift_ux: %s wrong; expected 0x%llx: actual 0x%x\r\n", num_c, num_d, out);
      }

      return 0;
    }
  }
  else {
    c3_d out_d = u3r_chub(0, out);

    if ( num_d != out_d ) {
        fprintf(stderr, "sift_ux: %s wrong; expected 0x%llx: actual 0x%llx\r\n", num_c, num_d, out_d);

        u3z(out);
        return 0;
    }

    u3z(out);
  }

  return 1;
}

static inline c3_i
_ux_fail(const c3_c* num_c)
{
  u3_weak out;
  if ( u3_none != (out = u3s_sift_ux_bytes(strlen(num_c), (c3_y*)num_c)) ) {
    u3m_p("out", out);
    fprintf(stderr, "sift_ux: %s expected fail\r\n", num_c);
    return 0;
  }

  return 1;
}

static c3_i
_test_sift_ux(void)
{
  c3_i ret_i = 1;

  ret_i &= _ux_good(0x0, "0x0");
  ret_i &= _ux_good(0x1, "0x1");
  ret_i &= _ux_good(0x12, "0x12");
  ret_i &= _ux_good(0x1a3, "0x1a3");
  ret_i &= _ux_good(0x123b, "0x123b");
  ret_i &= _ux_good(0x1234c, "0x1.234c");
  ret_i &= _ux_good(0x12e3e56, "0x12e.3e56");
  ret_i &= _ux_good(0x1234e67, "0x123.4e67");
  ret_i &= _ux_good(0x1234567f, "0x1234.567f");
  ret_i &= _ux_good(0x123456789, "0x1.2345.6789");
  ret_i &= _ux_good(0x100000000, "0x1.0000.0000");
  ret_i &= _ux_good(0x101101101, "0x1.0110.1101");
  ret_i &= _ux_good(0x201201201, "0x2.0120.1201");
  ret_i &= _ux_good(0x302201100, "0x3.0220.1100");

  ret_i &= _ux_fail("0x");
  ret_i &= _ux_fail("x0");
  ret_i &= _ux_fail("0x01");
  ret_i &= _ux_fail("0x12.345");
  ret_i &= _ux_fail("0x12.3456.789");
  ret_i &= _ux_fail("0x1.2.3456.789");

  {
    c3_c* num_c = "0x1.0000.0000";
    u3_weak out = u3s_sift_ux_bytes(strlen(num_c), (c3_y*)num_c);
    u3_atom pro = u3qc_bex(32);

    if ( u3_none == out ) {
      fprintf(stderr, "sift_ux: (bex 32) fail\r\n");
      ret_i = 0;
    }

    else {
      if ( c3n == u3r_sing(pro, out) ) {
        u3m_p("out", out);
        fprintf(stderr, "sift_ux: (bex 32) wrong\r\n");
        ret_i = 0;
      }
    }

    u3z(out); u3z(pro);
  }

  {
    c3_c* num_c = "0x1.1234.5678.9abc.def0.1234.5678.9abc.def0";
    c3_c* bnum_c = "0x1234.5678.9abc.def0";

    u3_weak out = u3s_sift_ux_bytes(strlen(num_c), (c3_y*)num_c);
    u3_atom bout = u3s_sift_ux_bytes(strlen(bnum_c), (c3_y*)bnum_c);

    u3_atom pro = u3qc_bex(128);
    u3_atom bpro = u3qa_add(pro, bout);
    u3_atom cpro = u3qc_lsh(6,1, bout);
    u3_atom dpro = u3qa_add(bpro, cpro);

    if ( u3_none == out ) {
      fprintf(stderr, "sift_ux: big hex fail\r\n");
      ret_i = 0;
    }

    else {
      if ( c3n == u3r_sing(dpro, out) ) {
        u3m_p("out", out);
        fprintf(stderr, "sift_ux: big hex wrong\r\n");
        ret_i = 0;
      }
    }

    u3z(out); u3z(bout);
    u3z(pro); u3z(bpro); 
    u3z(cpro); u3z(dpro);
  }

  return ret_i;
}

static c3_i
_test_en_base16(void)
{
  c3_i ret_i = 1;

  {
    u3_atom dat = 0xaa;
    u3_atom pro = u3qe_en_base16(u3r_met(3, dat), dat);

    if ( c3n == u3r_sing_c("aa", pro) ) {
      fprintf(stderr, "en_base16: fail (a)\r\n");
      ret_i = 0;
    }

    u3z(pro);
  }

  {
    u3_atom dat = 0x1234;
    u3_atom pro = u3qe_en_base16(u3r_met(3, dat), dat);

    if ( c3n == u3r_sing_c("1234", pro) ) {
      fprintf(stderr, "en_base16: fail (b)\r\n");
      ret_i = 0;
    }

    u3z(pro);
  }

  {
    u3_atom dat = 0xf012;
    u3_atom pro = u3qe_en_base16(u3r_met(3, dat), dat);

    if ( c3n == u3r_sing_c("f012", pro) ) {
      fprintf(stderr, "en_base16: fail (c)\r\n");
      ret_i = 0;
    }

    u3z(pro);
  }

  {
    u3_atom dat = 0x10b;
    u3_atom pro = u3qe_en_base16(u3r_met(3, dat), dat);

    if ( c3n == u3r_sing_c("010b", pro) ) {
      fprintf(stderr, "en_base16: fail (d)\r\n");
      ret_i = 0;
    }

    u3z(pro);
  }

  {
    u3_atom pro = u3qe_en_base16(3, 0x1234);

    if ( c3n == u3r_sing_c("001234", pro) ) {
      fprintf(stderr, "en_base16: fail (e)\r\n");
      ret_i = 0;
    }

    u3z(pro);
  }

  {
    u3_atom pro = u3qe_en_base16(1, 0x1234);

    if ( c3n == u3r_sing_c("34", pro) ) {
      fprintf(stderr, "en_base16: fail (f)\r\n");
      ret_i = 0;
    }

    u3z(pro);
  }

  return ret_i;
}


static c3_i
_test_de_base16(void)
{
  c3_i ret_i = 1;

  {
    u3_noun inp = u3i_string("aa");
    u3_noun pro = u3qe_de_base16(inp);
    u3_atom len, dat;

    if ( c3n == u3r_pq(pro, u3_nul, &len, &dat) ) {
      fprintf(stderr, "de_base16: fail cell (a)\r\n");
      ret_i = 0;
    }

    if ( 1 != len ) {
      fprintf(stderr, "de_base16: fail len (a)\r\n");
      ret_i = 0;
    }

    if ( 0xaa != dat ) {
      fprintf(stderr, "de_base16: fail dat (a)\r\n");
      ret_i = 0;
    }

    u3z(inp); u3z(pro);
  }

  {
    u3_noun inp = u3i_string("1234");
    u3_noun pro = u3qe_de_base16(inp);
    u3_atom len, dat;

    if ( c3n == u3r_pq(pro, u3_nul, &len, &dat) ) {
      fprintf(stderr, "de_base16: fail cell (b)\r\n");
      ret_i = 0;
    }

    if ( 2 != len ) {
      fprintf(stderr, "de_base16: fail len (b)\r\n");
      ret_i = 0;
    }

    if ( 0x1234 != dat ) {
      fprintf(stderr, "de_base16: fail dat (b)\r\n");
      ret_i = 0;
    }

    u3z(inp); u3z(pro);
  }

  {
    u3_noun inp = u3i_string("f012");
    u3_noun pro = u3qe_de_base16(inp);
    u3_atom len, dat;

    if ( c3n == u3r_pq(pro, u3_nul, &len, &dat) ) {
      fprintf(stderr, "de_base16: fail cell (c)\r\n");
      ret_i = 0;
    }

    if ( 2 != len ) {
      fprintf(stderr, "de_base16: fail len (c)\r\n");
      ret_i = 0;
    }

    if ( 0xf012 != dat ) {
      fprintf(stderr, "de_base16: fail dat (c)\r\n");
      ret_i = 0;
    }

    u3z(inp); u3z(pro);
  }

  {
    u3_noun inp = u3i_string("010b");
    u3_noun pro = u3qe_de_base16(inp);
    u3_atom len, dat;

    if ( c3n == u3r_pq(pro, u3_nul, &len, &dat) ) {
      fprintf(stderr, "de_base16: fail cell (d)\r\n");
      ret_i = 0;
    }

    if ( 2 != len ) {
      fprintf(stderr, "de_base16: fail len (d)\r\n");
      ret_i = 0;
    }

    if ( 0x10b != dat ) {
      fprintf(stderr, "de_base16: fail dat (d)\r\n");
      ret_i = 0;
    }

    u3z(inp); u3z(pro);
  }

  {
    u3_noun inp = u3i_string("10b");
    u3_noun pro = u3qe_de_base16(inp);
    u3_atom len, dat;

    if ( c3n == u3r_pq(pro, u3_nul, &len, &dat) ) {
      fprintf(stderr, "de_base16: fail cell (e)\r\n");
      ret_i = 0;
    }

    if ( 2 != len ) {
      fprintf(stderr, "de_base16: fail len (e)\r\n");
      ret_i = 0;
    }

    if ( 0x10b != dat ) {
      fprintf(stderr, "de_base16: fail dat (e)\r\n");
      ret_i = 0;
    }

    u3z(inp); u3z(pro);
  }

  {
    u3_noun inp = u3i_string("001234");
    u3_noun pro = u3qe_de_base16(inp);
    u3_atom len, dat;

    if ( c3n == u3r_pq(pro, u3_nul, &len, &dat) ) {
      fprintf(stderr, "de_base16: fail cell (f)\r\n");
      ret_i = 0;
    }

    if ( 3 != len ) {
      fprintf(stderr, "de_base16: fail len (f)\r\n");
      ret_i = 0;
    }

    if ( 0x1234 != dat ) {
      fprintf(stderr, "de_base16: fail dat (f)\r\n");
      ret_i = 0;
    }

    u3z(inp); u3z(pro);
  }

  return ret_i;
}

static c3_i
_test_base16(void)
{
  c3_i ret_i = 1;

  ret_i &= _test_en_base16();
  ret_i &= _test_de_base16();

  return ret_i;
}

static c3_w
_fein_ob_w(c3_w inp_w)
{
  u3_atom inp = u3i_word(inp_w);
  u3_atom act = u3qe_fein_ob(inp);
  c3_w  act_w = u3r_word(0, act);
  u3z(inp); u3z(act);
  return act_w;
}

static c3_i
_expect_fein_ob_w(c3_w inp_w, c3_w exp_w)
{
  c3_w act_w = _fein_ob_w(inp_w);

  if ( act_w != exp_w ) {
    fprintf(stderr, "fein: inp=0x%08x exp=0x%08x act=0x%08x\n",
                    inp_w, exp_w, act_w);
    return 0;
  }

  return 1;
}

static c3_i
_test_fein_ob(void)
{
  c3_i ret_i = 1;

  ret_i &= _expect_fein_ob_w(0, 0);
  ret_i &= _expect_fein_ob_w(0xffff, 0xffff);
  ret_i &= _expect_fein_ob_w(0x1b08f, 0x76b920e5);
  ret_i &= _expect_fein_ob_w(0x10000, 0x423e60bf);
  ret_i &= _expect_fein_ob_w(0x10001, 0xd4400acb);
  ret_i &= _expect_fein_ob_w(0x10002, 0xf429043);
  ret_i &= _expect_fein_ob_w(0x10000000, 0xa04bc7fa);
  ret_i &= _expect_fein_ob_w(0x1234abcd, 0x686f6c25);
  ret_i &= _expect_fein_ob_w(0xabcd1234, 0x4a220c8);
  ret_i &= _expect_fein_ob_w(0xdeadbeef, 0x909bc4a9);
  ret_i &= _expect_fein_ob_w(0xfffff, 0x6746b96b);
  ret_i &= _expect_fein_ob_w(0xffffffff, 0xbba4dcce);

  return ret_i;
}

static c3_w
_fynd_ob_w(c3_w inp_w)
{
  u3_atom inp = u3i_word(inp_w);
  u3_atom act = u3qe_fynd_ob(inp);
  c3_w  act_w = u3r_word(0, act);
  u3z(inp); u3z(act);
  return act_w;
}

static c3_i
_expect_fynd_ob_w(c3_w exp_w, c3_w inp_w)
{
  c3_w act_w = _fynd_ob_w(inp_w);

  if ( act_w != exp_w ) {
    fprintf(stderr, "fynd: inp=0x%08x exp=0x%08x act=0x%08x\n",
                    inp_w, exp_w, act_w);
    return 0;
  }

  return 1;
}

static c3_i
_test_fynd_ob(void)
{
  c3_i ret_i = 1;

  ret_i &= _expect_fynd_ob_w(0, 0);
  ret_i &= _expect_fynd_ob_w(0xffff, 0xffff);
  ret_i &= _expect_fynd_ob_w(0x10000, 0x423e60bf);
  ret_i &= _expect_fynd_ob_w(0x10001, 0xd4400acb);
  ret_i &= _expect_fynd_ob_w(0x10002, 0xf429043);
  ret_i &= _expect_fynd_ob_w(0x10000000, 0xa04bc7fa);
  ret_i &= _expect_fynd_ob_w(0x1234abcd, 0x686f6c25);
  ret_i &= _expect_fynd_ob_w(0xabcd1234, 0x4a220c8);
  ret_i &= _expect_fynd_ob_w(0xdeadbeef, 0x909bc4a9);
  ret_i &= _expect_fynd_ob_w(0xfffff, 0x6746b96b);
  ret_i &= _expect_fynd_ob_w(0xffffffff, 0xbba4dcce);

  return ret_i;
}

static c3_i
_exhaust_roundtrip_fein_fynd_ob(void)
{
  c3_i ret_i = 1;
  c3_w fyn_w, i_w;

  {
    u3_atom fen, fyn;

    for ( i_w = 0x10000; i_w < 0x80000000; i_w++ ) {
      fen   = u3qe_fein_ob(i_w);
      fyn   = u3qe_fynd_ob(fen);
      fyn_w = u3r_word(0, fyn);

      if ( i_w != fyn_w ) {
        fprintf(stderr, "fein/fynd: inp=0x%08x fein=0x%08x fynd=0x%08x\n",
                        i_w, u3r_word(0, fen), fyn_w);
        ret_i = 0;
      }
      u3z(fen); u3z(fyn);

      if ( !(i_w % 0x1000000) ) {
        fprintf(stderr, "fein/fynd: 0x%x done\n", i_w);
      }
    }
  }

  {
    c3_w fen_w;

    do {
      fen_w = _fein_ob_w(i_w);
      fyn_w = _fynd_ob_w(fen_w);
      if ( i_w != fyn_w ) {
        fprintf(stderr, "fein/fynd: inp=0x%08x fein=0x%08x fynd=0x%08x\n",
                        i_w, fen_w, fyn_w);
        ret_i = 0;
      }

      if ( !(i_w % 0x1000000) ) {
        fprintf(stderr, "fein/fynd: 0x%x done\n", i_w);
      }
    }
    while ( ++i_w );
  }

  return ret_i;
}

static c3_i
_test_ob(void)
{
  c3_i ret_i = 1;
  ret_i &= _test_fein_ob();
  ret_i &= _test_fynd_ob();
  //  disabled, takes almost ~m15
  //
  // ret_i &= _exhaust_roundtrip_fein_fynd_ob();
  return ret_i;
}

static c3_i
_test_jets(void)
{
  c3_i ret_i = 1;

  if ( !_test_etch_da() ) {
    fprintf(stderr, "test jets: etch_da: failed\r\n");
    ret_i = 0;
  }

  if ( !_test_etch_ud() ) {
    fprintf(stderr, "test jets: etch_ud: failed\r\n");
    ret_i = 0;
  }

  if ( !_test_etch_uv() ) {
    fprintf(stderr, "test jets: etch_uv: failed\r\n");
    ret_i = 0;
  }

  if ( !_test_etch_uw() ) {
    fprintf(stderr, "test jets: etch_uw: failed\r\n");
    ret_i = 0;
  }

  if ( !_test_etch_ux() ) {
    fprintf(stderr, "test jets: etch_ux: failed\r\n");
    ret_i = 0;
  }

  if ( !_test_sift_da() ) {
    fprintf(stderr, "test jets: sift_da: failed\r\n");
    ret_i = 0;
  }

  if ( !_test_sift_ud() ) {
    fprintf(stderr, "test jets: sift_ud: failed\r\n");
    ret_i = 0;
  }

  if ( !_test_sift_ui() ) {
    fprintf(stderr, "test jets: sift_ui: failed\r\n");
    ret_i = 0;
  }

  if ( !_test_sift_uv() ) {
    fprintf(stderr, "test jets: sift_uv: failed\r\n");
    ret_i = 0;
  }

  if ( !_test_sift_ux() ) {
    fprintf(stderr, "test jets: sift_ux: failed\r\n");
    ret_i = 0;
  }

  if ( !_test_base16() ) {
    fprintf(stderr, "test jets: base16: failed\r\n");
    ret_i = 0;
  }

  if ( !_test_ob() ) {
    fprintf(stderr, "test jets: ob: failed\r\n");
    ret_i = 0;
  }

  return ret_i;
}

/* main(): run all test cases.
*/
int
main(int argc, char* argv[])
{
  _setup();

  if ( !_test_jets() ) {
    fprintf(stderr, "test jets: failed\r\n");
    u3m_grab(u3_none);
    exit(1);
  }

  //  GC
  //
  u3m_grab(u3_none);

  fprintf(stderr, "test jets: ok\r\n");
  return 0;
}
