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


#include "MerkleTree.h"

/* SNIPPET_START: hash_r_alloc */

static uint8_t *hash_r_alloc(uint32_t s)
{
  KRML_CHECK_SIZE(sizeof (uint8_t), s);
  uint8_t *buf = KRML_HOST_CALLOC(s, sizeof (uint8_t));
  return buf;
}

/* SNIPPET_END: hash_r_alloc */

/* SNIPPET_START: hash_r_free */

static void hash_r_free(uint32_t uu___1, uint8_t *v)
{
  KRML_HOST_FREE(v);
}

/* SNIPPET_END: hash_r_free */

/* SNIPPET_START: hash_copy */

static void hash_copy(uint32_t s, uint8_t *src, uint8_t *dst)
{
  memcpy(dst, src, s * sizeof (uint8_t));
}

/* SNIPPET_END: hash_copy */

/* SNIPPET_START: alloc_reserve___uint8_t_ */

static LowStar_Vector_vector_str___uint8_t_ alloc_reserve___uint8_t_(uint32_t len, uint8_t *ia)
{
  KRML_CHECK_SIZE(sizeof (uint8_t *), len);
  uint8_t **buf = KRML_HOST_MALLOC(sizeof (uint8_t *) * len);
  for (uint32_t _i = 0U; _i < len; ++_i)
    buf[_i] = ia;
  return ((LowStar_Vector_vector_str___uint8_t_){ .sz = (uint32_t)0U, .cap = len, .vs = buf });
}

/* SNIPPET_END: alloc_reserve___uint8_t_ */

/* SNIPPET_START: hash_vec_r_alloc */

static LowStar_Vector_vector_str___uint8_t_ hash_vec_r_alloc(uint32_t hsz)
{
  return alloc_reserve___uint8_t_((uint32_t)1U, NULL);
}

/* SNIPPET_END: hash_vec_r_alloc */

/* SNIPPET_START: index___uint8_t_ */

static uint8_t *index___uint8_t_(LowStar_Vector_vector_str___uint8_t_ vec, uint32_t i)
{
  return vec.vs[i];
}

/* SNIPPET_END: index___uint8_t_ */

/* SNIPPET_START: regional__uint32_t__uint8_t_ */

typedef struct regional__uint32_t__uint8_t__s
{
  uint32_t state;
  uint8_t *dummy;
  uint8_t *(*r_alloc)(uint32_t x0);
  void (*r_free)(uint32_t x0, uint8_t *x1);
}
regional__uint32_t__uint8_t_;

/* SNIPPET_END: regional__uint32_t__uint8_t_ */

/* SNIPPET_START: free_elems___uint8_t__uint32_t */

static void
free_elems___uint8_t__uint32_t(
  regional__uint32_t__uint8_t_ rg,
  LowStar_Vector_vector_str___uint8_t_ rv,
  uint32_t idx
)
{
  uint8_t *uu____0 = index___uint8_t_(rv, idx);
  rg.r_free(rg.state, uu____0);
  if (idx != (uint32_t)0U)
  {
    free_elems___uint8_t__uint32_t(rg, rv, idx - (uint32_t)1U);
    return;
  }
}

/* SNIPPET_END: free_elems___uint8_t__uint32_t */

/* SNIPPET_START: free___uint8_t_ */

static void free___uint8_t_(LowStar_Vector_vector_str___uint8_t_ vec)
{
  KRML_HOST_FREE(vec.vs);
}

/* SNIPPET_END: free___uint8_t_ */

/* SNIPPET_START: free___uint8_t__uint32_t */

static void
free___uint8_t__uint32_t(
  regional__uint32_t__uint8_t_ rg,
  LowStar_Vector_vector_str___uint8_t_ rv
)
{
  if (!(rv.sz == (uint32_t)0U))
  {
    free_elems___uint8_t__uint32_t(rg, rv, rv.sz - (uint32_t)1U);
  }
  free___uint8_t_(rv);
}

/* SNIPPET_END: free___uint8_t__uint32_t */

/* SNIPPET_START: hash_vec_r_free */

static void hash_vec_r_free(uint32_t hsz, LowStar_Vector_vector_str___uint8_t_ v)
{
  free___uint8_t__uint32_t((
      (regional__uint32_t__uint8_t_){
        .state = hsz,
        .dummy = NULL,
        .r_alloc = hash_r_alloc,
        .r_free = hash_r_free
      }
    ),
    v);
}

/* SNIPPET_END: hash_vec_r_free */

/* SNIPPET_START: mt_init_hash */

/*
  Constructor for hashes
*/
inline uint8_t *mt_init_hash(uint32_t hash_size)
{
  return MerkleTree_Low_Hashfunctions_init_hash(hash_size);
}

/* SNIPPET_END: mt_init_hash */

/* SNIPPET_START: mt_free_hash */

/*
  Destructor for hashes
*/
inline void mt_free_hash(uint8_t *h)
{
  MerkleTree_Low_Hashfunctions_free_hash(h);
}

/* SNIPPET_END: mt_free_hash */

/* SNIPPET_START: mt_init_path */

/*
  Constructor for paths
*/
inline MerkleTree_Low_path *mt_init_path(uint32_t hash_size)
{
  return MerkleTree_Low_init_path(hash_size);
}

/* SNIPPET_END: mt_init_path */

/* SNIPPET_START: mt_free_path */

/*
  Destructor for paths
*/
inline void mt_free_path(MerkleTree_Low_path *path1)
{
  MerkleTree_Low_free_path(path1);
}

/* SNIPPET_END: mt_free_path */

/* SNIPPET_START: mt_get_path_length */

/*
  Length of a path

  @param[in] p Path

  return The length of the path
*/
inline uint32_t mt_get_path_length(const MerkleTree_Low_path *path1)
{
  return MerkleTree_Low_mt_get_path_length(path1);
}

/* SNIPPET_END: mt_get_path_length */

/* SNIPPET_START: insert___uint8_t_ */

static LowStar_Vector_vector_str___uint8_t_
insert___uint8_t_(LowStar_Vector_vector_str___uint8_t_ vec, uint8_t *v)
{
  uint32_t sz = vec.sz;
  uint32_t cap = vec.cap;
  uint8_t **vs = vec.vs;
  if (sz == cap)
  {
    uint32_t ncap = LowStar_Vector_new_capacity(cap);
    KRML_CHECK_SIZE(sizeof (uint8_t *), ncap);
    uint8_t **nvs = KRML_HOST_MALLOC(sizeof (uint8_t *) * ncap);
    for (uint32_t _i = 0U; _i < ncap; ++_i)
      nvs[_i] = v;
    memcpy(nvs, vs, sz * sizeof (uint8_t *));
    nvs[sz] = v;
    KRML_HOST_FREE(vs);
    return
      ((LowStar_Vector_vector_str___uint8_t_){ .sz = sz + (uint32_t)1U, .cap = ncap, .vs = nvs });
  }
  vs[sz] = v;
  return
    ((LowStar_Vector_vector_str___uint8_t_){ .sz = sz + (uint32_t)1U, .cap = cap, .vs = vs });
}

/* SNIPPET_END: insert___uint8_t_ */

/* SNIPPET_START: mt_path_insert */

/*
  Insert hash into path

  @param[in] p Path
  @param[in] hash Hash to insert
*/
inline void mt_path_insert(MerkleTree_Low_path *path1, uint8_t *hash1)
{
  MerkleTree_Low_path x0 = path1[0U];
  uint32_t hash_size = x0.hash_size;
  MerkleTree_Low_path pth = *path1;
  LowStar_Vector_vector_str___uint8_t_ pv = pth.hashes;
  LowStar_Vector_vector_str___uint8_t_ ipv = insert___uint8_t_(pv, hash1);
  *path1 = ((MerkleTree_Low_path){ .hash_size = hash_size, .hashes = ipv });
}

/* SNIPPET_END: mt_path_insert */

/* SNIPPET_START: mt_get_path_step */

/*
  Get step on a path

  @param[in] p Path
  @param[in] i Path step index

  return The hash at step i of p
*/
inline uint8_t *mt_get_path_step(const MerkleTree_Low_path *path1, uint32_t i)
{
  return MerkleTree_Low_mt_get_path_step(path1, i);
}

/* SNIPPET_END: mt_get_path_step */

/* SNIPPET_START: mt_get_path_step_pre */

/*
  Precondition predicate for mt_get_path_step
*/
inline bool mt_get_path_step_pre(const MerkleTree_Low_path *path1, uint32_t i)
{
  return MerkleTree_Low_mt_get_path_step_pre(path1, i);
}

/* SNIPPET_END: mt_get_path_step_pre */

/* SNIPPET_START: mt_create_custom */

/*
  Construction with custom hash functions

  @param[in]  hash_size Hash size (in bytes)
  @param[in]  i         The initial hash

  return The new Merkle tree
*/
inline MerkleTree_Low_merkle_tree
*mt_create_custom(
  uint32_t hash_size,
  uint8_t *i,
  void (*hash_fun)(uint8_t *x0, uint8_t *x1, uint8_t *x2)
)
{
  return MerkleTree_Low_mt_create_custom(hash_size, i, hash_fun);
}

/* SNIPPET_END: mt_create_custom */

/* SNIPPET_START: mt_free */

/*
  Destruction

  @param[in]  mt  The Merkle tree
*/
inline void mt_free(MerkleTree_Low_merkle_tree *mt)
{
  MerkleTree_Low_mt_free(mt);
}

/* SNIPPET_END: mt_free */

/* SNIPPET_START: mt_insert */

/*
  Insertion

  @param[in]  mt  The Merkle tree
  @param[in]  v   The tree does not take ownership of the hash, it makes a copy of its content.

 Note: The content of the hash will be overwritten with an arbitrary value.
*/
inline void mt_insert(MerkleTree_Low_merkle_tree *mt, uint8_t *v)
{
  MerkleTree_Low_mt_insert(mt, v);
}

/* SNIPPET_END: mt_insert */

/* SNIPPET_START: mt_insert_pre */

/*
  Precondition predicate for mt_insert
*/
inline bool mt_insert_pre(const MerkleTree_Low_merkle_tree *mt, uint8_t *v)
{
  return MerkleTree_Low_mt_insert_pre(mt, v);
}

/* SNIPPET_END: mt_insert_pre */

/* SNIPPET_START: mt_get_root */

/*
  Getting the Merkle root

  @param[in]  mt   The Merkle tree
  @param[out] root The Merkle root
*/
inline void mt_get_root(const MerkleTree_Low_merkle_tree *mt, uint8_t *root)
{
  MerkleTree_Low_mt_get_root(mt, root);
}

/* SNIPPET_END: mt_get_root */

/* SNIPPET_START: mt_get_root_pre */

/*
  Precondition predicate for mt_get_root
*/
inline bool mt_get_root_pre(const MerkleTree_Low_merkle_tree *mt, uint8_t *root)
{
  return MerkleTree_Low_mt_get_root_pre(mt, root);
}

/* SNIPPET_END: mt_get_root_pre */

/* SNIPPET_START: mt_get_path */

/*
  Getting a Merkle path

  @param[in]  mt   The Merkle tree
  @param[in]  idx  The index of the target hash
  @param[out] path A resulting Merkle path that contains the leaf hash.
  @param[out] root The Merkle root

  return The number of elements in the tree

  Notes:
  - The resulting path contains pointers to hashes in the tree, not copies of
    the hash values.
  - idx must be within the currently held indices in the tree (past the
    last flush index).
*/
inline uint32_t
mt_get_path(
  const MerkleTree_Low_merkle_tree *mt,
  uint64_t idx,
  MerkleTree_Low_path *path1,
  uint8_t *root
)
{
  return MerkleTree_Low_mt_get_path(mt, idx, path1, root);
}

/* SNIPPET_END: mt_get_path */

/* SNIPPET_START: mt_get_path_pre */

/*
  Precondition predicate for mt_get_path
*/
inline bool
mt_get_path_pre(
  const MerkleTree_Low_merkle_tree *mt,
  uint64_t idx,
  const MerkleTree_Low_path *path1,
  uint8_t *root
)
{
  return MerkleTree_Low_mt_get_path_pre(mt, idx, path1, root);
}

/* SNIPPET_END: mt_get_path_pre */

/* SNIPPET_START: mt_flush */

/*
  Flush the Merkle tree

  @param[in]  mt   The Merkle tree
*/
inline void mt_flush(MerkleTree_Low_merkle_tree *mt)
{
  MerkleTree_Low_mt_flush(mt);
}

/* SNIPPET_END: mt_flush */

/* SNIPPET_START: mt_flush_pre */

/*
  Precondition predicate for mt_flush
*/
inline bool mt_flush_pre(const MerkleTree_Low_merkle_tree *mt)
{
  return MerkleTree_Low_mt_flush_pre(mt);
}

/* SNIPPET_END: mt_flush_pre */

/* SNIPPET_START: mt_flush_to */

/*
  Flush the Merkle tree up to a given index

  @param[in]  mt   The Merkle tree
  @param[in]  idx  The index up to which to flush the tree
*/
inline void mt_flush_to(MerkleTree_Low_merkle_tree *mt, uint64_t idx)
{
  MerkleTree_Low_mt_flush_to(mt, idx);
}

/* SNIPPET_END: mt_flush_to */

/* SNIPPET_START: mt_flush_to_pre */

/*
  Precondition predicate for mt_flush_to
*/
bool mt_flush_to_pre(const MerkleTree_Low_merkle_tree *mt, uint64_t idx)
{
  return MerkleTree_Low_mt_flush_to_pre(mt, idx);
}

/* SNIPPET_END: mt_flush_to_pre */

/* SNIPPET_START: mt_retract_to */

/*
  Retract the Merkle tree down to a given index

  @param[in]  mt   The Merkle tree
  @param[in]  idx  The index to retract the tree to

 Note: The element and idx will remain in the tree.
*/
inline void mt_retract_to(MerkleTree_Low_merkle_tree *mt, uint64_t idx)
{
  MerkleTree_Low_mt_retract_to(mt, idx);
}

/* SNIPPET_END: mt_retract_to */

/* SNIPPET_START: mt_retract_to_pre */

/*
  Precondition predicate for mt_retract_to
*/
inline bool mt_retract_to_pre(const MerkleTree_Low_merkle_tree *mt, uint64_t idx)
{
  return MerkleTree_Low_mt_retract_to_pre(mt, idx);
}

/* SNIPPET_END: mt_retract_to_pre */

/* SNIPPET_START: mt_verify */

/*
  Client-side verification

  @param[in]  mt   The Merkle tree
  @param[in]  tgt  The index of the target hash
  @param[in]  max  The maximum index + 1 of the tree when the path was generated
  @param[in]  path The Merkle path to verify
  @param[in]  root

  return true if the verification succeeded, false otherwise

  Note: max - tgt must be less than 2^32.
*/
inline bool
mt_verify(
  const MerkleTree_Low_merkle_tree *mt,
  uint64_t tgt,
  uint64_t max,
  const MerkleTree_Low_path *path1,
  uint8_t *root
)
{
  return MerkleTree_Low_mt_verify(mt, tgt, max, path1, root);
}

