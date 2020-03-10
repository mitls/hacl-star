open Unsigned

open SharedDefs
open SharedFunctors
open CBytes

module Lib_RandomBuffer_System = Lib_RandomBuffer_System_bindings.Bindings(Lib_RandomBuffer_System_stubs)
module Hacl_Chacha20Poly1305_32 = Hacl_Chacha20Poly1305_32_bindings.Bindings(Hacl_Chacha20Poly1305_32_stubs)
module Hacl_Chacha20Poly1305_128 = Hacl_Chacha20Poly1305_128_bindings.Bindings(Hacl_Chacha20Poly1305_128_stubs)
module Hacl_Chacha20Poly1305_256 = Hacl_Chacha20Poly1305_256_bindings.Bindings(Hacl_Chacha20Poly1305_256_stubs)
module Hacl_Curve25519_51 = Hacl_Curve25519_51_bindings.Bindings(Hacl_Curve25519_51_stubs)
module Hacl_Curve25519_64 = Hacl_Curve25519_64_bindings.Bindings(Hacl_Curve25519_64_stubs)
module Hacl_Curve25519_64_Slow = Hacl_Curve25519_64_Slow_bindings.Bindings(Hacl_Curve25519_64_Slow_stubs)
module Hacl_Ed25519 = Hacl_Ed25519_bindings.Bindings(Hacl_Ed25519_stubs)
module Hacl_Hash = Hacl_Hash_bindings.Bindings(Hacl_Hash_stubs)
module Hacl_SHA3 = Hacl_SHA3_bindings.Bindings(Hacl_SHA3_stubs)
module Hacl_HMAC = Hacl_HMAC_bindings.Bindings(Hacl_HMAC_stubs)
module Hacl_Poly1305_32 = Hacl_Poly1305_32_bindings.Bindings(Hacl_Poly1305_32_stubs)
module Hacl_Poly1305_128 = Hacl_Poly1305_128_bindings.Bindings(Hacl_Poly1305_128_stubs)
module Hacl_Poly1305_256 = Hacl_Poly1305_256_bindings.Bindings(Hacl_Poly1305_256_stubs)
module Hacl_HKDF = Hacl_HKDF_bindings.Bindings(Hacl_HKDF_stubs)
module Hacl_NaCl = Hacl_NaCl_bindings.Bindings(Hacl_NaCl_stubs)
module Hacl_Blake2b_32 = Hacl_Blake2b_32_bindings.Bindings(Hacl_Blake2b_32_stubs)
module Hacl_Blake2b_256 = Hacl_Blake2b_256_bindings.Bindings(Hacl_Blake2b_256_stubs)
module Hacl_ECDSA = Hacl_ECDSA_bindings.Bindings(Hacl_ECDSA_stubs)

module RandomBuffer = struct
  let randombytes buf = Lib_RandomBuffer_System.randombytes (ctypes_buf buf) (size_uint32 buf)
end

module Chacha20_Poly1305_32 : Chacha20_Poly1305 =
  Make_Chacha20_Poly1305 (struct
    let encrypt = Hacl_Chacha20Poly1305_32.hacl_Chacha20Poly1305_32_aead_encrypt
    let decrypt = Hacl_Chacha20Poly1305_32.hacl_Chacha20Poly1305_32_aead_decrypt
  end)

module Chacha20_Poly1305_128 : Chacha20_Poly1305 =
  Make_Chacha20_Poly1305 (struct
    let encrypt = Hacl_Chacha20Poly1305_128.hacl_Chacha20Poly1305_128_aead_encrypt
    let decrypt = Hacl_Chacha20Poly1305_128.hacl_Chacha20Poly1305_128_aead_decrypt
  end)

module Chacha20_Poly1305_256 : Chacha20_Poly1305 =
  Make_Chacha20_Poly1305 (struct
    let encrypt = Hacl_Chacha20Poly1305_256.hacl_Chacha20Poly1305_256_aead_encrypt
    let decrypt = Hacl_Chacha20Poly1305_256.hacl_Chacha20Poly1305_256_aead_decrypt
  end)

