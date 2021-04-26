/* MIT License
 *
 * Copyright (c) 2016-2020 INRIA, CMU and Microsoft Corporation
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */


#include "Hacl_GenericField32.h"

/*******************************************************************************

A verified field arithmetic library.

This is a 32-bit optimized version, where bignums are represented as an array
of `len` unsigned 32-bit integers, i.e. uint32_t[len].

All the arithmetic operations are performed in the Montgomery domain.

All the functions below preserve the following invariant for a bignum `aM` in
Montgomery form.
  • aM < n

*******************************************************************************/


/*
Check whether this library will work for a modulus `n`.

  The function returns false if any of the following preconditions are violated,
  true otherwise.
  • n % 2 = 1
  • 1 < n 
*/
bool Hacl_GenericField32_field_modulus_check(uint32_t len, uint32_t *n)
{
  uint32_t m = Hacl_Bignum_Montgomery_bn_check_modulus_u32(len, n);
  return m == (uint32_t)0xFFFFFFFFU;
}

/*
Heap-allocate and initialize a montgomery context.

  The argument n is meant to be `len` limbs in size, i.e. uint32_t[len].

  Before calling this function, the caller will need to ensure that the following
  preconditions are observed.
  • n % 2 = 1
  • 1 < n

  The caller will need to call Hacl_GenericField32_field_free on the return value
  to avoid memory leaks.
*/
Hacl_Bignum_MontArithmetic_bn_mont_ctx_u32
*Hacl_GenericField32_field_init(uint32_t len, uint32_t *n)
{
  KRML_CHECK_SIZE(sizeof (uint32_t), len);
  uint32_t *r2 = KRML_HOST_CALLOC(len, sizeof (uint32_t));
  KRML_CHECK_SIZE(sizeof (uint32_t), len);
  uint32_t *n1 = KRML_HOST_CALLOC(len, sizeof (uint32_t));
  uint32_t *r21 = r2;
  uint32_t *n11 = n1;
  memcpy(n11, n, len * sizeof (uint32_t));
  uint32_t nBits = (uint32_t)32U * Hacl_Bignum_Lib_bn_get_top_index_u32(len, n);
  Hacl_Bignum_Montgomery_bn_precomp_r2_mod_n_u32(len, nBits, n, r21);
  uint32_t mu = Hacl_Bignum_ModInvLimb_mod_inv_uint32(n[0U]);
  Hacl_Bignum_MontArithmetic_bn_mont_ctx_u32 res = { .len = len, .n = n11, .mu = mu, .r2 = r21 };
  KRML_CHECK_SIZE(sizeof (Hacl_Bignum_MontArithmetic_bn_mont_ctx_u32), (uint32_t)1U);
  Hacl_Bignum_MontArithmetic_bn_mont_ctx_u32
  *buf = KRML_HOST_MALLOC(sizeof (Hacl_Bignum_MontArithmetic_bn_mont_ctx_u32));
  buf[0U] = res;
  return buf;
}

/*
Deallocate the memory previously allocated by Hacl_GenericField32_field_init.

  The argument k is a montgomery context obtained through Hacl_GenericField32_field_init.
*/
void Hacl_GenericField32_field_free(Hacl_Bignum_MontArithmetic_bn_mont_ctx_u32 *k)
{
  Hacl_Bignum_MontArithmetic_bn_mont_ctx_u32 k1 = *k;
  uint32_t *n = k1.n;
  uint32_t *r2 = k1.r2;
  KRML_HOST_FREE(n);
  KRML_HOST_FREE(r2);
  KRML_HOST_FREE(k);
}

/*
Return the size of a modulus `n` in limbs.

  The argument k is a montgomery context obtained through Hacl_GenericField32_field_init.
*/
uint32_t Hacl_GenericField32_field_get_len(Hacl_Bignum_MontArithmetic_bn_mont_ctx_u32 *k)
{
  Hacl_Bignum_MontArithmetic_bn_mont_ctx_u32 k1 = *k;
  return k1.len;
}