/* SNIPPET_END: mt_verify */

/* SNIPPET_START: mt_verify_pre */

/*
  Precondition predicate for mt_verify
*/
inline bool
mt_verify_pre(
  const MerkleTree_Low_merkle_tree *mt,
  uint64_t tgt,
  uint64_t max,
  const MerkleTree_Low_path *path1,
  uint8_t *root
)
{
  return MerkleTree_Low_mt_verify_pre(mt, tgt, max, path1, root);
}

/* SNIPPET_END: mt_verify_pre */

/* SNIPPET_START: mt_serialize_size */

/*
  Serialization size

  @param[in]  mt   The Merkle tree

  return the number of bytes required to serialize the tree
*/
inline uint64_t mt_serialize_size(const MerkleTree_Low_merkle_tree *mt)
{
  return MerkleTree_Low_Serialization_mt_serialize_size(mt);
}

/* SNIPPET_END: mt_serialize_size */

/* SNIPPET_START: mt_serialize */

/*
  Merkle tree serialization

  @param[in]  mt   The Merkle tree
  @param[out] buf  The buffer to serialize the tree into
  @param[in]  len  Length of buf

  return the number of bytes written

  Note: buf must be a buffer of size mt_serialize_size(mt) or larger, but
  smaller than 2^32 (larger buffers are currently not supported).
*/
inline uint64_t mt_serialize(const MerkleTree_Low_merkle_tree *mt, uint8_t *buf, uint64_t len)
{
  return MerkleTree_Low_Serialization_mt_serialize(mt, buf, len);
}

/* SNIPPET_END: mt_serialize */

/* SNIPPET_START: mt_deserialize */

/*
  Merkle tree deserialization

  @param[in]  expected_hash_size Expected hash size to match hash_fun
  @param[in]  buf  The buffer to deserialize the tree from
  @param[in]  len  Length of buf
  @param[in]  hash_fun Hash function

  return pointer to the new tree if successful, NULL otherwise

  Note: buf must point to an allocated buffer.
*/
inline MerkleTree_Low_merkle_tree
*mt_deserialize(
  const uint8_t *buf,
  uint64_t len,
  void (*hash_fun)(uint8_t *x0, uint8_t *x1, uint8_t *x2)
)
{
  return MerkleTree_Low_Serialization_mt_deserialize(buf, len, hash_fun);
}

/* SNIPPET_END: mt_deserialize */

/* SNIPPET_START: mt_serialize_path */

/*
  Path serialization

  @param[in]  path The path
  @param[out] buf  The buffer to serialize the path into
  @param[in]  len  Length of buf

  return the number of bytes written
*/
inline uint64_t mt_serialize_path(const MerkleTree_Low_path *path1, uint8_t *buf, uint64_t len)
{
  return MerkleTree_Low_Serialization_mt_serialize_path(path1, buf, len);
}

/* SNIPPET_END: mt_serialize_path */

/* SNIPPET_START: mt_deserialize_path */

/*
  Path deserialization

  @param[in]  buf  The buffer to deserialize the path from
  @param[in]  len  Length of buf

  return pointer to the new path if successful, NULL otherwise

 Note: buf must point to an allocated buffer.
*/
inline MerkleTree_Low_path *mt_deserialize_path(const uint8_t *buf, uint64_t len)
{
  return MerkleTree_Low_Serialization_mt_deserialize_path(buf, len);
}

/* SNIPPET_END: mt_deserialize_path */

/* SNIPPET_START: mt_sha256_compress */

/*
  Default hash function
*/
void mt_sha256_compress(uint8_t *src1, uint8_t *src2, uint8_t *dst)
{
  uint32_t hash_size = (uint32_t)32U;
  Spec_Hash_Definitions_hash_alg hash_alg = Spec_Hash_Definitions_SHA2_256;
  uint8_t cb[64U] = { 0U };
  memcpy(cb, src1, hash_size * sizeof (uint8_t));
  memcpy(cb + (uint32_t)32U, src2, hash_size * sizeof (uint8_t));
  uint32_t buf0[4U];
  uint32_t buf1[5U];
  uint32_t buf2[8U];
  uint32_t buf3[8U];
  uint64_t buf4[8U];
  uint64_t buf5[8U];
  uint32_t buf6[16U];
  uint64_t buf[16U];
  EverCrypt_Hash_state_s s;
  switch (hash_alg)
  {
    case Spec_Hash_Definitions_MD5:
      {
        uint32_t init = (uint32_t)0U;
        for (uint32_t i = (uint32_t)0U; i < (uint32_t)4U; i++)
        {
          buf0[i] = init;
        }
        s = ((EverCrypt_Hash_state_s){ .tag = EverCrypt_Hash_MD5_s, { .case_MD5_s = buf0 } });
        break;
      }
    case Spec_Hash_Definitions_SHA1:
      {
        uint32_t init = (uint32_t)0U;
        for (uint32_t i = (uint32_t)0U; i < (uint32_t)5U; i++)
        {
          buf1[i] = init;
        }
        s = ((EverCrypt_Hash_state_s){ .tag = EverCrypt_Hash_SHA1_s, { .case_SHA1_s = buf1 } });
        break;
      }
    case Spec_Hash_Definitions_SHA2_224:
      {
        uint32_t init = (uint32_t)0U;
        for (uint32_t i = (uint32_t)0U; i < (uint32_t)8U; i++)
        {
          buf2[i] = init;
        }
        s =
          (
            (EverCrypt_Hash_state_s){
              .tag = EverCrypt_Hash_SHA2_224_s,
              { .case_SHA2_224_s = buf2 }
            }
          );
        break;
      }
    case Spec_Hash_Definitions_SHA2_256:
      {
        uint32_t init = (uint32_t)0U;
        for (uint32_t i = (uint32_t)0U; i < (uint32_t)8U; i++)
        {
          buf3[i] = init;
        }
        s =
          (
            (EverCrypt_Hash_state_s){
              .tag = EverCrypt_Hash_SHA2_256_s,
              { .case_SHA2_256_s = buf3 }
            }
          );
        break;
      }
    case Spec_Hash_Definitions_SHA2_384:
      {
        uint64_t init = (uint64_t)0U;
        for (uint32_t i = (uint32_t)0U; i < (uint32_t)8U; i++)
        {
          buf4[i] = init;
        }
        s =
          (
            (EverCrypt_Hash_state_s){
              .tag = EverCrypt_Hash_SHA2_384_s,
              { .case_SHA2_384_s = buf4 }
            }
          );
        break;
      }
    case Spec_Hash_Definitions_SHA2_512:
      {
        uint64_t init = (uint64_t)0U;
        for (uint32_t i = (uint32_t)0U; i < (uint32_t)8U; i++)
        {
          buf5[i] = init;
        }
        s =
          (
            (EverCrypt_Hash_state_s){
              .tag = EverCrypt_Hash_SHA2_512_s,
              { .case_SHA2_512_s = buf5 }
            }
          );
        break;
      }
    case Spec_Hash_Definitions_Blake2S:
      {
        uint32_t init = (uint32_t)0U;
        for (uint32_t i = (uint32_t)0U; i < (uint32_t)16U; i++)
        {
          buf6[i] = init;
        }
        s =
          ((EverCrypt_Hash_state_s){ .tag = EverCrypt_Hash_Blake2S_s, { .case_Blake2S_s = buf6 } });
        break;
      }
    case Spec_Hash_Definitions_Blake2B:
      {
        uint64_t init = (uint64_t)0U;
        for (uint32_t i = (uint32_t)0U; i < (uint32_t)16U; i++)
        {
          buf[i] = init;
        }
        s = ((EverCrypt_Hash_state_s){ .tag = EverCrypt_Hash_Blake2B_s, { .case_Blake2B_s = buf } });
        break;
      }
    default:
      {
        KRML_HOST_EPRINTF("KreMLin incomplete match at %s:%d\n", __FILE__, __LINE__);
        KRML_HOST_EXIT(253U);
      }
  }
  EverCrypt_Hash_state_s st = s;
  EverCrypt_Hash_init(&st);
  EverCrypt_Hash_update2(&st, (uint64_t)0U, cb);
  EverCrypt_Hash_finish(&st, dst);
}

/* SNIPPET_END: mt_sha256_compress */

/* SNIPPET_START: mt_create */

/*
  Construction wired to sha256 from EverCrypt

  @param[in]  init   The initial hash
*/
inline MerkleTree_Low_merkle_tree *mt_create(uint8_t *init)
{
  return MerkleTree_Low_mt_create_custom((uint32_t)32U, init, mt_sha256_compress);
}

/* SNIPPET_END: mt_create */

/* SNIPPET_START: MerkleTree_Low_uint32_32_max */

uint32_t MerkleTree_Low_uint32_32_max = (uint32_t)4294967295U;

/* SNIPPET_END: MerkleTree_Low_uint32_32_max */

/* SNIPPET_START: MerkleTree_Low_uint32_max */

uint64_t MerkleTree_Low_uint32_max = (uint64_t)4294967295U;

/* SNIPPET_END: MerkleTree_Low_uint32_max */

/* SNIPPET_START: MerkleTree_Low_uint64_max */

uint64_t MerkleTree_Low_uint64_max = (uint64_t)18446744073709551615U;

/* SNIPPET_END: MerkleTree_Low_uint64_max */

/* SNIPPET_START: MerkleTree_Low_offset_range_limit */

uint64_t MerkleTree_Low_offset_range_limit = (uint64_t)4294967295U;

/* SNIPPET_END: MerkleTree_Low_offset_range_limit */

/* SNIPPET_START: MerkleTree_Low_merkle_tree_size_lg */

uint32_t MerkleTree_Low_merkle_tree_size_lg = (uint32_t)32U;

/* SNIPPET_END: MerkleTree_Low_merkle_tree_size_lg */

/* SNIPPET_START: MerkleTree_Low_uu___is_MT */

bool MerkleTree_Low_uu___is_MT(MerkleTree_Low_merkle_tree projectee)
{
  return true;
}

/* SNIPPET_END: MerkleTree_Low_uu___is_MT */

/* SNIPPET_START: MerkleTree_Low___proj__MT__item__hash_size */

uint32_t MerkleTree_Low___proj__MT__item__hash_size(MerkleTree_Low_merkle_tree projectee)
{
  return projectee.hash_size;
}

/* SNIPPET_END: MerkleTree_Low___proj__MT__item__hash_size */

/* SNIPPET_START: MerkleTree_Low___proj__MT__item__offset */

uint64_t MerkleTree_Low___proj__MT__item__offset(MerkleTree_Low_merkle_tree projectee)
{
  return projectee.offset;
}

/* SNIPPET_END: MerkleTree_Low___proj__MT__item__offset */

/* SNIPPET_START: MerkleTree_Low___proj__MT__item__i */

uint32_t MerkleTree_Low___proj__MT__item__i(MerkleTree_Low_merkle_tree projectee)
{
  return projectee.i;
}

/* SNIPPET_END: MerkleTree_Low___proj__MT__item__i */

/* SNIPPET_START: MerkleTree_Low___proj__MT__item__j */

uint32_t MerkleTree_Low___proj__MT__item__j(MerkleTree_Low_merkle_tree projectee)
{
  return projectee.j;
}

/* SNIPPET_END: MerkleTree_Low___proj__MT__item__j */

/* SNIPPET_START: MerkleTree_Low___proj__MT__item__hs */

LowStar_Vector_vector_str__LowStar_Vector_vector_str___uint8_t_
MerkleTree_Low___proj__MT__item__hs(MerkleTree_Low_merkle_tree projectee)
{
  return projectee.hs;
}

/* SNIPPET_END: MerkleTree_Low___proj__MT__item__hs */

/* SNIPPET_START: MerkleTree_Low___proj__MT__item__rhs_ok */

bool MerkleTree_Low___proj__MT__item__rhs_ok(MerkleTree_Low_merkle_tree projectee)
{
  return projectee.rhs_ok;
}

/* SNIPPET_END: MerkleTree_Low___proj__MT__item__rhs_ok */

/* SNIPPET_START: MerkleTree_Low___proj__MT__item__rhs */

LowStar_Vector_vector_str___uint8_t_
MerkleTree_Low___proj__MT__item__rhs(MerkleTree_Low_merkle_tree projectee)
{
  return projectee.rhs;
}

/* SNIPPET_END: MerkleTree_Low___proj__MT__item__rhs */

/* SNIPPET_START: MerkleTree_Low___proj__MT__item__mroot */

uint8_t *MerkleTree_Low___proj__MT__item__mroot(MerkleTree_Low_merkle_tree projectee)
{
  return projectee.mroot;
}

/* SNIPPET_END: MerkleTree_Low___proj__MT__item__mroot */

/* SNIPPET_START: MerkleTree_Low___proj__MT__item__hash_fun */

void
(*MerkleTree_Low___proj__MT__item__hash_fun(MerkleTree_Low_merkle_tree projectee))(
  uint8_t *x0,
  uint8_t *x1,
  uint8_t *x2
)
{
  return projectee.hash_fun;
}

/* SNIPPET_END: MerkleTree_Low___proj__MT__item__hash_fun */

/* SNIPPET_START: MerkleTree_Low_merkle_tree_conditions */

bool
MerkleTree_Low_merkle_tree_conditions(
  uint64_t offset,
  uint32_t i,
  uint32_t j,
  LowStar_Vector_vector_str__LowStar_Vector_vector_str___uint8_t_ hs,
  bool rhs_ok,
  LowStar_Vector_vector_str___uint8_t_ rhs,
  uint8_t *mroot
)
{
  return
    j
    >= i
    && MerkleTree_Low_uint64_max - offset >= (uint64_t)j
    && hs.sz == (uint32_t)32U
    && rhs.sz == (uint32_t)32U;
}

/* SNIPPET_END: MerkleTree_Low_merkle_tree_conditions */

/* SNIPPET_START: MerkleTree_Low_offset_of */

uint32_t MerkleTree_Low_offset_of(uint32_t i)
{
  if (i % (uint32_t)2U == (uint32_t)0U)
  {
    return i;
  }
  return i - (uint32_t)1U;
}

/* SNIPPET_END: MerkleTree_Low_offset_of */

/* SNIPPET_START: alloc_rid__LowStar_Vector_vector_str__uint8_t_ */