module Curve25519_51 : Curve25519 =
  Make_Curve25519 (struct
    let secret_to_public = Hacl_Curve25519_51.hacl_Curve25519_51_secret_to_public
    let scalarmult = Hacl_Curve25519_51.hacl_Curve25519_51_scalarmult
    let ecdh = Hacl_Curve25519_51.hacl_Curve25519_51_ecdh
  end)

module Curve25519_64 : Curve25519 =
  Make_Curve25519 (struct
    let secret_to_public = Hacl_Curve25519_64.hacl_Curve25519_64_secret_to_public
    let scalarmult = Hacl_Curve25519_64.hacl_Curve25519_64_scalarmult
    let ecdh = Hacl_Curve25519_64.hacl_Curve25519_64_ecdh
  end)

module Curve25519_64_Slow : Curve25519 =
  Make_Curve25519 (struct
    let secret_to_public = Hacl_Curve25519_64_Slow.hacl_Curve25519_64_Slow_secret_to_public
    let scalarmult = Hacl_Curve25519_64_Slow.hacl_Curve25519_64_Slow_scalarmult
    let ecdh = Hacl_Curve25519_64_Slow.hacl_Curve25519_64_Slow_ecdh
  end)

(* TODO: needs testing *)
module Curve25519_51_Internal = struct
  open Ctypes
  include Curve25519_51
  let uint64_ptr buf = from_voidp uint64_t (to_voidp (bigarray_start array1 buf))
  let fadd out f1 f2 = Hacl_Curve25519_51.hacl_Impl_Curve25519_Field51_fadd (uint64_ptr out) (uint64_ptr f1) (uint64_ptr f2)
  let fsub out f1 f2 = Hacl_Curve25519_51.hacl_Impl_Curve25519_Field51_fsub (uint64_ptr out) (uint64_ptr f1) (uint64_ptr f2)
  let fmul1 out f1 f2 = Hacl_Curve25519_51.hacl_Impl_Curve25519_Field51_fmul1 (uint64_ptr out) (uint64_ptr f1) f2
end

module Ed25519 : EdDSA =
  Make_EdDSA (struct
  let secret_to_public = Hacl_Ed25519.hacl_Ed25519_secret_to_public
  let sign = Hacl_Ed25519.hacl_Ed25519_sign
  let verify = Hacl_Ed25519.hacl_Ed25519_verify
  let expand_keys = Hacl_Ed25519.hacl_Ed25519_expand_keys
  let sign_expanded = Hacl_Ed25519.hacl_Ed25519_sign_expanded
  end)

module SHA2_224 : HashFunction =
  Make_HashFunction (struct
    let hash_alg = Some HashDefs.SHA2_224
    let hash = Hacl_Hash.hacl_Hash_SHA2_hash_224
end)

module SHA2_256 : HashFunction =
  Make_HashFunction (struct
    let hash_alg = Some HashDefs.SHA2_256
    let hash = Hacl_Hash.hacl_Hash_SHA2_hash_256
end)

module SHA2_384 : HashFunction =
  Make_HashFunction (struct
    let hash_alg = Some HashDefs.SHA2_384
    let hash = Hacl_Hash.hacl_Hash_SHA2_hash_384
end)

module SHA2_512 : HashFunction =
  Make_HashFunction (struct
    let hash_alg = Some HashDefs.SHA2_512
    let hash = Hacl_Hash.hacl_Hash_SHA2_hash_512
end)

module SHA3_224 : HashFunction =
  Make_HashFunction (struct
    let hash_alg = None
    let hash input input_len output = Hacl_SHA3.hacl_SHA3_sha3_224 input_len input output
end)

module SHA3_256 : HashFunction =
  Make_HashFunction (struct
    let hash_alg = None
    let hash input input_len output = Hacl_SHA3.hacl_SHA3_sha3_256 input_len input output
end)