/*
Convert a bignum from the regular representation to the Montgomery representation.

  Write `a * R mod n` in `aM`.

  The argument a and the outparam aM are meant to be `len` limbs in size, i.e. uint32_t[len].
  The argument k is a montgomery context obtained through Hacl_GenericField32_field_init.
*/
void
Hacl_GenericField32_to_field(
  Hacl_Bignum_MontArithmetic_bn_mont_ctx_u32 *k,
  uint32_t *a,
  uint32_t *aM
)
{
  uint32_t len1 = Hacl_GenericField32_field_get_len(k);
  Hacl_Bignum_MontArithmetic_bn_mont_ctx_u32 k1 = *k;
  Hacl_Bignum_Montgomery_bn_to_mont_u32(len1, k1.n, k1.mu, k1.r2, a, aM);
}

/*
Convert a result back from the Montgomery representation to the regular representation.

  Write `aM / R mod n` in `a`, i.e.
  Hacl_GenericField32_from_field(k, Hacl_GenericField32_to_field(k, a)) == a % n

  The argument aM and the outparam a are meant to be `len` limbs in size, i.e. uint32_t[len].
  The argument k is a montgomery context obtained through Hacl_GenericField32_field_init.
*/
void
Hacl_GenericField32_from_field(
  Hacl_Bignum_MontArithmetic_bn_mont_ctx_u32 *k,
  uint32_t *aM,
  uint32_t *a
)
{
  uint32_t len1 = Hacl_GenericField32_field_get_len(k);
  Hacl_Bignum_MontArithmetic_bn_mont_ctx_u32 k1 = *k;
  Hacl_Bignum_Montgomery_bn_from_mont_u32(len1, k1.n, k1.mu, aM, a);
}

/*
Write `aM + bM mod n` in `cM`.

  The arguments aM, bM, and the outparam cM are meant to be `len` limbs in size, i.e. uint32_t[len].
  The argument k is a montgomery context obtained through Hacl_GenericField32_field_init.
*/
void
Hacl_GenericField32_add(
  Hacl_Bignum_MontArithmetic_bn_mont_ctx_u32 *k,
  uint32_t *aM,
  uint32_t *bM,
  uint32_t *cM
)
{
  uint32_t len1 = Hacl_GenericField32_field_get_len(k);
  Hacl_Bignum_MontArithmetic_bn_mont_ctx_u32 k1 = *k;
  Hacl_Bignum_bn_add_mod_n_u32(len1, k1.n, aM, bM, cM);
}