static LowStar_Vector_vector_str__LowStar_Vector_vector_str___uint8_t_
alloc_rid__LowStar_Vector_vector_str__uint8_t_(
  uint32_t len,
  LowStar_Vector_vector_str___uint8_t_ v
)
{
  KRML_CHECK_SIZE(sizeof (LowStar_Vector_vector_str___uint8_t_), len);
  LowStar_Vector_vector_str___uint8_t_
  *buf = KRML_HOST_MALLOC(sizeof (LowStar_Vector_vector_str___uint8_t_) * len);
  for (uint32_t _i = 0U; _i < len; ++_i)
    buf[_i] = v;
  return
    (
      (LowStar_Vector_vector_str__LowStar_Vector_vector_str___uint8_t_){
        .sz = len,
        .cap = len,
        .vs = buf
      }
    );
}

/* SNIPPET_END: alloc_rid__LowStar_Vector_vector_str__uint8_t_ */

/* SNIPPET_START: regional__uint32_t_LowStar_Vector_vector_str___uint8_t_ */

typedef struct regional__uint32_t_LowStar_Vector_vector_str___uint8_t__s
{
  uint32_t state;
  LowStar_Vector_vector_str___uint8_t_ dummy;
  LowStar_Vector_vector_str___uint8_t_ (*r_alloc)(uint32_t x0);
  void (*r_free)(uint32_t x0, LowStar_Vector_vector_str___uint8_t_ x1);
}
regional__uint32_t_LowStar_Vector_vector_str___uint8_t_;

/* SNIPPET_END: regional__uint32_t_LowStar_Vector_vector_str___uint8_t_ */

/* SNIPPET_START: assign__LowStar_Vector_vector_str__uint8_t_ */

static void
assign__LowStar_Vector_vector_str__uint8_t_(
  LowStar_Vector_vector_str__LowStar_Vector_vector_str___uint8_t_ vec,
  uint32_t i,
  LowStar_Vector_vector_str___uint8_t_ v
)
{
  (vec.vs + i)[0U] = v;
}

/* SNIPPET_END: assign__LowStar_Vector_vector_str__uint8_t_ */

/* SNIPPET_START: alloc___LowStar_Vector_vector_str__uint8_t__uint32_t */

static void
alloc___LowStar_Vector_vector_str__uint8_t__uint32_t(
  regional__uint32_t_LowStar_Vector_vector_str___uint8_t_ rg,
  LowStar_Vector_vector_str__LowStar_Vector_vector_str___uint8_t_ rv,
  uint32_t cidx
)
{
  if (!(cidx == (uint32_t)0U))
  {
    LowStar_Vector_vector_str___uint8_t_ v = rg.r_alloc(rg.state);
    assign__LowStar_Vector_vector_str__uint8_t_(rv, cidx - (uint32_t)1U, v);
    alloc___LowStar_Vector_vector_str__uint8_t__uint32_t(rg, rv, cidx - (uint32_t)1U);
    return;
  }
}

/* SNIPPET_END: alloc___LowStar_Vector_vector_str__uint8_t__uint32_t */

/* SNIPPET_START: alloc_rid__LowStar_Vector_vector_str__uint8_t__uint32_t */

static LowStar_Vector_vector_str__LowStar_Vector_vector_str___uint8_t_
alloc_rid__LowStar_Vector_vector_str__uint8_t__uint32_t(
  regional__uint32_t_LowStar_Vector_vector_str___uint8_t_ rg,
  uint32_t len
)
{
  LowStar_Vector_vector_str__LowStar_Vector_vector_str___uint8_t_
  vec = alloc_rid__LowStar_Vector_vector_str__uint8_t_(len, rg.dummy);
  alloc___LowStar_Vector_vector_str__uint8_t__uint32_t(rg, vec, len);
  return vec;
}

/* SNIPPET_END: alloc_rid__LowStar_Vector_vector_str__uint8_t__uint32_t */

/* SNIPPET_START: alloc_rid___uint8_t_ */

static LowStar_Vector_vector_str___uint8_t_ alloc_rid___uint8_t_(uint32_t len, uint8_t *v)
{
  KRML_CHECK_SIZE(sizeof (uint8_t *), len);
  uint8_t **buf = KRML_HOST_MALLOC(sizeof (uint8_t *) * len);
  for (uint32_t _i = 0U; _i < len; ++_i)
    buf[_i] = v;
  return ((LowStar_Vector_vector_str___uint8_t_){ .sz = len, .cap = len, .vs = buf });
}

/* SNIPPET_END: alloc_rid___uint8_t_ */

/* SNIPPET_START: assign___uint8_t_ */

static void assign___uint8_t_(LowStar_Vector_vector_str___uint8_t_ vec, uint32_t i, uint8_t *v)
{
  (vec.vs + i)[0U] = v;
}

/* SNIPPET_END: assign___uint8_t_ */

/* SNIPPET_START: alloc____uint8_t__uint32_t */

static void
alloc____uint8_t__uint32_t(
  regional__uint32_t__uint8_t_ rg,
  LowStar_Vector_vector_str___uint8_t_ rv,
  uint32_t cidx
)
{
  if (!(cidx == (uint32_t)0U))
  {
    uint8_t *v = rg.r_alloc(rg.state);
    assign___uint8_t_(rv, cidx - (uint32_t)1U, v);
    alloc____uint8_t__uint32_t(rg, rv, cidx - (uint32_t)1U);
    return;
  }
}

/* SNIPPET_END: alloc____uint8_t__uint32_t */

/* SNIPPET_START: alloc_rid___uint8_t__uint32_t */

static LowStar_Vector_vector_str___uint8_t_
alloc_rid___uint8_t__uint32_t(regional__uint32_t__uint8_t_ rg, uint32_t len)
{
  LowStar_Vector_vector_str___uint8_t_ vec = alloc_rid___uint8_t_(len, rg.dummy);
  alloc____uint8_t__uint32_t(rg, vec, len);
  return vec;
}

/* SNIPPET_END: alloc_rid___uint8_t__uint32_t */

/* SNIPPET_START: create_empty_mt */

static MerkleTree_Low_merkle_tree
*create_empty_mt(uint32_t hsz, void (*hash_fun)(uint8_t *x0, uint8_t *x1, uint8_t *x2))
{
  LowStar_Vector_vector_str__LowStar_Vector_vector_str___uint8_t_
  hs =
    alloc_rid__LowStar_Vector_vector_str__uint8_t__uint32_t((
        (regional__uint32_t_LowStar_Vector_vector_str___uint8_t_){
          .state = hsz,
          .dummy = { .sz = (uint32_t)0U, .cap = (uint32_t)0U, .vs = NULL },
          .r_alloc = hash_vec_r_alloc,
          .r_free = hash_vec_r_free
        }
      ),
      (uint32_t)32U);
  LowStar_Vector_vector_str___uint8_t_
  rhs =
    alloc_rid___uint8_t__uint32_t((
        (regional__uint32_t__uint8_t_){
          .state = hsz,
          .dummy = NULL,
          .r_alloc = hash_r_alloc,
          .r_free = hash_r_free
        }
      ),
      (uint32_t)32U);
  regional__uint32_t__uint8_t_
  x00 = { .state = hsz, .dummy = NULL, .r_alloc = hash_r_alloc, .r_free = hash_r_free };
  regional__uint32_t__uint8_t_
  x0 = { .state = hsz, .dummy = NULL, .r_alloc = hash_r_alloc, .r_free = hash_r_free };
  uint8_t *mroot = x00.r_alloc(x0.state);
  KRML_CHECK_SIZE(sizeof (MerkleTree_Low_merkle_tree), (uint32_t)1U);
  MerkleTree_Low_merkle_tree *mt = KRML_HOST_MALLOC(sizeof (MerkleTree_Low_merkle_tree));
  mt[0U]
  =
    (
      (MerkleTree_Low_merkle_tree){
        .hash_size = hsz,
        .offset = (uint64_t)0U,
        .i = (uint32_t)0U,
        .j = (uint32_t)0U,
        .hs = hs,
        .rhs_ok = false,
        .rhs = rhs,
        .mroot = mroot,
        .hash_fun = hash_fun
      }
    );
  return mt;
}

/* SNIPPET_END: create_empty_mt */

/* SNIPPET_START: index__LowStar_Vector_vector_str__uint8_t_ */

static LowStar_Vector_vector_str___uint8_t_
index__LowStar_Vector_vector_str__uint8_t_(
  LowStar_Vector_vector_str__LowStar_Vector_vector_str___uint8_t_ vec,
  uint32_t i
)
{
  return vec.vs[i];
}

/* SNIPPET_END: index__LowStar_Vector_vector_str__uint8_t_ */

/* SNIPPET_START: free_elems__LowStar_Vector_vector_str__uint8_t__uint32_t */

static void
free_elems__LowStar_Vector_vector_str__uint8_t__uint32_t(
  regional__uint32_t_LowStar_Vector_vector_str___uint8_t_ rg,
  LowStar_Vector_vector_str__LowStar_Vector_vector_str___uint8_t_ rv,
  uint32_t idx
)
{
  LowStar_Vector_vector_str___uint8_t_
  uu____0 = index__LowStar_Vector_vector_str__uint8_t_(rv, idx);
  rg.r_free(rg.state, uu____0);
  if (idx != (uint32_t)0U)
  {
    free_elems__LowStar_Vector_vector_str__uint8_t__uint32_t(rg, rv, idx - (uint32_t)1U);
    return;
  }
}

/* SNIPPET_END: free_elems__LowStar_Vector_vector_str__uint8_t__uint32_t */

/* SNIPPET_START: free__LowStar_Vector_vector_str__uint8_t_ */

static void
free__LowStar_Vector_vector_str__uint8_t_(
  LowStar_Vector_vector_str__LowStar_Vector_vector_str___uint8_t_ vec
)
{
  KRML_HOST_FREE(vec.vs);
}

/* SNIPPET_END: free__LowStar_Vector_vector_str__uint8_t_ */

/* SNIPPET_START: free__LowStar_Vector_vector_str__uint8_t__uint32_t */

static void
free__LowStar_Vector_vector_str__uint8_t__uint32_t(
  regional__uint32_t_LowStar_Vector_vector_str___uint8_t_ rg,
  LowStar_Vector_vector_str__LowStar_Vector_vector_str___uint8_t_ rv
)
{
  if (!(rv.sz == (uint32_t)0U))
  {
    free_elems__LowStar_Vector_vector_str__uint8_t__uint32_t(rg, rv, rv.sz - (uint32_t)1U);
  }
  free__LowStar_Vector_vector_str__uint8_t_(rv);
}

/* SNIPPET_END: free__LowStar_Vector_vector_str__uint8_t__uint32_t */

/* SNIPPET_START: MerkleTree_Low_mt_free */

void MerkleTree_Low_mt_free(MerkleTree_Low_merkle_tree *mt)
{
  MerkleTree_Low_merkle_tree mtv = *mt;
  free__LowStar_Vector_vector_str__uint8_t__uint32_t((
      (regional__uint32_t_LowStar_Vector_vector_str___uint8_t_){
        .state = mtv.hash_size,
        .dummy = { .sz = (uint32_t)0U, .cap = (uint32_t)0U, .vs = NULL },
        .r_alloc = hash_vec_r_alloc,
        .r_free = hash_vec_r_free
      }
    ),
    mtv.hs);
  free___uint8_t__uint32_t((
      (regional__uint32_t__uint8_t_){
        .state = mtv.hash_size,
        .dummy = NULL,
        .r_alloc = hash_r_alloc,
        .r_free = hash_r_free
      }
    ),
    mtv.rhs);
  regional__uint32_t__uint8_t_
  x00 = { .state = mtv.hash_size, .dummy = NULL, .r_alloc = hash_r_alloc, .r_free = hash_r_free };
  regional__uint32_t__uint8_t_
  x0 = { .state = mtv.hash_size, .dummy = NULL, .r_alloc = hash_r_alloc, .r_free = hash_r_free };
  x00.r_free(x0.state, mtv.mroot);
  KRML_HOST_FREE(mt);
}

/* SNIPPET_END: MerkleTree_Low_mt_free */

/* SNIPPET_START: insert___uint8_t__uint32_t */

static LowStar_Vector_vector_str___uint8_t_
insert___uint8_t__uint32_t(LowStar_Vector_vector_str___uint8_t_ rv, uint8_t *v)
{
  LowStar_Vector_vector_str___uint8_t_ irv = insert___uint8_t_(rv, v);
  return irv;
}

/* SNIPPET_END: insert___uint8_t__uint32_t */

/* SNIPPET_START: copyable__uint32_t__uint8_t_ */

typedef void (*copyable__uint32_t__uint8_t_)(uint32_t x0, uint8_t *x1, uint8_t *x2);

/* SNIPPET_END: copyable__uint32_t__uint8_t_ */

/* SNIPPET_START: insert_copy___uint8_t__uint32_t */

static LowStar_Vector_vector_str___uint8_t_
insert_copy___uint8_t__uint32_t(
  regional__uint32_t__uint8_t_ rg,
  void (*cp)(uint32_t x0, uint8_t *x1, uint8_t *x2),
  LowStar_Vector_vector_str___uint8_t_ rv,
  uint8_t *v
)
{
  uint8_t *nv = rg.r_alloc(rg.state);
  cp(rg.state, v, nv);
  return insert___uint8_t__uint32_t(rv, nv);
}

/* SNIPPET_END: insert_copy___uint8_t__uint32_t */

/* SNIPPET_START: assign__LowStar_Vector_vector_str__uint8_t__uint32_t */

static void
assign__LowStar_Vector_vector_str__uint8_t__uint32_t(
  LowStar_Vector_vector_str__LowStar_Vector_vector_str___uint8_t_ rv,
  uint32_t i,
  LowStar_Vector_vector_str___uint8_t_ v
)
{
  assign__LowStar_Vector_vector_str__uint8_t_(rv, i, v);
}

/* SNIPPET_END: assign__LowStar_Vector_vector_str__uint8_t__uint32_t */

/* SNIPPET_START: insert_ */

static void
insert_(
  uint32_t hsz,
  uint32_t lv,
  uint32_t j,
  LowStar_Vector_vector_str__LowStar_Vector_vector_str___uint8_t_ hs,
  uint8_t *acc,
  void (*hash_fun)(uint8_t *x0, uint8_t *x1, uint8_t *x2)
)
{
  LowStar_Vector_vector_str___uint8_t_
  uu____0 = index__LowStar_Vector_vector_str__uint8_t_(hs, lv);
  LowStar_Vector_vector_str___uint8_t_
  ihv =
    insert_copy___uint8_t__uint32_t((
        (regional__uint32_t__uint8_t_){
          .state = hsz,
          .dummy = NULL,
          .r_alloc = hash_r_alloc,
          .r_free = hash_r_free
        }
      ),
      hash_copy,
      uu____0,
      acc);
  assign__LowStar_Vector_vector_str__uint8_t__uint32_t(hs, lv, ihv);
  if (j % (uint32_t)2U == (uint32_t)1U)
  {
    LowStar_Vector_vector_str___uint8_t_ lvhs = index__LowStar_Vector_vector_str__uint8_t_(hs, lv);
    hash_fun(index___uint8_t_(lvhs, lvhs.sz - (uint32_t)2U), acc, acc);
    insert_(hsz, lv + (uint32_t)1U, j / (uint32_t)2U, hs, acc, hash_fun);
    return;
  }
}

