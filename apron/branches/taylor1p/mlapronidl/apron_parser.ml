type token =
  | TK_EOF
  | TK_VERTEX
  | TK_RAY
  | TK_LINE
  | TK_RAYMOD
  | TK_LINEMOD
  | TK_SUPEG
  | TK_INFEG
  | TK_SUP
  | TK_INF
  | TK_EG
  | TK_DISEG
  | TK_MOD
  | TK_LBRACKET
  | TK_RBRACKET
  | TK_SEMICOLON
  | TK_LPAR
  | TK_RPAR
  | TK_MUL of ((Texpr1.typ * Texpr1.round))
  | TK_ADD of ((Texpr1.typ * Texpr1.round))
  | TK_SUB of ((Texpr1.typ * Texpr1.round))
  | TK_DIV of ((Texpr1.typ * Texpr1.round))
  | TK_MODULO of ((Texpr1.typ * Texpr1.round))
  | TK_CAST of ((Texpr1.typ * Texpr1.round))
  | TK_SQRT of ((Texpr1.typ * Texpr1.round))
  | TK_MPQF of (Mpqf.t)
  | TK_FLOAT of (float)
  | TK_VAR of (string)

open Parsing;;
# 6 "apron_parser.mly"
(* This file is part of the APRON Library, released under LGPL license.
   Please read the COPYING file packaged in the distribution  *)

let rec neg acc = function
  | [] -> acc
  | (var,coeff)::l ->
      let nacc = 
      if Coeff.is_zero coeff then
	acc
      else
	(var,(Coeff.neg coeff))::acc
      in
      neg nacc l

# 48 "apron_parser.ml"
let yytransl_const = [|
  257 (* TK_EOF *);
  258 (* TK_VERTEX *);
  259 (* TK_RAY *);
  260 (* TK_LINE *);
  261 (* TK_RAYMOD *);
  262 (* TK_LINEMOD *);
  263 (* TK_SUPEG *);
  264 (* TK_INFEG *);
  265 (* TK_SUP *);
  266 (* TK_INF *);
  267 (* TK_EG *);
  268 (* TK_DISEG *);
  269 (* TK_MOD *);
  270 (* TK_LBRACKET *);
  271 (* TK_RBRACKET *);
  272 (* TK_SEMICOLON *);
  273 (* TK_LPAR *);
  274 (* TK_RPAR *);
    0|]

let yytransl_block = [|
  275 (* TK_MUL *);
  276 (* TK_ADD *);
  277 (* TK_SUB *);
  278 (* TK_DIV *);
  279 (* TK_MODULO *);
  280 (* TK_CAST *);
  281 (* TK_SQRT *);
  282 (* TK_MPQF *);
  283 (* TK_FLOAT *);
  284 (* TK_VAR *);
    0|]

let yylhs = "\255\255\
\001\000\001\000\001\000\001\000\001\000\001\000\001\000\002\000\
\002\000\002\000\002\000\002\000\003\000\006\000\006\000\006\000\
\008\000\008\000\008\000\008\000\008\000\004\000\011\000\011\000\
\011\000\011\000\011\000\011\000\011\000\005\000\012\000\012\000\
\012\000\013\000\013\000\013\000\013\000\014\000\014\000\015\000\
\015\000\015\000\015\000\015\000\010\000\007\000\007\000\017\000\
\017\000\016\000\016\000\009\000\009\000\000\000\000\000\000\000\
\000\000\000\000"

let yylen = "\002\000\
\004\000\006\000\004\000\004\000\004\000\004\000\004\000\003\000\
\003\000\003\000\003\000\003\000\002\000\003\000\003\000\001\000\
\002\000\003\000\001\000\001\000\002\000\002\000\003\000\005\000\
\003\000\003\000\003\000\003\000\003\000\002\000\003\000\003\000\
\001\000\003\000\003\000\003\000\001\000\002\000\001\000\002\000\
\002\000\003\000\001\000\001\000\001\000\001\000\001\000\001\000\
\002\000\001\000\005\000\001\000\002\000\002\000\002\000\002\000\
\002\000\002\000"