/*
Write `aM - bM mod n` to `cM`.

  The arguments aM, bM, and the outparam cM are meant to be `len` limbs in size, i.e. uint32_t[len].
  The argument k is a montgomery context obtained through Hacl_GenericField32_field_init.
*/
void
Hacl_GenericField32_sub(
  Hacl_Bignum_MontArithmetic_bn_mont_ctx_u32 *k,
  uint32_t *aM,
  uint32_t *bM,
  uint32_t *cM
)
{
  uint32_t len1 = Hacl_GenericField32_field_get_len(k);
  Hacl_Bignum_MontArithmetic_bn_mont_ctx_u32 k1 = *k;
  uint32_t c0 = (uint32_t)0U;
  for (uint32_t i = (uint32_t)0U; i < k1.len / (uint32_t)4U * (uint32_t)4U / (uint32_t)4U; i++)
  {
    uint32_t t1 = aM[(uint32_t)4U * i];
    uint32_t t20 = bM[(uint32_t)4U * i];
    uint32_t *res_i0 = cM + (uint32_t)4U * i;
    c0 = Lib_IntTypes_Intrinsics_sub_borrow_u32(c0, t1, t20, res_i0);
    uint32_t t10 = aM[(uint32_t)4U * i + (uint32_t)1U];
    uint32_t t21 = bM[(uint32_t)4U * i + (uint32_t)1U];
    uint32_t *res_i1 = cM + (uint32_t)4U * i + (uint32_t)1U;
    c0 = Lib_IntTypes_Intrinsics_sub_borrow_u32(c0, t10, t21, res_i1);
    uint32_t t11 = aM[(uint32_t)4U * i + (uint32_t)2U];
    uint32_t t22 = bM[(uint32_t)4U * i + (uint32_t)2U];
    uint32_t *res_i2 = cM + (uint32_t)4U * i + (uint32_t)2U;
    c0 = Lib_IntTypes_Intrinsics_sub_borrow_u32(c0, t11, t22, res_i2);
    uint32_t t12 = aM[(uint32_t)4U * i + (uint32_t)3U];
    uint32_t t2 = bM[(uint32_t)4U * i + (uint32_t)3U];
    uint32_t *res_i = cM + (uint32_t)4U * i + (uint32_t)3U;
    c0 = Lib_IntTypes_Intrinsics_sub_borrow_u32(c0, t12, t2, res_i);
  }
  for (uint32_t i = k1.len / (uint32_t)4U * (uint32_t)4U; i < k1.len; i++)
  {
    uint32_t t1 = aM[i];
    uint32_t t2 = bM[i];
    uint32_t *res_i = cM + i;
    c0 = Lib_IntTypes_Intrinsics_sub_borrow_u32(c0, t1, t2, res_i);
  }
  uint32_t c00 = c0;
  KRML_CHECK_SIZE(sizeof (uint32_t), k1.len);
  uint32_t *tmp = alloca(k1.len * sizeof (uint32_t));
  memset(tmp, 0U, k1.len * sizeof (uint32_t));
  uint32_t c = (uint32_t)0U;
  for (uint32_t i = (uint32_t)0U; i < k1.len / (uint32_t)4U * (uint32_t)4U / (uint32_t)4U; i++)
  {
    uint32_t t1 = cM[(uint32_t)4U * i];
    uint32_t t20 = k1.n[(uint32_t)4U * i];
    uint32_t *res_i0 = tmp + (uint32_t)4U * i;
    c = Lib_IntTypes_Intrinsics_add_carry_u32(c, t1, t20, res_i0);
    uint32_t t10 = cM[(uint32_t)4U * i + (uint32_t)1U];
    uint32_t t21 = k1.n[(uint32_t)4U * i + (uint32_t)1U];
    uint32_t *res_i1 = tmp + (uint32_t)4U * i + (uint32_t)1U;
    c = Lib_IntTypes_Intrinsics_add_carry_u32(c, t10, t21, res_i1);
    uint32_t t11 = cM[(uint32_t)4U * i + (uint32_t)2U];
    uint32_t t22 = k1.n[(uint32_t)4U * i + (uint32_t)2U];
    uint32_t *res_i2 = tmp + (uint32_t)4U * i + (uint32_t)2U;
    c = Lib_IntTypes_Intrinsics_add_carry_u32(c, t11, t22, res_i2);
    uint32_t t12 = cM[(uint32_t)4U * i + (uint32_t)3U];
    uint32_t t2 = k1.n[(uint32_t)4U * i + (uint32_t)3U];
    uint32_t *res_i = tmp + (uint32_t)4U * i + (uint32_t)3U;
    c = Lib_IntTypes_Intrinsics_add_carry_u32(c, t12, t2, res_i);
  }
  for (uint32_t i = k1.len / (uint32_t)4U * (uint32_t)4U; i < k1.len; i++)
  {
    uint32_t t1 = cM[i];
    uint32_t t2 = k1.n[i];
    uint32_t *res_i = tmp + i;
    c = Lib_IntTypes_Intrinsics_add_carry_u32(c, t1, t2, res_i);
  }
  uint32_t c1 = c;
  uint32_t c2 = (uint32_t)0U - c00;
  for (uint32_t i = (uint32_t)0U; i < k1.len; i++)
  {
    uint32_t *os = cM;
    uint32_t x = (c2 & tmp[i]) | (~c2 & cM[i]);
    os[i] = x;
  }
}