/* SNIPPET_END: insert_ */

/* SNIPPET_START: MerkleTree_Low_mt_insert_pre */

bool MerkleTree_Low_mt_insert_pre(const MerkleTree_Low_merkle_tree *mt, uint8_t *v)
{
  MerkleTree_Low_merkle_tree mt1 = *(MerkleTree_Low_merkle_tree *)mt;
  return
    mt1.j
    < MerkleTree_Low_uint32_32_max
    && MerkleTree_Low_uint64_max - mt1.offset >= (uint64_t)(mt1.j + (uint32_t)1U);
}

/* SNIPPET_END: MerkleTree_Low_mt_insert_pre */

/* SNIPPET_START: MerkleTree_Low_mt_insert */

void MerkleTree_Low_mt_insert(MerkleTree_Low_merkle_tree *mt, uint8_t *v)
{
  MerkleTree_Low_merkle_tree mtv = *mt;
  LowStar_Vector_vector_str__LowStar_Vector_vector_str___uint8_t_ hs = mtv.hs;
  uint32_t hsz1 = mtv.hash_size;
  insert_(hsz1, (uint32_t)0U, mtv.j, hs, v, mtv.hash_fun);
  *mt
  =
    (
      (MerkleTree_Low_merkle_tree){
        .hash_size = mtv.hash_size,
        .offset = mtv.offset,
        .i = mtv.i,
        .j = mtv.j + (uint32_t)1U,
        .hs = mtv.hs,
        .rhs_ok = false,
        .rhs = mtv.rhs,
        .mroot = mtv.mroot,
        .hash_fun = mtv.hash_fun
      }
    );
}

/* SNIPPET_END: MerkleTree_Low_mt_insert */

/* SNIPPET_START: MerkleTree_Low_mt_create_custom */

MerkleTree_Low_merkle_tree
*MerkleTree_Low_mt_create_custom(
  uint32_t hsz,
  uint8_t *init,
  void (*hash_fun)(uint8_t *x0, uint8_t *x1, uint8_t *x2)
)
{
  MerkleTree_Low_merkle_tree *mt = create_empty_mt(hsz, hash_fun);
  MerkleTree_Low_mt_insert(mt, init);
  return mt;
}

/* SNIPPET_END: MerkleTree_Low_mt_create_custom */

/* SNIPPET_START: MerkleTree_Low_uu___is_Path */

bool MerkleTree_Low_uu___is_Path(MerkleTree_Low_path projectee)
{
  return true;
}

/* SNIPPET_END: MerkleTree_Low_uu___is_Path */

/* SNIPPET_START: MerkleTree_Low___proj__Path__item__hash_size */

uint32_t MerkleTree_Low___proj__Path__item__hash_size(MerkleTree_Low_path projectee)
{
  return projectee.hash_size;
}

/* SNIPPET_END: MerkleTree_Low___proj__Path__item__hash_size */

/* SNIPPET_START: MerkleTree_Low___proj__Path__item__hashes */

LowStar_Vector_vector_str___uint8_t_
MerkleTree_Low___proj__Path__item__hashes(MerkleTree_Low_path projectee)
{
  return projectee.hashes;
}

/* SNIPPET_END: MerkleTree_Low___proj__Path__item__hashes */

/* SNIPPET_START: MerkleTree_Low_init_path */

MerkleTree_Low_path *MerkleTree_Low_init_path(uint32_t hsz)
{
  regional__uint32_t_LowStar_Vector_vector_str___uint8_t_
  x00 =
    {
      .state = hsz,
      .dummy = { .sz = (uint32_t)0U, .cap = (uint32_t)0U, .vs = NULL },
      .r_alloc = hash_vec_r_alloc,
      .r_free = hash_vec_r_free
    };
  regional__uint32_t_LowStar_Vector_vector_str___uint8_t_
  x0 =
    {
      .state = hsz,
      .dummy = { .sz = (uint32_t)0U, .cap = (uint32_t)0U, .vs = NULL },
      .r_alloc = hash_vec_r_alloc,
      .r_free = hash_vec_r_free
    };
  KRML_CHECK_SIZE(sizeof (MerkleTree_Low_path), (uint32_t)1U);
  MerkleTree_Low_path *buf = KRML_HOST_MALLOC(sizeof (MerkleTree_Low_path));
  buf[0U] = ((MerkleTree_Low_path){ .hash_size = hsz, .hashes = x00.r_alloc(x0.state) });
  return buf;
}

/* SNIPPET_END: MerkleTree_Low_init_path */

/* SNIPPET_START: clear___uint8_t_ */

static LowStar_Vector_vector_str___uint8_t_
clear___uint8_t_(LowStar_Vector_vector_str___uint8_t_ vec)
{
  return
    ((LowStar_Vector_vector_str___uint8_t_){ .sz = (uint32_t)0U, .cap = vec.cap, .vs = vec.vs });
}

/* SNIPPET_END: clear___uint8_t_ */

/* SNIPPET_START: MerkleTree_Low_clear_path */

void MerkleTree_Low_clear_path(MerkleTree_Low_path *p)
{
  MerkleTree_Low_path pv = *p;
  *p
  = ((MerkleTree_Low_path){ .hash_size = pv.hash_size, .hashes = clear___uint8_t_(pv.hashes) });
}

/* SNIPPET_END: MerkleTree_Low_clear_path */

/* SNIPPET_START: MerkleTree_Low_free_path */

void MerkleTree_Low_free_path(MerkleTree_Low_path *p)
{
  MerkleTree_Low_path pv = *p;
  free___uint8_t_(pv.hashes);
  KRML_HOST_FREE(p);
}

/* SNIPPET_END: MerkleTree_Low_free_path */

/* SNIPPET_START: assign_copy___uint8_t__uint32_t */

static void
assign_copy___uint8_t__uint32_t(
  regional__uint32_t__uint8_t_ rg,
  void (*cp)(uint32_t x0, uint8_t *x1, uint8_t *x2),
  LowStar_Vector_vector_str___uint8_t_ rv,
  uint32_t i,
  uint8_t *v
)
{
  uint8_t *uu____0 = index___uint8_t_(rv, i);
  cp(rg.state, v, uu____0);
}

/* SNIPPET_END: assign_copy___uint8_t__uint32_t */

/* SNIPPET_START: construct_rhs */

static void
construct_rhs(
  uint32_t hsz,
  uint32_t lv,
  LowStar_Vector_vector_str__LowStar_Vector_vector_str___uint8_t_ hs,
  LowStar_Vector_vector_str___uint8_t_ rhs,
  uint32_t i,
  uint32_t j,
  uint8_t *acc,
  bool actd,
  void (*hash_fun)(uint8_t *x0, uint8_t *x1, uint8_t *x2)
)
{
  if (!(j == (uint32_t)0U))
  {
    uint32_t ofs = MerkleTree_Low_offset_of(i);
    if (j % (uint32_t)2U == (uint32_t)0U)
    {
      construct_rhs(hsz,
        lv + (uint32_t)1U,
        hs,
        rhs,
        i / (uint32_t)2U,
        j / (uint32_t)2U,
        acc,
        actd,
        hash_fun);
      return;
    }
    if (actd)
    {
      assign_copy___uint8_t__uint32_t((
          (regional__uint32_t__uint8_t_){
            .state = hsz,
            .dummy = NULL,
            .r_alloc = hash_r_alloc,
            .r_free = hash_r_free
          }
        ),
        hash_copy,
        rhs,
        lv,
        acc);
      hash_fun(index___uint8_t_(index__LowStar_Vector_vector_str__uint8_t_(hs, lv),
          j - (uint32_t)1U - ofs),
        acc,
        acc);
    }
    else
    {
      hash_copy(hsz,
        index___uint8_t_(index__LowStar_Vector_vector_str__uint8_t_(hs, lv), j - (uint32_t)1U - ofs),
        acc);
    }
    construct_rhs(hsz,
      lv + (uint32_t)1U,
      hs,
      rhs,
      i / (uint32_t)2U,
      j / (uint32_t)2U,
      acc,
      true,
      hash_fun);
    return;
  }
}

/* SNIPPET_END: construct_rhs */

/* SNIPPET_START: MerkleTree_Low_mt_get_root_pre */

bool MerkleTree_Low_mt_get_root_pre(const MerkleTree_Low_merkle_tree *mt, uint8_t *rt)
{
  MerkleTree_Low_merkle_tree *mt1 = (MerkleTree_Low_merkle_tree *)mt;
  MerkleTree_Low_merkle_tree mt2 = *mt1;
  return true;
}

/* SNIPPET_END: MerkleTree_Low_mt_get_root_pre */

/* SNIPPET_START: MerkleTree_Low_mt_get_root */

void MerkleTree_Low_mt_get_root(const MerkleTree_Low_merkle_tree *mt, uint8_t *rt)
{
  MerkleTree_Low_merkle_tree *mt1 = (MerkleTree_Low_merkle_tree *)mt;
  MerkleTree_Low_merkle_tree mtv = *mt1;
  uint64_t prefix = mtv.offset;
  uint32_t i = mtv.i;
  uint32_t j = mtv.j;
  LowStar_Vector_vector_str__LowStar_Vector_vector_str___uint8_t_ hs = mtv.hs;
  LowStar_Vector_vector_str___uint8_t_ rhs = mtv.rhs;
  uint8_t *mroot = mtv.mroot;
  uint32_t hash_size = mtv.hash_size;
  void (*hash_fun)(uint8_t *x0, uint8_t *x1, uint8_t *x2) = mtv.hash_fun;
  if (mtv.rhs_ok)
  {
    hash_copy(hash_size, mroot, rt);
    return;
  }
  construct_rhs(hash_size, (uint32_t)0U, hs, rhs, i, j, rt, false, hash_fun);
  hash_copy(hash_size, rt, mroot);
  *mt1
  =
    (
      (MerkleTree_Low_merkle_tree){
        .hash_size = hash_size,
        .offset = prefix,
        .i = i,
        .j = j,
        .hs = hs,
        .rhs_ok = true,
        .rhs = rhs,
        .mroot = mroot,
        .hash_fun = hash_fun
      }
    );
}

/* SNIPPET_END: MerkleTree_Low_mt_get_root */

/* SNIPPET_START: MerkleTree_Low_mt_path_insert */

void MerkleTree_Low_mt_path_insert(uint32_t hsz, MerkleTree_Low_path *p, uint8_t *hp)
{
  MerkleTree_Low_path pth = *p;
  LowStar_Vector_vector_str___uint8_t_ pv = pth.hashes;
  LowStar_Vector_vector_str___uint8_t_ ipv = insert___uint8_t_(pv, hp);
  *p = ((MerkleTree_Low_path){ .hash_size = hsz, .hashes = ipv });
}

/* SNIPPET_END: MerkleTree_Low_mt_path_insert */

/* SNIPPET_START: mt_path_length_step */

static uint32_t mt_path_length_step(uint32_t k, uint32_t j, bool actd)
{
  if (j == (uint32_t)0U)
  {
    return (uint32_t)0U;
  }
  if (k % (uint32_t)2U == (uint32_t)0U)
  {
    if (j == k || (j == k + (uint32_t)1U && !actd))
    {
      return (uint32_t)0U;
    }
    return (uint32_t)1U;
  }
  return (uint32_t)1U;
}

/* SNIPPET_END: mt_path_length_step */

/* SNIPPET_START: mt_path_length */

static uint32_t mt_path_length(uint32_t lv, uint32_t k, uint32_t j, bool actd)
{
  if (j == (uint32_t)0U)
  {
    return (uint32_t)0U;
  }
  bool nactd = actd || j % (uint32_t)2U == (uint32_t)1U;
  return
    mt_path_length_step(k,
      j,
      actd)
    + mt_path_length(lv + (uint32_t)1U, k / (uint32_t)2U, j / (uint32_t)2U, nactd);
}

/* SNIPPET_END: mt_path_length */

/* SNIPPET_START: MerkleTree_Low_mt_get_path_length */

uint32_t MerkleTree_Low_mt_get_path_length(const MerkleTree_Low_path *p)
{
  MerkleTree_Low_path pd = *(MerkleTree_Low_path *)p;
  return pd.hashes.sz;
}

/* SNIPPET_END: MerkleTree_Low_mt_get_path_length */

/* SNIPPET_START: MerkleTree_Low_mt_get_path_step_pre */

bool MerkleTree_Low_mt_get_path_step_pre(const MerkleTree_Low_path *p, uint32_t i)
{
  MerkleTree_Low_path *p1 = (MerkleTree_Low_path *)p;
  MerkleTree_Low_path x0 = *p1;
  LowStar_Vector_vector_str___uint8_t_ scrut = x0.hashes;
  return i < scrut.sz;
}

/* SNIPPET_END: MerkleTree_Low_mt_get_path_step_pre */

/* SNIPPET_START: MerkleTree_Low_mt_get_path_step */

uint8_t *MerkleTree_Low_mt_get_path_step(const MerkleTree_Low_path *p, uint32_t i)
{
  MerkleTree_Low_path pd = *(MerkleTree_Low_path *)p;
  return index___uint8_t_(pd.hashes, i);
}

/* SNIPPET_END: MerkleTree_Low_mt_get_path_step */

/* SNIPPET_START: mt_get_path_ */

static void
mt_get_path_(
  uint32_t hsz,
  uint32_t lv,
  LowStar_Vector_vector_str__LowStar_Vector_vector_str___uint8_t_ hs,
  LowStar_Vector_vector_str___uint8_t_ rhs,
  uint32_t i,
  uint32_t j,
  uint32_t k,
  MerkleTree_Low_path *p,
  bool actd
)
{
  uint32_t ofs = MerkleTree_Low_offset_of(i);
  if (!(j == (uint32_t)0U))
  {
    MerkleTree_Low_path pth = *p;
    uint32_t ofs1 = MerkleTree_Low_offset_of(i);
    if (k % (uint32_t)2U == (uint32_t)1U)
    {
      uint8_t
      *uu____0 =
        index___uint8_t_(index__LowStar_Vector_vector_str__uint8_t_(hs, lv),
          k - (uint32_t)1U - ofs1);
      MerkleTree_Low_path pth1 = *p;
      LowStar_Vector_vector_str___uint8_t_ pv = pth1.hashes;
      LowStar_Vector_vector_str___uint8_t_ ipv = insert___uint8_t_(pv, uu____0);
      *p = ((MerkleTree_Low_path){ .hash_size = hsz, .hashes = ipv });
    }
    else if (!(k == j))
    {
      if (k + (uint32_t)1U == j)
      {
        if (actd)
        {
          uint8_t *uu____1 = index___uint8_t_(rhs, lv);
          MerkleTree_Low_path pth1 = *p;
          LowStar_Vector_vector_str___uint8_t_ pv = pth1.hashes;
          LowStar_Vector_vector_str___uint8_t_ ipv = insert___uint8_t_(pv, uu____1);
          *p = ((MerkleTree_Low_path){ .hash_size = hsz, .hashes = ipv });
        }
      }
      else
      {
        uint8_t
        *uu____2 =
          index___uint8_t_(index__LowStar_Vector_vector_str__uint8_t_(hs, lv),
            k + (uint32_t)1U - ofs1);
        MerkleTree_Low_path pth1 = *p;
        LowStar_Vector_vector_str___uint8_t_ pv = pth1.hashes;
        LowStar_Vector_vector_str___uint8_t_ ipv = insert___uint8_t_(pv, uu____2);
        *p = ((MerkleTree_Low_path){ .hash_size = hsz, .hashes = ipv });
      }
    }
    bool ite;
    if (j % (uint32_t)2U == (uint32_t)0U)
    {
      ite = actd;
    }
    else
    {
      ite = true;
    }
    mt_get_path_(hsz,
      lv + (uint32_t)1U,
      hs,
      rhs,
      i / (uint32_t)2U,
      j / (uint32_t)2U,
      k / (uint32_t)2U,
      p,
      ite);
    return;
  }
}

