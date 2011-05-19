(* This file is part of the APRON Library, released under LGPL license.
   Please read the COPYING file packaged in the distribution  *)

(** APRON Parsing of expressions *)

(** {2 Introduction}

  This small module implements the parsing of expressions, constraints and
  generators. The allowed syntax is simple for linear expressions (no
  parenthesis) but supports interval expressions. The syntax is more flexible
  for tree expressions.

  {3 Syntax}

  [lincons ::= linexpr ('>' | '>=' | '=' | '!=' | '=' | '<=' | '<') linexpr |
  linexpr = linexpr 'mod' scalar]

  [gen ::= ('V:' | 'R:' | 'L:' | 'RM:' | 'LM:') linexpr]

  [linexpr ::= linexpr '+' linterm | linexpr '-' linterm | linterm]

  [linterm ::= coeff ['*'] identifier | coeff | ['-'] identifier]

  [tcons ::= texpr ('>' | '>=' | '=' | '!=' | '=' | '<=' | '<') texpr | texpr =
  texpr 'mod' scalar]

  [texpr ::= coeff | identifier | unop texpr | texpr binop texpr | '(' texpr ')']

  [binop ::=
  ('+'|'-'|'*'|'/'|'%')['_'('i'|'f'|'d'|'l'|'q')][','('n'|'0'|'+oo'|'-oo')]]

  [unop ::= ('cast' |
  'sqrt')['_'('i'|'f'|'d'|'l'|'q')][','('n'|'0'|'+oo'|'-oo')]]

  [coeff ::= scalar | ['-'] '['scalar ';' scalar ']']

  [scalar ::= ['-'] (integer | rational | floating_point_number)]

  For tree expressions [texpr], by default the operations have an exact
  arithmetic semantics in the real numbers (even if involved variables are of
  integer). The type qualifiers modify this default semantics. Their meaning is
  as follows:
  - [i] integer semantics
  - [f] IEEE754 32 bits floating-point semantics
  - [d] IEEE754 64 bits floating-point semantics
  - [l] IEEE754 80 bits floating-point semantics
  - [q] IEEE754 129 bits floating-point semantics

  By default, the rounding mode is "any" (this applies only in non-real
  semantics), which allows to emulate all the following rounding modes:
  - [n] nearest
  - [0] towards zero
  - [+oo] towards infinity
  - [-oo] towards minus infinity
  - [?] any

  {3 Examples}

  [let (linexpr:Linexpr1.t) = Parser.linexpr1_of_string env "z+0.4x+2y"]

  [let (tab:Lincons1.earray) = Parser.lincons1_of_lstring env
  ["1/2x+2/3y=1";"[1;2]<=z+2w";"z+2w<=4";"0<=u";"u<=5"]]

  [let (lingen:Lingen1.t) = Parser.lingen1_of_string env "R:x+2y"]

  [let (texpr:Texpr1.t) = Parser.texpr1_of_string env "a %_i,? b +_f,0 c"]

  {3 Remarks}

  There is the possibility to parse directly from a lexing buffer, or from a
  string (from which one can generate a buffer with the function
  [Lexing.from_string].

  This module uses the underlying modules [Apron_lexer] and [Apron_parser].
*)

open Common

exception Error of string

let raise_error str = raise (Error str)

(*  **********************************************************************)
(** {2 Internal functions} *)
(*  **********************************************************************)

type scalar = (float,Mpqf.t,unit) Common.t
type coeff = scalar * scalar

let numD_of_scalar (scalar:scalar) intern =
  let res = NumD.init() in
  ignore
    begin match scalar with
    | D x -> NumD.set_double res x intern
    | MPQ x -> NumD.set_mpq res x intern
    | _ -> failwith ""
    end;
  res

let numMPQ_of_scalar (scalar:scalar) intern =
  let res = NumMPQ.init() in
  begin match scalar with
  | D x -> Mpq.set_d res x
  | MPQ x -> NumMPQ.set res x
  | _ -> failwith ""
  end;
  res

let eitvD_of_coeff (coeff:coeff) intern =
  let (neginf,sup) = coeff in
  let neginf = numD_of_scalar neginf intern in
  NumD.neg neginf neginf;
  let sup = numD_of_scalar sup intern in
  EitvD.init_set_num2 neginf sup

let eitvMPQ_of_coeff (coeff:coeff) intern =
  let (neginf,sup) = coeff in
  let neginf = numMPQ_of_scalar neginf intern in
  NumMPQ.neg neginf neginf;
  let sup = numMPQ_of_scalar sup intern in
  EitvMPQ.init_set_num2 neginf sup

let coeff_of_coeff intern (coeff:coeff) : Coeff.f Coeff.tt =
  let (neginf,sup) = coeff in
  begin match (neginf,sup) with
  | (Common.D _, Common.D _) -> Common.D(eitvD_of_coeff coeff intern)
  | _ -> Common.MPQ(eitvMPQ_of_coeff coeff intern)
  end

let check_list intern eitv_of_coeff list
    =
  let rec check = function
    | (var1,_)::(((var2,_)::_) as l)->
	if (String.compare var1 var2)=0 then
	  raise (Error (Format.sprintf "%s appears twice in an expression"
	    (if var1=""
	    then "Constant coefficient"
	    else ("Variable "^var1))))
	else
	  check l
    | _ -> ()
  in
  let list =
     List.sort
       (fun (var1,_) (var2,_) -> String.compare var1 var2)
       list
  in
  check list;
  let (cst,list) =
    List.fold_left
      (begin fun (cst,list) (name,coeff) ->
	let eitv = eitv_of_coeff coeff intern in
	if name="" then
	  (eitv::cst,list)
	else
	  (cst, (Var.of_string name, eitv)::list)
      end)
      ([],[]) list
  in
  let cst = match cst with
    | [] -> None
    | [x] -> Some x
    | _ -> failwith ""
  in
  (cst,list)



let linexpr1_of_linexpr intern (discr:Common.discr) (env:Environment.t) (list:(string*coeff) list)
  :
  Linexpr1.t
  =
  match discr with
  | D() ->
      let (cst,list) = check_list intern eitvD_of_coeff list in
      let linexprD = LinexprD.of_list1 env ?cst list in
      Linexpr1.init_set_linexprD linexprD env
  | MPQ() ->
      let (cst,list) = check_list intern eitvMPQ_of_coeff list in
      let linexprMPQ = LinexprMPQ.of_list1 env ?cst list in
      Linexpr1.init_set_linexprMPQ linexprMPQ env
  | _ -> failwith ""

let lincons1_of_lincons intern (discr:Common.discr) (env:Environment.t) (lincons:Common.constyp * (string*coeff) list * Mpqf.t option)
  :
  Lincons1.t
  =
  match discr with
  | D() ->
      let (constyp,list,ompq) = lincons in
      let (cst,list) = check_list intern eitvD_of_coeff list in
      let linconsD = LinconsD.of_list1 env ?cst list in
      LinconsD.set_constyp linconsD constyp;
      begin match ompq with
      | Some mpq -> LinconsD.set_mpq linconsD mpq
      | None -> ()
      end;
      Lincons1.init_set_linconsD linconsD env
  | MPQ() ->
      let (constyp,list,ompq) = lincons in
      let (cst,list) = check_list intern eitvMPQ_of_coeff list in
      let linconsMPQ = LinconsMPQ.of_list1 env ?cst list in
      LinconsMPQ.set_constyp linconsMPQ constyp;
      begin match ompq with
      | Some mpq -> LinconsMPQ.set_mpq linconsMPQ mpq
      | None -> ()
      end;
      Lincons1.init_set_linconsMPQ linconsMPQ env
  | _ -> failwith ""

let lingen1_of_lingen intern (discr:Common.discr) (env:Environment.t) (lingen:Common.gentyp * (string*coeff) list)
  :
  Lingen1.t
  =
  match discr with
  | D() ->
      let (gentyp,list) = lingen in
      let (cst,list) = check_list intern eitvD_of_coeff list in
      let lingenD = LingenD.of_list1 env ?cst list in
      LingenD.set_gentyp lingenD gentyp;
      Lingen1.init_set_lingenD lingenD env
  | MPQ() ->
      let (gentyp,list) = lingen in
      let (cst,list) = check_list intern eitvMPQ_of_coeff list in
      let lingenMPQ = LingenMPQ.of_list1 env ?cst list in
      LingenMPQ.set_gentyp lingenMPQ gentyp;
      Lingen1.init_set_lingenMPQ lingenMPQ env
  | _ -> failwith ""

(*  **********************************************************************)
(** {2 Exported functions: using lexbuf} *)
(*  **********************************************************************)

let linexpr1_of_lexbuf intern (discr:Common.discr) (env:Environment.t) (lexbuf:Lexing.lexbuf)
  :
  Linexpr1.t
  =
  let x = Apron_parser.linexpr Apron_lexer.lex lexbuf in
  linexpr1_of_linexpr intern discr env x

let lincons1_of_lexbuf intern (discr:Common.discr) (env:Environment.t) (lexbuf:Lexing.lexbuf)
  :
  Lincons1.t
  =
  let x = Apron_parser.lincons Apron_lexer.lex lexbuf in
  lincons1_of_lincons intern discr env x

let lingen1_of_lexbuf intern (discr:Common.discr) (env:Environment.t) (lexbuf:Lexing.lexbuf)
  :
  Lingen1.t
  =
  let x = Apron_parser.lingen Apron_lexer.lex lexbuf in
  lingen1_of_lingen intern discr env x

let expr_of_gexpr intern e =
  Texpr0.map_gexpr
    (coeff_of_coeff intern)
    Var.of_string
    e

let texpr1_of_lexbuf intern (env:Environment.t) (lexbuf:Lexing.lexbuf)
  :
  Texpr1.t
  =
  let e = Apron_parser.texpr Apron_lexer.lex lexbuf in
  let e = expr_of_gexpr intern e in
  Texpr1.of_expr env e

let tcons1_of_lexbuf intern (env:Environment.t) (lexbuf:Lexing.lexbuf)
  :
  Tcons1.t
  =
  let (t,e,ompq) = Apron_parser.tcons Apron_lexer.lex lexbuf in
  let e = expr_of_gexpr intern e in
  Tcons1.make (Texpr1.of_expr env e) t ompq

(*  **********************************************************************)
(** {2 Exported functions: using strings} *)
(*  **********************************************************************)

let linyyy1_of_string linyyy1_of_lexbuf msg intern (discr:Common.discr) (env:Environment.t) (str:string) =
  try
    let lexbuf = Lexing.from_string str in
    try linyyy1_of_lexbuf intern discr env lexbuf
    with Parsing.Parse_error ->
      raise_error
	(Format.sprintf
	  "Syntaxical error, characters %d-%d in %s %s"
	  (Lexing.lexeme_start lexbuf)
	  (Lexing.lexeme_end lexbuf)
	  msg
	  str)
  with Apron_lexer.Error (s,e) ->
    raise_error
      (Format.sprintf
	"Lexical error, characters %d-%d in %s %s"
	s e msg str)

let linexpr1_of_string intern (discr:Common.discr) (env:Environment.t) (str:string) : Linexpr1.t
  =
  linyyy1_of_string linexpr1_of_lexbuf "expression" intern discr env str

let lincons1_of_string intern (discr:Common.discr) (env:Environment.t) (str:string) : Lincons1.t
  =
  linyyy1_of_string lincons1_of_lexbuf "constraint" intern discr env str

let lincons1_of_string intern (discr:Common.discr) (env:Environment.t) (str:string) : Lincons1.t
  =
  linyyy1_of_string lincons1_of_lexbuf "generator" intern discr env str

let tyyy1_of_string tyyy1_of_lexbuf msg intern env str =
  try
    let lexbuf = Lexing.from_string str in
    try tyyy1_of_lexbuf intern env lexbuf
    with Parsing.Parse_error ->
      raise_error
	(Format.sprintf
	  "Syntaxical error, characters %d-%d in %s %s"
	  (Lexing.lexeme_start lexbuf)
	  (Lexing.lexeme_end lexbuf)
	  msg
	  str)
  with Apron_lexer.Error (s,e) ->
    raise_error
      (Format.sprintf
	"Lexical error, characters %d-%d in %s %s"
	s e msg str)

let texpr1_of_string intern (env:Environment.t) (str:string) : Texpr1.t
  =
  tyyy1_of_string texpr1_of_lexbuf "expression" intern env str

let tcons1_of_string intern (env:Environment.t) (str:string) : Tcons1.t
  =
  tyyy1_of_string tcons1_of_lexbuf "constraint" intern env str

let lincons1_of_lstring intern (discr:Common.discr) (env:Environment.t) (lstr:string list)
  :
  Lincons1.earray
  =
  let length = List.length lstr in
  let res = Lincons1.array_init discr length env in
  let i = ref (-1) in
  List.iter
    (fun str ->
      incr i;
      let x = lincons1_of_string intern discr env str in
      ignore (Lincons1.array_set_index res !i x intern)
    )
    lstr
  ;
  res

let tcons1_of_lstring (env:Environment.t) (lstr:string list)
  :
  Tcons1.earray
  =
  let length = List.length lstr in
  let res = Tcons1.array_make env length in
  let i = ref (-1) in
  List.iter
    (fun str ->
      incr i;
      let x = tcons1_of_string env str in
      Tcons1.array_set res !i x
    )
    lstr
  ;
  res

let of_lstring (man:'a Manager.t) (env:Environment.t) (lstr:string list)
  :
  'a Abstract1.t
  =
  let array = lincons1_of_lstring env lstr in
  Abstract1.of_lincons_array man env array