let yydefred = "\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\046\000\047\000\045\000\054\000\000\000\050\000\016\000\000\000\
\020\000\052\000\000\000\000\000\000\000\000\000\000\000\055\000\
\056\000\000\000\000\000\000\000\000\000\000\000\057\000\044\000\
\000\000\000\000\000\000\037\000\039\000\043\000\058\000\000\000\
\000\000\048\000\000\000\021\000\053\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\017\000\000\000\
\000\000\000\000\000\000\000\000\013\000\000\000\038\000\040\000\
\041\000\022\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\030\000\049\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\014\000\015\000\
\018\000\008\000\009\000\010\000\011\000\012\000\042\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\034\000\
\035\000\036\000\000\000\005\000\006\000\004\000\007\000\001\000\
\000\000\003\000\000\000\051\000\000\000\024\000\002\000"

let yydgoto = "\006\000\
\012\000\024\000\025\000\031\000\039\000\013\000\014\000\015\000\
\016\000\032\000\033\000\034\000\035\000\036\000\037\000\038\000\
\043\000"

let yysindex = "\238\000\
\205\255\242\255\205\255\246\254\246\254\000\000\169\255\201\255\
\000\000\000\000\000\000\000\000\177\255\000\000\000\000\247\254\
\000\000\000\000\205\255\205\255\205\255\205\255\205\255\000\000\
\000\000\011\255\246\254\246\254\196\255\196\255\000\000\000\000\
\004\255\197\255\230\255\000\000\000\000\000\000\000\000\074\255\
\232\254\000\000\249\254\000\000\000\000\205\255\205\255\205\255\
\205\255\205\255\205\255\205\255\205\255\248\254\000\000\082\255\
\084\255\091\255\112\255\156\255\000\000\024\255\000\000\000\000\
\000\000\000\000\246\254\246\254\246\254\246\254\246\254\246\254\
\246\254\246\254\246\254\246\254\246\254\000\000\000\000\169\255\
\161\255\171\255\173\255\179\255\061\255\182\255\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\040\255\
\040\255\040\255\040\255\217\255\040\255\230\255\230\255\000\000\
\000\000\000\000\007\255\000\000\000\000\000\000\000\000\000\000\
\232\254\000\000\232\254\000\000\032\255\000\000\000\000"

let yyrindex = "\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\158\255\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\028\255\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\042\255\
\046\255\049\255\069\255\071\255\083\255\128\255\143\255\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\000\
\000\000\000\000\000\000\000\000\000\000\000\000\000\000"

let yygindex = "\000\000\
\000\000\000\000\000\000\000\000\000\000\077\000\249\255\057\000\
\000\000\005\000\000\000\252\255\069\000\002\000\101\000\068\000\
\006\000"

let yytablesize = 253
let yytable = "\042\000\
\040\000\009\000\010\000\007\000\066\000\017\000\027\000\017\000\
\080\000\054\000\028\000\061\000\044\000\029\000\030\000\009\000\
\010\000\011\000\011\000\011\000\055\000\116\000\062\000\017\000\
\017\000\017\000\017\000\017\000\033\000\063\000\052\000\053\000\
\119\000\079\000\033\000\033\000\033\000\033\000\033\000\033\000\
\033\000\095\000\027\000\073\000\074\000\033\000\028\000\033\000\
\033\000\026\000\017\000\017\000\017\000\017\000\017\000\017\000\
\017\000\017\000\089\000\073\000\074\000\112\000\096\000\097\000\
\098\000\099\000\100\000\101\000\018\000\029\000\018\000\023\000\
\042\000\113\000\078\000\045\000\104\000\105\000\106\000\026\000\
\052\000\053\000\090\000\025\000\091\000\107\000\018\000\018\000\
\018\000\018\000\018\000\092\000\000\000\073\000\074\000\056\000\
\057\000\058\000\059\000\060\000\000\000\052\000\053\000\052\000\
\053\000\117\000\000\000\118\000\087\000\088\000\052\000\053\000\
\093\000\018\000\018\000\018\000\018\000\018\000\018\000\018\000\
\018\000\000\000\081\000\082\000\083\000\084\000\085\000\086\000\
\031\000\064\000\065\000\052\000\053\000\000\000\031\000\031\000\
\031\000\031\000\031\000\031\000\031\000\102\000\103\000\032\000\
\000\000\031\000\000\000\031\000\031\000\032\000\032\000\032\000\
\032\000\032\000\032\000\032\000\094\000\000\000\019\000\000\000\
\032\000\108\000\032\000\032\000\019\000\019\000\019\000\019\000\
\019\000\019\000\019\000\109\000\000\000\110\000\000\000\052\000\
\053\000\019\000\019\000\111\000\052\000\053\000\114\000\046\000\
\047\000\048\000\049\000\050\000\051\000\041\000\052\000\053\000\
\052\000\053\000\009\000\010\000\052\000\053\000\052\000\053\000\
\000\000\052\000\053\000\067\000\068\000\069\000\070\000\071\000\
\072\000\007\000\000\000\000\000\027\000\000\000\007\000\000\000\
\073\000\074\000\007\000\029\000\030\000\009\000\010\000\011\000\
\000\000\008\000\009\000\010\000\011\000\115\000\009\000\010\000\
\011\000\000\000\000\000\000\000\073\000\074\000\001\000\002\000\
\003\000\004\000\005\000\019\000\020\000\021\000\022\000\023\000\
\075\000\000\000\000\000\076\000\077\000"