/* SNIPPET_END: mt_get_path_ */

/* SNIPPET_START: MerkleTree_Low_mt_get_path_pre */

bool
MerkleTree_Low_mt_get_path_pre(
  const MerkleTree_Low_merkle_tree *mt,
  uint64_t idx,
  const MerkleTree_Low_path *p,
  uint8_t *root
)
{
  MerkleTree_Low_merkle_tree *mt1 = (MerkleTree_Low_merkle_tree *)mt;
  MerkleTree_Low_path *p1 = (MerkleTree_Low_path *)p;
  MerkleTree_Low_merkle_tree mtv = *mt1;
  MerkleTree_Low_path uu____0 = *p1;
  return
    idx
    >= mtv.offset
    && idx - mtv.offset <= MerkleTree_Low_offset_range_limit
    && uu____0.hash_size == mtv.hash_size
    &&
      mtv.i
      <= (uint32_t)(idx - mtv.offset)
      && (uint32_t)(idx - mtv.offset) < mtv.j
      && uu____0.hashes.sz == (uint32_t)0U;
}

/* SNIPPET_END: MerkleTree_Low_mt_get_path_pre */

/* SNIPPET_START: MerkleTree_Low_mt_get_path */

uint32_t
MerkleTree_Low_mt_get_path(
  const MerkleTree_Low_merkle_tree *mt,
  uint64_t idx,
  MerkleTree_Low_path *p,
  uint8_t *root
)
{
  MerkleTree_Low_merkle_tree *ncmt = (MerkleTree_Low_merkle_tree *)mt;
  MerkleTree_Low_mt_get_root(mt, root);
  MerkleTree_Low_merkle_tree mtv = *ncmt;
  uint32_t hsz1 = mtv.hash_size;
  uint32_t idx1 = (uint32_t)(idx - mtv.offset);
  uint32_t i = mtv.i;
  uint32_t ofs = MerkleTree_Low_offset_of(mtv.i);
  uint32_t j = mtv.j;
  LowStar_Vector_vector_str__LowStar_Vector_vector_str___uint8_t_ hs = mtv.hs;
  LowStar_Vector_vector_str___uint8_t_ rhs = mtv.rhs;
  uint8_t
  *ih =
    index___uint8_t_(index__LowStar_Vector_vector_str__uint8_t_(hs, (uint32_t)0U),
      idx1 - ofs);
  MerkleTree_Low_path pth = *p;
  LowStar_Vector_vector_str___uint8_t_ pv = pth.hashes;
  LowStar_Vector_vector_str___uint8_t_ ipv = insert___uint8_t_(pv, ih);
  *p = ((MerkleTree_Low_path){ .hash_size = hsz1, .hashes = ipv });
  mt_get_path_(mtv.hash_size, (uint32_t)0U, hs, rhs, i, j, idx1, p, false);
  return j;
}

/* SNIPPET_END: MerkleTree_Low_mt_get_path */

/* SNIPPET_START: mt_flush_to_ */

static void
mt_flush_to_(
  uint32_t hsz,
  uint32_t lv,
  LowStar_Vector_vector_str__LowStar_Vector_vector_str___uint8_t_ hs,
  uint32_t pi,
  uint32_t i
)
{
  uint32_t oi = MerkleTree_Low_offset_of(i);
  uint32_t opi = MerkleTree_Low_offset_of(pi);
  if (!(oi == opi))
  {
    uint32_t ofs = oi - opi;
    LowStar_Vector_vector_str___uint8_t_ hvec = index__LowStar_Vector_vector_str__uint8_t_(hs, lv);
    if (!(ofs == (uint32_t)0U))
    {
      free_elems___uint8_t__uint32_t((
          (regional__uint32_t__uint8_t_){
            .state = hsz,
            .dummy = NULL,
            .r_alloc = hash_r_alloc,
            .r_free = hash_r_free
          }
        ),
        hvec,
        ofs - (uint32_t)1U);
    }
    LowStar_Vector_vector_str___uint8_t_ frv;
    if (ofs >= hvec.sz)
    {
      frv =
        (
          (LowStar_Vector_vector_str___uint8_t_){
            .sz = (uint32_t)0U,
            .cap = hvec.cap,
            .vs = hvec.vs
          }
        );
    }
    else if (ofs == (uint32_t)0U)
    {
      frv = hvec;
    }
    else
    {
      uint32_t n_shifted = hvec.sz - ofs;
      for (uint32_t i0 = (uint32_t)0U; i0 < n_shifted; i0++)
      {
        uint8_t *uu____0 = hvec.vs[ofs + i0];
        hvec.vs[(uint32_t)0U + i0] = uu____0;
      }
      frv =
        ((LowStar_Vector_vector_str___uint8_t_){ .sz = n_shifted, .cap = hvec.cap, .vs = hvec.vs });
    }
    LowStar_Vector_vector_str___uint8_t_ flushed = frv;
    assign__LowStar_Vector_vector_str__uint8_t__uint32_t(hs, lv, flushed);
    mt_flush_to_(hsz, lv + (uint32_t)1U, hs, pi / (uint32_t)2U, i / (uint32_t)2U);
    return;
  }
}

/* SNIPPET_END: mt_flush_to_ */

/* SNIPPET_START: MerkleTree_Low_mt_flush_to_pre */

bool MerkleTree_Low_mt_flush_to_pre(const MerkleTree_Low_merkle_tree *mt, uint64_t idx)
{
  MerkleTree_Low_merkle_tree *mt1 = (MerkleTree_Low_merkle_tree *)mt;
  MerkleTree_Low_merkle_tree mtv = *mt1;
  return
    idx
    >= mtv.offset
    && idx - mtv.offset <= MerkleTree_Low_offset_range_limit
    && (uint32_t)(idx - mtv.offset) >= mtv.i && (uint32_t)(idx - mtv.offset) < mtv.j;
}

/* SNIPPET_END: MerkleTree_Low_mt_flush_to_pre */

/* SNIPPET_START: MerkleTree_Low_mt_flush_to */

void MerkleTree_Low_mt_flush_to(MerkleTree_Low_merkle_tree *mt, uint64_t idx)
{
  MerkleTree_Low_merkle_tree mtv = *mt;
  uint64_t offset = mtv.offset;
  uint32_t hsz = mtv.hash_size;
  uint32_t idx1 = (uint32_t)(idx - offset);
  LowStar_Vector_vector_str__LowStar_Vector_vector_str___uint8_t_ hs = mtv.hs;
  mt_flush_to_(hsz, (uint32_t)0U, hs, mtv.i, idx1);
  *mt
  =
    (
      (MerkleTree_Low_merkle_tree){
        .hash_size = mtv.hash_size,
        .offset = mtv.offset,
        .i = idx1,
        .j = mtv.j,
        .hs = hs,
        .rhs_ok = mtv.rhs_ok,
        .rhs = mtv.rhs,
        .mroot = mtv.mroot,
        .hash_fun = mtv.hash_fun
      }
    );
}

/* SNIPPET_END: MerkleTree_Low_mt_flush_to */

/* SNIPPET_START: MerkleTree_Low_mt_flush_pre */

bool MerkleTree_Low_mt_flush_pre(const MerkleTree_Low_merkle_tree *mt)
{
  MerkleTree_Low_merkle_tree uu____0 = *(MerkleTree_Low_merkle_tree *)mt;
  return uu____0.j > uu____0.i;
}

/* SNIPPET_END: MerkleTree_Low_mt_flush_pre */

/* SNIPPET_START: MerkleTree_Low_mt_flush */

void MerkleTree_Low_mt_flush(MerkleTree_Low_merkle_tree *mt)
{
  MerkleTree_Low_merkle_tree mtv = *mt;
  uint64_t off = mtv.offset;
  uint32_t j = mtv.j;
  uint32_t j1 = j - (uint32_t)1U;
  uint64_t jo = off + (uint64_t)j1;
  MerkleTree_Low_mt_flush_to(mt, jo);
}

/* SNIPPET_END: MerkleTree_Low_mt_flush */

/* SNIPPET_START: free_elems_from___uint8_t__uint32_t */

static void
free_elems_from___uint8_t__uint32_t(
  regional__uint32_t__uint8_t_ rg,
  LowStar_Vector_vector_str___uint8_t_ rv,
  uint32_t idx
)
{
  uint8_t *uu____0 = index___uint8_t_(rv, idx);
  rg.r_free(rg.state, uu____0);
  if (idx + (uint32_t)1U < rv.sz)
  {
    free_elems_from___uint8_t__uint32_t(rg, rv, idx + (uint32_t)1U);
    return;
  }
}

/* SNIPPET_END: free_elems_from___uint8_t__uint32_t */

/* SNIPPET_START: shrink___uint8_t_ */

static LowStar_Vector_vector_str___uint8_t_
shrink___uint8_t_(LowStar_Vector_vector_str___uint8_t_ vec, uint32_t new_size)
{
  return
    ((LowStar_Vector_vector_str___uint8_t_){ .sz = new_size, .cap = vec.cap, .vs = vec.vs });
}

/* SNIPPET_END: shrink___uint8_t_ */

/* SNIPPET_START: shrink___uint8_t__uint32_t */

static LowStar_Vector_vector_str___uint8_t_
shrink___uint8_t__uint32_t(
  regional__uint32_t__uint8_t_ rg,
  LowStar_Vector_vector_str___uint8_t_ rv,
  uint32_t new_size
)
{
  uint32_t size = rv.sz;
  if (new_size >= size)
  {
    return rv;
  }
  free_elems_from___uint8_t__uint32_t(rg, rv, new_size);
  LowStar_Vector_vector_str___uint8_t_ frv = shrink___uint8_t_(rv, new_size);
  return frv;
}

/* SNIPPET_END: shrink___uint8_t__uint32_t */

/* SNIPPET_START: mt_retract_to_ */

static void
mt_retract_to_(
  uint32_t hsz,
  LowStar_Vector_vector_str__LowStar_Vector_vector_str___uint8_t_ hs,
  uint32_t lv,
  uint32_t i,
  uint32_t s,
  uint32_t j
)
{
  if (!(lv >= hs.sz))
  {
    LowStar_Vector_vector_str___uint8_t_ hvec = index__LowStar_Vector_vector_str__uint8_t_(hs, lv);
    uint32_t old_len = j - MerkleTree_Low_offset_of(i);
    uint32_t new_len = s - MerkleTree_Low_offset_of(i);
    LowStar_Vector_vector_str___uint8_t_
    retracted =
      shrink___uint8_t__uint32_t((
          (regional__uint32_t__uint8_t_){
            .state = hsz,
            .dummy = NULL,
            .r_alloc = hash_r_alloc,
            .r_free = hash_r_free
          }
        ),
        hvec,
        new_len);
    assign__LowStar_Vector_vector_str__uint8_t__uint32_t(hs, lv, retracted);
    if (lv + (uint32_t)1U < hs.sz)
    {
      mt_retract_to_(hsz,
        hs,
        lv + (uint32_t)1U,
        i / (uint32_t)2U,
        s / (uint32_t)2U,
        j / (uint32_t)2U);
      return;
    }
    return;
  }
}

/* SNIPPET_END: mt_retract_to_ */

/* SNIPPET_START: MerkleTree_Low_mt_retract_to_pre */

bool MerkleTree_Low_mt_retract_to_pre(const MerkleTree_Low_merkle_tree *mt, uint64_t r)
{
  MerkleTree_Low_merkle_tree *mt1 = (MerkleTree_Low_merkle_tree *)mt;
  MerkleTree_Low_merkle_tree mtv = *mt1;
  return
    r
    >= mtv.offset
    && r - mtv.offset <= MerkleTree_Low_offset_range_limit
    && mtv.i <= (uint32_t)(r - mtv.offset) && (uint32_t)(r - mtv.offset) < mtv.j;
}

/* SNIPPET_END: MerkleTree_Low_mt_retract_to_pre */

/* SNIPPET_START: MerkleTree_Low_mt_retract_to */

void MerkleTree_Low_mt_retract_to(MerkleTree_Low_merkle_tree *mt, uint64_t r)
{
  MerkleTree_Low_merkle_tree mtv = *mt;
  uint64_t offset = mtv.offset;
  uint32_t r1 = (uint32_t)(r - offset);
  LowStar_Vector_vector_str__LowStar_Vector_vector_str___uint8_t_ hs = mtv.hs;
  mt_retract_to_(mtv.hash_size, hs, (uint32_t)0U, mtv.i, r1 + (uint32_t)1U, mtv.j);
  *mt
  =
    (
      (MerkleTree_Low_merkle_tree){
        .hash_size = mtv.hash_size,
        .offset = mtv.offset,
        .i = mtv.i,
        .j = r1 + (uint32_t)1U,
        .hs = hs,
        .rhs_ok = false,
        .rhs = mtv.rhs,
        .mroot = mtv.mroot,
        .hash_fun = mtv.hash_fun
      }
    );
}

/* SNIPPET_END: MerkleTree_Low_mt_retract_to */

/* SNIPPET_START: mt_verify_ */

