module MerkleTree.Low.Serialization

open FStar.Integers
open FStar.Mul
open LowStar.Modifies
open LowStar.BufferOps
open LowStar.Vector
open LowStar.RVector
open LowStar.Regional
open LowStar.Regional.Instances

open MerkleTree.Low

module HS = FStar.HyperStack
module HST = FStar.HyperStack.ST
module HH = FStar.Monotonic.HyperHeap

module B = LowStar.Buffer
module CB = LowStar.ConstBuffer
module V = LowStar.Vector
module RV = LowStar.RVector
module RVI = LowStar.Regional.Instances

module U16 = FStar.UInt16
module U32 = FStar.UInt32
module U64 = FStar.UInt64
module U8 = FStar.UInt8

open MerkleTree.Low.Datastructures
open MerkleTree.Low.Hashfunctions
module MTS = MerkleTree.Spec

let uint8_t = U8.t
let uint16_t = U16.t
let uint32_t = U32.t
let uint64_t = U64.t

let uint8_p = B.buffer uint8_t

type const_uint8_p = const_pointer uint8_t


#reset-options "--z3rlimit 5 --initial_fuel 0 --max_fuel 0 --initial_ifuel 0 --max_ifuel 0"

private let serialize_bool (ok:bool) (x:bool) (buf:uint8_p) (sz:uint32_t{B.len buf = sz}) (pos:uint32_t) : HST.ST (bool & uint32_t)
  (requires (fun h0 -> B.live h0 buf))
  (ensures (fun h0 _ h1 -> modifies (B.loc_buffer buf) h0 h1))
= if not ok || pos >= sz then (false, 0ul)
  else begin
    B.upd buf pos (if x then 1uy else 0uy);
    (true, pos + 1ul)
  end

private let serialize_uint8_t (ok:bool) (x:uint8_t) (buf:uint8_p) (sz:uint32_t{B.len buf = sz}) (pos:uint32_t) : HST.ST (bool & uint32_t)
  (requires (fun h0 -> B.live h0 buf))
  (ensures (fun h0 _ h1 -> modifies (B.loc_buffer buf) h0 h1))
= if not ok || pos >= sz then (false, 0ul)
  else begin B.upd buf pos x;
    (true, pos + 1ul)
  end

private let serialize_uint16_t (ok:bool) (x:uint16_t) (buf:uint8_p) (sz:uint32_t{B.len buf = sz}) (pos:uint32_t) : HST.ST (bool & uint32_t)
  (requires (fun h0 -> B.live h0 buf))
  (ensures (fun h0 _ h1 -> modifies (B.loc_buffer buf) h0 h1))
= let ok, pos = serialize_uint8_t ok (Int.Cast.uint16_to_uint8 (U16.shift_right x 8ul)) buf sz pos in
  serialize_uint8_t ok (Int.Cast.uint16_to_uint8 x) buf sz pos

private let serialize_uint32_t (ok:bool) (x:uint32_t) (buf:uint8_p) (sz:uint32_t{B.len buf = sz}) (pos:uint32_t) : HST.ST (bool & uint32_t)
  (requires (fun h0 -> B.live h0 buf))
  (ensures (fun h0 _ h1 -> modifies (B.loc_buffer buf) h0 h1))
= let ok, pos = serialize_uint16_t ok (Int.Cast.uint32_to_uint16 (U32.shift_right x 16ul)) buf sz pos in
  serialize_uint16_t ok (Int.Cast.uint32_to_uint16 x) buf sz pos

private let serialize_uint64_t (ok:bool) (x:uint64_t) (buf:uint8_p) (sz:uint32_t{B.len buf = sz}) (pos:uint32_t) : HST.ST (bool & uint32_t)
  (requires (fun h0 -> B.live h0 buf))
  (ensures (fun h0 _ h1 -> modifies (B.loc_buffer buf) h0 h1))
= let ok, pos = serialize_uint32_t ok (Int.Cast.uint64_to_uint32 (U64.shift_right x 32ul)) buf sz pos in
  serialize_uint32_t ok (Int.Cast.uint64_to_uint32 x) buf sz pos

private let serialize_offset_t = serialize_uint64_t
private let serialize_index_t = serialize_uint32_t