/*
Write `aM * bM mod n` in `cM`.

  The arguments aM, bM, and the outparam cM are meant to be `len` limbs in size, i.e. uint32_t[len].
  The argument k is a montgomery context obtained through Hacl_GenericField32_field_init.
*/
void
Hacl_GenericField32_mul(
  Hacl_Bignum_MontArithmetic_bn_mont_ctx_u32 *k,
  uint32_t *aM,
  uint32_t *bM,
  uint32_t *cM
)
{
  uint32_t len1 = Hacl_GenericField32_field_get_len(k);
  Hacl_Bignum_MontArithmetic_bn_mont_ctx_u32 k1 = *k;
  Hacl_Bignum_Montgomery_bn_mont_mul_u32(len1, k1.n, k1.mu, aM, bM, cM);
}

/*
Write `aM * aM mod n` in `cM`.

  The argument aM and the outparam cM are meant to be `len` limbs in size, i.e. uint32_t[len].
  The argument k is a montgomery context obtained through Hacl_GenericField32_field_init.
*/
void
Hacl_GenericField32_sqr(
  Hacl_Bignum_MontArithmetic_bn_mont_ctx_u32 *k,
  uint32_t *aM,
  uint32_t *cM
)
{
  uint32_t len1 = Hacl_GenericField32_field_get_len(k);
  Hacl_Bignum_MontArithmetic_bn_mont_ctx_u32 k1 = *k;
  Hacl_Bignum_Montgomery_bn_mont_sqr_u32(len1, k1.n, k1.mu, aM, cM);
}

/*
Convert a bignum `one` to its Montgomery representation.

  The outparam oneM is meant to be `len` limbs in size, i.e. uint32_t[len].
  The argument k is a montgomery context obtained through Hacl_GenericField32_field_init.
*/
void Hacl_GenericField32_one(Hacl_Bignum_MontArithmetic_bn_mont_ctx_u32 *k, uint32_t *oneM)
{
  uint32_t len1 = Hacl_GenericField32_field_get_len(k);
  Hacl_Bignum_MontArithmetic_bn_mont_ctx_u32 k1 = *k;
  Hacl_Bignum_Montgomery_bn_from_mont_u32(len1, k1.n, k1.mu, k1.r2, oneM);
}