static void
mt_verify_(
  uint32_t hsz,
  uint32_t k,
  uint32_t j,
  const MerkleTree_Low_path *p,
  uint32_t ppos,
  uint8_t *acc,
  bool actd,
  void (*hash_fun)(uint8_t *x0, uint8_t *x1, uint8_t *x2)
)
{
  MerkleTree_Low_path *ncp = (MerkleTree_Low_path *)p;
  if (!(j == (uint32_t)0U))
  {
    bool nactd = actd || j % (uint32_t)2U == (uint32_t)1U;
    if (k % (uint32_t)2U == (uint32_t)0U)
    {
      if (j == k || (j == k + (uint32_t)1U && !actd))
      {
        mt_verify_(hsz, k / (uint32_t)2U, j / (uint32_t)2U, p, ppos, acc, nactd, hash_fun);
        return;
      }
      MerkleTree_Low_path ncpd = *ncp;
      uint8_t *phash = index___uint8_t_(ncpd.hashes, ppos);
      hash_fun(acc, phash, acc);
      mt_verify_(hsz,
        k / (uint32_t)2U,
        j / (uint32_t)2U,
        p,
        ppos + (uint32_t)1U,
        acc,
        nactd,
        hash_fun);
      return;
    }
    MerkleTree_Low_path ncpd = *ncp;
    uint8_t *phash = index___uint8_t_(ncpd.hashes, ppos);
    hash_fun(phash, acc, acc);
    mt_verify_(hsz,
      k / (uint32_t)2U,
      j / (uint32_t)2U,
      p,
      ppos + (uint32_t)1U,
      acc,
      nactd,
      hash_fun);
    return;
  }
}

/* SNIPPET_END: mt_verify_ */

/* SNIPPET_START: MerkleTree_Low_mt_verify_pre */

bool
MerkleTree_Low_mt_verify_pre(
  const MerkleTree_Low_merkle_tree *mt,
  uint64_t k,
  uint64_t j,
  const MerkleTree_Low_path *p,
  uint8_t *rt
)
{
  MerkleTree_Low_merkle_tree *mt1 = (MerkleTree_Low_merkle_tree *)mt;
  MerkleTree_Low_path *p1 = (MerkleTree_Low_path *)p;
  MerkleTree_Low_merkle_tree mtv = *mt1;
  MerkleTree_Low_path uu____0 = *p1;
  return
    k
    < j
    && k >= mtv.offset && k - mtv.offset <= MerkleTree_Low_offset_range_limit
    && j >= mtv.offset && j - mtv.offset <= MerkleTree_Low_offset_range_limit
    && mtv.hash_size == uu____0.hash_size
    &&
      uu____0.hashes.sz
      ==
        (uint32_t)1U
        +
          mt_path_length((uint32_t)0U,
            (uint32_t)(k - mtv.offset),
            (uint32_t)(j - mtv.offset),
            false);
}

/* SNIPPET_END: MerkleTree_Low_mt_verify_pre */

/* SNIPPET_START: MerkleTree_Low_mt_verify */

bool
MerkleTree_Low_mt_verify(
  const MerkleTree_Low_merkle_tree *mt,
  uint64_t k,
  uint64_t j,
  const MerkleTree_Low_path *p,
  uint8_t *rt
)
{
  MerkleTree_Low_merkle_tree *ncmt = (MerkleTree_Low_merkle_tree *)mt;
  MerkleTree_Low_path *ncp = (MerkleTree_Low_path *)p;
  MerkleTree_Low_merkle_tree mtv = *ncmt;
  uint32_t hash_size = mtv.hash_size;
  regional__uint32_t__uint8_t_
  hrg = { .state = hash_size, .dummy = NULL, .r_alloc = hash_r_alloc, .r_free = hash_r_free };
  uint32_t k1 = (uint32_t)(k - mtv.offset);
  uint32_t j1 = (uint32_t)(j - mtv.offset);
  uint8_t *ih = hrg.r_alloc(hrg.state);
  MerkleTree_Low_path pth = *ncp;
  uint8_t *first = index___uint8_t_(pth.hashes, (uint32_t)0U);
  hash_copy(hash_size, first, ih);
  mt_verify_(hash_size, k1, j1, p, (uint32_t)1U, ih, false, mtv.hash_fun);
  uint8_t res = (uint8_t)255U;
  for (uint32_t i = (uint32_t)0U; i < hash_size; i++)
  {
    uint8_t uu____0 = FStar_UInt8_eq_mask(ih[i], rt[i]);
    res = uu____0 & res;
  }
  uint8_t z = res;
  bool r = z == (uint8_t)255U;
  hrg.r_free(hrg.state, ih);
  return r;
}

/* SNIPPET_END: MerkleTree_Low_mt_verify */

/* SNIPPET_START: __bool_uint32_t */

typedef struct __bool_uint32_t_s
{
  bool fst;
  uint32_t snd;
}
__bool_uint32_t;

/* SNIPPET_END: __bool_uint32_t */

/* SNIPPET_START: serialize_bool */

static __bool_uint32_t serialize_bool(bool ok, bool x, uint8_t *buf, uint32_t sz, uint32_t pos)
{
  if (!ok || pos >= sz)
  {
    return ((__bool_uint32_t){ .fst = false, .snd = (uint32_t)0U });
  }
  uint8_t ite;
  if (x)
  {
    ite = (uint8_t)1U;
  }
  else
  {
    ite = (uint8_t)0U;
  }
  buf[pos] = ite;
  return ((__bool_uint32_t){ .fst = true, .snd = pos + (uint32_t)1U });
}

/* SNIPPET_END: serialize_bool */

/* SNIPPET_START: serialize_uint8_t */

static __bool_uint32_t
serialize_uint8_t(bool ok, uint8_t x, uint8_t *buf, uint32_t sz, uint32_t pos)
{
  if (!ok || pos >= sz)
  {
    return ((__bool_uint32_t){ .fst = false, .snd = (uint32_t)0U });
  }
  buf[pos] = x;
  return ((__bool_uint32_t){ .fst = true, .snd = pos + (uint32_t)1U });
}

/* SNIPPET_END: serialize_uint8_t */

/* SNIPPET_START: serialize_uint16_t */

static __bool_uint32_t
serialize_uint16_t(bool ok, uint16_t x, uint8_t *buf, uint32_t sz, uint32_t pos)
{
  __bool_uint32_t scrut = serialize_uint8_t(ok, (uint8_t)(x >> (uint32_t)8U), buf, sz, pos);
  bool ok1 = scrut.fst;
  uint32_t pos1 = scrut.snd;
  return serialize_uint8_t(ok1, (uint8_t)x, buf, sz, pos1);
}

/* SNIPPET_END: serialize_uint16_t */

/* SNIPPET_START: serialize_uint32_t */

static __bool_uint32_t
serialize_uint32_t(bool ok, uint32_t x, uint8_t *buf, uint32_t sz, uint32_t pos)
{
  __bool_uint32_t scrut = serialize_uint16_t(ok, (uint16_t)(x >> (uint32_t)16U), buf, sz, pos);
  bool ok1 = scrut.fst;
  uint32_t pos1 = scrut.snd;
  return serialize_uint16_t(ok1, (uint16_t)x, buf, sz, pos1);
}

/* SNIPPET_END: serialize_uint32_t */

/* SNIPPET_START: serialize_uint64_t */

static __bool_uint32_t
serialize_uint64_t(bool ok, uint64_t x, uint8_t *buf, uint32_t sz, uint32_t pos)
{
  __bool_uint32_t scrut = serialize_uint32_t(ok, (uint32_t)(x >> (uint32_t)32U), buf, sz, pos);
  bool ok1 = scrut.fst;
  uint32_t pos1 = scrut.snd;
  return serialize_uint32_t(ok1, (uint32_t)x, buf, sz, pos1);
}

/* SNIPPET_END: serialize_uint64_t */

/* SNIPPET_START: serialize_offset_t */

static __bool_uint32_t
(*serialize_offset_t)(bool x0, uint64_t x1, uint8_t *x2, uint32_t x3, uint32_t x4) =
  serialize_uint64_t;

/* SNIPPET_END: serialize_offset_t */

/* SNIPPET_START: serialize_hash_i */

static __bool_uint32_t
serialize_hash_i(
  uint32_t hash_size,
  bool ok,
  uint8_t *x,
  uint8_t *buf,
  uint32_t sz,
  uint32_t pos,
  uint32_t i
)
{
  if (!ok || pos >= sz)
  {
    return ((__bool_uint32_t){ .fst = false, .snd = (uint32_t)0U });
  }
  uint8_t b = x[i];
  __bool_uint32_t scrut = serialize_uint8_t(ok, b, buf, sz, pos);
  bool ok1 = scrut.fst;
  uint32_t pos1 = scrut.snd;
  uint32_t j = i + (uint32_t)1U;
  if (j < hash_size)
  {
    return serialize_hash_i(hash_size, ok1, x, buf, sz, pos1, j);
  }
  return ((__bool_uint32_t){ .fst = ok1, .snd = pos1 });
}

/* SNIPPET_END: serialize_hash_i */

/* SNIPPET_START: serialize_hash */

static __bool_uint32_t
serialize_hash(
  uint32_t hash_size,
  bool ok,
  uint8_t *x,
  uint8_t *buf,
  uint32_t sz,
  uint32_t pos
)
{
  if (!ok || pos >= sz)
  {
    return ((__bool_uint32_t){ .fst = false, .snd = (uint32_t)0U });
  }
  return serialize_hash_i(hash_size, ok, x, buf, sz, pos, (uint32_t)0U);
}

/* SNIPPET_END: serialize_hash */

/* SNIPPET_START: serialize_hash_vec_i */

static __bool_uint32_t
serialize_hash_vec_i(
  uint32_t hash_size,
  bool ok,
  LowStar_Vector_vector_str___uint8_t_ x,
  uint8_t *buf,
  uint32_t sz,
  uint32_t pos,
  uint32_t i
)
{
  if (!ok || pos >= sz)
  {
    return ((__bool_uint32_t){ .fst = false, .snd = (uint32_t)0U });
  }
  uint8_t *vi = index___uint8_t_(x, i);
  __bool_uint32_t scrut = serialize_hash(hash_size, ok, vi, buf, sz, pos);
  bool ok1 = scrut.fst;
  uint32_t pos1 = scrut.snd;
  uint32_t j = i + (uint32_t)1U;
  if (j < x.sz)
  {
    return serialize_hash_vec_i(hash_size, ok1, x, buf, sz, pos1, j);
  }
  return ((__bool_uint32_t){ .fst = ok1, .snd = pos1 });
}

/* SNIPPET_END: serialize_hash_vec_i */

/* SNIPPET_START: serialize_hash_vec */

static __bool_uint32_t
serialize_hash_vec(
  uint32_t hash_size,
  bool ok,
  LowStar_Vector_vector_str___uint8_t_ x,
  uint8_t *buf,
  uint32_t sz,
  uint32_t pos
)
{
  if (!ok || pos >= sz)
  {
    return ((__bool_uint32_t){ .fst = false, .snd = (uint32_t)0U });
  }
  __bool_uint32_t scrut = serialize_uint32_t(ok, x.sz, buf, sz, pos);
  bool ok1 = scrut.fst;
  uint32_t pos1 = scrut.snd;
  if (ok1 && x.sz > (uint32_t)0U)
  {
    return serialize_hash_vec_i(hash_size, ok1, x, buf, sz, pos1, (uint32_t)0U);
  }
  return ((__bool_uint32_t){ .fst = ok1, .snd = pos1 });
}

/* SNIPPET_END: serialize_hash_vec */

/* SNIPPET_START: hash_vv_bytes_i */

static uint64_t
hash_vv_bytes_i(
  uint32_t hash_size,
  LowStar_Vector_vector_str__LowStar_Vector_vector_str___uint8_t_ vv,
  uint32_t i
)
{
  if (i >= vv.sz)
  {
    return (uint64_t)4U;
  }
  LowStar_Vector_vector_str___uint8_t_ vvi = index__LowStar_Vector_vector_str__uint8_t_(vv, i);
  uint64_t vs_hs = (uint64_t)vvi.sz * (uint64_t)hash_size;
  uint64_t r;
  if (MerkleTree_Low_uint64_max - vs_hs >= (uint64_t)4U)
  {
    r = vs_hs + (uint64_t)4U;
  }
  else
  {
    r = MerkleTree_Low_uint64_max;
  }
  uint64_t rest = hash_vv_bytes_i(hash_size, vv, i + (uint32_t)1U);
  if (MerkleTree_Low_uint64_max - r >= rest)
  {
    return r + rest;
  }
  return MerkleTree_Low_uint64_max;
}

/* SNIPPET_END: hash_vv_bytes_i */

/* SNIPPET_START: serialize_hash_vv_i */

static __bool_uint32_t
serialize_hash_vv_i(
  uint32_t hash_size,
  bool ok,
  LowStar_Vector_vector_str__LowStar_Vector_vector_str___uint8_t_ x,
  uint8_t *buf,
  uint32_t sz,
  uint32_t pos,
  uint32_t i
)
{
  if (!ok || pos >= sz)
  {
    return ((__bool_uint32_t){ .fst = false, .snd = (uint32_t)0U });
  }
  LowStar_Vector_vector_str___uint8_t_ vi = index__LowStar_Vector_vector_str__uint8_t_(x, i);
  __bool_uint32_t scrut = serialize_hash_vec(hash_size, ok, vi, buf, sz, pos);
  bool ok1 = scrut.fst;
  uint32_t pos1 = scrut.snd;
  uint32_t j = i + (uint32_t)1U;
  if (j < x.sz)
  {
    return serialize_hash_vv_i(hash_size, ok1, x, buf, sz, pos1, j);
  }
  return ((__bool_uint32_t){ .fst = ok1, .snd = pos1 });
}

/* SNIPPET_END: serialize_hash_vv_i */

/* SNIPPET_START: serialize_hash_vv */

static __bool_uint32_t
serialize_hash_vv(
  uint32_t hash_size,
  bool ok,
  LowStar_Vector_vector_str__LowStar_Vector_vector_str___uint8_t_ x,
  uint8_t *buf,
  uint32_t sz,
  uint32_t pos
)
{
  if (!ok || pos >= sz)
  {
    return ((__bool_uint32_t){ .fst = false, .snd = (uint32_t)0U });
  }
  __bool_uint32_t scrut = serialize_uint32_t(ok, x.sz, buf, sz, pos);
  bool ok1 = scrut.fst;
  uint32_t pos1 = scrut.snd;
  if (x.sz > (uint32_t)0U)
  {
    return serialize_hash_vv_i(hash_size, ok1, x, buf, sz, pos1, (uint32_t)0U);
  }
  return ((__bool_uint32_t){ .fst = ok1, .snd = pos1 });
}

/* SNIPPET_END: serialize_hash_vv */

/* SNIPPET_START: __bool_uint32_t_bool */

typedef struct __bool_uint32_t_bool_s
{
  bool fst;
  uint32_t snd;
  bool thd;
}
__bool_uint32_t_bool;

/* SNIPPET_END: __bool_uint32_t_bool */

/* SNIPPET_START: deserialize_bool */

static __bool_uint32_t_bool
deserialize_bool(bool ok, const uint8_t *buf, uint32_t sz, uint32_t pos)
{
  if (!ok || pos >= sz)
  {
    return ((__bool_uint32_t_bool){ .fst = false, .snd = pos, .thd = false });
  }
  bool sw;
  switch (buf[pos])
  {
    case 0U:
      {
        sw = false;
        break;
      }
    default:
      {
        sw = true;
      }
  }
  return ((__bool_uint32_t_bool){ .fst = true, .snd = pos + (uint32_t)1U, .thd = sw });
}

