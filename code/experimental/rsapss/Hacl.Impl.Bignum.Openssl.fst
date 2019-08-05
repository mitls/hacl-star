module Hacl.Impl.Bignum.Openssl

open FStar.HyperStack
open FStar.HyperStack.ST
open FStar.Mul

open LowStar.Buffer

open Hacl.Impl.Bignum.Core
open Hacl.Spec.Bignum

open Lib.IntTypes
open Lib.Math.Algebra
open Lib.Buffer

val ossl_mod_exp:
     #nLen:bn_len_strict{v nLen * 128 < max_size_t}
  -> #expLen:bn_len_strict
  -> n:lbignum nLen
  -> a:lbignum nLen
  -> b:lbignum expLen
  -> res:lbignum nLen
  -> Stack unit
    (requires fun h ->
      live h n /\ live h a /\ live h b /\ live h res /\
      disjoint a res /\ disjoint b res /\ disjoint n res /\
      as_snat h n > 1)
    (ensures  fun h0 _ h1 -> modifies1 res h0 h1 /\
      live h1 n /\ live h1 a /\ live h1 b /\ live h1 res /\
      (let n = as_snat h0 n in
       as_snat h1 res = mexp (to_fe #n (as_snat h0 a)) (as_snat h0 b)))