/*
Write `aM ^ b mod n` in `resM`.

  The argument aM and the outparam resM are meant to be `len` limbs in size, i.e. uint32_t[len].
  The argument k is a montgomery context obtained through Hacl_GenericField32_field_init.

  The argument b is a bignum of any size, and bBits is an upper bound on the
  number of significant bits of b. A tighter bound results in faster execution
  time. When in doubt, the number of bits for the bignum size is always a safe
  default, e.g. if b is a 256-bit bignum, bBits should be 256.

  This function is constant-time over its argument b, at the cost of a slower
  execution time than exp_vartime.

  Before calling this function, the caller will need to ensure that the following
  precondition is observed.
  • b < pow2 bBits 
*/
void
Hacl_GenericField32_exp_consttime(
  Hacl_Bignum_MontArithmetic_bn_mont_ctx_u32 *k,
  uint32_t *aM,
  uint32_t bBits,
  uint32_t *b,
  uint32_t *resM
)
{
  uint32_t len1 = Hacl_GenericField32_field_get_len(k);
  Hacl_Bignum_MontArithmetic_bn_mont_ctx_u32 k1 = *k;
  KRML_CHECK_SIZE(sizeof (uint32_t), k1.len);
  uint32_t *aMc = alloca(k1.len * sizeof (uint32_t));
  memset(aMc, 0U, k1.len * sizeof (uint32_t));
  memcpy(aMc, aM, k1.len * sizeof (uint32_t));
  if (bBits < (uint32_t)200U)
  {
    Hacl_Bignum_Montgomery_bn_from_mont_u32(len1, k1.n, k1.mu, k1.r2, resM);
    uint32_t sw = (uint32_t)0U;
    for (uint32_t i0 = (uint32_t)0U; i0 < bBits; i0++)
    {
      uint32_t i1 = (bBits - i0 - (uint32_t)1U) / (uint32_t)32U;
      uint32_t j = (bBits - i0 - (uint32_t)1U) % (uint32_t)32U;
      uint32_t tmp = b[i1];
      uint32_t bit = tmp >> j & (uint32_t)1U;
      uint32_t sw1 = bit ^ sw;
      for (uint32_t i = (uint32_t)0U; i < len1; i++)
      {
        uint32_t dummy = ((uint32_t)0U - sw1) & (resM[i] ^ aMc[i]);
        resM[i] = resM[i] ^ dummy;
        aMc[i] = aMc[i] ^ dummy;
      }
      Hacl_Bignum_Montgomery_bn_mont_mul_u32(len1, k1.n, k1.mu, aMc, resM, aMc);
      Hacl_Bignum_Montgomery_bn_mont_sqr_u32(len1, k1.n, k1.mu, resM, resM);
      sw = bit;
    }
    uint32_t sw0 = sw;
    for (uint32_t i = (uint32_t)0U; i < len1; i++)
    {
      uint32_t dummy = ((uint32_t)0U - sw0) & (resM[i] ^ aMc[i]);
      resM[i] = resM[i] ^ dummy;
      aMc[i] = aMc[i] ^ dummy;
    }
  }
  else
  {
    uint32_t bLen;
    if (bBits == (uint32_t)0U)
    {
      bLen = (uint32_t)1U;
    }
    else
    {
      bLen = (bBits - (uint32_t)1U) / (uint32_t)32U + (uint32_t)1U;
    }
    Hacl_Bignum_Montgomery_bn_from_mont_u32(len1, k1.n, k1.mu, k1.r2, resM);
    uint32_t table_len = (uint32_t)16U;
    KRML_CHECK_SIZE(sizeof (uint32_t), table_len * len1);
    uint32_t *table = alloca(table_len * len1 * sizeof (uint32_t));
    memset(table, 0U, table_len * len1 * sizeof (uint32_t));
    memcpy(table, resM, len1 * sizeof (uint32_t));
    uint32_t *t1 = table + len1;
    memcpy(t1, aMc, len1 * sizeof (uint32_t));
    for (uint32_t i = (uint32_t)0U; i < table_len - (uint32_t)2U; i++)
    {
      uint32_t *t11 = table + (i + (uint32_t)1U) * len1;
      uint32_t *t2 = table + (i + (uint32_t)2U) * len1;
      Hacl_Bignum_Montgomery_bn_mont_mul_u32(len1, k1.n, k1.mu, t11, aMc, t2);
    }
    for (uint32_t i0 = (uint32_t)0U; i0 < bBits / (uint32_t)4U; i0++)
    {
      for (uint32_t i = (uint32_t)0U; i < (uint32_t)4U; i++)
      {
        Hacl_Bignum_Montgomery_bn_mont_sqr_u32(len1, k1.n, k1.mu, resM, resM);
      }
      uint32_t mask_l = (uint32_t)16U - (uint32_t)1U;
      uint32_t i1 = (bBits - (uint32_t)4U * i0 - (uint32_t)4U) / (uint32_t)32U;
      uint32_t j = (bBits - (uint32_t)4U * i0 - (uint32_t)4U) % (uint32_t)32U;
      uint32_t p1 = b[i1] >> j;
      uint32_t ite;
      if (i1 + (uint32_t)1U < bLen && (uint32_t)0U < j)
      {
        ite = p1 | b[i1 + (uint32_t)1U] << ((uint32_t)32U - j);
      }
      else
      {
        ite = p1;
      }
      uint32_t bits_l = ite & mask_l;
      KRML_CHECK_SIZE(sizeof (uint32_t), len1);
      uint32_t *a_bits_l = alloca(len1 * sizeof (uint32_t));
      memset(a_bits_l, 0U, len1 * sizeof (uint32_t));
      memcpy(a_bits_l, table, len1 * sizeof (uint32_t));
      for (uint32_t i2 = (uint32_t)0U; i2 < table_len - (uint32_t)1U; i2++)
      {
        uint32_t c = FStar_UInt32_eq_mask(bits_l, i2 + (uint32_t)1U);
        uint32_t *res_j = table + (i2 + (uint32_t)1U) * len1;
        for (uint32_t i = (uint32_t)0U; i < len1; i++)
        {
          uint32_t *os = a_bits_l;
          uint32_t x = (c & res_j[i]) | (~c & a_bits_l[i]);
          os[i] = x;
        }
      }
      Hacl_Bignum_Montgomery_bn_mont_mul_u32(len1, k1.n, k1.mu, resM, a_bits_l, resM);
    }
    if (!(bBits % (uint32_t)4U == (uint32_t)0U))
    {
      uint32_t c = bBits % (uint32_t)4U;
      for (uint32_t i = (uint32_t)0U; i < c; i++)
      {
        Hacl_Bignum_Montgomery_bn_mont_sqr_u32(len1, k1.n, k1.mu, resM, resM);
      }
      uint32_t c10 = bBits % (uint32_t)4U;
      uint32_t mask_l = ((uint32_t)1U << c10) - (uint32_t)1U;
      uint32_t i0 = (uint32_t)0U;
      uint32_t j = (uint32_t)0U;
      uint32_t p1 = b[i0] >> j;
      uint32_t ite;
      if (i0 + (uint32_t)1U < bLen && (uint32_t)0U < j)
      {
        ite = p1 | b[i0 + (uint32_t)1U] << ((uint32_t)32U - j);
      }
      else
      {
        ite = p1;
      }
      uint32_t bits_c = ite & mask_l;
      uint32_t bits_c0 = bits_c;
      KRML_CHECK_SIZE(sizeof (uint32_t), len1);
      uint32_t *a_bits_c = alloca(len1 * sizeof (uint32_t));
      memset(a_bits_c, 0U, len1 * sizeof (uint32_t));
      memcpy(a_bits_c, table, len1 * sizeof (uint32_t));
      for (uint32_t i1 = (uint32_t)0U; i1 < table_len - (uint32_t)1U; i1++)
      {
        uint32_t c1 = FStar_UInt32_eq_mask(bits_c0, i1 + (uint32_t)1U);
        uint32_t *res_j = table + (i1 + (uint32_t)1U) * len1;
        for (uint32_t i = (uint32_t)0U; i < len1; i++)
        {
          uint32_t *os = a_bits_c;
          uint32_t x = (c1 & res_j[i]) | (~c1 & a_bits_c[i]);
          os[i] = x;
        }
      }
      Hacl_Bignum_Montgomery_bn_mont_mul_u32(len1, k1.n, k1.mu, resM, a_bits_c, resM);
    }
  }
}