/* SNIPPET_END: deserialize_bool */

/* SNIPPET_START: __bool_uint32_t_uint8_t */

typedef struct __bool_uint32_t_uint8_t_s
{
  bool fst;
  uint32_t snd;
  uint8_t thd;
}
__bool_uint32_t_uint8_t;

/* SNIPPET_END: __bool_uint32_t_uint8_t */

/* SNIPPET_START: deserialize_uint8_t */

static __bool_uint32_t_uint8_t
deserialize_uint8_t(bool ok, const uint8_t *buf, uint32_t sz, uint32_t pos)
{
  if (!ok || pos >= sz)
  {
    return ((__bool_uint32_t_uint8_t){ .fst = false, .snd = pos, .thd = (uint8_t)0U });
  }
  return ((__bool_uint32_t_uint8_t){ .fst = true, .snd = pos + (uint32_t)1U, .thd = buf[pos] });
}

/* SNIPPET_END: deserialize_uint8_t */

/* SNIPPET_START: __bool_uint32_t_uint16_t */

typedef struct __bool_uint32_t_uint16_t_s
{
  bool fst;
  uint32_t snd;
  uint16_t thd;
}
__bool_uint32_t_uint16_t;

/* SNIPPET_END: __bool_uint32_t_uint16_t */

/* SNIPPET_START: deserialize_uint16_t */

static __bool_uint32_t_uint16_t
deserialize_uint16_t(bool ok, const uint8_t *buf, uint32_t sz, uint32_t pos)
{
  if (!ok || pos >= sz)
  {
    return ((__bool_uint32_t_uint16_t){ .fst = false, .snd = pos, .thd = (uint16_t)0U });
  }
  __bool_uint32_t_uint8_t scrut0 = deserialize_uint8_t(ok, buf, sz, pos);
  bool ok1 = scrut0.fst;
  uint32_t pos1 = scrut0.snd;
  uint8_t b0 = scrut0.thd;
  __bool_uint32_t_uint8_t scrut = deserialize_uint8_t(ok1, buf, sz, pos1);
  bool ok2 = scrut.fst;
  uint32_t pos2 = scrut.snd;
  uint8_t b1 = scrut.thd;
  return
    (
      (__bool_uint32_t_uint16_t){
        .fst = ok2,
        .snd = pos2,
        .thd = ((uint16_t)b0 << (uint32_t)8U) + (uint16_t)b1
      }
    );
}

/* SNIPPET_END: deserialize_uint16_t */

/* SNIPPET_START: __bool_uint32_t_uint32_t */

typedef struct __bool_uint32_t_uint32_t_s
{
  bool fst;
  uint32_t snd;
  uint32_t thd;
}
__bool_uint32_t_uint32_t;

/* SNIPPET_END: __bool_uint32_t_uint32_t */

/* SNIPPET_START: deserialize_uint32_t */

static __bool_uint32_t_uint32_t
deserialize_uint32_t(bool ok, const uint8_t *buf, uint32_t sz, uint32_t pos)
{
  if (!ok || pos >= sz)
  {
    return ((__bool_uint32_t_uint32_t){ .fst = false, .snd = pos, .thd = (uint32_t)0U });
  }
  __bool_uint32_t_uint16_t scrut0 = deserialize_uint16_t(ok, buf, sz, pos);
  bool ok1 = scrut0.fst;
  uint32_t pos1 = scrut0.snd;
  uint16_t b0 = scrut0.thd;
  __bool_uint32_t_uint16_t scrut = deserialize_uint16_t(ok1, buf, sz, pos1);
  bool ok2 = scrut.fst;
  uint32_t pos2 = scrut.snd;
  uint16_t b1 = scrut.thd;
  return
    (
      (__bool_uint32_t_uint32_t){
        .fst = ok2,
        .snd = pos2,
        .thd = ((uint32_t)b0 << (uint32_t)16U) + (uint32_t)b1
      }
    );
}

/* SNIPPET_END: deserialize_uint32_t */

/* SNIPPET_START: __bool_uint32_t_uint64_t */

typedef struct __bool_uint32_t_uint64_t_s
{
  bool fst;
  uint32_t snd;
  uint64_t thd;
}
__bool_uint32_t_uint64_t;

/* SNIPPET_END: __bool_uint32_t_uint64_t */

/* SNIPPET_START: deserialize_uint64_t */

static __bool_uint32_t_uint64_t
deserialize_uint64_t(bool ok, const uint8_t *buf, uint32_t sz, uint32_t pos)
{
  if (!ok || pos >= sz)
  {
    return ((__bool_uint32_t_uint64_t){ .fst = false, .snd = pos, .thd = (uint64_t)0U });
  }
  __bool_uint32_t_uint32_t scrut0 = deserialize_uint32_t(ok, buf, sz, pos);
  bool ok1 = scrut0.fst;
  uint32_t pos1 = scrut0.snd;
  uint32_t b0 = scrut0.thd;
  __bool_uint32_t_uint32_t scrut = deserialize_uint32_t(ok1, buf, sz, pos1);
  bool ok2 = scrut.fst;
  uint32_t pos2 = scrut.snd;
  uint32_t b1 = scrut.thd;
  return
    (
      (__bool_uint32_t_uint64_t){
        .fst = ok2,
        .snd = pos2,
        .thd = ((uint64_t)b0 << (uint32_t)32U) + (uint64_t)b1
      }
    );
}

/* SNIPPET_END: deserialize_uint64_t */

/* SNIPPET_START: deserialize_offset_t */

static __bool_uint32_t_uint64_t
(*deserialize_offset_t)(bool x0, const uint8_t *x1, uint32_t x2, uint32_t x3) =
  deserialize_uint64_t;

/* SNIPPET_END: deserialize_offset_t */

/* SNIPPET_START: deserialize_index_t */

static __bool_uint32_t_uint32_t
(*deserialize_index_t)(bool x0, const uint8_t *x1, uint32_t x2, uint32_t x3) =
  deserialize_uint32_t;

/* SNIPPET_END: deserialize_index_t */

/* SNIPPET_START: __bool_uint32_t__uint8_t_ */

typedef struct __bool_uint32_t__uint8_t__s
{
  bool fst;
  uint32_t snd;
  uint8_t *thd;
}
__bool_uint32_t__uint8_t_;

/* SNIPPET_END: __bool_uint32_t__uint8_t_ */

/* SNIPPET_START: deserialize_hash */

static __bool_uint32_t__uint8_t_
deserialize_hash(uint32_t hash_size, bool ok, const uint8_t *buf, uint32_t sz, uint32_t pos)
{
  regional__uint32_t__uint8_t_
  rg = { .state = hash_size, .dummy = NULL, .r_alloc = hash_r_alloc, .r_free = hash_r_free };
  if (!ok || pos >= sz)
  {
    return ((__bool_uint32_t__uint8_t_){ .fst = false, .snd = pos, .thd = rg.dummy });
  }
  if (sz - pos < hash_size)
  {
    return ((__bool_uint32_t__uint8_t_){ .fst = false, .snd = pos, .thd = rg.dummy });
  }
  uint8_t *hash = rg.r_alloc(rg.state);
  memcpy(hash, (uint8_t *)buf + pos, hash_size * sizeof (uint8_t));
  return ((__bool_uint32_t__uint8_t_){ .fst = true, .snd = pos + hash_size, .thd = hash });
}

/* SNIPPET_END: deserialize_hash */

/* SNIPPET_START: deserialize_hash_vec_i */

static __bool_uint32_t
deserialize_hash_vec_i(
  uint32_t hash_size,
  bool ok,
  const uint8_t *buf,
  uint32_t sz,
  uint32_t pos,
  LowStar_Vector_vector_str___uint8_t_ res,
  uint32_t i
)
{
  if (!ok || pos >= sz)
  {
    return ((__bool_uint32_t){ .fst = false, .snd = pos });
  }
  __bool_uint32_t__uint8_t_ scrut = deserialize_hash(hash_size, ok, buf, sz, pos);
  bool ok1 = scrut.fst;
  uint32_t pos1 = scrut.snd;
  uint8_t *h = scrut.thd;
  if (!ok1)
  {
    return ((__bool_uint32_t){ .fst = false, .snd = pos1 });
  }
  assign___uint8_t_(res, i, h);
  uint32_t j = i + (uint32_t)1U;
  if (j < res.sz)
  {
    return deserialize_hash_vec_i(hash_size, ok1, buf, sz, pos1, res, j);
  }
  return ((__bool_uint32_t){ .fst = true, .snd = pos1 });
}

/* SNIPPET_END: deserialize_hash_vec_i */

/* SNIPPET_START: alloc___uint8_t_ */

static LowStar_Vector_vector_str___uint8_t_ alloc___uint8_t_(uint32_t len, uint8_t *v)
{
  return alloc_rid___uint8_t_(len, v);
}

/* SNIPPET_END: alloc___uint8_t_ */

/* SNIPPET_START: __bool_uint32_t_LowStar_Vector_vector_str___uint8_t_ */

typedef struct __bool_uint32_t_LowStar_Vector_vector_str___uint8_t__s
{
  bool fst;
  uint32_t snd;
  LowStar_Vector_vector_str___uint8_t_ thd;
}
__bool_uint32_t_LowStar_Vector_vector_str___uint8_t_;

/* SNIPPET_END: __bool_uint32_t_LowStar_Vector_vector_str___uint8_t_ */

/* SNIPPET_START: deserialize_hash_vec */

static __bool_uint32_t_LowStar_Vector_vector_str___uint8_t_
deserialize_hash_vec(
  uint32_t hash_size,
  bool ok,
  const uint8_t *buf,
  uint32_t sz,
  uint32_t pos
)
{
  regional__uint32_t__uint8_t_
  hrg = { .state = hash_size, .dummy = NULL, .r_alloc = hash_r_alloc, .r_free = hash_r_free };
  regional__uint32_t_LowStar_Vector_vector_str___uint8_t_
  hvrg =
    {
      .state = hash_size,
      .dummy = { .sz = (uint32_t)0U, .cap = (uint32_t)0U, .vs = NULL },
      .r_alloc = hash_vec_r_alloc,
      .r_free = hash_vec_r_free
    };
  if (!ok || pos >= sz)
  {
    return
      (
        (__bool_uint32_t_LowStar_Vector_vector_str___uint8_t_){
          .fst = false,
          .snd = pos,
          .thd = hvrg.dummy
        }
      );
  }
  LowStar_Vector_vector_str___uint8_t_
  empty_vec = alloc_reserve___uint8_t_((uint32_t)1U, hrg.dummy);
  __bool_uint32_t_uint32_t scrut0 = deserialize_uint32_t(ok, buf, sz, pos);
  bool ok1 = scrut0.fst;
  uint32_t pos1 = scrut0.snd;
  uint32_t n = scrut0.thd;
  if (!ok1)
  {
    return
      (
        (__bool_uint32_t_LowStar_Vector_vector_str___uint8_t_){
          .fst = false,
          .snd = pos1,
          .thd = empty_vec
        }
      );
  }
  if (n == (uint32_t)0U)
  {
    return
      (
        (__bool_uint32_t_LowStar_Vector_vector_str___uint8_t_){
          .fst = true,
          .snd = pos1,
          .thd = empty_vec
        }
      );
  }
  LowStar_Vector_vector_str___uint8_t_ res = alloc___uint8_t_(n, hrg.dummy);
  __bool_uint32_t
  scrut = deserialize_hash_vec_i(hash_size, ok1, buf, sz, pos1, res, (uint32_t)0U);
  bool ok2 = scrut.fst;
  uint32_t pos2 = scrut.snd;
  return
    ((__bool_uint32_t_LowStar_Vector_vector_str___uint8_t_){ .fst = ok2, .snd = pos2, .thd = res });
}

/* SNIPPET_END: deserialize_hash_vec */

/* SNIPPET_START: deserialize_hash_vv_i */

static __bool_uint32_t
deserialize_hash_vv_i(
  uint32_t hash_size,
  bool ok,
  const uint8_t *buf,
  uint32_t sz,
  uint32_t pos,
  LowStar_Vector_vector_str__LowStar_Vector_vector_str___uint8_t_ res,
  uint32_t i
)
{
  if (!ok || pos >= sz)
  {
    return ((__bool_uint32_t){ .fst = false, .snd = (uint32_t)0U });
  }
  __bool_uint32_t_LowStar_Vector_vector_str___uint8_t_
  scrut = deserialize_hash_vec(hash_size, ok, buf, sz, pos);
  bool ok1 = scrut.fst;
  uint32_t pos1 = scrut.snd;
  LowStar_Vector_vector_str___uint8_t_ hv = scrut.thd;
  if (!ok1)
  {
    return ((__bool_uint32_t){ .fst = false, .snd = pos1 });
  }
  assign__LowStar_Vector_vector_str__uint8_t_(res, i, hv);
  uint32_t j = i + (uint32_t)1U;
  if (j == res.sz)
  {
    return ((__bool_uint32_t){ .fst = true, .snd = pos1 });
  }
  return deserialize_hash_vv_i(hash_size, ok1, buf, sz, pos1, res, j);
}

/* SNIPPET_END: deserialize_hash_vv_i */

/* SNIPPET_START: alloc_reserve__LowStar_Vector_vector_str__uint8_t_ */

static LowStar_Vector_vector_str__LowStar_Vector_vector_str___uint8_t_
alloc_reserve__LowStar_Vector_vector_str__uint8_t_(
  uint32_t len,
  LowStar_Vector_vector_str___uint8_t_ ia
)
{
  KRML_CHECK_SIZE(sizeof (LowStar_Vector_vector_str___uint8_t_), len);
  LowStar_Vector_vector_str___uint8_t_
  *buf = KRML_HOST_MALLOC(sizeof (LowStar_Vector_vector_str___uint8_t_) * len);
  for (uint32_t _i = 0U; _i < len; ++_i)
    buf[_i] = ia;
  return
    (
      (LowStar_Vector_vector_str__LowStar_Vector_vector_str___uint8_t_){
        .sz = (uint32_t)0U,
        .cap = len,
        .vs = buf
      }
    );
}

/* SNIPPET_END: alloc_reserve__LowStar_Vector_vector_str__uint8_t_ */

/* SNIPPET_START: alloc__LowStar_Vector_vector_str__uint8_t_ */

static LowStar_Vector_vector_str__LowStar_Vector_vector_str___uint8_t_
alloc__LowStar_Vector_vector_str__uint8_t_(
  uint32_t len,
  LowStar_Vector_vector_str___uint8_t_ v
)
{
  return alloc_rid__LowStar_Vector_vector_str__uint8_t_(len, v);
}

