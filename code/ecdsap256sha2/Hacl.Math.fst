module Hacl.Math

open FStar.Math.Lemmas
open FStar.Math
open FStar.Mul

open Hacl.Spec.P256.Lemmas

#set-options "--fuel 0 --ifuel 0 --z3rlimit 100"

noextract
let prime256: (p: pos {p > 3}) =
  assert_norm (pow2 256 - pow2 224 + pow2 192 + pow2 96 -1 > 3);
  pow2 256 - pow2 224 + pow2 192 + pow2 96 -1
// 115792089210356248762697446949407573530086143415290314195533631308867097853951

val mod_sub: n:pos -> a:int -> b:int -> Lemma
  (requires a % n = b % n)
  (ensures  (a - b) % n = 0)
let mod_sub n a b =
  mod_add_both a b (-b) n

val sub_mod: n:pos -> a:int -> b:int -> Lemma
  (requires (a - b) % n = 0)
  (ensures  a % n = b % n)
let sub_mod n a b =
  mod_add_both (a - b) 0 b n

val mod_same: n:pos -> Lemma (n % n = 0)
let mod_same n = ()

val euclid: n:pos -> a:int -> b:int -> r:int -> s:int -> Lemma
  (requires (a * b) % n = 0 /\ r * n + s * a = 1)
  (ensures  b % n = 0)
let euclid n a b r s =
  assert (r * n * b + s * (a * b) = b);
  calc (==) {
    b % n;
    == { }
    (r * n * b + s * (a * b)) % n;
    == { FStar.Math.Lemmas.modulo_distributivity (r * n * b) (s * (a * b)) n }
    ((r * n * b) % n + s * (a * b) % n) % n;
    == { FStar.Math.Lemmas.lemma_mod_mul_distr_r s (a * b) n }
    ((r * n * b) % n + s * ((a * b) % n) % n) % n;
    == { }
    ((r * n * b) % n + s * 0 % n) % n;
    == { }
    ((r * n * b) % n + 0 % n) % n;
    == { FStar.Math.Lemmas.modulo_lemma 0 n }
    ((r * n * b) % n) % n;
    == { FStar.Math.Lemmas.lemma_mod_twice (r * n * b) n }
    (r * n * b) % n;
    == { FStar.Math.Lemmas.swap_mul r n; FStar.Math.Lemmas.paren_mul_right n r b }
    (n * (r * b)) % n;
    == { FStar.Math.Lemmas.lemma_mod_mul_distr_l n (r * b) n}
    n % n * (r * b) % n;
    == { mod_same n }
    (0 * r * b) % n;
    == { }
    0;
  }

val lemma_modular_multiplication_p256_2_left:
  a:nat{a < prime256} -> b:nat{b < prime256} -> Lemma
  (requires a * pow2 256 % prime256 = b * pow2 256 % prime256)
  (ensures  a == b)

let lemma_modular_multiplication_p256_2_left a b =
  mod_sub prime256 (a * pow2 256) (b * pow2 256);
  assert (pow2 256 * (a - b) % prime256 = 0);
  let r = 26959946654596436323893653559348051827142583427821597254581997273087 in
  let s = -26959946648319334592891477706824406424704094582978235142356758167551 in
  assert_norm (r * prime256 + s * pow2 256 = 1);
  euclid prime256 (pow2 256) (a - b) r s;
  assert ((a - b) % prime256 = 0);
  sub_mod prime256 a b;
  assert (a % prime256 = b % prime256);
  FStar.Math.Lemmas.modulo_lemma a prime256;
  FStar.Math.Lemmas.modulo_lemma b prime256

val lemma_modular_multiplication_p256_2: a: nat{a < prime256} -> b: nat{b < prime256} ->
  Lemma
  (a * pow2 256 % prime256 = b * pow2 256 % prime256 <==> a == b)

let lemma_modular_multiplication_p256_2 a b =
  Classical.move_requires_2 lemma_modular_multiplication_p256_2_left a b

noextract
let prime_p256_order:pos =
  assert_norm (115792089210356248762697446949407573529996955224135760342422259061068512044369> 0);
  115792089210356248762697446949407573529996955224135760342422259061068512044369