/*
Write `aM ^ b mod n` in `resM`.

  The argument aM and the outparam resM are meant to be `len` limbs in size, i.e. uint32_t[len].
  The argument k is a montgomery context obtained through Hacl_GenericField32_field_init.

  The argument b is a bignum of any size, and bBits is an upper bound on the
  number of significant bits of b. A tighter bound results in faster execution
  time. When in doubt, the number of bits for the bignum size is always a safe
  default, e.g. if b is a 256-bit bignum, bBits should be 256.

  The function is *NOT* constant-time on the argument b. See the
  exp_consttime function for constant-time variant.

  Before calling this function, the caller will need to ensure that the following
  precondition is observed.
  • b < pow2 bBits 
*/
void
Hacl_GenericField32_exp_vartime(
  Hacl_Bignum_MontArithmetic_bn_mont_ctx_u32 *k,
  uint32_t *aM,
  uint32_t bBits,
  uint32_t *b,
  uint32_t *resM
)
{
  uint32_t len1 = Hacl_GenericField32_field_get_len(k);
  Hacl_Bignum_MontArithmetic_bn_mont_ctx_u32 k1 = *k;
  KRML_CHECK_SIZE(sizeof (uint32_t), k1.len);
  uint32_t *aMc = alloca(k1.len * sizeof (uint32_t));
  memset(aMc, 0U, k1.len * sizeof (uint32_t));
  memcpy(aMc, aM, k1.len * sizeof (uint32_t));
  if (bBits < (uint32_t)200U)
  {
    Hacl_Bignum_Montgomery_bn_from_mont_u32(len1, k1.n, k1.mu, k1.r2, resM);
    for (uint32_t i = (uint32_t)0U; i < bBits; i++)
    {
      uint32_t i1 = i / (uint32_t)32U;
      uint32_t j = i % (uint32_t)32U;
      uint32_t tmp = b[i1];
      uint32_t bit = tmp >> j & (uint32_t)1U;
      if (!(bit == (uint32_t)0U))
      {
        Hacl_Bignum_Montgomery_bn_mont_mul_u32(len1, k1.n, k1.mu, resM, aMc, resM);
      }
      Hacl_Bignum_Montgomery_bn_mont_sqr_u32(len1, k1.n, k1.mu, aMc, aMc);
    }
  }
  else
  {
    uint32_t bLen;
    if (bBits == (uint32_t)0U)
    {
      bLen = (uint32_t)1U;
    }
    else
    {
      bLen = (bBits - (uint32_t)1U) / (uint32_t)32U + (uint32_t)1U;
    }
    Hacl_Bignum_Montgomery_bn_from_mont_u32(len1, k1.n, k1.mu, k1.r2, resM);
    uint32_t table_len = (uint32_t)16U;
    KRML_CHECK_SIZE(sizeof (uint32_t), table_len * len1);
    uint32_t *table = alloca(table_len * len1 * sizeof (uint32_t));
    memset(table, 0U, table_len * len1 * sizeof (uint32_t));
    memcpy(table, resM, len1 * sizeof (uint32_t));
    uint32_t *t1 = table + len1;
    memcpy(t1, aMc, len1 * sizeof (uint32_t));
    for (uint32_t i = (uint32_t)0U; i < table_len - (uint32_t)2U; i++)
    {
      uint32_t *t11 = table + (i + (uint32_t)1U) * len1;
      uint32_t *t2 = table + (i + (uint32_t)2U) * len1;
      Hacl_Bignum_Montgomery_bn_mont_mul_u32(len1, k1.n, k1.mu, t11, aMc, t2);
    }
    for (uint32_t i = (uint32_t)0U; i < bBits / (uint32_t)4U; i++)
    {
      for (uint32_t i0 = (uint32_t)0U; i0 < (uint32_t)4U; i0++)
      {
        Hacl_Bignum_Montgomery_bn_mont_sqr_u32(len1, k1.n, k1.mu, resM, resM);
      }
      uint32_t mask_l = (uint32_t)16U - (uint32_t)1U;
      uint32_t i1 = (bBits - (uint32_t)4U * i - (uint32_t)4U) / (uint32_t)32U;
      uint32_t j = (bBits - (uint32_t)4U * i - (uint32_t)4U) % (uint32_t)32U;
      uint32_t p1 = b[i1] >> j;
      uint32_t ite;
      if (i1 + (uint32_t)1U < bLen && (uint32_t)0U < j)
      {
        ite = p1 | b[i1 + (uint32_t)1U] << ((uint32_t)32U - j);
      }
      else
      {
        ite = p1;
      }
      uint32_t bits_l = ite & mask_l;
      uint32_t bits_l32 = bits_l;
      uint32_t *a_bits_l = table + bits_l32 * len1;
      Hacl_Bignum_Montgomery_bn_mont_mul_u32(len1, k1.n, k1.mu, resM, a_bits_l, resM);
    }
    if (!(bBits % (uint32_t)4U == (uint32_t)0U))
    {
      uint32_t c = bBits % (uint32_t)4U;
      for (uint32_t i = (uint32_t)0U; i < c; i++)
      {
        Hacl_Bignum_Montgomery_bn_mont_sqr_u32(len1, k1.n, k1.mu, resM, resM);
      }
      uint32_t c1 = bBits % (uint32_t)4U;
      uint32_t mask_l = ((uint32_t)1U << c1) - (uint32_t)1U;
      uint32_t i = (uint32_t)0U;
      uint32_t j = (uint32_t)0U;
      uint32_t p1 = b[i] >> j;
      uint32_t ite;
      if (i + (uint32_t)1U < bLen && (uint32_t)0U < j)
      {
        ite = p1 | b[i + (uint32_t)1U] << ((uint32_t)32U - j);
      }
      else
      {
        ite = p1;
      }
      uint32_t bits_c = ite & mask_l;
      uint32_t bits_c0 = bits_c;
      uint32_t bits_c32 = bits_c0;
      uint32_t *a_bits_c = table + bits_c32 * len1;
      Hacl_Bignum_Montgomery_bn_mont_mul_u32(len1, k1.n, k1.mu, resM, a_bits_c, resM);
    }
  }
}