private let rec serialize_hash_i
  (#hash_size:hash_size_t)
  (ok:bool) (x:hash #hash_size) (buf:uint8_p) (sz:uint32_t{B.len buf = sz}) (pos:uint32_t) (i:uint32_t{i < hash_size})
: HST.ST (bool & uint32_t)
    (requires (fun h0 -> B.live h0 buf /\ B.live h0 x /\ B.len x = hash_size))
    (ensures (fun h0 _ h1 -> modifies (B.loc_buffer buf) h0 h1))
= if not ok || pos >= sz then (false, 0ul)
  else let b = x.(i) in
       let ok, pos = serialize_uint8_t ok (Lib.RawIntTypes.u8_to_UInt8 b) buf sz pos in
       let j = i + 1ul in
       if j < hash_size then serialize_hash_i #hash_size ok x buf sz pos j
       else (ok, pos)

private
let serialize_hash
  (#hash_size:hash_size_t)
  (ok:bool) (x:hash #hash_size) (buf:uint8_p) (sz:uint32_t{B.len buf = sz}) (pos:uint32_t)
: HST.ST (bool & uint32_t)
  (requires (fun h0 -> B.live h0 buf /\ B.live h0 x /\ B.len x = hash_size))
  (ensures (fun h0 _ h1 -> modifies (B.loc_buffer buf) h0 h1))
= if not ok || pos >= sz then (false, 0ul)
  else serialize_hash_i ok x buf sz pos 0ul

private inline_for_extraction
let u64_add_fits (x:uint64_t) (y:uint64_t): Tot (r:bool{r ==> UInt.size (U64.v x + U64.v y) 64}) = uint64_max - x >= y

#push-options "--z3rlimit 10 --initial_fuel 1 --max_fuel 1"
private inline_for_extraction
let hash_vec_bytes
  (#hash_size:hash_size_t)
  (v:hash_vec #hash_size)
: Tot uint64_t
= let vs_hs = U64.mul (u32_64 (V.size_of v)) (u32_64 hash_size) in
  if u64_add_fits vs_hs 4UL then vs_hs + 4UL else uint64_max
#pop-options

private
let rec serialize_hash_vec_i
  (#hash_size:hash_size_t)
  (ok:bool) (x:hash_vec #hash_size) (buf:uint8_p) (sz:uint32_t{B.len buf = sz}) (pos:uint32_t) (i:uint32_t{i < V.size_of x})
: HST.ST (bool & uint32_t)
  (requires (fun h0 -> B.live h0 buf /\ RV.rv_inv h0 x /\ loc_disjoint (B.loc_buffer buf) (loc_rvector x)))
  (ensures (fun h0 _ h1 -> RV.rv_inv h1 x /\ modifies (B.loc_buffer buf) h0 h1))
= if not ok || pos >= sz then (false, 0ul)
  else begin
    let vi = V.index x i in
    let ok, pos = serialize_hash ok vi buf sz pos in
    let j = i + 1ul in
    if j < V.size_of x then serialize_hash_vec_i ok x buf sz pos j
    else (ok, pos)
  end

private
let serialize_hash_vec
  (#hash_size:hash_size_t)
  (ok:bool) (x:hash_vec #hash_size) (buf:uint8_p) (sz:uint32_t{B.len buf = sz}) (pos:uint32_t)
: HST.ST (bool & uint32_t)
  (requires (fun h0 -> B.live h0 buf /\ RV.rv_inv h0 x /\ HS.disjoint (B.frameOf buf) (Rgl?.region_of (hvreg hash_size) x)))
  (ensures (fun h0 _ h1 -> RV.rv_inv h1 x /\ modifies (B.loc_buffer buf) h0 h1))
= if not ok || pos >= sz then (false, 0ul)
  else begin
    let h0 = HST.get() in
    let ok, pos = serialize_uint32_t ok (V.size_of x) buf sz pos in
    let h1 = HST.get() in
    RV.rv_inv_preserved x (B.loc_buffer buf) h0 h1;
    if ok && V.size_of x > 0ul then serialize_hash_vec_i ok x buf sz pos 0ul
    else (ok, pos)
  end

private inline_for_extraction
let rec hash_vv_bytes_i
  (#hash_size:hash_size_t)
  (vv:hash_vv hash_size)
  (i:uint32_t)
: HST.ST uint64_t
  (requires (fun h0 -> V.live h0 vv))
  (ensures (fun h0 _ h1 -> h0 == h1))
= if i >= V.size_of vv then 4UL
  else begin
    let vvi = V.index vv i in
    let r = hash_vec_bytes vvi in
    let rest = hash_vv_bytes_i vv (i+1ul) in
    if u64_add_fits r rest then begin
      assert (UInt.size (U64.v r + U64.v rest) 64);
      r + rest
    end
    else uint64_max
  end

private inline_for_extraction
let hash_vv_bytes
  (#hash_size:hash_size_t)
  (vv:hash_vv hash_size {V.size_of vv = merkle_tree_size_lg})
: HST.ST uint64_t
  (requires (fun h0 -> V.live h0 vv))
  (ensures (fun h0 _ h1 -> h0 == h1))
= hash_vv_bytes_i vv 0ul

private
let rec serialize_hash_vv_i
  (#hash_size:hash_size_t)
  (ok:bool) (x:hash_vv hash_size) (buf:uint8_p) (sz:uint32_t{B.len buf = sz}) (pos:uint32_t) (i:uint32_t{i < V.size_of x})
: HST.ST (bool & uint32_t)
  (requires (fun h0 -> B.live h0 buf /\ RV.rv_inv h0 x /\ HS.disjoint (B.frameOf buf) (Rgl?.region_of (hvvreg hash_size) x)))
  (ensures (fun h0 _ h1 -> modifies (B.loc_buffer buf) h0 h1))
= if not ok || pos >= sz then (false, 0ul)
  else begin
    let vi = V.index x i in
    let h0 = HST.get() in
    let ok, pos = serialize_hash_vec #hash_size ok vi buf sz pos in
    let h1 = HST.get() in
    RV.rv_inv_preserved x (B.loc_buffer buf) h0 h1;
    let j = i + 1ul in
    if j < V.size_of x then
      serialize_hash_vv_i #hash_size ok x buf sz pos j
    else (ok, pos)
  end

private
let serialize_hash_vv
  (#hash_size:hash_size_t)
  (ok:bool) (x:hash_vv hash_size) (buf:uint8_p) (sz:uint32_t{B.len buf = sz}) (pos:uint32_t)
: HST.ST (bool & uint32_t)
  (requires (fun h0 -> B.live h0 buf /\ RV.rv_inv h0 x /\ HS.disjoint (B.frameOf buf) (Rgl?.region_of (hvvreg hash_size) x)))
  (ensures (fun h0 _ h1 -> modifies (B.loc_buffer buf) h0 h1))
= if not ok || pos >= sz then (false, 0ul)
  else begin
    let h0 = HST.get() in
    let ok, pos = serialize_uint32_t ok (V.size_of x) buf sz pos in
    let h1 = HST.get() in
    RV.rv_inv_preserved x (B.loc_buffer buf) h0 h1;
    if (V.size_of x > 0ul) then serialize_hash_vv_i ok x buf sz pos 0ul
    else (ok, pos)
  end

private
let deserialize_bool (ok:bool) (buf:const_uint8_p) (sz:uint32_t{CB.length buf = U32.v sz}) (pos:uint32_t): HST.ST (bool & uint32_t & bool)
  (requires (fun h0 -> CB.live h0 buf))
  (ensures  (fun h0 _ h1 -> h0 == h1))
= if not ok || pos >= sz then (false, pos, false)
  else (true, pos + 1ul, (match CB.index buf pos with| 0uy -> false | _ -> true))

private
let deserialize_uint8_t (ok:bool) (buf:const_uint8_p) (sz:uint32_t{CB.length buf = U32.v sz}) (pos:uint32_t): HST.ST (bool & uint32_t & uint8_t)
  (requires (fun h0 -> CB.live h0 buf))
  (ensures  (fun h0 _ h1 -> h0 == h1))
= if not ok || pos >= sz then (false, pos, 0uy)
  else (true, pos + 1ul, CB.index buf pos)

private
let deserialize_uint16_t (ok:bool) (buf:const_uint8_p) (sz:uint32_t{CB.length buf = U32.v sz}) (pos:uint32_t): HST.ST (bool & uint32_t & uint16_t)
  (requires (fun h0 -> CB.live h0 buf))
  (ensures  (fun h0 _ h1 -> h0 == h1))
= if not ok || pos >= sz then (false, pos, 0us)
  else begin
    let ok, pos, b0 = deserialize_uint8_t ok buf sz pos in
    let ok, pos, b1 = deserialize_uint8_t ok buf sz pos in
    (ok, pos, (U16.shift_left (Int.Cast.uint8_to_uint16 b0) 8ul) + Int.Cast.uint8_to_uint16 b1)
  end

private
let deserialize_uint32_t (ok:bool) (buf:const_uint8_p) (sz:uint32_t{CB.length buf = U32.v sz}) (pos:uint32_t): HST.ST (bool & uint32_t & uint32_t)
  (requires (fun h0 -> CB.live h0 buf))
  (ensures  (fun h0 _ h1 -> h0 == h1))
= if not ok || pos >= sz then (false, pos, 0ul)
  else begin
    let ok, pos, b0 = deserialize_uint16_t ok buf sz pos in
    let ok, pos, b1 = deserialize_uint16_t ok buf sz pos in
    (ok, pos, (U32.shift_left (Int.Cast.uint16_to_uint32 b0) 16ul) + Int.Cast.uint16_to_uint32 b1)
  end

private
let deserialize_uint64_t (ok:bool) (buf:const_uint8_p) (sz:uint32_t{CB.length buf = U32.v sz}) (pos:uint32_t): HST.ST (bool & uint32_t & uint64_t)
  (requires (fun h0 -> CB.live h0 buf))
  (ensures  (fun h0 _ h1 -> h0 == h1))
= if not ok || pos >= sz then (false, pos, 0UL)
  else begin
    let ok, pos, b0 = deserialize_uint32_t ok buf sz pos in
    let ok, pos, b1 = deserialize_uint32_t ok buf sz pos in
    (ok, pos, (U64.shift_left (u32_64 b0) 32ul) + u32_64 b1)
  end

private let deserialize_offset_t = deserialize_uint64_t
private let deserialize_index_t = deserialize_uint32_t

private
let deserialize_hash
  (#hash_size:hash_size_t)
  (ok:bool) (buf:const_uint8_p) (sz:uint32_t{CB.length buf = U32.v sz}) (r:HST.erid) (pos:uint32_t)
: HST.ST (bool & uint32_t & hash #hash_size)
  (requires (fun h0 -> CB.live h0 buf))
  (ensures (fun h0 (k, _, h) h1 -> (k ==> Rgl?.r_inv (hreg hash_size) h1 h) /\
                                   loc_disjoint (loc_buffer (CB.cast buf)) (loc_buffer h) /\
                                   modifies B.loc_none h0 h1))
= let rg = hreg hash_size in
  if not ok || pos >= sz then (false, pos, rg_dummy rg)
  else if sz - pos < hash_size then (false, pos, rg_dummy rg)
  else begin
    let hash = rg_alloc rg r in
    Lib.RawBuffer.blit (CB.cast buf) pos hash 0ul hash_size;
    (true, pos + hash_size, hash)
  end

private
let rec deserialize_hash_vec_i
  (#hash_size:hash_size_t)
  (ok:bool) (buf:const_uint8_p) (sz:uint32_t{CB.length buf = U32.v sz}) (r:HST.erid) (pos:uint32_t) (res:hash_vec #hash_size) (i:uint32_t{i < V.size_of res})
: HST.ST (bool & uint32_t)
  (requires (fun h0 -> CB.live h0 buf /\ V.live h0 res))
  (ensures (fun h0 _ h1 -> B.modifies (B.loc_buffer (V.Vec?.vs res)) h0 h1))
= if not ok || pos >= sz then (false, pos)
  else begin
    let ok, pos, h = deserialize_hash ok buf sz r pos in
    if not ok then (false, pos)
    else begin
      V.assign res i h;
      (*
       * AR: 04/01: The call deserialize_hash_vec_i below needs liveness of buf
       *            So we have to frame buf liveness for the V.assign call
       *            V.assign provides a modifies postcondition in terms of
       *              loc_vector_within, which is a recursive predicate and
       *              I guess hard to reason about directly
       *            Whereas to reason about liveness of buf, we only need an
       *              overapproximation that V.assign modifies V.Vec?.vs res
       *            Looking at the Vector library, I found the following lemma
       *              that does the trick
       *)
      V.loc_vector_within_included res i (i + 1ul);
      let j = i + 1ul in
      if j < V.size_of res then deserialize_hash_vec_i ok buf sz r pos res j
      else (true, pos)
    end
  end

private
let deserialize_hash_vec
  (#hash_size:hash_size_t)
  (ok:bool) (buf:const_uint8_p) (sz:uint32_t{CB.length buf = U32.v sz}) (r:HST.erid) (pos:uint32_t)
: HST.ST (bool & uint32_t & hash_vec #hash_size)
  (requires (fun h0 -> CB.live h0 buf))
  (ensures (fun h0 _ h1 -> B.modifies B.loc_none h0 h1))
= let hrg = hreg hash_size in
  let hvrg = hvreg hash_size in
  if not ok || pos >= sz then (false, pos, rg_dummy hvrg)
  else begin
    let empty_vec = V.alloc_reserve 1ul (rg_dummy hrg) r in
    let ok, pos, n = deserialize_uint32_t ok buf sz pos in
    if not ok then (false, pos, empty_vec)
    else if n = 0ul then (true, pos, empty_vec)
    else begin
      let res = V.alloc n (rg_dummy hrg) in
      let ok, pos = deserialize_hash_vec_i ok buf sz r pos res 0ul in
      (ok, pos, res)
    end
  end

private
let rec deserialize_hash_vv_i
  (#hash_size:hash_size_t)
  (ok:bool) (buf:const_uint8_p) (sz:uint32_t{CB.length buf = U32.v sz}) (r:HST.erid) (pos:uint32_t) (res:hash_vv hash_size) (i:uint32_t{i < V.size_of res})
: HST.ST (bool & uint32_t)
  (requires (fun h0 -> CB.live h0 buf /\ V.live h0 res /\
                       B.loc_disjoint (CB.loc_buffer buf) (V.loc_vector res)))
  (ensures (fun h0 _ h1 -> modifies (B.loc_buffer (V.Vec?.vs res)) h0 h1))
=
  if not ok || pos >= sz then (false, 0ul)
  else begin
    let ok, pos, hv = deserialize_hash_vec ok buf sz r pos in
    let h0 = HST.get() in
    if not ok then (false, pos)
    else begin
      V.assign res i hv;
      (*
       * AR: 04/01: The call deserialize_hash_vv_i below needs liveness of buf
       *            So we have to frame buf liveness for the V.assign call
       *            V.assign provides a modifies postcondition in terms of
       *              loc_vector_within, which is a recursive predicate and
       *              I guess hard to reason about directly
       *            Whereas to reason about liveness of buf, we only need an
       *              overapproximation that V.assign modifies V.Vec?.vs res
       *            Looking at the Vector library, I found the following lemma
       *              that does the trick
       *)
      V.loc_vector_within_included res i (i + 1ul);
      let j = i + 1ul in
      if j = V.size_of res then (true, pos)
      else deserialize_hash_vv_i ok buf sz r pos res j
    end
  end

private let deserialize_hash_vv
  (#hash_size:hash_size_t)
  (ok:bool) (buf:const_uint8_p) (sz:uint32_t{CB.length buf = U32.v sz}) (r:HST.erid) (pos:uint32_t)
: HST.ST (bool & uint32_t & hash_vv hash_size)
  (requires (fun h0 -> CB.live h0 buf))
  (ensures (fun h0 _ h1 -> modifies B.loc_none h0 h1))
= let rg = hvreg hash_size in
  let empty_vec = V.alloc_reserve 1ul (rg_dummy rg) r in
  if not ok || pos >= sz then (false, pos, empty_vec)
  else begin
    let ok, pos, n = deserialize_uint32_t ok buf sz pos in
    if not ok then (false, pos, empty_vec)
    else if n = 0ul then (true, pos, empty_vec)
    else begin
      let res = V.alloc n (rg_dummy rg) in
      let ok, pos = deserialize_hash_vv_i ok buf sz r pos res 0ul in
      (ok, pos, res)
    end
  end

#push-options "--z3rlimit 10"
val mt_serialize_size: mt:const_mt_p -> HST.ST uint64_t
  (requires (fun h0 -> mt_safe h0 (CB.cast mt)))
  (ensures (fun _ _ h1 -> mt_safe h1 (CB.cast mt)))
let mt_serialize_size mt =
  let mtv = !*(CB.cast mt) in
  let hs = MT?.hs mtv in
  let rhs_ok = MT?.rhs_ok mtv in
  let rhs = MT?.rhs mtv in
  let hs_sz = hash_vv_bytes hs in
  if hs_sz < uint32_max then
    1UL +                // format version
    4UL +                // hash_size
    8UL +                // offset
    4UL + 4UL +          // i, j
    hs_sz +              // hs
    1UL +                // rhs_ok
    hash_vec_bytes rhs + // rhs
    u32_64 (MT?.hash_size mtv) // mroot
  else
     uint64_max
#pop-options

#push-options "--z3rlimit 15 --initial_fuel 1 --max_fuel 1"
val mt_serialize: mt:const_mt_p -> output:uint8_p -> sz:uint64_t -> HST.ST uint64_t
  (requires (fun h0 -> mt_safe h0 (CB.cast mt) /\ B.live h0 output /\ B.length output = U64.v sz /\
                       HS.disjoint (B.frameOf output) (B.frameOf (CB.cast mt))))
  (ensures (fun h0 _ h1 -> mt_safe h1 (CB.cast mt) /\ modifies (B.loc_buffer output) h0 h1))
let mt_serialize mt output sz =
  let mt = CB.cast mt in
  let sz = FStar.Int.Cast.uint64_to_uint32 sz in
  let mtv = !*mt in
  let h0 = HST.get() in
  let ok, pos = serialize_uint8_t true 1uy output sz 0ul in // format version = 1uy
  let h1 = HST.get() in mt_safe_preserved mt (B.loc_buffer output) h0 h1;
  let ok, pos = serialize_uint32_t ok (MT?.hash_size mtv) output sz pos in
  let h2 = HST.get() in mt_safe_preserved mt (B.loc_buffer output) h1 h2;
  let ok, pos = serialize_offset_t ok (MT?.offset mtv) output sz pos in
  let h3 = HST.get() in mt_safe_preserved mt (B.loc_buffer output) h2 h3;
  let ok, pos = serialize_uint32_t ok (MT?.i mtv) output sz pos in
  let h4 = HST.get() in mt_safe_preserved mt (B.loc_buffer output) h3 h4;
  let ok, pos = serialize_uint32_t ok (MT?.j mtv) output sz pos in
  let h5 = HST.get() in mt_safe_preserved mt (B.loc_buffer output) h4 h5;
  let ok, pos = serialize_hash_vv ok (MT?.hs mtv) output sz pos in
  let h6 = HST.get() in mt_safe_preserved mt (B.loc_buffer output) h5 h6;
  let ok, pos = serialize_bool ok (MT?.rhs_ok mtv) output sz pos in
  let h7 = HST.get() in mt_safe_preserved mt (B.loc_buffer output) h6 h7;
  let ok, pos = serialize_hash_vec ok (MT?.rhs mtv) output sz pos in
  let h8 = HST.get() in mt_safe_preserved mt (B.loc_buffer output) h7 h8;
  let ok, pos = serialize_hash ok (MT?.mroot mtv) output sz pos in
  let h9 = HST.get() in mt_safe_preserved mt (B.loc_buffer output) h8 h9;
  if ok then (FStar.Int.Cast.uint32_to_uint64 pos) else 0UL
#pop-options

#push-options "--z3rlimit 15 --initial_fuel 2 --max_fuel 2 --initial_ifuel 1 --max_ifuel 1"
val mt_deserialize:
  #hsz:Ghost.erased hash_size_t ->
  rid:HST.erid ->
  input:const_uint8_p ->
  sz:uint64_t{CB.length input = U64.v sz} ->
  hash_spec:Ghost.erased(MTS.hash_fun_t #(U32.v (Ghost.reveal hsz))) ->
  hash_fun:hash_fun_t #(Ghost.reveal hsz) #hash_spec
-> HST.ST (B.pointer_or_null merkle_tree)
  (requires (fun h0 -> CB.live h0 input /\
                       HS.disjoint rid (B.frameOf (CB.cast input))))
  (ensures (fun h0 r h1 -> modifies B.loc_none h0 h1 /\
                        (not (g_is_null r)) ==> MT?.hash_size (B.get h1 r 0) = Ghost.reveal hsz))
let mt_deserialize #ghsz rid input sz hash_spec hash_fun =
  let sz = FStar.Int.Cast.uint64_to_uint32 sz in
  let hrid = HST.new_region rid in
  let hvrid = HST.new_region rid in
  let hvvrid = HST.new_region rid in
  let ok, pos, format_version = deserialize_uint8_t true input sz 0ul in
  let ok = ok && format_version = 1uy in
  let ok, pos, hsz = deserialize_uint32_t ok input sz pos in
  if hsz = 0ul then B.null #merkle_tree else begin
    let ok, pos, offset = deserialize_offset_t ok input sz pos in
    let ok, pos, i = deserialize_index_t ok input sz pos in
    let ok, pos, j = deserialize_index_t ok input sz pos in
    let ok, pos, hs = deserialize_hash_vv #hsz ok input sz hvvrid pos in
    let ok, pos, rhs_ok = deserialize_bool ok input sz pos in
    let ok, pos, rhs = deserialize_hash_vec #hsz ok input sz hvrid pos in
    let ok, pos, mroot = deserialize_hash #hsz ok input sz hrid pos in
    begin
      if not ok ||
        not (merkle_tree_conditions #hsz offset i j hs rhs_ok rhs mroot)
      then B.null #merkle_tree
      else begin
        assume (hsz = Ghost.reveal ghsz); // We trust the user to provide a suitable hash_fun.
        B.malloc rid (MT hsz offset i j hs rhs_ok rhs mroot hash_spec hash_fun) 1ul
      end
    end
  end

val mt_serialize_path: #hsz:Ghost.erased hash_size_t -> p:const_path_p -> output:uint8_p -> sz:uint64_t -> HST.ST uint64_t
  (requires (fun h0 -> let ncp = CB.cast p in
                       let phv = B.get h0 ncp 0 in
                       Path?.hash_size phv = Ghost.reveal hsz /\
                       path_safe h0 (B.frameOf (CB.cast p)) ncp /\ RV.rv_inv #(hash #hsz) #hash_size_t #(hreg hsz) h0 (Path?.hashes phv) /\
                       B.live h0 output /\ B.length output = U64.v sz /\
                       HS.disjoint (B.frameOf output) (B.frameOf ncp)))
  (ensures (fun h0 _ h1 -> path_safe h1 (B.frameOf (CB.cast p)) (CB.cast p) /\
                           modifies (B.loc_buffer output) h0 h1))
let mt_serialize_path #hsz p output sz =
  let hsz = Path?.hash_size !*(CB.cast p) in
  let sz = FStar.Int.Cast.uint64_to_uint32 sz in
  let ncp = CB.cast p in
  let h0 = HST.get() in
  let ok, pos = serialize_uint32_t true hsz output sz 0ul in
  let h1 = HST.get() in
  let ncpd = !*ncp in
  let ok, pos = serialize_hash_vec #hsz ok (Path?.hashes ncpd) output sz pos in
  if ok then (FStar.Int.Cast.uint32_to_uint64 pos) else 0UL

val mt_deserialize_path:
  rid:HST.erid -> input:const_uint8_p -> sz:uint64_t{CB.length input = U64.v sz}
-> HST.ST (B.pointer_or_null path)
  (requires (fun h0 -> CB.live h0 input /\ HS.disjoint rid (B.frameOf (CB.cast input))))
  (ensures (fun h0 r h1 -> modifies B.loc_none h0 h1))
let mt_deserialize_path rid input sz =
  let sz = FStar.Int.Cast.uint64_to_uint32 sz in
  let hvvrid = HST.new_region rid in
  let ok, pos, hash_size = deserialize_uint32_t true input sz 0ul in
  if not ok || hash_size = 0ul then B.null #path
  else
    let ok, pos, hs = deserialize_hash_vec #hash_size ok input sz hvvrid pos in
    begin
      if not ok
      then (B.null #path)
      else (B.malloc rid (Path hash_size hs) 1ul)
    end
#pop-options