module SHA3_384 : HashFunction =
  Make_HashFunction (struct
    let hash_alg = None
    let hash input input_len output = Hacl_SHA3.hacl_SHA3_sha3_384 input_len input output
end)

module SHA3_512 : HashFunction =
  Make_HashFunction (struct
    let hash_alg = None
    let hash input input_len output = Hacl_SHA3.hacl_SHA3_sha3_512 input_len input output
end)

module Keccak = struct
  let keccak rate capacity suffix input output =
    Hacl_SHA3.hacl_Impl_SHA3_keccak (UInt32.of_int rate) (UInt32.of_int capacity) (size_uint32 input) (ctypes_buf input) (UInt8.of_int suffix) (size_uint32 output) (ctypes_buf output)
  let shake128 input output =
    Hacl_SHA3.hacl_SHA3_shake128_hacl (size_uint32 input) (ctypes_buf input) (size_uint32 output) (ctypes_buf output)
  let shake256 input output =
    Hacl_SHA3.hacl_SHA3_shake256_hacl (size_uint32 input) (ctypes_buf input) (size_uint32 output) (ctypes_buf output)
end

module SHA1 : HashFunction =
  Make_HashFunction (struct
    let hash_alg = Some HashDefs.(Legacy SHA1)
    let hash = Hacl_Hash.hacl_Hash_SHA1_legacy_hash
end) [@@deprecated]

module MD5 : HashFunction =
  Make_HashFunction (struct
    let hash_alg = Some HashDefs.(Legacy MD5)
    let hash = Hacl_Hash.hacl_Hash_MD5_legacy_hash
end) [@@deprecated]

module HMAC_SHA2_256 : MAC =
  Make_HMAC (struct
    let mac = Hacl_HMAC.hacl_HMAC_compute_sha2_256
end)

module HMAC_SHA2_384 : MAC =
  Make_HMAC (struct
    let mac = Hacl_HMAC.hacl_HMAC_compute_sha2_384
end)

module HMAC_SHA2_512 : MAC =
  Make_HMAC (struct
    let mac = Hacl_HMAC.hacl_HMAC_compute_sha2_512
end)

module Poly1305_32 : MAC =
  Make_Poly1305 (struct
    let mac = Hacl_Poly1305_32.hacl_Poly1305_32_poly1305_mac
end)

module Poly1305_128 : MAC =
  Make_Poly1305 (struct
    let mac = Hacl_Poly1305_128.hacl_Poly1305_128_poly1305_mac
end)

module Poly1305_256 : MAC =
  Make_Poly1305 (struct
    let mac = Hacl_Poly1305_256.hacl_Poly1305_256_poly1305_mac
end)

module HKDF_SHA2_256 : HKDF =
  Make_HKDF (struct
    let expand = Hacl_HKDF.hacl_HKDF_expand_sha2_256
    let extract = Hacl_HKDF.hacl_HKDF_extract_sha2_256
  end)

module HKDF_SHA2_512 : HKDF =
  Make_HKDF (struct
    let expand = Hacl_HKDF.hacl_HKDF_expand_sha2_512
    let extract = Hacl_HKDF.hacl_HKDF_extract_sha2_512
  end)

