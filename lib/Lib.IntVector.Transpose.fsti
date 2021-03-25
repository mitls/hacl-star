module Lib.IntVector.Transpose

open FStar.Mul
open Lib.IntTypes
open Lib.Sequence
open Lib.IntVector

#set-options "--z3rlimit 50 --fuel 0 --ifuel 0"

inline_for_extraction
let vec_t4 (t:v_inttype) = vec_t t 4 & vec_t t 4 & vec_t t 4 & vec_t t 4

inline_for_extraction
let vec_t8 (t:v_inttype) = vec_t t 8 & vec_t t 8 & vec_t t 8 & vec_t t 8 & vec_t t 8 & vec_t t 8 & vec_t t 8 & vec_t t 8


inline_for_extraction
val transpose4x4: #t:v_inttype{t = U32 \/ t = U64} -> vec_t4 t -> vec_t4 t

inline_for_extraction
val transpose8x8: #t:v_inttype{t = U32} -> vec_t8 t -> vec_t8 t


inline_for_extraction
let transpose4x4_lseq (#t:v_inttype{t = U32 \/ t = U64}) (vs:lseq (vec_t t 4) 4) : lseq (vec_t t 4) 4 =
  let (v0,v1,v2,v3) = (vs.[0],vs.[1],vs.[2],vs.[3]) in
  let (r0,r1,r2,r3) = transpose4x4 (v0,v1,v2,v3) in
  create4 r0 r1 r2 r3

inline_for_extraction
let transpose8x8_lseq (#t:v_inttype{t = U32}) (vs:lseq (vec_t t 8) 8) : lseq (vec_t t 8) 8 =
  let (v0,v1,v2,v3,v4,v5,v6,v7) = (vs.[0],vs.[1],vs.[2],vs.[3],vs.[4],vs.[5],vs.[6],vs.[7]) in
  let (r0,r1,r2,r3,r4,r5,r6,r7) = transpose8x8 (v0,v1,v2,v3,v4,v5,v6,v7) in
  create8 r0 r1 r2 r3 r4 r5 r6 r7


val transpose4x4_lemma: #t:v_inttype{t = U32 \/ t = U64} -> vs:lseq (vec_t t 4) 4 ->
  Lemma (forall (i:nat{i < 4}) (j:nat{j < 4}). (vec_v (transpose4x4_lseq vs).[i]).[j] == (vec_v vs.[j]).[i])

val transpose8x8_lemma: #t:v_inttype{t = U32} -> vs:lseq (vec_t t 8) 8 ->
  Lemma (forall (i:nat{i < 8}) (j:nat{j < 8}). (vec_v (transpose8x8_lseq vs).[i]).[j] == (vec_v vs.[j]).[i])