val lemma_montgomery_mod_inverse_addition: a:nat -> Lemma (
  a * modp_inv2_prime (pow2 64) prime256 * modp_inv2_prime (pow2 64) prime256 % prime256 ==
  a * modp_inv2_prime (pow2 128) prime256 % prime256)

let lemma_montgomery_mod_inverse_addition a =
  calc (==) {
    a * modp_inv2_prime (pow2 64) prime256 * modp_inv2_prime (pow2 64) prime256 % prime256;
    == { FStar.Math.Lemmas.paren_mul_right a (modp_inv2_prime (pow2 64) prime256) (modp_inv2_prime (pow2 64) prime256)}
    a * (modp_inv2_prime (pow2 64) prime256 * modp_inv2_prime (pow2 64) prime256) % prime256;
    == { FStar.Math.Lemmas.lemma_mod_mul_distr_r a
    (modp_inv2_prime (pow2 64) prime256 * modp_inv2_prime (pow2 64) prime256) prime256 }
    a * (modp_inv2_prime (pow2 64) prime256 * modp_inv2_prime (pow2 64) prime256 % prime256) % prime256;
    == { assert_norm (modp_inv2_prime (pow2 64) prime256 * modp_inv2_prime (pow2 64) prime256 % prime256 ==
    modp_inv2_prime (pow2 128) prime256 % prime256) }
    a * (modp_inv2_prime (pow2 128) prime256 % prime256) % prime256;
    == { FStar.Math.Lemmas.lemma_mod_mul_distr_r a (modp_inv2_prime (pow2 128) prime256) prime256 }
    a * modp_inv2_prime (pow2 128) prime256 % prime256;
  }

val lemma_montgomery_mod_inverse_addition2: a:nat -> Lemma (
  a * modp_inv2_prime (pow2 128) prime256 * modp_inv2_prime (pow2 128) prime256 % prime256 ==
  a * modp_inv2_prime (pow2 256) prime256 % prime256)

let lemma_montgomery_mod_inverse_addition2 a =
  calc (==) {
    a * modp_inv2_prime (pow2 128) prime256 * modp_inv2_prime (pow2 128) prime256 % prime256;
    == { FStar.Math.Lemmas.paren_mul_right a (modp_inv2_prime (pow2 128) prime256) (modp_inv2_prime (pow2 128) prime256)}
    a * (modp_inv2_prime (pow2 128) prime256 * modp_inv2_prime (pow2 128) prime256) % prime256;
    == { FStar.Math.Lemmas.lemma_mod_mul_distr_r a
    (modp_inv2_prime (pow2 128) prime256 * modp_inv2_prime (pow2 128) prime256) prime256 }
    a * (modp_inv2_prime (pow2 128) prime256 * modp_inv2_prime (pow2 128) prime256 % prime256) % prime256;
    == { assert_norm (modp_inv2_prime (pow2 128) prime256 * modp_inv2_prime (pow2 128) prime256 % prime256 ==
    modp_inv2_prime (pow2 256) prime256 % prime256) }
    a * (modp_inv2_prime (pow2 256) prime256 % prime256) % prime256;
    == { FStar.Math.Lemmas.lemma_mod_mul_distr_r a (modp_inv2_prime (pow2 256) prime256) prime256 }
    a * modp_inv2_prime (pow2 256) prime256 % prime256;
  }

(* Fermat's Little Theorem
   applied to r = modp_inv2_prime (pow2 256) prime_p256_order

  Verified in Sage:
   prime256 = Zmod(Integer(115792089210356248762697446949407573530086143415290314195533631308867097853951))
   p = 41058363725152142129326129780047268409114441015993725554835256314039467401291
   C = EllipticCurve(prime256, [-3, p])
   prime_p256_order = C.cardinality()
   Z = Integers(prime_p256_order)
   r = Z(inverse_mod(2**256, prime_p256_order))
   r ^ (prime_p256_order - 1)
*)
assume
val lemma_l_ferm: unit -> Lemma
  (let r = modp_inv2_prime (pow2 256) prime_p256_order in
  (pow r (prime_p256_order - 1) % prime_p256_order == 1))