let yycheck = "\007\000\
\005\000\026\001\027\001\014\001\001\001\001\000\017\001\003\000\
\016\001\019\001\021\001\001\001\008\000\024\001\025\001\026\001\
\027\001\028\001\028\001\028\001\016\000\015\001\027\000\019\000\
\020\000\021\000\022\000\023\000\001\001\028\000\020\001\021\001\
\001\001\041\000\007\001\008\001\009\001\010\001\011\001\012\001\
\013\001\018\001\001\001\020\001\021\001\018\001\001\001\020\001\
\021\001\001\001\046\000\047\000\048\000\049\000\050\000\051\000\
\052\000\053\000\054\000\020\001\021\001\001\001\067\000\068\000\
\069\000\070\000\071\000\072\000\001\000\001\001\003\000\001\001\
\080\000\013\001\001\001\008\000\075\000\076\000\077\000\003\000\
\020\001\021\001\001\001\001\001\001\001\080\000\019\000\020\000\
\021\000\022\000\023\000\001\001\255\255\020\001\021\001\019\000\
\020\000\021\000\022\000\023\000\255\255\020\001\021\001\020\001\
\021\001\113\000\255\255\115\000\052\000\053\000\020\001\021\001\
\001\001\046\000\047\000\048\000\049\000\050\000\051\000\052\000\
\053\000\255\255\046\000\047\000\048\000\049\000\050\000\051\000\
\001\001\029\000\030\000\020\001\021\001\255\255\007\001\008\001\
\009\001\010\001\011\001\012\001\013\001\073\000\074\000\001\001\
\255\255\018\001\255\255\020\001\021\001\007\001\008\001\009\001\
\010\001\011\001\012\001\013\001\001\001\255\255\001\001\255\255\
\018\001\001\001\020\001\021\001\007\001\008\001\009\001\010\001\
\011\001\012\001\013\001\001\001\255\255\001\001\255\255\020\001\
\021\001\020\001\021\001\001\001\020\001\021\001\001\001\007\001\
\008\001\009\001\010\001\011\001\012\001\021\001\020\001\021\001\
\020\001\021\001\026\001\027\001\020\001\021\001\020\001\021\001\
\255\255\020\001\021\001\007\001\008\001\009\001\010\001\011\001\
\012\001\014\001\255\255\255\255\017\001\255\255\014\001\255\255\
\020\001\021\001\014\001\024\001\025\001\026\001\027\001\028\001\
\255\255\021\001\026\001\027\001\028\001\013\001\026\001\027\001\
\028\001\255\255\255\255\255\255\020\001\021\001\001\000\002\000\
\003\000\004\000\005\000\002\001\003\001\004\001\005\001\006\001\
\019\001\255\255\255\255\022\001\023\001"