module NaCl = struct
  open Hacl_NaCl

  let get_result r =
    if r = UInt32.zero then
      true
    else
    if r = UInt32.max_int then
      false
    else
      failwith "Unknown return value"
  let box_beforenm k pk sk = get_result @@ hacl_NaCl_crypto_box_beforenm (ctypes_buf k) (ctypes_buf pk) (ctypes_buf sk)
  module Easy = struct
    let box ct pt n pk sk = get_result @@ hacl_NaCl_crypto_box_easy (ctypes_buf ct) (ctypes_buf pt) (size_uint32 pt) (ctypes_buf n) (ctypes_buf pk) (ctypes_buf sk)
    let box_open pt ct n pk sk = get_result @@ hacl_NaCl_crypto_box_open_easy (ctypes_buf pt) (ctypes_buf ct) (size_uint32 ct) (ctypes_buf n) (ctypes_buf pk) (ctypes_buf sk)
    let box_afternm ct pt n k = get_result @@ hacl_NaCl_crypto_box_easy_afternm (ctypes_buf ct) (ctypes_buf pt) (size_uint32 pt) (ctypes_buf n) (ctypes_buf k)
    let box_open_afternm pt ct n k = get_result @@ hacl_NaCl_crypto_box_open_easy_afternm (ctypes_buf pt) (ctypes_buf ct) (size_uint32 ct) (ctypes_buf n) (ctypes_buf k)
    let secretbox ct pt n k = get_result @@ hacl_NaCl_crypto_secretbox_easy (ctypes_buf ct) (ctypes_buf pt) (size_uint32 pt) (ctypes_buf n) (ctypes_buf k)
    let secretbox_open pt ct n k = get_result @@ hacl_NaCl_crypto_secretbox_open_easy (ctypes_buf pt) (ctypes_buf ct) (size_uint32 ct) (ctypes_buf n) (ctypes_buf k)
  end
  module Detached = struct
    let box ct tag pt n pk sk = get_result @@ hacl_NaCl_crypto_box_detached (ctypes_buf ct) (ctypes_buf tag) (ctypes_buf pt) (size_uint32 pt) (ctypes_buf n) (ctypes_buf pk) (ctypes_buf sk)
    let box_open pt ct tag n pk sk = get_result @@ hacl_NaCl_crypto_box_open_detached (ctypes_buf pt) (ctypes_buf ct) (ctypes_buf tag) (size_uint32 ct) (ctypes_buf n) (ctypes_buf pk) (ctypes_buf sk)
    let box_afternm ct tag pt n k = get_result @@ hacl_NaCl_crypto_box_detached_afternm (ctypes_buf ct) (ctypes_buf tag) (ctypes_buf pt) (size_uint32 pt) (ctypes_buf n) (ctypes_buf k)
    let box_open_afternm pt ct tag n k = get_result @@ hacl_NaCl_crypto_box_open_detached_afternm (ctypes_buf pt) (ctypes_buf ct) (ctypes_buf tag) (size_uint32 ct) (ctypes_buf n) (ctypes_buf k)
    let secretbox ct tag pt n k = get_result @@ hacl_NaCl_crypto_secretbox_detached (ctypes_buf ct) (ctypes_buf tag) (ctypes_buf pt) (size_uint32 pt) (ctypes_buf n) (ctypes_buf k)
    let secretbox_open pt ct tag n k = get_result @@ hacl_NaCl_crypto_secretbox_open_detached (ctypes_buf pt) (ctypes_buf ct) (ctypes_buf tag) (size_uint32 ct) (ctypes_buf n) (ctypes_buf k)
  end
end

module Blake2b_32 : Blake2b =
  Make_Blake2b (struct
    let blake2b = Hacl_Blake2b_32.hacl_Blake2b_32_blake2b
  end)

module Blake2b_256 : Blake2b =
  Make_Blake2b (struct
    let blake2b = Hacl_Blake2b_256.hacl_Blake2b_256_blake2b
  end)

module ECDSA = struct
  let get_result r =
    if r = UInt64.zero then
      true
    else
    if r = UInt64.max_int then
      false
    else
      failwith "Unknown return value"
  let sign signature priv msg k =
    assert (Bytes.length signature = 64);
    get_result @@ Hacl_ECDSA.hacl_Impl_ECDSA_ecdsa_p256_sha2_sign (ctypes_buf signature) (size_uint32 msg) (ctypes_buf msg) (ctypes_buf priv) (ctypes_buf k)
  let verify pub msg signature =
    assert (Bytes.length signature = 64);
    let r, s = Bytes.sub signature 0 32, Bytes.sub signature 32 32 in
    Hacl_ECDSA.hacl_Impl_ECDSA_ecdsa_p256_sha2_verify (size_uint32 msg) (ctypes_buf msg) (ctypes_buf pub) (ctypes_buf r) (ctypes_buf s)
end