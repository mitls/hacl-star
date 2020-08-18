module Hacl.Impl.P256.LowLevel

open FStar.HyperStack.All
open FStar.HyperStack
module ST = FStar.HyperStack.ST

open Lib.IntTypes
open Lib.Buffer

open Hacl.Spec.P256.Definition
open Hacl.Lemmas.P256
(* open Spec.ECDSA.Lemmas *)
open Spec.P256
open Spec.ECDSA

open FStar.Math
open FStar.Math.Lemmas
open FStar.Mul

open FStar.Tactics
open FStar.Tactics.Canon 

(* open Spec.P256.Lemmas *)
open Lib.IntTypes.Intrinsics

#set-options "--fuel 0 --ifuel 0 --z3rlimit 200"

(*
inline_for_extraction noextract
val load_buffer8: 
  a0: uint64 -> a1: uint64 -> 
  a2: uint64 -> a3: uint64 -> 
  a4: uint64 -> a5: uint64 -> 
  a6: uint64 -> a7: uint64 ->  
  o: lbuffer uint64 (size 8) -> 
  Stack unit
    (requires fun h -> live h o)
    (ensures fun h0 _ h1 -> modifies (loc o) h0 h1 /\ wide_as_nat #P256 h1 o == wide_as_nat4 (a0, a1, a2, a3, a4, a5, a6, a7))

let load_buffer8 a0 a1 a2 a3 a4 a5 a6 a7  o = 
    let h0 = ST.get() in 
  assert_norm (pow2 64 * pow2 64 = pow2 128);
  assert_norm (pow2 64 * pow2 64 * pow2 64 = pow2 192);
  assert_norm (pow2 64 * pow2 64 * pow2 64 * pow2 64 = pow2 256);
  assert_norm (pow2 64 * pow2 64 * pow2 64 * pow2 64 * pow2 64 = pow2 (5 * 64));
  assert_norm (pow2 64 * pow2 64 * pow2 64 * pow2 64 * pow2 64 * pow2 64 = pow2 (6 * 64));
  assert_norm (pow2 64 * pow2 64 * pow2 64 * pow2 64 * pow2 64 * pow2 64 * pow2 64 = pow2 (7 * 64));

  upd o (size 0) a0;
  upd o (size 1) a1;
  upd o (size 2) a2;
  upd o (size 3) a3;
  
  upd o (size 4) a4;
  upd o (size 5) a5;
  upd o (size 6) a6;
  upd o (size 7) a7
*)


(** This is unused *)
inline_for_extraction noextract
val copy_conditional_u64: a: uint64 -> b: uint64 -> mask: uint64 {uint_v mask = 0 \/ uint_v mask = pow2 64 - 1} -> 
  Tot (r: uint64 {if uint_v mask = 0 then uint_v r = uint_v a else uint_v r = uint_v b})

let copy_conditional_u64 a b mask = 
  lemma_xor_copy_cond a b mask;
  logxor a (logand mask (logxor a b))


val add4: x: felem P256 -> y: felem P256 -> result: felem P256 -> 
  Stack uint64
    (requires fun h -> live h x /\ live h y /\ live h result /\ eq_or_disjoint x result /\ eq_or_disjoint y result)
    (ensures fun h0 c h1 -> modifies (loc result) h0 h1 /\ v c <= 1 /\ 
      as_nat P256 h1 result + v c * pow2 256 == as_nat P256 h0 x + as_nat P256 h0 y)   

let add4 x y result =    
  let h0 = ST.get() in
  
  let r0 = sub result (size 0) (size 1) in 
  let r1 = sub result (size 1) (size 1) in 
  let r2 = sub result (size 2) (size 1) in 
  let r3 = sub result (size 3) (size 1) in 

    assert(let r1_0 = as_seq h0 r1 in let r0_ = as_seq h0 result in Seq.index r0_ 1 == Seq.index r1_0 0);
    assert(let r2_0 = as_seq h0 r2 in let r0_ = as_seq h0 result in Seq.index r0_ 2 == Seq.index r2_0 0);
    assert(let r3_0 = as_seq h0 r3 in let r0_ = as_seq h0 result in Seq.index r0_ 3 == Seq.index r3_0 0);   
    
  let cc0 = add_carry_u64 (u64 0) x.(0ul) y.(0ul) r0 in 
  let cc1 = add_carry_u64 cc0 x.(1ul) y.(1ul) r1 in 
  let cc2 = add_carry_u64 cc1 x.(2ul) y.(2ul) r2 in 
  let cc3 = add_carry_u64 cc2 x.(3ul) y.(3ul) r3 in 

  assert_norm (pow2 64 * pow2 64 = pow2 128);
  assert_norm (pow2 64 * pow2 64 * pow2 64 = pow2 192);
  assert_norm (pow2 64 * pow2 64 * pow2 64 * pow2 64 = pow2 256);

  cc3


val add4_with_carry: c: uint64 ->  x: felem P256 -> y: felem P256 -> result: felem P256 -> 
  Stack uint64
    (requires fun h -> uint_v c <= 1 /\ live h x /\ live h y /\ live h result /\ eq_or_disjoint x result /\ 
      eq_or_disjoint y result)
    (ensures fun h0 carry h1 -> modifies (loc result) h0 h1 /\ uint_v carry <= 1 /\ 
      as_nat P256 h1 result + v carry * pow2 256 == as_nat P256 h0 x + as_nat P256 h0 y + uint_v c)   

let add4_with_carry c x y result =    
    let h0 = ST.get() in
  
    let r0 = sub result (size 0) (size 1) in 
    let r1 = sub result (size 1) (size 1) in 
    let r2 = sub result (size 2) (size 1) in 
    let r3 = sub result (size 3) (size 1) in 
    
    let cc = add_carry_u64 c x.(0ul) y.(0ul) r0 in 
    let cc = add_carry_u64 cc x.(1ul) y.(1ul) r1 in 
    let cc = add_carry_u64 cc x.(2ul) y.(2ul) r2 in 
    let cc = add_carry_u64 cc x.(3ul) y.(3ul) r3 in   
    
      assert(let r1_0 = as_seq h0 r1 in let r0_ = as_seq h0 result in Seq.index r0_ 1 == Seq.index r1_0 0);
      assert(let r2_0 = as_seq h0 r2 in let r0_ = as_seq h0 result in Seq.index r0_ 2 == Seq.index r2_0 0);
      assert(let r3_0 = as_seq h0 r3 in let r0_ = as_seq h0 result in Seq.index r0_ 3 == Seq.index r3_0 0);

      assert_norm (pow2 64 * pow2 64 = pow2 128);
      assert_norm (pow2 64 * pow2 64 * pow2 64 = pow2 192);
      assert_norm (pow2 64 * pow2 64 * pow2 64 * pow2 64 = pow2 256);
    
    cc


val add8: x: widefelem P256 -> y: widefelem P256 -> result: widefelem P256 -> Stack uint64 
  (requires fun h -> live h x /\ live h y /\ live h result /\ eq_or_disjoint x result /\ eq_or_disjoint y result)
  (ensures fun h0 c h1 -> modifies (loc result) h0 h1 /\ v c <= 1 /\ 
    wide_as_nat P256 h1 result + v c * pow2 512 == wide_as_nat P256 h0 x + wide_as_nat P256 h0 y)

let add8 x y result = 
  assert_norm (pow2 64 * pow2 64 * pow2 64 * pow2 64 == pow2 256);

  let h0 = ST.get() in 
    let a0 = sub x (size 0) (size 4) in 
    let a1 = sub x (size 4) (size 4) in 
    
    let b0 = sub y (size 0) (size 4) in 
    let b1 = sub y (size 4) (size 4) in 

    let c0 = sub result (size 0) (size 4) in 
    let c1 = sub result (size 4) (size 4) in 

    let carry0 = add4 a0 b0 c0 in
    let carry1 = add4_with_carry carry0 a1 b1 c1 in 
      let h1 = ST.get() in 

    calc (==)
    {
      wide_as_nat P256 h0 x + wide_as_nat P256 h0 y;
      (==) 
      {
  distributivity_add_left (as_nat P256 h0 a1) (as_nat P256 h0 b1) (pow2 256)
      } 
      wide_as_nat P256 h1 result + uint_v carry1 * pow2 256 * pow2 256; 
      (==) 
      {
  assert_norm (pow2 256 * pow2 256 = pow2 512)
      }
      wide_as_nat P256 h1 result + uint_v carry1 * pow2 512; 
   };
   
  carry1


val add_dep_prime_p256: x: felem P256 -> t: uint64 {uint_v t == 0 \/ uint_v t == 1} ->
  result: felem P256 -> 
  Stack uint64
    (requires fun h -> live h x /\ live h result /\ eq_or_disjoint x result)
    (ensures fun h0 c h1 -> modifies (loc result) h0 h1 /\ (
      if uint_v t = 1 then 
	as_nat P256 h1 result + uint_v c * pow2 256 == as_nat P256 h0 x + prime256
      else
	as_nat P256 h1 result  == as_nat P256 h0 x))  

let add_dep_prime_p256 x t result = 
  let h0 = ST.get() in 

  let y0 = (u64 0) -. t in 
  let y1 = ((u64 0) -. t) >>. (size 32) in 
  let y2 = u64 0 in 
  let y3 = t -. (t <<. (size 32)) in 

  let r0 = sub result (size 0) (size 1) in      
  let r1 = sub result (size 1) (size 1) in 
  let r2 = sub result (size 2) (size 1) in 
  let r3 = sub result (size 3) (size 1) in 

  let cc = add_carry_u64 (u64 0) x.(0ul) y0 r0 in 
  let cc = add_carry_u64 cc x.(1ul) y1 r1 in 
  let cc = add_carry_u64 cc x.(2ul) y2 r2 in 
  let cc = add_carry_u64 cc x.(3ul) y3 r3 in     

  let h1 = ST.get() in 
  assert_norm(18446744073709551615 + 4294967295 * pow2 64 + 18446744069414584321 * pow2 192 = prime256);

  assert_norm (pow2 64 * pow2 64 = pow2 128);
  assert_norm (pow2 64 * pow2 64 * pow2 64 = pow2 192);
  assert_norm (pow2 64 * pow2 64 * pow2 64 * pow2 64 = pow2 256);
    
  assert(let r1_0 = as_seq h0 r1 in let r0_ = as_seq h0 result in Seq.index r0_ 1 == Seq.index r1_0 0);
  assert(let r2_0 = as_seq h0 r2 in let r0_ = as_seq h0 result in Seq.index r0_ 2 == Seq.index r2_0 0);
  assert(let r3_0 = as_seq h0 r3 in let r0_ = as_seq h0 result in Seq.index r0_ 3 == Seq.index r3_0 0); 

  cc


val sub4_il: x: felem P256 -> y: glbuffer uint64 (size 4) -> result: felem P256 -> 
  Stack uint64
    (requires fun h -> live h x /\ live h y /\ live h result /\ disjoint x result /\ disjoint result y)
    (ensures fun h0 c h1 -> modifies (loc result) h0 h1 /\ v c <= 1 /\
      (
  as_nat P256 h1 result - v c * pow2 256 == as_nat P256 h0 x  - as_nat_il P256 h0 y /\
  (if uint_v c = 0 then as_nat P256 h0 x >= as_nat_il P256 h0 y else as_nat P256 h0 x < as_nat_il P256 h0 y)
      )
    )

let sub4_il x y result = 
    let r0 = sub result (size 0) (size 1) in 
    let r1 = sub result (size 1) (size 1) in 
    let r2 = sub result (size 2) (size 1) in 
    let r3 = sub result (size 3) (size 1) in 

    let cc = sub_borrow_u64 (u64 0) x.(size 0) y.(size 0) r0 in 
    let cc = sub_borrow_u64 cc x.(size 1) y.(size 1) r1 in 
    let cc = sub_borrow_u64 cc x.(size 2) y.(size 2) r2 in 
    let cc = sub_borrow_u64 cc x.(size 3) y.(size 3) r3 in 

      assert_norm (pow2 64 * pow2 64 = pow2 128);
      assert_norm (pow2 64 * pow2 64 * pow2 64 = pow2 192);
      assert_norm (pow2 64 * pow2 64 * pow2 64 * pow2 64 = pow2 256);
    
    cc


val sub4: x: felem P256 -> y:felem P256 -> result: felem P256-> 
  Stack uint64
    (requires fun h -> live h x /\ live h y /\ live h result /\ eq_or_disjoint x result /\ eq_or_disjoint y result)
    (ensures fun h0 c h1 -> modifies1 result h0 h1 /\ v c <= 1 /\ as_nat P256 h1 result - v c * pow2 256 == as_nat P256 h0 x - as_nat P256 h0 y)

let sub4 x y result = 
  let h0 = ST.get() in 
  
  let r0 = sub result (size 0) (size 1) in 
  let r1 = sub result (size 1) (size 1) in 
  let r2 = sub result (size 2) (size 1) in 
  let r3 = sub result (size 3) (size 1) in 
      
  let cc = sub_borrow_u64 (u64 0) x.(size 0) y.(size 0) r0 in 
  let cc = sub_borrow_u64 cc x.(size 1) y.(size 1) r1 in 
  let cc = sub_borrow_u64 cc x.(size 2) y.(size 2) r2 in 
  let cc = sub_borrow_u64 cc x.(size 3) y.(size 3) r3 in 
    
    assert(let r1_0 = as_seq h0 r1 in let r0_ = as_seq h0 result in Seq.index r0_ 1 == Seq.index r1_0 0);
    assert(let r2_0 = as_seq h0 r2 in let r0_ = as_seq h0 result in Seq.index r0_ 2 == Seq.index r2_0 0);
    assert(let r3_0 = as_seq h0 r3 in let r0_ = as_seq h0 result in Seq.index r0_ 3 == Seq.index r3_0 0);

    assert_norm (pow2 64 * pow2 64 = pow2 128);
    assert_norm (pow2 64 * pow2 64 * pow2 64 = pow2 192);
    assert_norm (pow2 64 * pow2 64 * pow2 64 * pow2 64 = pow2 256);
    
  cc


val mul64: x: uint64 -> y: uint64 -> result: lbuffer uint64 (size 1) -> temp: lbuffer uint64 (size 1) ->
  Stack unit
    (requires fun h -> live h result /\ live h temp /\ disjoint result temp)
  (ensures fun h0 _ h1 -> modifies (loc result |+| loc temp) h0 h1 /\ 
    (
      let h0 = Seq.index (as_seq h1 temp) 0 in 
      let result = Seq.index (as_seq h1 result) 0 in 
      uint_v result + uint_v h0 * pow2 64 = uint_v x * uint_v y     
      )
    )

let mul64 x y result temp = 
  let res = mul64_wide x y in 
  let l0, h0 = to_u64 res, to_u64 (res >>. 64ul) in 
  upd result (size 0) l0;
  upd temp (size 0) h0


inline_for_extraction noextract
val mult64_0: x: felem P256 -> u: uint64 -> result: lbuffer uint64 (size 1) -> temp: lbuffer uint64 (size 1) -> Stack unit 
  (requires fun h -> live h x /\ live h result /\ live h temp /\ disjoint result temp)
  (ensures fun h0 _ h1 -> 
    let result_ = Seq.index (as_seq h1 result) 0 in 
    let c = Seq.index (as_seq h1 temp) 0 in 
    let f0 = Seq.index (as_seq h0 x) 0 in 
    uint_v result_ + uint_v c * pow2 64 = uint_v f0 * uint_v u /\ modifies (loc result |+| loc temp) h0 h1)

let mult64_0 x u result temp = 
  let f0 = index x (size 0) in 
  mul64 f0 u result temp


inline_for_extraction noextract
val mult64_0il: x: glbuffer uint64 (size 4) -> u: uint64 -> result:  lbuffer uint64 (size 1) -> temp: lbuffer uint64 (size 1) -> Stack unit 
  (requires fun h -> live h x /\ live h result /\ live h temp /\ disjoint result temp)
  (ensures fun h0 _ h1 -> 
    let result_ = Seq.index (as_seq h1 result) 0 in 
    let c = Seq.index (as_seq h1 temp) 0 in 
    let f0 = Seq.index (as_seq h0 x) 0 in 
    uint_v result_ + uint_v c * pow2 64 = uint_v f0 * uint_v u /\ modifies (loc result |+| loc temp) h0 h1)

let mult64_0il x u result temp = 
  let f0 = index x (size 0) in 
  mul64 f0 u result temp


inline_for_extraction noextract
val mult64_c: x: uint64 -> u: uint64 -> cin: uint64{uint_v cin <= 1} -> result: lbuffer uint64 (size 1) -> temp: lbuffer uint64 (size 1) -> Stack uint64 
  (requires fun h -> live h result /\ live h temp /\ disjoint result temp)
  (ensures fun h0 c2 h1 -> modifies (loc result |+| loc temp) h0 h1 /\ uint_v c2 <= 1 /\
    (
      let r = Seq.index (as_seq h1 result) 0 in 
      let h1 = Seq.index (as_seq h1 temp) 0 in 
      let h0 = Seq.index (as_seq h0 temp) 0 in 
      uint_v r + uint_v c2 * pow2 64 == uint_v x * uint_v u - uint_v h1 * pow2 64 + uint_v h0 + uint_v cin)
  )

let mult64_c x u cin result temp = 
  let h = index temp (size 0) in 
  mul64 x u result temp;
  let l = index result (size 0) in     
  add_carry_u64 cin l h result


inline_for_extraction noextract
val mul1_il: f:  glbuffer uint64 (size 4) -> u: uint64 -> result: lbuffer uint64 (size 4) -> Stack uint64
  (requires fun h -> live h result /\ live h f)
  (ensures fun h0 c h1 -> modifies (loc result) h0 h1 /\ 
    as_nat_il P256 h0 f * uint_v u = uint_v c * pow2 64 * pow2 64 * pow2 64 * pow2 64 + as_nat P256 h1 result /\ 
    as_nat_il P256 h0 f * uint_v u < pow2 320 /\
    uint_v c < pow2 64 - 1 
  )


let mul1_il f u result = 
  push_frame();

    assert_norm (pow2 64 * pow2 64 = pow2 128);
    assert_norm (pow2 64 * pow2 64 * pow2 64 = pow2 192);
    assert_norm (pow2 64 * pow2 64 * pow2 64 * pow2 64 = pow2 256);  
    assert_norm (pow2 64 * pow2 64 * pow2 64 * pow2 64 * pow2 64 = pow2 320); 

  let temp = create (size 1) (u64 0) in 

  let f0 = index f (size 0) in 
  let f1 = index f (size 1) in 
  let f2 = index f (size 2) in 
  let f3 = index f (size 3) in 
    
  let o0 = sub result (size 0) (size 1) in 
  let o1 = sub result (size 1) (size 1) in 
  let o2 = sub result (size 2) (size 1) in 
  let o3 = sub result (size 3) (size 1) in 
    
    let h0 = ST.get() in 
  mult64_0il f u o0 temp;
    let h1 = ST.get() in 
  let c1 = mult64_c f1 u (u64 0) o1 temp in 
    let h2 = ST.get() in 
  let c2 = mult64_c f2 u c1 o2 temp in 
    let h3 = ST.get() in 
  let c3 = mult64_c f3 u c2 o3 temp in 
    let h4 = ST.get() in 
  let temp0 = index temp (size 0) in 
    lemma_low_level0 (uint_v(Seq.index (as_seq h1 o0) 0)) (uint_v (Seq.index (as_seq h2 o1) 0)) (uint_v (Seq.index (as_seq h3 o2) 0)) (uint_v (Seq.index (as_seq h4 o3) 0)) (uint_v f0) (uint_v f1) (uint_v f2) (uint_v f3) (uint_v u) (uint_v (Seq.index (as_seq h2 temp) 0)) (uint_v c1) (uint_v c2) (uint_v c3) (uint_v (Seq.index (as_seq h3 temp) 0)) (uint_v temp0); 
    
  mul_lemma_4 (as_nat_il P256 h0 f) (uint_v u) (pow2 256 - 1) (pow2 64 - 1);
  assert_norm((pow2 256 - 1) * (pow2 64 - 1) == pow2 320 - pow2 256 - pow2 64 + 1);
  assert_norm((pow2 320 - pow2 256) / pow2 256 == pow2 64 - 1);

  pop_frame();  
  c3 +! temp0


inline_for_extraction noextract
val mul1: f: lbuffer uint64 (size 4) -> u: uint64 -> result: lbuffer uint64 (size 4) -> Stack uint64
  (requires fun h -> live h result /\ live h f)
  (ensures fun h0 c h1 -> modifies (loc result) h0 h1 /\ 
    as_nat P256 h0 f * uint_v u = uint_v c * pow2 256 + as_nat P256 h1 result /\ 
    as_nat P256 h0 f * uint_v u < pow2 320 /\
    uint_v c < pow2 64 - 1
  )


let mul1 f u result = 
  push_frame();

    assert_norm (pow2 64 * pow2 64 = pow2 128);
    assert_norm (pow2 64 * pow2 64 * pow2 64 = pow2 192);
    assert_norm (pow2 64 * pow2 64 * pow2 64 * pow2 64 = pow2 256);  
    assert_norm (pow2 64 * pow2 64 * pow2 64 * pow2 64 * pow2 64 = pow2 320); 

  let temp = create (size 1) (u64 0) in 

  let f0 = index f (size 0) in 
  let f1 = index f (size 1) in 
  let f2 = index f (size 2) in 
  let f3 = index f (size 3) in 
    
  let o0 = sub result (size 0) (size 1) in 
  let o1 = sub result (size 1) (size 1) in 
  let o2 = sub result (size 2) (size 1) in 
  let o3 = sub result (size 3) (size 1) in 
    
    let h0 = ST.get() in 
  mult64_0 f u o0 temp;
    let h1 = ST.get() in 
  let c1 = mult64_c f1 u (u64 0) o1 temp in 
    let h2 = ST.get() in 
  let c2 = mult64_c f2 u c1 o2 temp in 
    let h3 = ST.get() in 
  let c3 = mult64_c f3 u c2 o3 temp in 
    let h4 = ST.get() in 
  let temp0 = index temp (size 0) in 
    lemma_low_level0 (uint_v(Seq.index (as_seq h1 o0) 0)) (uint_v (Seq.index (as_seq h2 o1) 0)) (uint_v (Seq.index (as_seq h3 o2) 0)) (uint_v (Seq.index (as_seq h4 o3) 0)) (uint_v f0) (uint_v f1) (uint_v f2) (uint_v f3) (uint_v u) (uint_v (Seq.index (as_seq h2 temp) 0)) (uint_v c1) (uint_v c2) (uint_v c3) (uint_v (Seq.index (as_seq h3 temp) 0)) (uint_v temp0); 
    
  mul_lemma_4 (as_nat P256 h0 f) (uint_v u) (pow2 256 - 1) (pow2 64 - 1);
  assert_norm((pow2 256 - 1) * (pow2 64 - 1) == pow2 320 - pow2 256 - pow2 64 + 1);
  assert_norm((pow2 320 - pow2 256) / pow2 256 == pow2 64 - 1);

  pop_frame();  
  c3 +! temp0

inline_for_extraction noextract
val mul1_add: f1: felem P256 -> u2: uint64 -> f3: felem P256 -> result: felem P256 -> 
  Stack uint64 
  (requires fun h -> live h f1 /\ live h f3 /\ live h result /\ eq_or_disjoint f3 result /\ disjoint f1 result)
  (ensures fun h0 c h1 -> modifies (loc result) h0 h1  /\
    as_nat P256 h1 result + uint_v c * pow2 256 == as_nat P256 h0 f1 * uint_v u2 + as_nat P256 h0 f3 )

let mul1_add f1 u2 f3 result = 
  push_frame();
    let temp = create (size 4) (u64 0) in 
  let c = mul1 f1 u2 temp in 
  let c3 = add4 temp f3 result in 
  pop_frame();  
  c +! c3


val lemma_mul0: a: int -> b: int -> c: int -> d: int -> e: int -> Lemma 
  (requires
    (a + b * pow2 64 * pow2 64 * pow2 64 * pow2 64 == c * d + e))
  (ensures
    (a * pow2 64 * pow2 64 * pow2 64 + b * pow2 64 * pow2 64 * pow2 64 * pow2 64 * pow2 64 * pow2 64 * pow2 64 == c * d * pow2 64 * pow2 64 * pow2 64 + e * pow2 64 * pow2 64 * pow2 64))

let lemma_mul0 a b c d e = 
  assert((a + b * pow2 64 * pow2 64 * pow2 64 * pow2 64) * pow2 64 * pow2 64 * pow2 64 == (c * d + e) * pow2 64 * pow2 64 * pow2 64);
  assert_by_tactic ((a + b * pow2 64 * pow2 64 * pow2 64 * pow2 64) * pow2 64 * pow2 64 * pow2 64 ==
a * pow2 64 * pow2 64 * pow2 64 + b * pow2 64 * pow2 64 * pow2 64 * pow2 64 * pow2 64 * pow2 64 * pow2 64) canon;
  assert_by_tactic ((c * d + e) * pow2 64 * pow2 64 * pow2 64 ==  c * d * pow2 64 * pow2 64 * pow2 64 + e * pow2 64 * pow2 64 * pow2 64) canon


val lemma_mul1: a: int -> b: int -> c: int -> d: int -> Lemma
  ((a + b * pow2 64 + c * pow2 64 * pow2 64 + d * pow2 64 * pow2 64 * pow2 64) * pow2 64 * pow2 64 * pow2 64 == a * pow2 64 * pow2 64 * pow2 64 + b * pow2 64 * pow2 64 * pow2 64 * pow2 64 + c * pow2 64 * pow2 64 * pow2 64 * pow2 64 * pow2 64  + d * pow2 64 * pow2 64 * pow2 64 * pow2 64 * pow2 64 * pow2 64)

let lemma_mul1 a b c d = ()


val lemma_mul2: a: int -> b: int -> c: int -> d: int -> e: int -> Lemma 
  (requires
    (a + b * pow2 64 * pow2 64 * pow2 64 * pow2 64 == c * d + e))
  (ensures
    (a * pow2 64 * pow2 64 + b * pow2 64 * pow2 64 * pow2 64 * pow2 64 * pow2 64* pow2 64 == c * d * pow2 64 * pow2 64 + e * pow2 64 * pow2 64))

let lemma_mul2 a b c d e = ()


val lemma_mul3: a: int -> b: int -> c: int -> d: int -> Lemma
  ((a + b * pow2 64 + c * pow2 64 * pow2 64 + d * pow2 64 * pow2 64 * pow2 64) * pow2 64 * pow2 64 == a * pow2 64 * pow2 64 + b * pow2 64 * pow2 64 * pow2 64 + c * pow2 64 * pow2 64 * pow2 64 * pow2 64  + d * pow2 64 * pow2 64 * pow2 64 * pow2 64 * pow2 64)

let lemma_mul3 a b c d = ()


val lemma_mul4: a: int -> b: int -> c: int -> d: int -> e: int -> Lemma 
  (requires
    (a + b * pow2 64 * pow2 64 * pow2 64 * pow2 64 == c * d + e))
  (ensures
    (a * pow2 64 + b * pow2 64 * pow2 64 * pow2 64 * pow2 64 * pow2 64 == c * d * pow2 64 + e * pow2 64))

let lemma_mul4 a b c d e = ()


val lemma_mul5: a: int -> b: int -> c: int -> d: int -> Lemma
  ((a + b * pow2 64 + c * pow2 64 * pow2 64 + d * pow2 64 * pow2 64 * pow2 64) * pow2 64 == a * pow2 64  + b * pow2 64 * pow2 64  + c * pow2 64 * pow2 64 * pow2 64 + d * pow2 64 * pow2 64 * pow2 64 * pow2 64)

let lemma_mul5 a b c d = ()


val lemma_mul6: a: int -> b: int -> c: int -> d: int -> e: int -> 
  Lemma ((a * b + a * c * pow2 64 + a * d * pow2 64 * pow2 64  + a * e * pow2 64 * pow2 64 * pow2 64 == a * (b + (c * pow2 64) + (d * pow2 64 * pow2 64) + (e * pow2 64 * pow2 64 * pow2 64))))

let lemma_mul6 a b c d e = 
  assert_by_tactic ( ((a * b + a * c * pow2 64 + a * d * pow2 64 * pow2 64  + a * e * pow2 64 * pow2 64 * pow2 64 == a * (b + (c * pow2 64) + (d * pow2 64 * pow2 64) + (e * pow2 64 * pow2 64 * pow2 64))))) canon


val lemma_powers: unit -> Lemma
  (
    pow2 64 * pow2 64 * pow2 64 = pow2 (3 * 64) /\
    pow2 64 * pow2 64 * pow2 64 * pow2 64 = pow2 (4 * 64) /\ 
    pow2 64 * pow2 64 * pow2 64  * pow2 64 * pow2 64 = pow2 (5 * 64) /\
    pow2 64 * pow2 64 * pow2 64  * pow2 64 * pow2 64 * pow2 64 = pow2 (6 * 64) /\
    pow2 64 * pow2 64 * pow2 64  * pow2 64 * pow2 64* pow2 64 * pow2 64 = pow2 (7 * 64) /\
    pow2 64 * pow2 64 * pow2 64  * pow2 64 * pow2 64* pow2 64 * pow2 64 * pow2 64 = pow2 (8 * 64)
  )

let lemma_powers () = 
   assert_norm(pow2 64 * pow2 64 * pow2 64 = pow2 (3 * 64));
   assert_norm(pow2 64 * pow2 64 * pow2 64 * pow2 64 = pow2 (4 * 64));
   assert_norm(pow2 64 * pow2 64 * pow2 64  * pow2 64 * pow2 64 = pow2 (5 * 64));
   assert_norm(pow2 64 * pow2 64 * pow2 64  * pow2 64 * pow2 64 * pow2 64 = pow2 (6 * 64));
   assert_norm(pow2 64 * pow2 64 * pow2 64  * pow2 64 * pow2 64* pow2 64 * pow2 64 = pow2 (7 * 64));
   assert_norm(pow2 64 * pow2 64 * pow2 64  * pow2 64 * pow2 64* pow2 64 * pow2 64 * pow2 64 = pow2 (8 * 64))


#push-options "--z3rlimit 300"


inline_for_extraction noextract
val mul_p256: f: felem P256 -> r: felem P256 -> out: widefelem P256 -> 
  Stack unit
    (requires fun h -> live h out /\ live h f /\ live h r /\ disjoint r out)
    (ensures  fun h0 _ h1 -> modifies (loc out) h0 h1 /\ 
      wide_as_nat P256 h1 out = as_nat P256 h0 r * as_nat P256 h0 f
    )

let mul_p256 f r out =
  lemma_powers ();

  let f0 = f.(0ul) in
  let f1 = f.(1ul) in
  let f2 = f.(2ul) in
  let f3 = f.(3ul) in

    let h0 = ST.get() in 
  let b0 = sub out (size 0) (size 4) in 
  let c0 = mul1 r f0 b0 in 
    upd out (size 4) c0;

    let h1 = ST.get() in 
    let bk0 = sub out (size 0) (size 1) in 
    
    assert(as_nat P256 h0 r * uint_v f0 = uint_v (Lib.Sequence.index (as_seq h1 out) 4) * pow2 64 * pow2 64 * pow2 64 * pow2 64 + as_nat P256 h1 b0);

  let b1 = sub out (size 1) (size 4) in   
  let c1 = mul1_add r f1 b1 b1 in 
      upd out (size 5) c1; 
    let h2 = ST.get() in
    
    assert(as_nat P256 h2 b1 + uint_v (Lib.Sequence.index (as_seq h2 out) 5) * pow2 64 * pow2 64 * pow2 64 * pow2 64 == as_nat P256 h1 r * uint_v f1 + as_nat P256 h1 b1);

      let bk1 = sub out (size 0) (size 2) in 
  let b2 = sub out (size 2) (size 4) in 
  let c2 = mul1_add r f2 b2 b2 in 
    upd out (size 6) c2;
    let h3 = ST.get() in 
   
    assert(as_nat P256 h3 b2 + uint_v (Lib.Sequence.index (as_seq h3 out) 6) * pow2 64 * pow2 64 * pow2 64 * pow2 64 == as_nat P256 h2 r * uint_v f2 + as_nat P256 h2 b2);

     let bk2 = sub out (size 0) (size 3) in 
  let b3 = sub out (size 3) (size 4) in 
  let c3 = mul1_add r f3 b3 b3 in 
    upd out (size 7) c3;

    let h4 = ST.get() in 

    assert(as_nat P256 h4 b3 + uint_v (Lib.Sequence.index (as_seq h4 out) 7) * pow2 64 * pow2 64 * pow2 64 * pow2 64 == as_nat P256 h3 r * uint_v f3 + as_nat P256 h3 b3);
    
    let h5 = ST.get() in

    calc (==) {
    wide_as_nat P256 h5 out;
    (==) {}
    uint_v (Lib.Sequence.index (as_seq h4 out) 0) +  
    uint_v (Lib.Sequence.index (as_seq h4 out) 1) * pow2 64 + 
    uint_v (Lib.Sequence.index (as_seq h4 out) 2) * pow2 64 * pow2 64 + 
    as_nat P256 h4 b3 * pow2 64 * pow2 64 * pow2 64 + 
    uint_v (Lib.Sequence.index (as_seq h4 out) 7) * pow2 64 * pow2 64 * pow2 64 * pow2 64 * pow2 64 * pow2 64 * pow2 64;
    
    (==) {
      lemma_mul0 (as_nat P256 h4 b3) (uint_v (Lib.Sequence.index (as_seq h4 out) 7)) (as_nat P256 h3 r) (uint_v f3) (as_nat P256 h3 b3)}

    uint_v (Lib.Sequence.index (as_seq h4 out) 0) +  
    uint_v (Lib.Sequence.index (as_seq h4 out) 1) * pow2 64 + 
    uint_v (Lib.Sequence.index (as_seq h4 out) 2) * pow2 64 * pow2 64 + 
    as_nat P256 h3 r * uint_v f3 * pow2 64 * pow2 64 * pow2 64 + 
    as_nat P256 h3 b3 * pow2 64 * pow2 64 * pow2 64;

    (==)
    {
      assert(Lib.Sequence.index (as_seq h3 bk2) 0 == Lib.Sequence.index (as_seq h3 out) 0);
      assert(Lib.Sequence.index (as_seq h3 bk2) 1 == Lib.Sequence.index (as_seq h3 out) 1);
      assert(Lib.Sequence.index (as_seq h3 bk2) 2 == Lib.Sequence.index (as_seq h3 out) 2)
    }
    
    uint_v (Lib.Sequence.index (as_seq h3 out) 0) +  
    uint_v (Lib.Sequence.index (as_seq h3 out) 1) * pow2 64 + 
    uint_v (Lib.Sequence.index (as_seq h3 out) 2) * pow2 64 * pow2 64 + 
    (
      uint_v (Lib.Sequence.index (as_seq h3 out) 3) + 
      uint_v (Lib.Sequence.index (as_seq h3 out) 4) * pow2 64 + 
      uint_v (Lib.Sequence.index (as_seq h3 out) 5) * pow2 64 * pow2 64 + 
      uint_v (Lib.Sequence.index (as_seq h3 out) 6) * pow2 64 * pow2 64 * pow2 64
    ) * pow2 64 * pow2 64 * pow2 64 + 
    as_nat P256 h3 r * uint_v f3 * pow2 64 * pow2 64 * pow2 64;
  
    (==) {
      lemma_mul1 (uint_v (Lib.Sequence.index (as_seq h3 out) 3)) (uint_v (Lib.Sequence.index (as_seq h3 out) 4)) (uint_v (Lib.Sequence.index (as_seq h3 out) 5)) (uint_v (Lib.Sequence.index (as_seq h3 out) 6))}

    uint_v (Lib.Sequence.index (as_seq h3 out) 0) +  
    uint_v (Lib.Sequence.index (as_seq h3 out) 1) * pow2 64 + 
    
    as_nat P256 h3 b2 * pow2 64 * pow2 64 + 
    
    uint_v (Lib.Sequence.index (as_seq h3 out) 6) * pow2 64 * pow2 64 * pow2 64 * pow2 64 * pow2 64 * pow2 64 + 
    
    as_nat P256 h3 r * uint_v f3 * pow2 64 * pow2 64 * pow2 64;
    (==){
      lemma_mul2 (as_nat P256 h3 b2) (uint_v (Lib.Sequence.index (as_seq h3 out) 6)) (as_nat P256 h2 r) (uint_v f2) (as_nat P256 h2 b2)}
  
    uint_v (Lib.Sequence.index (as_seq h3 out) 0) +  
    uint_v (Lib.Sequence.index (as_seq h3 out) 1) * pow2 64 + 
    
    as_nat P256 h2 r * uint_v f2 * pow2 64 * pow2 64 + 
    as_nat P256 h2 b2 * pow2 64 * pow2 64 + 
    as_nat P256 h3 r * uint_v f3 * pow2 64 * pow2 64 * pow2 64;

    (==){    
      assert(Lib.Sequence.index (as_seq h2 bk1) 0 == Lib.Sequence.index (as_seq h2 out) 0);
      assert(Lib.Sequence.index (as_seq h2 bk1) 1 == Lib.Sequence.index (as_seq h2 out) 1)}

    uint_v (Lib.Sequence.index (as_seq h2 out) 0) +  
    uint_v (Lib.Sequence.index (as_seq h2 out) 1) * pow2 64 + 
    (
      uint_v (Lib.Sequence.index (as_seq h2 out) 2) + 
      uint_v (Lib.Sequence.index (as_seq h2 out) 3) * pow2 64 + 
      uint_v (Lib.Sequence.index (as_seq h2 out) 4) * pow2 64 * pow2 64 + 
      uint_v (Lib.Sequence.index (as_seq h2 out) 5) * pow2 64 * pow2 64 * pow2 64
    ) * pow2 64 * pow2 64 + 
    as_nat P256 h2 r * uint_v f2 * pow2 64 * pow2 64 + 
    as_nat P256 h3 r * uint_v f3 * pow2 64 * pow2 64 * pow2 64;

    (==) {
      lemma_mul3 (uint_v (Lib.Sequence.index (as_seq h2 out) 2)) (uint_v (Lib.Sequence.index (as_seq h2 out) 3)) (uint_v (Lib.Sequence.index (as_seq h2 out) 4)) (uint_v (Lib.Sequence.index (as_seq h2 out) 5))}

    uint_v (Lib.Sequence.index (as_seq h2 out) 0) +  
    as_nat P256 h2 b1 * pow2 64 + 
    uint_v (Lib.Sequence.index (as_seq h2 out) 5) * pow2 64 * pow2 64 * pow2 64 * pow2 64 * pow2 64 + 
    as_nat P256 h2 r * uint_v f2 * pow2 64 * pow2 64 + 
    as_nat P256 h3 r * uint_v f3 * pow2 64 * pow2 64 * pow2 64;

    (==){
       lemma_mul4 (as_nat P256 h2 b1) (uint_v (Lib.Sequence.index (as_seq h2 out) 5)) (as_nat P256 h1 r) (uint_v f1) (as_nat P256 h1 b1)}

    uint_v (Lib.Sequence.index (as_seq h2 out) 0) +  
    as_nat P256 h1 r * uint_v f1 * pow2 64 + as_nat P256 h1 b1 * pow2 64 +
    as_nat P256 h2 r * uint_v f2 * pow2 64 * pow2 64 + 
    as_nat P256 h3 r  * uint_v f3 * pow2 64 * pow2 64 * pow2 64;

    (==){
      assert(Lib.Sequence.index (as_seq h1 bk0) 0 == Lib.Sequence.index (as_seq h1 out) 0)}

    uint_v (Lib.Sequence.index (as_seq h1 out) 0) +  
    ( 
      uint_v (Lib.Sequence.index (as_seq h1 out) 1) + 
      uint_v (Lib.Sequence.index (as_seq h1 out) 2) * pow2 64 + 
      uint_v (Lib.Sequence.index (as_seq h1 out) 3) * pow2 64 * pow2 64 + 
      uint_v (Lib.Sequence.index (as_seq h1 out) 4) * pow2 64 * pow2 64 * pow2 64) * pow2 64 +
      
      as_nat P256 h1 r * uint_v f1 * pow2 64 + 
      as_nat P256 h2 r * uint_v f2 * pow2 64 * pow2 64 + 
      as_nat P256 h3 r * uint_v f3 * pow2 64 * pow2 64 * pow2 64;

    (==) {
     lemma_mul5 (uint_v (Lib.Sequence.index (as_seq h1 out) 1)) (uint_v (Lib.Sequence.index (as_seq h1 out) 2)) (uint_v (Lib.Sequence.index (as_seq h1 out) 3)) (uint_v (Lib.Sequence.index (as_seq h1 out) 4))}

     as_nat P256 h0 r * uint_v f0 +
     as_nat P256 h0 r * uint_v f1 * pow2 64 + 
     as_nat P256 h0 r * uint_v f2 * pow2 64 * pow2 64 + 
     as_nat P256 h0 r * uint_v f3 * pow2 64 * pow2 64 * pow2 64;

    (==) {lemma_mul6 (as_nat P256 h0 r) (uint_v f0) (uint_v f1) (uint_v f2) (uint_v f3)}

    as_nat P256 h0 r * as_nat P256 h0 f;}

#pop-options

val lemma_320: a: uint64 -> b: uint64 -> c: uint64 -> d: uint64 -> u: uint64 -> Lemma 
  (uint_v u * uint_v a +  (uint_v u * uint_v b) * pow2 64 + (uint_v u * uint_v c) * pow2 64 * pow2 64 + (uint_v u * uint_v d) * pow2 64 * pow2 64 * pow2 64 < pow2 320)
  
let lemma_320 a b c d u = 
  lemma_mult_le_left (uint_v a) (uint_v u) (pow2 64 - 1);
  lemma_mult_le_right (uint_v u) (uint_v a) (pow2 64 - 1);  
  
  lemma_mult_le_left (uint_v b) (uint_v u) (pow2 64 - 1);
  lemma_mult_le_right (uint_v u) (uint_v b) (pow2 64 - 1);

  lemma_mult_le_left (uint_v c) (uint_v u) (pow2 64 - 1);
  lemma_mult_le_right (uint_v u) (uint_v c) (pow2 64 - 1);  

  lemma_mult_le_left (uint_v d) (uint_v u) (pow2 64 - 1);
  lemma_mult_le_right (uint_v u) (uint_v d) (pow2 64 - 1);  
    
  assert_norm((pow2 64 - 1) * (pow2 64 - 1) +  
    ((pow2 64 - 1) * (pow2 64 - 1)) * pow2 64 + 
    ((pow2 64 - 1) * (pow2 64 - 1)) * pow2 64 * pow2 64 + 
    ((pow2 64 - 1) * (pow2 64 - 1)) * pow2 64 * pow2 64 * pow2 64 < pow2 320)


val lemma_320_64:a: uint64 -> b: uint64 -> c: uint64 -> d: uint64 -> e: uint64 -> u: uint64 -> Lemma 
  (uint_v u * uint_v a +  (uint_v u * uint_v b) * pow2 64 + (uint_v u * uint_v c) * pow2 64 * pow2 64 + (uint_v u * uint_v d) * pow2 64 * pow2 64 * pow2 64 + uint_v e  < pow2 320)
  
let lemma_320_64 a b c d e u = 

  lemma_mult_le_left (uint_v a) (uint_v u) (pow2 64 - 1);
  lemma_mult_le_right (uint_v u) (uint_v a) (pow2 64 - 1);  
  
  lemma_mult_le_left (uint_v b) (uint_v u) (pow2 64 - 1);
  lemma_mult_le_right (uint_v u) (uint_v b) (pow2 64 - 1);

  lemma_mult_le_left (uint_v c) (uint_v u) (pow2 64 - 1);
  lemma_mult_le_right (uint_v u) (uint_v c) (pow2 64 - 1);  

  lemma_mult_le_left (uint_v d) (uint_v u) (pow2 64 - 1);
  lemma_mult_le_right (uint_v u) (uint_v d) (pow2 64 - 1);  

  assert_norm((pow2 64 - 1) * (pow2 64 - 1) +  
    ((pow2 64 - 1) * (pow2 64 - 1)) * pow2 64 + 
    ((pow2 64 - 1) * (pow2 64 - 1)) * pow2 64 * pow2 64 + 
    ((pow2 64 - 1) * (pow2 64 - 1)) * pow2 64 * pow2 64 * pow2 64 + (pow2 64 - 1) < pow2 320)


inline_for_extraction noextract
val sq0: f:  lbuffer uint64 (size 4) -> result: lbuffer uint64 (size 4) -> memory: lbuffer uint64 (size 12) -> temp: lbuffer uint64 (size 5) -> Stack uint64
  (requires fun h -> live h result /\ live h f /\ live h memory /\ live h temp /\ 
    disjoint result temp /\ disjoint result memory /\ disjoint memory temp 
  )
  (ensures fun h0 c h1 -> modifies (loc result |+| loc memory |+| loc temp) h0 h1 /\ 
    (
      let f0 = Lib.Sequence.index (as_seq h0 f) 0 in 
      as_nat P256 h1 result + uint_v c * pow2 64 * pow2 64 * pow2 64 * pow2 64  = uint_v f0 * as_nat P256 h0 f) /\
      (
	let f = as_seq h0 f in 
	let f0 = Lib.Sequence.index f 0 in 
	let f1 = Lib.Sequence.index f 1 in 
	let f2 = Lib.Sequence.index f 2 in 
	let f3 = Lib.Sequence.index f 3 in 
	
	let memory = as_seq h1 memory in 
	let m0 = Lib.Sequence.index memory 0 in 
	let m1 = Lib.Sequence.index memory 1 in 
	let m2 = Lib.Sequence.index memory 2 in 
	let m3 = Lib.Sequence.index memory 3 in 
	let m4 = Lib.Sequence.index memory 4 in 
	let m5 = Lib.Sequence.index memory 5 in 

	uint_v m0 + uint_v m1 * pow2 64 == uint_v f0 * uint_v f1 /\
	uint_v m2 + uint_v m3 * pow2 64 == uint_v f0 * uint_v f2 /\
	uint_v m4 + uint_v m5 * pow2 64 == uint_v f0 * uint_v f3
      )
  )

inline_for_extraction noextract
val sq0_0: f: lbuffer uint64 (size 4) -> result: lbuffer uint64 (size 4) -> memory: lbuffer uint64 (size 12) -> temp: lbuffer uint64 (size 1) -> Stack uint64
  (requires fun h -> live h result /\ live h f /\ live h memory /\ live h temp /\
    disjoint result temp /\ disjoint result memory /\ disjoint memory temp)
  (ensures fun h0 c h1 -> modifies (loc result |+| loc temp |+| loc memory) h0 h1 /\
    ( 
      let memory = as_seq h1 memory in 
      let m0 = Lib.Sequence.index memory 0 in 
      let m1 = Lib.Sequence.index memory 1 in 
      let m2 = Lib.Sequence.index memory 2 in 
      let m3 = Lib.Sequence.index memory 3 in 
      
      let f0 = Lib.Sequence.index (as_seq h0 f) 0 in 
      let f1 = Lib.Sequence.index (as_seq h0 f) 1 in 
      let f2 = Lib.Sequence.index (as_seq h0 f) 2 in 
      
      uint_v (Lib.Sequence.index (as_seq h1 result) 2)  * pow2 64 * pow2 64  +
      v c * pow2 64  * pow2 64 * pow2 64 
      +  uint_v (Lib.Sequence.index (as_seq h1 result) 1) * pow2 64
      +  uint_v (Lib.Sequence.index (as_seq h1 result) 0) 
      + uint_v (Lib.Sequence.index (as_seq h1 temp) 0) * pow2 64  * pow2 64 * pow2 64   
      =
      uint_v f0 * uint_v f2  * pow2 64 * pow2 64   +
      uint_v f0 * uint_v f0 + uint_v f0 * uint_v f1 * pow2 64 /\

      uint_v m0 + uint_v m1 * pow2 64 == uint_v f0 * uint_v f1 /\
      uint_v m2 + uint_v m3 * pow2 64 == uint_v f0 * uint_v f2 /\

      uint_v c <= 1
   )
)

let sq0_0 f result memory temp = 
    let h0 = ST.get() in 
  
  let f0 = index f (size 0) in 
  let f1 = index f (size 1) in 
  let f2 = index f (size 2) in 
  
  let o0 = sub result (size 0) (size 1) in 
  let o1 = sub result (size 1) (size 1) in  
  let o2 = sub result (size 2) (size 1) in 
  
  mul64 f0 f0 o0 temp;
  let h_0 = index temp (size 0) in 
  
    let h1 = ST.get() in 
    assert(Lib.Sequence.index (as_seq h0 o0) 0 == Lib.Sequence.index (as_seq h0 result) 0);
    assert(Lib.Sequence.index (as_seq h0 o1) 0 == Lib.Sequence.index (as_seq h0 result) 1);
    assert(Lib.Sequence.index (as_seq h0 o2) 0 == Lib.Sequence.index (as_seq h0 result) 2);


  mul64 f0 f1 o1 temp;
  let l = index o1 (size 0) in   

  upd memory (size 0) l;
  upd memory (size 1) (index temp (size 0));  
    
  let c1 = add_carry_u64 (u64 0) l h_0 o1 in 
  let h_1 = index temp (size 0) in

  mul64 f0 f2 o2 temp; 
  let l = index o2 (size 0) in   
  upd memory (size 2) l;
  upd memory (size 3) (index temp (size 0));
  add_carry_u64 c1 l h_1 o2


val lemma_distr_4: a: int -> b: int -> c: int -> d: int -> e: int -> Lemma (
  a * b + a * c * pow2 64 + a * d * pow2 64 * pow2 64 + a * e * pow2 64 * pow2 64 * pow2 64 == a * (b + c * pow2 64 + d * pow2 64 * pow2 64 + e * pow2 64 * pow2 64 * pow2 64))

let lemma_distr_4 a b c e d = ()


let sq0 f result memory temp = 
  let h0 = ST.get() in 
  
  assert_norm (pow2 64 * pow2 64 = pow2 128);
  assert_norm (pow2 64 * pow2 64 * pow2 64 = pow2 192);
  assert_norm (pow2 64 * pow2 64 * pow2 64 * pow2 64 = pow2 256);  
  assert_norm (pow2 64 * pow2 64 * pow2 64 * pow2 64 * pow2 64 = pow2 320); 

  let f0 = index f (size 0) in 
  let f1 = index f (size 1) in 
  let f2 = index f (size 2) in 
  let f3 = index f (size 3) in 

  let o0 = sub result (size 0) (size 3) in 
  let o3 = sub result (size 3) (size 1) in
  
  let temp = sub temp (size 0) (size 1) in 

  let c2 = sq0_0 f result memory temp in 
  let h_2 = index temp (size 0) in

  mul64 f0 f3 o3 temp;
  let l = index o3 (size 0) in    

    let h2 = ST.get() in 
 
  upd memory (size 4) l;
  upd memory (size 5) (index temp (size 0));
  let c3 = add_carry_u64 c2 l h_2 o3 in 
  let temp0 = index temp (size 0) in

  assert(Lib.Sequence.index (as_seq h2 result) 2 == Lib.Sequence.index (as_seq h2 o0) 2);
  assert(Lib.Sequence.index (as_seq h2 result) 1 == Lib.Sequence.index (as_seq h2 o0) 1);
  assert(Lib.Sequence.index (as_seq h2 result) 0 == Lib.Sequence.index (as_seq h2 o0) 0);

  distributivity_add_left  (v c3) (uint_v temp0) (pow2 64 * pow2 64 * pow2 64 * pow2 64);
  lemma_distr_4 (v f0) (v f0) (v f1) (v f2) (v f3);
  
   lemma_mult_le_left (as_nat P256 h0 f) (v f0) (pow2 64);
   lemma_mult_le_left (v f0) (as_nat P256 h0 f) (pow2 256);

   lemma_div_lt_nat (v c3 + uint_v temp0) 320 256;
   
  c3 +! temp0


inline_for_extraction noextract
val sq1: f: felem P256 -> f4: felem P256 -> result: felem P256 -> memory: lbuffer uint64 (size 12) -> 
  temp: lbuffer uint64 (size 5) -> 
  Stack uint64 
  (requires fun h -> live h f /\ live h f4 /\ live h result /\ live h temp /\ 
    eq_or_disjoint f4 result /\ disjoint f4 memory /\ disjoint f4 temp /\ 
    disjoint f result /\ live h memory /\ disjoint temp result /\ disjoint memory temp /\ disjoint memory result /\
    (
      let f = as_seq h f in 
      let f0 = Lib.Sequence.index f 0 in 
      let f1 = Lib.Sequence.index f 1 in 
      let f2 = Lib.Sequence.index f 2 in 
      let f3 = Lib.Sequence.index f 3 in 
    
      let memory = as_seq h memory in 
      let m0 = Lib.Sequence.index memory 0 in 
      let m1 = Lib.Sequence.index memory 1 in 
      let m2 = Lib.Sequence.index memory 2 in 
      let m3 = Lib.Sequence.index memory 3 in 
      let m4 = Lib.Sequence.index memory 4 in 
      let m5 = Lib.Sequence.index memory 5 in 

      uint_v m0 + uint_v m1 * pow2 64 == uint_v f0 * uint_v f1 /\
      uint_v m2 + uint_v m3 * pow2 64 == uint_v f0 * uint_v f2 /\
      uint_v m4 + uint_v m5 * pow2 64 == uint_v f0 * uint_v f3
    ) 
  )
  (ensures fun h0 c h1 -> modifies (loc result |+| loc memory |+| loc temp) h0 h1 /\
  (

      let f0 = Lib.Sequence.index (as_seq h0 f) 0 in 
      let f1 = Lib.Sequence.index (as_seq h0 f) 1  in 
      let f2 = Lib.Sequence.index (as_seq h0 f) 2 in 
      let f3 = Lib.Sequence.index (as_seq h0 f) 3 in 
    
      let memory = as_seq h1 memory in 
      let m0 = Lib.Sequence.index memory 0 in 
      let m1 = Lib.Sequence.index memory 1 in 
      let m2 = Lib.Sequence.index memory 2 in 
      let m3 = Lib.Sequence.index memory 3 in 
      let m4 = Lib.Sequence.index memory 4 in 
      let m5 = Lib.Sequence.index memory 5 in 
      let m6 = Lib.Sequence.index memory 6 in 
      let m7 = Lib.Sequence.index memory 7 in 
      let m8 = Lib.Sequence.index memory 8 in 
      let m9 = Lib.Sequence.index memory 9 in 

      uint_v m0 + uint_v m1 * pow2 64 == uint_v f0 * uint_v f1 /\
      uint_v m2 + uint_v m3 * pow2 64 == uint_v f0 * uint_v f2 /\
      uint_v m4 + uint_v m5 * pow2 64 == uint_v f0 * uint_v f3 /\
      
      uint_v m6 + uint_v m7 * pow2 64 == uint_v f1 * uint_v f2 /\
      uint_v m8 + uint_v m9 * pow2 64 == uint_v f1 * uint_v f3 /\

      as_nat P256 h1 result + uint_v c * pow2 256 = uint_v f1 * as_nat P256 h0 f + as_nat P256 h0 f4
    ) 
  )


val lemma_320_1: a: nat -> b: nat -> c: nat {c < pow2 64} -> d: nat {d < pow2 256} -> e: nat {e < pow2 256 /\ a + b * pow2 256 = c * d + e}  -> Lemma (b < pow2 64)

let lemma_320_1 a b c d e = 
  assert_norm ((pow2 64 - 1) * (pow2 256 - 1) + pow2 256 < pow2 320);
  lemma_mult_le_left d c (pow2 64 - 1);
  lemma_mult_le_right c d (pow2 256 - 1);
  lemma_div_lt_nat (b * pow2 256) 320 256; 
  pow2_multiplication_division_lemma_1 b 256 256


let sq1 f f4 result memory tempBuffer = 
  let h0 = ST.get() in 
  assert_norm (pow2 64 * pow2 64 * pow2 64 * pow2 64 = pow2 256);
  
  let temp = sub tempBuffer (size 0) (size 1) in 
  let tempBufferResult = sub tempBuffer  (size 1) (size 4) in 

  let f0 = index f (size 0) in 
  let f1 = index f (size 1) in 
  let f2 = index f (size 2) in 
  let f3 = index f (size 3) in 
    
  let o0 = sub tempBufferResult (size 0) (size 1) in 
  let o1 = sub tempBufferResult (size 1) (size 1) in 
  let o2 = sub tempBufferResult (size 2) (size 1) in 
  let o3 = sub tempBufferResult (size 3) (size 1) in 

  upd o0 (size 0) (index memory (size 0));
  let h_0 = index memory (size 1) in 
  mul64 f1 f1 o1 temp;
  let l = index o1 (size 0) in     
  let c1 = add_carry_u64 (u64 0) l h_0 o1 in 
  let h_1 = index temp (size 0) in 

  mul64 f1 f2 o2 temp;
  let l = index o2 (size 0) in  
  upd memory (size 6) l;
  upd memory (size 7) (index temp (size 0));
  let c2 = add_carry_u64 c1 l h_1 o2 in
  let h_2 = index temp (size 0) in   


  mul64 f1 f3 o3 temp;
  let l = index o3 (size 0) in  
  upd memory (size 8) l;
  upd memory (size 9) (index temp (size 0));
  let c3 = add_carry_u64 c2 l h_2 o3 in
  let h_3 = index temp (size 0) in 

    let h6 = ST.get() in 
    
  calc (==) {
    uint_v f1 * as_nat P256 h0 f;
    (==) { }
    uint_v f1 * (uint_v f0 +  uint_v f1 * pow2 64 + uint_v f2 * pow2 64 * pow2 64 + uint_v f3 * pow2 64 * pow2 64 * pow2 64);
    (==){
      assert_by_tactic (uint_v f1 * (uint_v f0 +  uint_v f1 * pow2 64 + uint_v f2 * pow2 64 * pow2 64 + uint_v f3 * pow2 64 * pow2 64 * pow2 64) == 
      uint_v f0 * uint_v f1 +  
      uint_v f1 * uint_v f1 * pow2 64 + 
      uint_v f2 * uint_v f1 * pow2 64 * pow2 64 + 
      uint_v f3 * uint_v f1 * pow2 64 * pow2 64 * pow2 64) canon}
      
    as_nat P256 h6 tempBufferResult + (uint_v c3 + uint_v h_3) * (pow2 64 * pow2 64 * pow2 64 * pow2 64);
    (==){assert_norm (pow2 64 * pow2 64 * pow2 64 * pow2 64 = pow2 256)}
    
    as_nat P256 h6 tempBufferResult + (uint_v c3 + uint_v h_3) * pow2 256;};


  let c4 = add4 tempBufferResult f4 result in   
  let h7 = ST.get() in 

  assert_by_tactic (uint_v c4 * pow2 256 + (uint_v c3 + uint_v h_3) * pow2 256 = 
    (uint_v c4 + uint_v c3 + uint_v h_3) * pow2 256) canon;

  lemma_320_1 (as_nat P256 h7 result) (uint_v c4 + uint_v c3 + uint_v h_3) (uint_v f1) (as_nat P256 h0 f) (as_nat P256 h0 f4);

  c3 +! h_3 +! c4


inline_for_extraction noextract
val sq2: f: felem P256 -> f4: felem P256 -> result: felem P256 -> memory: lbuffer uint64 (size 12) -> temp: lbuffer uint64 (size 5) -> 
  Stack uint64 
  (requires fun h -> live h f /\ live h f4 /\ live h result /\ live h temp /\ live h memory /\ eq_or_disjoint f4 result /\ disjoint f4 memory /\ disjoint f4 temp /\ disjoint f result /\ disjoint temp result /\ disjoint memory temp /\ disjoint memory result /\
    (

      let f0 = Lib.Sequence.index (as_seq h f) 0 in 
      let f1 = Lib.Sequence.index (as_seq h f) 1  in 
      let f2 = Lib.Sequence.index (as_seq h f) 2 in 
      let f3 = Lib.Sequence.index (as_seq h f) 3 in 
    
      let memory = as_seq h memory in 
      let m2 = Lib.Sequence.index memory 2 in 
      let m3 = Lib.Sequence.index memory 3 in 
      let m4 = Lib.Sequence.index memory 4 in 
      let m5 = Lib.Sequence.index memory 5 in 
      let m6 = Lib.Sequence.index memory 6 in 
      let m7 = Lib.Sequence.index memory 7 in 
      let m8 = Lib.Sequence.index memory 8 in 
      let m9 = Lib.Sequence.index memory 9 in 

      uint_v m2 + uint_v m3 * pow2 64 == uint_v f0 * uint_v f2 /\
      uint_v m4 + uint_v m5 * pow2 64 == uint_v f0 * uint_v f3 /\
      
      uint_v m6 + uint_v m7 * pow2 64 == uint_v f1 * uint_v f2 /\
      uint_v m8 + uint_v m9 * pow2 64 == uint_v f1 * uint_v f3 
    )
)
 (ensures fun h0 c h1 -> modifies (loc result |+| loc memory |+| loc temp) h0 h1   /\
       (

      let f0 = Lib.Sequence.index (as_seq h0 f) 0 in 
      let f1 = Lib.Sequence.index (as_seq h0 f) 1  in 
      let f2 = Lib.Sequence.index (as_seq h0 f) 2 in 
      let f3 = Lib.Sequence.index (as_seq h0 f) 3 in 
    
      let memory = as_seq h1 memory in 
      let m4 = Lib.Sequence.index memory 4 in 
      let m5 = Lib.Sequence.index memory 5 in 
      let m8 = Lib.Sequence.index memory 8 in 
      let m9 = Lib.Sequence.index memory 9 in 
      let m10 = Lib.Sequence.index memory 10 in 
      let m11 = Lib.Sequence.index memory 11 in 

      uint_v m4 + uint_v m5 * pow2 64 == uint_v f0 * uint_v f3 /\
      uint_v m8 + uint_v m9 * pow2 64 == uint_v f1 * uint_v f3 /\ 
      uint_v m10 + uint_v m11 * pow2 64 == uint_v f2 * uint_v f3 /\
      as_nat P256 h1 result + uint_v c * pow2 256 = uint_v f2 * as_nat P256 h0 f + as_nat P256 h0 f4
    )  
 )


let sq2 f f4 result memory tempBuffer = 
  let h0 = ST.get() in 
  assert_norm (pow2 64 * pow2 64 * pow2 64 * pow2 64 = pow2 256);
  
  let temp = sub tempBuffer (size 0) (size 1) in 
  let tempBufferResult = sub tempBuffer  (size 1) (size 4) in 

  let f0 = index f (size 0) in 
  let f1 = index f (size 1) in 
  let f2 = index f (size 2) in 
  let f3 = index f (size 3) in 
    
  let o0 = sub tempBufferResult (size 0) (size 1) in 
  let o1 = sub tempBufferResult (size 1) (size 1) in 
  let o2 = sub tempBufferResult (size 2) (size 1) in 
  let o3 = sub tempBufferResult (size 3) (size 1) in 

  upd o0 (size 0) (index memory (size 2)); 
  let h_0 = index memory (size 3) in 

  upd o1 (size 0) (index memory (size 6));
  
  let l = index o1 (size 0) in     
  let c1 = add_carry_u64 (u64 0) l h_0 o1 in 
  let h_1 = index memory (size 7) in 


  mul64 f2 f2 o2 temp;
  let l = index o2 (size 0) in 

  let c2 = add_carry_u64 c1 l h_1 o2 in
  let h_2 = index temp (size 0) in 

  mul64 f2 f3 o3 temp; 
  let l = index o3 (size 0) in   
  
  upd memory (size 10) l;
  upd memory (size 11) (index temp (size 0));

  let c3 = add_carry_u64 c2 l h_2 o3 in
  let h_3 = index temp (size 0) in 

    let h6 = ST.get() in 

    calc (==) {

    uint_v f2 * as_nat P256 h0 f;
    (==) {}
    uint_v f2 * (uint_v f0 +  uint_v f1 * pow2 64 + uint_v f2 * pow2 64 * pow2 64 + uint_v f3 * pow2 64 * pow2 64 * pow2 64);
    (==){
      assert_by_tactic (uint_v f2 * (uint_v f0 +  uint_v f1 * pow2 64 + uint_v f2 * pow2 64 * pow2 64 + uint_v f3 * pow2 64 * pow2 64 * pow2 64) == 
      uint_v f0 * uint_v f2 +  
      uint_v f1 * uint_v f2 * pow2 64 + 
      uint_v f2 * uint_v f2 * pow2 64 * pow2 64 + 
      uint_v f3 * uint_v f2 * pow2 64 * pow2 64 * pow2 64) canon}
   
   as_nat P256 h6 tempBufferResult + (uint_v c3 + uint_v h_3) * (pow2 64 * pow2 64 * pow2 64 * pow2 64);
   
   (==){assert_norm (pow2 64 * pow2 64 * pow2 64 * pow2 64 = pow2 256)}
   
   as_nat P256 h6 tempBufferResult + (uint_v c3 + uint_v h_3) * pow2 256;
   };


  let c4 = add4 tempBufferResult f4 result in  
  let h7 = ST.get() in 

  assert_by_tactic (uint_v c4 * pow2 256 + (uint_v c3 + uint_v h_3) * pow2 256 = 
    (uint_v c4 + uint_v c3 + uint_v h_3) * pow2 256) canon;

  lemma_320_1 (as_nat P256 h7 result) (uint_v c4 + uint_v c3 + uint_v h_3) (uint_v f2) (as_nat P256 h0 f) (as_nat P256 h0 f4);

  c3 +! h_3 +! c4


inline_for_extraction noextract
val sq3: f: felem P256 -> f4: felem P256 -> result: felem P256 -> memory: lbuffer uint64 (size 12) -> temp: lbuffer uint64 (size 5) -> 
  Stack uint64 
  (requires fun h -> live h f /\ live h f4 /\ live h result /\ live h temp /\ live h memory /\ eq_or_disjoint f4 result /\ disjoint f4 memory /\ disjoint f4 temp /\ disjoint f result /\ disjoint temp result /\ disjoint memory temp /\ disjoint memory result /\
  (

      let f0 = Lib.Sequence.index (as_seq h f) 0 in 
      let f1 = Lib.Sequence.index (as_seq h f) 1  in 
      let f2 = Lib.Sequence.index (as_seq h f) 2 in 
      let f3 = Lib.Sequence.index (as_seq h f) 3 in 
    
      let memory = as_seq h memory in 
      let m4 = Lib.Sequence.index memory 4 in 
      let m5 = Lib.Sequence.index memory 5 in 
      let m8 = Lib.Sequence.index memory 8 in 
      let m9 = Lib.Sequence.index memory 9 in 
      let m10 = Lib.Sequence.index memory 10 in 
      let m11 = Lib.Sequence.index memory 11 in 

      uint_v m4 + uint_v m5 * pow2 64 == uint_v f0 * uint_v f3 /\
      uint_v m8 + uint_v m9 * pow2 64 == uint_v f1 * uint_v f3 /\ 
      uint_v m10 + uint_v m11 * pow2 64 == uint_v f2 * uint_v f3
    )  
)
  (ensures fun h0 c h1 -> modifies (loc result |+| loc memory |+| loc temp) h0 h1 /\
    (
      let f3 = Lib.Sequence.index (as_seq h0 f) 3 in 
      as_nat P256 h1 result + uint_v c * pow2 256 = uint_v f3 * as_nat P256 h0 f + as_nat P256 h0 f4
    )
  )


let sq3 f f4 result memory tempBuffer = 
  let h0 = ST.get() in 
  assert_norm (pow2 64 * pow2 64 * pow2 64 * pow2 64 = pow2 256);
  
  let temp = sub tempBuffer (size 0) (size 1) in 
  let tempBufferResult = sub tempBuffer  (size 1) (size 4) in 

  let f0 = index f (size 0) in 
  let f1 = index f (size 1) in 
  let f2 = index f (size 2) in 
  let f3 = index f (size 3) in 
    
  let o0 = sub tempBufferResult (size 0) (size 1) in 
  let o1 = sub tempBufferResult (size 1) (size 1) in 
  let o2 = sub tempBufferResult (size 2) (size 1) in 
  let o3 = sub tempBufferResult (size 3) (size 1) in 

  upd o0 (size 0) (index memory (size 4));
  let h = index memory (size 5) in 

  upd o1 (size 0) (index memory (size 8));
  let l = index o1 (size 0) in     
  let c1 = add_carry_u64 (u64 0) l h o1 in 
  let h = index memory (size 9) in 

  upd o2 (size 0) (index memory (size 10));
  let l = index o2 (size 0) in     
  let c2 = add_carry_u64 c1 l h o2 in
  let h = index memory (size 11) in 
  
  mul64 f3 f3 o3 temp;
  let l = index o3 (size 0) in     
  let c3 = add_carry_u64 c2 l h o3 in
  let h_3 = index temp (size 0) in 

    let h6 = ST.get() in 

    calc (==) {

    uint_v f3 * as_nat P256 h0 f;
    (==) {}
    uint_v f3 * (uint_v f0 +  uint_v f1 * pow2 64 + uint_v f2 * pow2 64 * pow2 64 + uint_v f3 * pow2 64 * pow2 64 * pow2 64);
    (==){
      assert_by_tactic (uint_v f3 * (uint_v f0 +  uint_v f1 * pow2 64 + uint_v f2 * pow2 64 * pow2 64 + uint_v f3 * pow2 64 * pow2 64 * pow2 64) == 
      uint_v f0 * uint_v f3 +  
      uint_v f1 * uint_v f3 * pow2 64 + 
      uint_v f2 * uint_v f3 * pow2 64 * pow2 64 + 
      uint_v f3 * uint_v f3 * pow2 64 * pow2 64 * pow2 64) canon}
   
   as_nat P256 h6 tempBufferResult + (uint_v c3 + uint_v h_3) * (pow2 64 * pow2 64 * pow2 64 * pow2 64);
   
   (==){assert_norm (pow2 64 * pow2 64 * pow2 64 * pow2 64 = pow2 256)}

   as_nat P256 h6 tempBufferResult + (uint_v c3 + uint_v h_3) * pow2 256;
   };

  let c4 = add4 tempBufferResult f4 result in 
  let h7 = ST.get() in 
    assert_by_tactic (uint_v c4 * pow2 256 + (uint_v c3 + uint_v h_3) * pow2 256 = 
    (uint_v c4 + uint_v c3 + uint_v h_3) * pow2 256) canon;
    lemma_320_1 (as_nat P256 h7 result) (uint_v c4 + uint_v c3 + uint_v h_3) (uint_v f3) (as_nat P256 h0 f) (as_nat P256 h0 f4);
  c3 +! h_3 +! c4


val square_p256: f: felem P256 -> out: widefelem P256 -> Stack unit
    (requires fun h -> live h out /\ live h f /\ eq_or_disjoint f out)
    (ensures  fun h0 _ h1 -> modifies (loc out) h0 h1 /\ wide_as_nat P256 h1 out = as_nat P256 h0 f * as_nat P256 h0 f)
      
let square_p256 f out =
  push_frame();
      assert_norm (pow2 64 * pow2 64 * pow2 64 * pow2 64 = pow2 256);

  let wb = create (size 17) (u64 0) in 
  
  let tb = sub wb (size 0) (size 5) in 
  let memory = sub wb (size 5) (size 12) in 
   
  let f0 = f.(0ul) in
  let f1 = f.(1ul) in
  let f2 = f.(2ul) in
  let f3 = f.(3ul) in
  let b0 = sub out (size 0) (size 4) in
    
    let h0 = ST.get() in
  let c0 = sq0 f b0 memory tb in 
 
    upd out (size 4) c0;
    let h1 = ST.get() in

    let bk0 = sub out (size 0) (size 1) in 
    assert(Lib.Sequence.index (as_seq h1 bk0) 0 == Lib.Sequence.index (as_seq h1 out) 0); 

  let b1 = sub out (size 1) (size 4) in
  let c1 = sq1 f b1 b1 memory tb in 
    upd out (size 5) c1; 
    let h2 = ST.get() in 
    
    let bk1 = sub out (size 0) (size 2) in 
    assert(Lib.Sequence.index (as_seq h2 bk1) 0 == Lib.Sequence.index (as_seq h2 out) 0);
    assert(Lib.Sequence.index (as_seq h2 bk1) 1 == Lib.Sequence.index (as_seq h2 out) 1);

  let b2 = sub out (size 2) (size 4) in 
  let c2 = sq2 f b2 b2 memory tb in 
    upd out (size 6) c2;

    let h3 = ST.get() in 
     let bk2 = sub out (size 0) (size 3) in 
     
    assert(Lib.Sequence.index (as_seq h3 bk2) 0 == Lib.Sequence.index (as_seq h3 out) 0);
    assert(Lib.Sequence.index (as_seq h3 bk2) 1 == Lib.Sequence.index (as_seq h3 out) 1);
    assert(Lib.Sequence.index (as_seq h3 bk2) 2 == Lib.Sequence.index (as_seq h3 out) 2);

  let b3 = sub out (size 3) (size 4) in 
  let c3 = sq3 f b3 b3 memory tb in 
    upd out (size 7) c3;

    let h4 = ST.get() in 

 assert(
    uint_v f0 * as_nat P256 h0 f + 
    uint_v f1 * as_nat P256 h0 f * pow2 64 + 
    uint_v f2 * as_nat P256 h0 f * pow2 64 * pow2 64 + 
    uint_v f3 * as_nat P256 h0 f * pow2 64 * pow2 64 * pow2 64 = 
    
    as_nat P256 h4 b3  * pow2 64 * pow2 64 * pow2 64 + 
    uint_v (Lib.Sequence.index (as_seq h4 out) 7) * pow2 64 * pow2 64 * pow2 64 * pow2 64  * pow2 64 * pow2 64 * pow2 64 +
    uint_v (Lib.Sequence.index (as_seq h4 out) 2) * pow2 64 * pow2 64 + 
    uint_v (Lib.Sequence.index (as_seq h4 out) 1) * pow2 64 + 
    uint_v (Lib.Sequence.index (as_seq h4 out) 0));

    calc (==) {
    
    as_nat P256 h4 b3  * pow2 64 * pow2 64 * pow2 64 + 
    uint_v (Lib.Sequence.index (as_seq h4 out) 7) * pow2 64 * pow2 64 * pow2 64 * pow2 64  * pow2 64 * pow2 64 * pow2 64 +
    uint_v (Lib.Sequence.index (as_seq h4 out) 2) * pow2 64 * pow2 64 + 
    uint_v (Lib.Sequence.index (as_seq h4 out) 1) * pow2 64 + 
    uint_v (Lib.Sequence.index (as_seq h4 out) 0);
    (==) {}
    wide_as_nat P256 h4 out;};

    calc (==)
    {

    as_nat P256 h0 f * as_nat P256 h0 f;
    (==) {assert_by_tactic (
    uint_v f0 * as_nat P256 h0 f + 
    uint_v f1 * as_nat P256 h0 f * pow2 64 + 
    uint_v f2 * as_nat P256 h0 f * pow2 64 * pow2 64 + 
    uint_v f3 * as_nat P256 h0 f * pow2 64 * pow2 64 * pow2 64 == as_nat P256 h0 f * (uint_v f0 + uint_v f1 * pow2 64 + uint_v f2 * pow2 64 * pow2 64 + uint_v f3 * pow2 64 * pow2 64 * pow2 64)) canon}

    uint_v f0 * as_nat P256 h0 f + 
    uint_v f1 * as_nat P256 h0 f * pow2 64 + 
    uint_v f2 * as_nat P256 h0 f * pow2 64 * pow2 64 + 
    uint_v f3 * as_nat P256 h0 f * pow2 64 * pow2 64 * pow2 64;
    };

  pop_frame()


inline_for_extraction noextract
val shortened_mul_p256: a: glbuffer uint64 (size 4) -> b: uint64 -> result: widefelem P256 -> Stack unit
  (requires fun h -> live h a /\ live h result /\ wide_as_nat P256 h result = 0)
  (ensures fun h0 _ h1 -> modifies (loc result) h0 h1 /\ 
    as_nat_il P256 h0 a * uint_v b = wide_as_nat P256 h1 result /\ 
    wide_as_nat P256 h1 result < getPower2 P256 * pow2 64)

let shortened_mul_p256 a b result = 
  let result04 = sub result (size 0) (size 4) in 
  let result48 = sub result (size 4) (size 4) in 
  let c = mul1_il a b result04 in 
    let h0 = ST.get() in 
  upd result (size 4) c;
  
    assert(Lib.Sequence.index (as_seq h0 result) 5 == Lib.Sequence.index (as_seq h0 result48) 1);
    assert(Lib.Sequence.index (as_seq h0 result) 6 == Lib.Sequence.index (as_seq h0 result48) 2);
    assert(Lib.Sequence.index (as_seq h0 result) 7 == Lib.Sequence.index (as_seq h0 result48) 3);

    assert_norm( pow2 64 * pow2 64 * pow2 64 * pow2 64 = pow2 256)
   



(* this piece of code is taken from Hacl.Curve25519 *)
(* I am not sure that it's used *)

inline_for_extraction noextract
val scalar_bit:
    #buf_type: buftype -> 
    s:lbuffer_t buf_type uint8 (size 32)
  -> n:size_t{v n < 256}
  -> Stack uint64
    (requires fun h0 -> live h0 s)
    (ensures  fun h0 r h1 -> h0 == h1 /\
      r == ith_bit #P256 (as_seq h0 s) (v n) /\ v r <= 1)
      
let scalar_bit #buf_type s n =
  let h0 = ST.get () in
  mod_mask_lemma ((Lib.Sequence.index (as_seq h0 s) (v n / 8)) >>. (n %. 8ul)) 1ul;
  assert_norm (1 = pow2 1 - 1);
  assert (v (mod_mask #U8 #SEC 1ul) == v (u8 1));
  to_u64 ((s.(n /. 8ul) >>. (n %. 8ul)) &. u8 1)