/*
Write `aM ^ (-1) mod n` in `aInvM`.

  The argument aM and the outparam aInvM are meant to be `len` limbs in size, i.e. uint32_t[len].
  The argument k is a montgomery context obtained through Hacl_GenericField32_field_init.

  Before calling this function, the caller will need to ensure that the following
  preconditions are observed.
  • n is a prime
  • 0 < aM 
*/
void
Hacl_GenericField32_inverse(
  Hacl_Bignum_MontArithmetic_bn_mont_ctx_u32 *k,
  uint32_t *aM,
  uint32_t *aInvM
)
{
  Hacl_Bignum_MontArithmetic_bn_mont_ctx_u32 k1 = *k;
  uint32_t len1 = k1.len;
  KRML_CHECK_SIZE(sizeof (uint32_t), len1);
  uint32_t *n2 = alloca(len1 * sizeof (uint32_t));
  memset(n2, 0U, len1 * sizeof (uint32_t));
  uint32_t c0 = Lib_IntTypes_Intrinsics_sub_borrow_u32((uint32_t)0U, k1.n[0U], (uint32_t)2U, n2);
  uint32_t c1;
  if ((uint32_t)1U < len1)
  {
    uint32_t rLen = len1 - (uint32_t)1U;
    uint32_t *a1 = k1.n + (uint32_t)1U;
    uint32_t *res1 = n2 + (uint32_t)1U;
    uint32_t c = c0;
    for (uint32_t i = (uint32_t)0U; i < rLen / (uint32_t)4U * (uint32_t)4U / (uint32_t)4U; i++)
    {
      uint32_t t1 = a1[(uint32_t)4U * i];
      uint32_t *res_i0 = res1 + (uint32_t)4U * i;
      c = Lib_IntTypes_Intrinsics_sub_borrow_u32(c, t1, (uint32_t)0U, res_i0);
      uint32_t t10 = a1[(uint32_t)4U * i + (uint32_t)1U];
      uint32_t *res_i1 = res1 + (uint32_t)4U * i + (uint32_t)1U;
      c = Lib_IntTypes_Intrinsics_sub_borrow_u32(c, t10, (uint32_t)0U, res_i1);
      uint32_t t11 = a1[(uint32_t)4U * i + (uint32_t)2U];
      uint32_t *res_i2 = res1 + (uint32_t)4U * i + (uint32_t)2U;
      c = Lib_IntTypes_Intrinsics_sub_borrow_u32(c, t11, (uint32_t)0U, res_i2);
      uint32_t t12 = a1[(uint32_t)4U * i + (uint32_t)3U];
      uint32_t *res_i = res1 + (uint32_t)4U * i + (uint32_t)3U;
      c = Lib_IntTypes_Intrinsics_sub_borrow_u32(c, t12, (uint32_t)0U, res_i);
    }
    for (uint32_t i = rLen / (uint32_t)4U * (uint32_t)4U; i < rLen; i++)
    {
      uint32_t t1 = a1[i];
      uint32_t *res_i = res1 + i;
      c = Lib_IntTypes_Intrinsics_sub_borrow_u32(c, t1, (uint32_t)0U, res_i);
    }
    uint32_t c10 = c;
    c1 = c10;
  }
  else
  {
    c1 = c0;
  }
  Hacl_GenericField32_exp_vartime(k, aM, k1.len * (uint32_t)32U, n2, aInvM);
}