/* SNIPPET_END: alloc__LowStar_Vector_vector_str__uint8_t_ */

/* SNIPPET_START: __bool_uint32_t_LowStar_Vector_vector_str__LowStar_Vector_vector_str___uint8_t_ */

typedef struct
__bool_uint32_t_LowStar_Vector_vector_str__LowStar_Vector_vector_str___uint8_t__s
{
  bool fst;
  uint32_t snd;
  LowStar_Vector_vector_str__LowStar_Vector_vector_str___uint8_t_ thd;
}
__bool_uint32_t_LowStar_Vector_vector_str__LowStar_Vector_vector_str___uint8_t_;

/* SNIPPET_END: __bool_uint32_t_LowStar_Vector_vector_str__LowStar_Vector_vector_str___uint8_t_ */

/* SNIPPET_START: deserialize_hash_vv */

static __bool_uint32_t_LowStar_Vector_vector_str__LowStar_Vector_vector_str___uint8_t_
deserialize_hash_vv(uint32_t hash_size, bool ok, const uint8_t *buf, uint32_t sz, uint32_t pos)
{
  regional__uint32_t_LowStar_Vector_vector_str___uint8_t_
  rg =
    {
      .state = hash_size,
      .dummy = { .sz = (uint32_t)0U, .cap = (uint32_t)0U, .vs = NULL },
      .r_alloc = hash_vec_r_alloc,
      .r_free = hash_vec_r_free
    };
  LowStar_Vector_vector_str__LowStar_Vector_vector_str___uint8_t_
  empty_vec = alloc_reserve__LowStar_Vector_vector_str__uint8_t_((uint32_t)1U, rg.dummy);
  if (!ok || pos >= sz)
  {
    return
      (
        (__bool_uint32_t_LowStar_Vector_vector_str__LowStar_Vector_vector_str___uint8_t_){
          .fst = false,
          .snd = pos,
          .thd = empty_vec
        }
      );
  }
  __bool_uint32_t_uint32_t scrut0 = deserialize_uint32_t(ok, buf, sz, pos);
  bool ok1 = scrut0.fst;
  uint32_t pos1 = scrut0.snd;
  uint32_t n = scrut0.thd;
  if (!ok1)
  {
    return
      (
        (__bool_uint32_t_LowStar_Vector_vector_str__LowStar_Vector_vector_str___uint8_t_){
          .fst = false,
          .snd = pos1,
          .thd = empty_vec
        }
      );
  }
  if (n == (uint32_t)0U)
  {
    return
      (
        (__bool_uint32_t_LowStar_Vector_vector_str__LowStar_Vector_vector_str___uint8_t_){
          .fst = true,
          .snd = pos1,
          .thd = empty_vec
        }
      );
  }
  LowStar_Vector_vector_str__LowStar_Vector_vector_str___uint8_t_
  res = alloc__LowStar_Vector_vector_str__uint8_t_(n, rg.dummy);
  __bool_uint32_t
  scrut = deserialize_hash_vv_i(hash_size, ok1, buf, sz, pos1, res, (uint32_t)0U);
  bool ok2 = scrut.fst;
  uint32_t pos2 = scrut.snd;
  return
    (
      (__bool_uint32_t_LowStar_Vector_vector_str__LowStar_Vector_vector_str___uint8_t_){
        .fst = ok2,
        .snd = pos2,
        .thd = res
      }
    );
}

/* SNIPPET_END: deserialize_hash_vv */

/* SNIPPET_START: MerkleTree_Low_Serialization_mt_serialize_size */

uint64_t MerkleTree_Low_Serialization_mt_serialize_size(const MerkleTree_Low_merkle_tree *mt)
{
  MerkleTree_Low_merkle_tree mtv = *(MerkleTree_Low_merkle_tree *)mt;
  LowStar_Vector_vector_str__LowStar_Vector_vector_str___uint8_t_ hs = mtv.hs;
  LowStar_Vector_vector_str___uint8_t_ rhs = mtv.rhs;
  uint64_t hs_sz = hash_vv_bytes_i(mtv.hash_size, hs, (uint32_t)0U);
  if (hs_sz < (uint64_t)4294967295U)
  {
    uint64_t vs_hs = (uint64_t)rhs.sz * (uint64_t)mtv.hash_size;
    uint64_t ite;
    if (MerkleTree_Low_uint64_max - vs_hs >= (uint64_t)4U)
    {
      ite = vs_hs + (uint64_t)4U;
    }
    else
    {
      ite = MerkleTree_Low_uint64_max;
    }
    return (uint64_t)21U + hs_sz + (uint64_t)1U + ite + (uint64_t)mtv.hash_size;
  }
  return MerkleTree_Low_uint64_max;
}

/* SNIPPET_END: MerkleTree_Low_Serialization_mt_serialize_size */

/* SNIPPET_START: MerkleTree_Low_Serialization_mt_serialize */

uint64_t
MerkleTree_Low_Serialization_mt_serialize(
  const MerkleTree_Low_merkle_tree *mt,
  uint8_t *output,
  uint64_t sz
)
{
  MerkleTree_Low_merkle_tree *mt1 = (MerkleTree_Low_merkle_tree *)mt;
  uint32_t sz1 = (uint32_t)sz;
  MerkleTree_Low_merkle_tree mtv = *mt1;
  __bool_uint32_t scrut = serialize_uint8_t(true, (uint8_t)1U, output, sz1, (uint32_t)0U);
  bool ok = scrut.fst;
  uint32_t pos = scrut.snd;
  __bool_uint32_t scrut0 = serialize_uint32_t(ok, mtv.hash_size, output, sz1, pos);
  bool ok1 = scrut0.fst;
  uint32_t pos1 = scrut0.snd;
  __bool_uint32_t scrut1 = serialize_offset_t(ok1, mtv.offset, output, sz1, pos1);
  bool ok2 = scrut1.fst;
  uint32_t pos2 = scrut1.snd;
  __bool_uint32_t scrut2 = serialize_uint32_t(ok2, mtv.i, output, sz1, pos2);
  bool ok3 = scrut2.fst;
  uint32_t pos3 = scrut2.snd;
  __bool_uint32_t scrut3 = serialize_uint32_t(ok3, mtv.j, output, sz1, pos3);
  bool ok4 = scrut3.fst;
  uint32_t pos4 = scrut3.snd;
  __bool_uint32_t scrut4 = serialize_hash_vv(mtv.hash_size, ok4, mtv.hs, output, sz1, pos4);
  bool ok5 = scrut4.fst;
  uint32_t pos5 = scrut4.snd;
  __bool_uint32_t scrut5 = serialize_bool(ok5, mtv.rhs_ok, output, sz1, pos5);
  bool ok6 = scrut5.fst;
  uint32_t pos6 = scrut5.snd;
  __bool_uint32_t scrut6 = serialize_hash_vec(mtv.hash_size, ok6, mtv.rhs, output, sz1, pos6);
  bool ok7 = scrut6.fst;
  uint32_t pos7 = scrut6.snd;
  __bool_uint32_t scrut7 = serialize_hash(mtv.hash_size, ok7, mtv.mroot, output, sz1, pos7);
  bool ok8 = scrut7.fst;
  uint32_t pos8 = scrut7.snd;
  if (ok8)
  {
    return (uint64_t)pos8;
  }
  return (uint64_t)0U;
}

/* SNIPPET_END: MerkleTree_Low_Serialization_mt_serialize */

/* SNIPPET_START: MerkleTree_Low_Serialization_mt_deserialize */

MerkleTree_Low_merkle_tree
*MerkleTree_Low_Serialization_mt_deserialize(
  const uint8_t *input,
  uint64_t sz,
  void (*hash_fun)(uint8_t *x0, uint8_t *x1, uint8_t *x2)
)
{
  uint32_t sz1 = (uint32_t)sz;
  __bool_uint32_t_uint8_t scrut0 = deserialize_uint8_t(true, input, sz1, (uint32_t)0U);
  bool ok = scrut0.fst;
  uint32_t pos = scrut0.snd;
  uint8_t format_version = scrut0.thd;
  bool ok1 = ok && format_version == (uint8_t)1U;
  __bool_uint32_t_uint32_t scrut1 = deserialize_uint32_t(ok1, input, sz1, pos);
  bool ok2 = scrut1.fst;
  uint32_t pos1 = scrut1.snd;
  uint32_t hsz = scrut1.thd;
  if (hsz == (uint32_t)0U)
  {
    return NULL;
  }
  __bool_uint32_t_uint64_t scrut2 = deserialize_offset_t(ok2, input, sz1, pos1);
  bool ok3 = scrut2.fst;
  uint32_t pos2 = scrut2.snd;
  uint64_t offset = scrut2.thd;
  __bool_uint32_t_uint32_t scrut3 = deserialize_index_t(ok3, input, sz1, pos2);
  bool ok4 = scrut3.fst;
  uint32_t pos3 = scrut3.snd;
  uint32_t i = scrut3.thd;
  __bool_uint32_t_uint32_t scrut4 = deserialize_index_t(ok4, input, sz1, pos3);
  bool ok5 = scrut4.fst;
  uint32_t pos4 = scrut4.snd;
  uint32_t j = scrut4.thd;
  __bool_uint32_t_LowStar_Vector_vector_str__LowStar_Vector_vector_str___uint8_t_
  scrut5 = deserialize_hash_vv(hsz, ok5, input, sz1, pos4);
  bool ok6 = scrut5.fst;
  uint32_t pos5 = scrut5.snd;
  LowStar_Vector_vector_str__LowStar_Vector_vector_str___uint8_t_ hs = scrut5.thd;
  __bool_uint32_t_bool scrut6 = deserialize_bool(ok6, input, sz1, pos5);
  bool ok7 = scrut6.fst;
  uint32_t pos6 = scrut6.snd;
  bool rhs_ok = scrut6.thd;
  __bool_uint32_t_LowStar_Vector_vector_str___uint8_t_
  scrut7 = deserialize_hash_vec(hsz, ok7, input, sz1, pos6);
  bool ok8 = scrut7.fst;
  uint32_t pos7 = scrut7.snd;
  LowStar_Vector_vector_str___uint8_t_ rhs = scrut7.thd;
  __bool_uint32_t__uint8_t_ scrut = deserialize_hash(hsz, ok8, input, sz1, pos7);
  bool ok9 = scrut.fst;
  uint8_t *mroot = scrut.thd;
  if
  (
    !ok9
    ||
      !(j
      >= i
      && MerkleTree_Low_uint64_max - offset >= (uint64_t)j
      && hs.sz == (uint32_t)32U
      && rhs.sz == (uint32_t)32U)
  )
  {
    return NULL;
  }
  KRML_CHECK_SIZE(sizeof (MerkleTree_Low_merkle_tree), (uint32_t)1U);
  MerkleTree_Low_merkle_tree *buf = KRML_HOST_MALLOC(sizeof (MerkleTree_Low_merkle_tree));
  buf[0U]
  =
    (
      (MerkleTree_Low_merkle_tree){
        .hash_size = hsz,
        .offset = offset,
        .i = i,
        .j = j,
        .hs = hs,
        .rhs_ok = rhs_ok,
        .rhs = rhs,
        .mroot = mroot,
        .hash_fun = hash_fun
      }
    );
  return buf;
}

/* SNIPPET_END: MerkleTree_Low_Serialization_mt_deserialize */

/* SNIPPET_START: MerkleTree_Low_Serialization_mt_serialize_path */

uint64_t
MerkleTree_Low_Serialization_mt_serialize_path(
  const MerkleTree_Low_path *p,
  uint8_t *output,
  uint64_t sz
)
{
  MerkleTree_Low_path x0 = *(MerkleTree_Low_path *)p;
  uint32_t hsz1 = x0.hash_size;
  uint32_t sz1 = (uint32_t)sz;
  MerkleTree_Low_path *ncp = (MerkleTree_Low_path *)p;
  __bool_uint32_t scrut = serialize_uint32_t(true, hsz1, output, sz1, (uint32_t)0U);
  bool ok = scrut.fst;
  uint32_t pos = scrut.snd;
  MerkleTree_Low_path ncpd = *ncp;
  __bool_uint32_t scrut0 = serialize_hash_vec(hsz1, ok, ncpd.hashes, output, sz1, pos);
  bool ok1 = scrut0.fst;
  uint32_t pos1 = scrut0.snd;
  if (ok1)
  {
    return (uint64_t)pos1;
  }
  return (uint64_t)0U;
}

/* SNIPPET_END: MerkleTree_Low_Serialization_mt_serialize_path */

/* SNIPPET_START: MerkleTree_Low_Serialization_mt_deserialize_path */

MerkleTree_Low_path
*MerkleTree_Low_Serialization_mt_deserialize_path(const uint8_t *input, uint64_t sz)
{
  uint32_t sz1 = (uint32_t)sz;
  __bool_uint32_t_uint32_t scrut0 = deserialize_uint32_t(true, input, sz1, (uint32_t)0U);
  bool ok = scrut0.fst;
  uint32_t pos = scrut0.snd;
  uint32_t hash_size = scrut0.thd;
  if (!ok || hash_size == (uint32_t)0U)
  {
    return NULL;
  }
  __bool_uint32_t_LowStar_Vector_vector_str___uint8_t_
  scrut = deserialize_hash_vec(hash_size, ok, input, sz1, pos);
  bool ok1 = scrut.fst;
  LowStar_Vector_vector_str___uint8_t_ hs = scrut.thd;
  if (!ok1)
  {
    return NULL;
  }
  KRML_CHECK_SIZE(sizeof (MerkleTree_Low_path), (uint32_t)1U);
  MerkleTree_Low_path *buf = KRML_HOST_MALLOC(sizeof (MerkleTree_Low_path));
  buf[0U] = ((MerkleTree_Low_path){ .hash_size = hash_size, .hashes = hs });
  return buf;
}

/* SNIPPET_END: MerkleTree_Low_Serialization_mt_deserialize_path */

/* SNIPPET_START: MerkleTree_Low_Hashfunctions_init_hash */

uint8_t *MerkleTree_Low_Hashfunctions_init_hash(uint32_t hsz)
{
  regional__uint32_t__uint8_t_
  x00 = { .state = hsz, .dummy = NULL, .r_alloc = hash_r_alloc, .r_free = hash_r_free };
  regional__uint32_t__uint8_t_
  x0 = { .state = hsz, .dummy = NULL, .r_alloc = hash_r_alloc, .r_free = hash_r_free };
  return x00.r_alloc(x0.state);
}

/* SNIPPET_END: MerkleTree_Low_Hashfunctions_init_hash */

/* SNIPPET_START: MerkleTree_Low_Hashfunctions_free_hash */

void MerkleTree_Low_Hashfunctions_free_hash(uint8_t *h)
{
  KRML_HOST_FREE(h);
}

/* SNIPPET_END: MerkleTree_Low_Hashfunctions_free_hash */

