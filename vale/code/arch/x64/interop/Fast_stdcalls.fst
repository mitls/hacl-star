module Fast_stdcalls

open FStar.HyperStack.ST
module HS = FStar.HyperStack
module B = LowStar.Buffer
module BV = LowStar.BufferView
open Types_s

open Interop.Base
module IX64 = Interop.X64
module VSig = Vale.AsLowStar.ValeSig
module LSig = Vale.AsLowStar.LowStarSig
module ME = X64.Memory
module V = X64.Vale.Decls
module IA = Interop.Assumptions
module W = Vale.AsLowStar.Wrapper
open X64.MemoryAdapters
module VS = X64.Vale.State
module MS = X64.Machine_s

module FU = X64.FastUtil

let b8 = B.buffer UInt8.t
let uint64 = UInt64.t


(* A little utility to trigger normalization in types *)
let as_t (#a:Type) (x:normal a) : a = x
let as_normal_t (#a:Type) (x:a) : normal a = x

[@__reduce__] unfold
let b64 = buf_t TUInt64
[@__reduce__] unfold
let t64_mod = TD_Buffer TUInt64 default_bq
[@__reduce__] unfold
let t64_no_mod = TD_Buffer TUInt64 ({modified=false; strict_disjointness=false; taint=MS.Secret})
[@__reduce__] unfold
let tuint64 = TD_Base TUInt64

[@__reduce__] unfold
let dom: IX64.arity_ok td =
  let y = [t64_mod; t64_no_mod; tuint64] in
  assert_norm (List.length y = 3);
  y

(* Need to rearrange the order of arguments *)
[@__reduce__]
let add1_pre : VSig.vale_pre 16 dom =
  fun (c:V.va_code)
    (out:b64)
    (f1:b64)
    (f2:uint64)
    (va_s0:V.va_state)
    (sb:IX64.stack_buffer 16) ->
      FU.va_req_fast_add1_stdcall c va_s0 IA.win (as_vale_buffer sb) 
        (as_vale_buffer out) (as_vale_buffer f1) (UInt64.v f2)

[@__reduce__]
let add1_post : VSig.vale_post 16 dom =
  fun (c:V.va_code)
    (out:b64)
    (f1:b64)
    (f2:uint64)
    (va_s0:V.va_state)
    (sb:IX64.stack_buffer 16)
    (va_s1:V.va_state)
    (f:V.va_fuel) ->
      FU.va_ens_fast_add1_stdcall c va_s0 IA.win (as_vale_buffer sb) (as_vale_buffer out) (as_vale_buffer f1) (UInt64.v f2) va_s1 f

module VS = X64.Vale.State

#set-options "--z3rlimit 20"

[@__reduce__] unfold
let add1_lemma'
    (code:V.va_code)
    (_win:bool)
    (out:b64)
    (f1:b64)
    (f2:uint64)
    (va_s0:V.va_state)
    (sb:IX64.stack_buffer 16)
 : Ghost (V.va_state & V.va_fuel)
     (requires
       add1_pre code out f1 f2 va_s0 sb)
     (ensures (fun (va_s1, f) ->
       V.eval_code code va_s0 f va_s1 /\
       VSig.vale_calling_conventions va_s0 va_s1 /\
       add1_post code out f1 f2 va_s0 sb va_s1 f /\
       ME.buffer_readable VS.(va_s1.mem) (as_vale_buffer f1) /\
       ME.buffer_readable VS.(va_s1.mem) (as_vale_buffer out) /\ 
       ME.buffer_writeable (as_vale_buffer out) /\ 
       ME.buffer_writeable (as_vale_buffer f1) /\ 
       ME.modifies (ME.loc_union (ME.loc_buffer (as_vale_buffer sb))
                   (ME.loc_union (ME.loc_buffer (as_vale_buffer out))
                                 ME.loc_none)) va_s0.VS.mem va_s1.VS.mem
 )) = 
   let va_s1, f = FU.va_lemma_fast_add1_stdcall code va_s0 IA.win (as_vale_buffer sb) (as_vale_buffer out) (as_vale_buffer f1) (UInt64.v f2) in
   Vale.AsLowStar.MemoryHelpers.buffer_writeable_reveal ME.TUInt64 out;   
   Vale.AsLowStar.MemoryHelpers.buffer_writeable_reveal ME.TUInt64 f1;   
   va_s1, f                                   

(* Prove that add1_lemma' has the required type *)
let add1_lemma = as_t #(VSig.vale_sig add1_pre add1_post) add1_lemma'

let code_add1 = FU.va_code_fast_add1_stdcall IA.win

(* Here's the type expected for the add1 wrapper *)
[@__reduce__]
let lowstar_add1_t =
  IX64.as_lowstar_sig_t_weak
    Interop.down_mem
    code_add1
    16
    dom
    []
    _
    _
    (W.mk_prediction code_add1 dom [] (add1_lemma code_add1 IA.win))

(* And here's the add1 wrapper itself *)
let lowstar_add1 : lowstar_add1_t  =
  IX64.wrap
    Interop.down_mem
    code_add1
    16
    dom
    (W.mk_prediction code_add1 dom [] (add1_lemma code_add1 IA.win))

let lowstar_add1_normal_t : normal lowstar_add1_t
  = as_normal_t #lowstar_add1_t lowstar_add1

module T = FStar.Tactics

open X64.CPU_Features_s

open Fast_defs
open Vale.AsLowStar.MemoryHelpers

let fast_add1
  (out:b8)
  (f1:b8)
  (f2:uint64) 
  : Stack uint64
  (requires fun h -> 
    adx_enabled /\ bmi2_enabled /\
    B.live h f1 /\ 
    B.live h out /\ 
    B.length out == 32 /\ 
    B.length f1 == 32 /\
    (B.disjoint out f1 \/ out == f1))
  (ensures fun h0 c h1 -> 
    B.live h1 out /\ B.live h1 f1 /\
    B.modifies (B.loc_buffer out) h0 h1 /\
    (
    let a0 = UInt64.v (low_buffer64_read h0 f1 0) in
    let a1 = UInt64.v (low_buffer64_read h0 f1 1) in
    let a2 = UInt64.v (low_buffer64_read h0 f1 2) in
    let a3 = UInt64.v (low_buffer64_read h0 f1 3) in    
    let d0 = UInt64.v (low_buffer64_read h1 out 0) in
    let d1 = UInt64.v (low_buffer64_read h1 out 1) in
    let d2 = UInt64.v (low_buffer64_read h1 out 2) in
    let d3 = UInt64.v (low_buffer64_read h1 out 3) in
    let a = pow2_four a0 a1 a2 a3 in
    let d = pow2_five d0 d1 d2 d3 (UInt64.v c) in
    d = a + UInt64.v f2
    )
    )
  = 
  let x, _ = lowstar_add1_normal_t out f1 f2 () in
  x
  
let as_nat (b:B.buffer UInt64.t{B.length b == 4}) (h:HS.mem) : GTot nat =
  let s = B.as_seq h b in
  let s0 = UInt64.v (Seq.index s 0) in
  let s1 = UInt64.v (Seq.index s 1) in
  let s2 = UInt64.v (Seq.index s 2) in
  let s3 = UInt64.v (Seq.index s 3) in
  pow2_four s0 s1 s2 s3

unfold
let u256 = b:B.buffer UInt64.t{B.length b == 4}


let ghost64_8 (b:u256) (b8:b8{B.length b8 == 32}) : Stack unit
  (requires fun h -> B.live h b /\ B.live h b8)
  (ensures fun h0 _ h -> 
    B.live h b /\ B.live h b8 /\ 
    B.modifies (B.loc_buffer b8) h0 h /\
    (forall (i:nat{i < 4}). {:pattern low_buffer64_read h b8 i} Seq.index (B.as_seq h0 b) i == low_buffer64_read h b8 i)) = admit()

let ghost8_64 (b:u256) (b8:b8{B.length b8 == 32}) : Stack unit
  (requires fun _ -> True)
  (ensures fun h0 _ h -> B.live h b /\ B.live h0 b8 /\ B.modifies (B.loc_buffer b) h0 h /\
    (forall (i:nat{i < 4}). Seq.index (B.as_seq h b) i == low_buffer64_read h0 b8 i)) = admit()

let ghost8_64_imm (b:u256) (b8:b8{B.length b8 == 32}) : Stack unit
  (requires fun h -> B.live h b /\ B.live h b8 /\
    (forall (i:nat{i < 4}). Seq.index (B.as_seq h b) i == low_buffer64_read h b8 i))
  (ensures fun h0 _ h -> h0 == h) = admit()


let fast_add1_test
  (out64:u256)
  (f164:u256)
  (out:b8)
  (f1:b8)
  (f2:uint64) 
  : Stack uint64
  (requires fun h -> 
    adx_enabled /\ bmi2_enabled /\
    B.live h f1 /\ 
    B.live h out /\ 
    B.live h f164 /\
    B.live h out64 /\
    B.length out == 32 /\ 
    B.length f1 == 32 /\
    (B.disjoint out f1) /\
    B.disjoint out64 out /\
    B.disjoint out64 f1 /\
    B.disjoint f164 out /\
    B.disjoint f164 f1 /\
    B.disjoint f164 out64
    )
  (ensures fun h0 c h1 -> 
    B.live h1 out64 /\ B.live h1 f164 /\
    B.modifies (B.loc_union
      (B.loc_union (B.loc_buffer out64) (B.loc_buffer f1))
      (B.loc_buffer out)) h0 h1 /\
    as_nat out64 h1 + pow2_256 `op_Multiply` UInt64.v c == as_nat f164 h0 + UInt64.v f2)
  = ghost64_8 out64 out;
    ghost64_8 f164 f1;
    let x = fast_add1 out f1 f2 in
    ghost8_64_imm f164 f1;
    ghost8_64 out64 out;
    x

let final_fast_add1
  (out:u256)
  (f1:u256)
  (f2:uint64)
  : Stack uint64
  (requires fun h ->
    adx_enabled /\ bmi2_enabled /\
    B.live h out /\ B.live h f1 /\
    B.disjoint out f1)
  (ensures fun h0 c h1 ->
    B.live h1 out /\ B.live h1 f1 /\
    B.modifies (B.loc_buffer out) h0 h1 /\
    as_nat out h1 + pow2_256 `op_Multiply` UInt64.v c == as_nat f1 h0 + UInt64.v f2)
  = push_frame();
    let out8 = B.alloca (UInt8.uint_to_t 0) (UInt32.uint_to_t 32) in
    let f18 = B.alloca (UInt8.uint_to_t 0) (UInt32.uint_to_t 32) in
    let x = fast_add1_test out f1 out8 f18 f2 in
    pop_frame();
    x
    