let yynames_const = "\
  TK_EOF\000\
  TK_VERTEX\000\
  TK_RAY\000\
  TK_LINE\000\
  TK_RAYMOD\000\
  TK_LINEMOD\000\
  TK_SUPEG\000\
  TK_INFEG\000\
  TK_SUP\000\
  TK_INF\000\
  TK_EG\000\
  TK_DISEG\000\
  TK_MOD\000\
  TK_LBRACKET\000\
  TK_RBRACKET\000\
  TK_SEMICOLON\000\
  TK_LPAR\000\
  TK_RPAR\000\
  "

let yynames_block = "\
  TK_MUL\000\
  TK_ADD\000\
  TK_SUB\000\
  TK_DIV\000\
  TK_MODULO\000\
  TK_CAST\000\
  TK_SQRT\000\
  TK_MPQF\000\
  TK_FLOAT\000\
  TK_VAR\000\
  "

let yyact = [|
  (fun _ -> failwith "parser")
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 3 : 'linexpr0) in
    let _3 = (Parsing.peek_val __caml_parser_env 1 : 'linexpr0) in
    Obj.repr(
# 59 "apron_parser.mly"
                                 ( (Lincons0.EQ, neg _1 _3) )
# 275 "apron_parser.ml"
               : Lincons0.typ * (string*Coeff.t) list))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 5 : 'linexpr0) in
    let _3 = (Parsing.peek_val __caml_parser_env 3 : 'linexpr0) in
    let _5 = (Parsing.peek_val __caml_parser_env 1 : 'scalar0) in
    Obj.repr(
# 60 "apron_parser.mly"
                                                ( (Lincons0.EQMOD(_5), neg _1 _3) )
# 284 "apron_parser.ml"
               : Lincons0.typ * (string*Coeff.t) list))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 3 : 'linexpr0) in
    let _3 = (Parsing.peek_val __caml_parser_env 1 : 'linexpr0) in
    Obj.repr(
# 61 "apron_parser.mly"
                                    ( failwith "!= not yet supported" )
# 292 "apron_parser.ml"
               : Lincons0.typ * (string*Coeff.t) list))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 3 : 'linexpr0) in
    let _3 = (Parsing.peek_val __caml_parser_env 1 : 'linexpr0) in
    Obj.repr(
# 62 "apron_parser.mly"
                                  ( (Lincons0.SUP, neg _1 _3) )
# 300 "apron_parser.ml"
               : Lincons0.typ * (string*Coeff.t) list))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 3 : 'linexpr0) in
    let _3 = (Parsing.peek_val __caml_parser_env 1 : 'linexpr0) in
    Obj.repr(
# 63 "apron_parser.mly"
                                    ( (Lincons0.SUPEQ, neg _1 _3) )
# 308 "apron_parser.ml"
               : Lincons0.typ * (string*Coeff.t) list))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 3 : 'linexpr0) in
    let _3 = (Parsing.peek_val __caml_parser_env 1 : 'linexpr0) in
    Obj.repr(
# 64 "apron_parser.mly"
                                    ( (Lincons0.SUPEQ, neg _3 _1) )
# 316 "apron_parser.ml"
               : Lincons0.typ * (string*Coeff.t) list))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 3 : 'linexpr0) in
    let _3 = (Parsing.peek_val __caml_parser_env 1 : 'linexpr0) in
    Obj.repr(
# 65 "apron_parser.mly"
                                  ( (Lincons0.SUP, neg _3 _1) )
# 324 "apron_parser.ml"
               : Lincons0.typ * (string*Coeff.t) list))
; (fun __caml_parser_env ->
    let _2 = (Parsing.peek_val __caml_parser_env 1 : 'linexpr0) in
    Obj.repr(
# 68 "apron_parser.mly"
                            ( (Generator0.VERTEX,_2) )
# 331 "apron_parser.ml"
               : Generator0.typ * (string*Coeff.t) list))
; (fun __caml_parser_env ->
    let _2 = (Parsing.peek_val __caml_parser_env 1 : 'linexpr0) in
    Obj.repr(
# 69 "apron_parser.mly"
                         ( (Generator0.RAY,_2) )
# 338 "apron_parser.ml"
               : Generator0.typ * (string*Coeff.t) list))
; (fun __caml_parser_env ->
    let _2 = (Parsing.peek_val __caml_parser_env 1 : 'linexpr0) in
    Obj.repr(
# 70 "apron_parser.mly"
                          ( (Generator0.LINE,_2) )
# 345 "apron_parser.ml"
               : Generator0.typ * (string*Coeff.t) list))
; (fun __caml_parser_env ->
    let _2 = (Parsing.peek_val __caml_parser_env 1 : 'linexpr0) in
    Obj.repr(
# 71 "apron_parser.mly"
                            ( (Generator0.RAYMOD,_2) )
# 352 "apron_parser.ml"
               : Generator0.typ * (string*Coeff.t) list))
; (fun __caml_parser_env ->
    let _2 = (Parsing.peek_val __caml_parser_env 1 : 'linexpr0) in
    Obj.repr(
# 72 "apron_parser.mly"
                             ( (Generator0.LINEMOD,_2) )
# 359 "apron_parser.ml"
               : Generator0.typ * (string*Coeff.t) list))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 1 : 'linexpr0) in
    Obj.repr(
# 75 "apron_parser.mly"
                  ( _1 )
# 366 "apron_parser.ml"
               : (string*Coeff.t) list))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 2 : 'linexpr0) in
    let _2 = (Parsing.peek_val __caml_parser_env 1 : (Texpr1.typ * Texpr1.round)) in
    let _3 = (Parsing.peek_val __caml_parser_env 0 : 'term) in
    Obj.repr(
# 79 "apron_parser.mly"
    ( _3::_1 )
# 375 "apron_parser.ml"
               : 'linexpr0))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 2 : 'linexpr0) in
    let _2 = (Parsing.peek_val __caml_parser_env 1 : (Texpr1.typ * Texpr1.round)) in
    let _3 = (Parsing.peek_val __caml_parser_env 0 : 'term) in
    Obj.repr(
# 81 "apron_parser.mly"
( let (var,coeff) = _3 in (var,Coeff.neg coeff)::_1 )
# 384 "apron_parser.ml"
               : 'linexpr0))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 0 : 'term) in
    Obj.repr(
# 82 "apron_parser.mly"
       ( [_1] )
# 391 "apron_parser.ml"
               : 'linexpr0))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 1 : 'coeff) in
    let _2 = (Parsing.peek_val __caml_parser_env 0 : 'id) in
    Obj.repr(
# 84 "apron_parser.mly"
           ( (_2,_1) )
# 399 "apron_parser.ml"
               : 'term))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 2 : 'coeff) in
    let _2 = (Parsing.peek_val __caml_parser_env 1 : (Texpr1.typ * Texpr1.round)) in
    let _3 = (Parsing.peek_val __caml_parser_env 0 : 'id) in
    Obj.repr(
# 85 "apron_parser.mly"
                  ( (_3,_1) )
# 408 "apron_parser.ml"
               : 'term))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 0 : 'coeff) in
    Obj.repr(
# 86 "apron_parser.mly"
        ( ("",_1) )
# 415 "apron_parser.ml"
               : 'term))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 0 : 'id) in
    Obj.repr(
# 87 "apron_parser.mly"
     ( (_1, Coeff.s_of_int 1) )
# 422 "apron_parser.ml"
               : 'term))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 1 : (Texpr1.typ * Texpr1.round)) in
    let _2 = (Parsing.peek_val __caml_parser_env 0 : 'id) in
    Obj.repr(
# 88 "apron_parser.mly"
            ( (_2, Coeff.s_of_int (-1)) )
# 430 "apron_parser.ml"
               : 'term))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 1 : 'tcons0) in
    Obj.repr(
# 91 "apron_parser.mly"
                ( _1 )
# 437 "apron_parser.ml"
               : Tcons0.typ * Texpr1.expr))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 2 : 'texpr0) in
    let _3 = (Parsing.peek_val __caml_parser_env 0 : 'texpr0) in
    Obj.repr(
# 94 "apron_parser.mly"
                      ( (Tcons0.EQ, (Texpr1.Binop (Texpr1.Sub,_1,_3,Texpr1.Real,Texpr1.Rnd))) )
# 445 "apron_parser.ml"
               : 'tcons0))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 4 : 'texpr0) in
    let _3 = (Parsing.peek_val __caml_parser_env 2 : 'texpr0) in
    let _5 = (Parsing.peek_val __caml_parser_env 0 : 'scalar0) in
    Obj.repr(
# 95 "apron_parser.mly"
                                     ( (Tcons0.EQMOD(_5), (Texpr1.Binop (Texpr1.Sub,_1,_3,Texpr1.Real,Texpr1.Rnd))) )
# 454 "apron_parser.ml"
               : 'tcons0))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 2 : 'texpr0) in
    let _3 = (Parsing.peek_val __caml_parser_env 0 : 'texpr0) in
    Obj.repr(
# 96 "apron_parser.mly"
                         ( failwith "!= not yet supported" )
# 462 "apron_parser.ml"
               : 'tcons0))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 2 : 'texpr0) in
    let _3 = (Parsing.peek_val __caml_parser_env 0 : 'texpr0) in
    Obj.repr(
# 97 "apron_parser.mly"
                       ( (Tcons0.SUP, (Texpr1.Binop (Texpr1.Sub,_1,_3,Texpr1.Real,Texpr1.Rnd))) )
# 470 "apron_parser.ml"
               : 'tcons0))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 2 : 'texpr0) in
    let _3 = (Parsing.peek_val __caml_parser_env 0 : 'texpr0) in
    Obj.repr(
# 98 "apron_parser.mly"
                         ( (Tcons0.SUPEQ, (Texpr1.Binop (Texpr1.Sub,_1,_3,Texpr1.Real,Texpr1.Rnd))) )
# 478 "apron_parser.ml"
               : 'tcons0))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 2 : 'texpr0) in
    let _3 = (Parsing.peek_val __caml_parser_env 0 : 'texpr0) in
    Obj.repr(
# 99 "apron_parser.mly"
                         ( (Tcons0.SUPEQ, (Texpr1.Binop (Texpr1.Sub,_3,_1,Texpr1.Real,Texpr1.Rnd))) )
# 486 "apron_parser.ml"
               : 'tcons0))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 2 : 'texpr0) in
    let _3 = (Parsing.peek_val __caml_parser_env 0 : 'texpr0) in
    Obj.repr(
# 100 "apron_parser.mly"
                       ( (Tcons0.SUP, (Texpr1.Binop (Texpr1.Sub,_3,_1,Texpr1.Real,Texpr1.Rnd))) )
# 494 "apron_parser.ml"
               : 'tcons0))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 1 : 'texpr0) in
    Obj.repr(
# 103 "apron_parser.mly"
                ( _1 )
# 501 "apron_parser.ml"
               : Texpr1.expr))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 2 : 'texpr0) in
    let _2 = (Parsing.peek_val __caml_parser_env 1 : (Texpr1.typ * Texpr1.round)) in
    let _3 = (Parsing.peek_val __caml_parser_env 0 : 'texpr0_1) in
    Obj.repr(
# 107 "apron_parser.mly"
    ( let (t,r) = _2 in Texpr1.Binop(Texpr1.Add,_1,_3,t,r) )
# 510 "apron_parser.ml"
               : 'texpr0))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 2 : 'texpr0) in
    let _2 = (Parsing.peek_val __caml_parser_env 1 : (Texpr1.typ * Texpr1.round)) in
    let _3 = (Parsing.peek_val __caml_parser_env 0 : 'texpr0_1) in
    Obj.repr(
# 109 "apron_parser.mly"
    ( let (t,r) = _2 in Texpr1.Binop(Texpr1.Sub,_1,_3,t,r) )
# 519 "apron_parser.ml"
               : 'texpr0))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 0 : 'texpr0_1) in
    Obj.repr(
# 111 "apron_parser.mly"
    ( _1 )
# 526 "apron_parser.ml"
               : 'texpr0))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 2 : 'texpr0_1) in
    let _2 = (Parsing.peek_val __caml_parser_env 1 : (Texpr1.typ * Texpr1.round)) in
    let _3 = (Parsing.peek_val __caml_parser_env 0 : 'texpr0_2) in
    Obj.repr(
# 115 "apron_parser.mly"
    ( let (t,r) = _2 in Texpr1.Binop(Texpr1.Mul,_1,_3,t,r) )
# 535 "apron_parser.ml"
               : 'texpr0_1))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 2 : 'texpr0_1) in
    let _2 = (Parsing.peek_val __caml_parser_env 1 : (Texpr1.typ * Texpr1.round)) in
    let _3 = (Parsing.peek_val __caml_parser_env 0 : 'texpr0_2) in
    Obj.repr(
# 117 "apron_parser.mly"
    ( let (t,r) = _2 in Texpr1.Binop(Texpr1.Div,_1,_3,t,r) )
# 544 "apron_parser.ml"
               : 'texpr0_1))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 2 : 'texpr0_1) in
    let _2 = (Parsing.peek_val __caml_parser_env 1 : (Texpr1.typ * Texpr1.round)) in
    let _3 = (Parsing.peek_val __caml_parser_env 0 : 'texpr0_2) in
    Obj.repr(
# 119 "apron_parser.mly"
    ( let (t,r) = _2 in Texpr1.Binop(Texpr1.Div,_1,_3,t,r) )
# 553 "apron_parser.ml"
               : 'texpr0_1))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 0 : 'texpr0_2) in
    Obj.repr(
# 121 "apron_parser.mly"
    ( _1 )
# 560 "apron_parser.ml"
               : 'texpr0_1))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 1 : (Texpr1.typ * Texpr1.round)) in
    let _2 = (Parsing.peek_val __caml_parser_env 0 : 'texpr0_2) in
    Obj.repr(
# 124 "apron_parser.mly"
    ( let (t,r) = _1 in Texpr1.Unop(Texpr1.Neg,_2,t,r) )
# 568 "apron_parser.ml"
               : 'texpr0_2))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 0 : 'texpr0_3) in
    Obj.repr(
# 126 "apron_parser.mly"
    ( _1 )
# 575 "apron_parser.ml"
               : 'texpr0_2))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 1 : (Texpr1.typ * Texpr1.round)) in
    let _2 = (Parsing.peek_val __caml_parser_env 0 : 'texpr0_3) in
    Obj.repr(
# 129 "apron_parser.mly"
    ( let (t,r) = _1 in Texpr1.Unop(Texpr1.Cast,_2,t,r) )
# 583 "apron_parser.ml"
               : 'texpr0_3))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 1 : (Texpr1.typ * Texpr1.round)) in
    let _2 = (Parsing.peek_val __caml_parser_env 0 : 'texpr0_3) in
    Obj.repr(
# 131 "apron_parser.mly"
    ( let (t,r) = _1 in Texpr1.Unop(Texpr1.Sqrt,_2,t,r) )
# 591 "apron_parser.ml"
               : 'texpr0_3))
; (fun __caml_parser_env ->
    let _2 = (Parsing.peek_val __caml_parser_env 1 : 'texpr0) in
    Obj.repr(
# 133 "apron_parser.mly"
    ( _2 )
# 598 "apron_parser.ml"
               : 'texpr0_3))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 0 : 'coeff0) in
    Obj.repr(
# 135 "apron_parser.mly"
    ( Texpr1.Cst(_1) )
# 605 "apron_parser.ml"
               : 'texpr0_3))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 0 : 'id) in
    Obj.repr(
# 137 "apron_parser.mly"
    ( Texpr1.Var(Var.of_string _1) )
# 612 "apron_parser.ml"
               : 'texpr0_3))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 0 : string) in
    Obj.repr(
# 140 "apron_parser.mly"
         ( _1 )
# 619 "apron_parser.ml"
               : 'id))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 0 : Mpqf.t) in
    Obj.repr(
# 143 "apron_parser.mly"
          ( Scalar.Mpqf(_1) )
# 626 "apron_parser.ml"
               : 'scalar0))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 0 : float) in
    Obj.repr(
# 144 "apron_parser.mly"
           ( Scalar.Float(_1) )
# 633 "apron_parser.ml"
               : 'scalar0))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 0 : 'scalar0) in
    Obj.repr(
# 146 "apron_parser.mly"
          ( _1 )
# 640 "apron_parser.ml"
               : 'scalar))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 1 : (Texpr1.typ * Texpr1.round)) in
    let _2 = (Parsing.peek_val __caml_parser_env 0 : 'scalar0) in
    Obj.repr(
# 147 "apron_parser.mly"
                 ( Scalar.neg _2 )
# 648 "apron_parser.ml"
               : 'scalar))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 0 : 'scalar0) in
    Obj.repr(
# 150 "apron_parser.mly"
    ( Coeff.Scalar _1 )
# 655 "apron_parser.ml"
               : 'coeff0))
; (fun __caml_parser_env ->
    let _2 = (Parsing.peek_val __caml_parser_env 3 : 'scalar) in
    let _4 = (Parsing.peek_val __caml_parser_env 1 : 'scalar) in
    Obj.repr(
# 152 "apron_parser.mly"
    ( Coeff.Interval(Interval.of_infsup _2 _4) )
# 663 "apron_parser.ml"
               : 'coeff0))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 0 : 'coeff0) in
    Obj.repr(
# 154 "apron_parser.mly"
         ( _1 )
# 670 "apron_parser.ml"
               : 'coeff))
; (fun __caml_parser_env ->
    let _1 = (Parsing.peek_val __caml_parser_env 1 : (Texpr1.typ * Texpr1.round)) in
    let _2 = (Parsing.peek_val __caml_parser_env 0 : 'coeff0) in
    Obj.repr(
# 155 "apron_parser.mly"
                ( Coeff.neg _2 )
# 678 "apron_parser.ml"
               : 'coeff))
(* Entry lincons *)
; (fun __caml_parser_env -> raise (Parsing.YYexit (Parsing.peek_val __caml_parser_env 0)))
(* Entry generator *)
; (fun __caml_parser_env -> raise (Parsing.YYexit (Parsing.peek_val __caml_parser_env 0)))
(* Entry linexpr *)
; (fun __caml_parser_env -> raise (Parsing.YYexit (Parsing.peek_val __caml_parser_env 0)))
(* Entry tcons *)
; (fun __caml_parser_env -> raise (Parsing.YYexit (Parsing.peek_val __caml_parser_env 0)))
(* Entry texpr *)
; (fun __caml_parser_env -> raise (Parsing.YYexit (Parsing.peek_val __caml_parser_env 0)))
|]
let yytables =
  { Parsing.actions=yyact;
    Parsing.transl_const=yytransl_const;
    Parsing.transl_block=yytransl_block;
    Parsing.lhs=yylhs;
    Parsing.len=yylen;
    Parsing.defred=yydefred;
    Parsing.dgoto=yydgoto;
    Parsing.sindex=yysindex;
    Parsing.rindex=yyrindex;
    Parsing.gindex=yygindex;
    Parsing.tablesize=yytablesize;
    Parsing.table=yytable;
    Parsing.check=yycheck;
    Parsing.error_function=parse_error;
    Parsing.names_const=yynames_const;
    Parsing.names_block=yynames_block }
let lincons (lexfun : Lexing.lexbuf -> token) (lexbuf : Lexing.lexbuf) =
   (Parsing.yyparse yytables 1 lexfun lexbuf : Lincons0.typ * (string*Coeff.t) list)
let generator (lexfun : Lexing.lexbuf -> token) (lexbuf : Lexing.lexbuf) =
   (Parsing.yyparse yytables 2 lexfun lexbuf : Generator0.typ * (string*Coeff.t) list)
let linexpr (lexfun : Lexing.lexbuf -> token) (lexbuf : Lexing.lexbuf) =
   (Parsing.yyparse yytables 3 lexfun lexbuf : (string*Coeff.t) list)
let tcons (lexfun : Lexing.lexbuf -> token) (lexbuf : Lexing.lexbuf) =
   (Parsing.yyparse yytables 4 lexfun lexbuf : Tcons0.typ * Texpr1.expr)
let texpr (lexfun : Lexing.lexbuf -> token) (lexbuf : Lexing.lexbuf) =
   (Parsing.yyparse yytables 5 lexfun lexbuf : Texpr1.expr)
