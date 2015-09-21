(* This file is part of the APRON Library, released under LGPL license
   with an exception allowing the redistribution of statically linked
   executables.

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
  ('+'|'-'|'*'|'/'|'%'|'^')['_'('i'|'f'|'d'|'l'|'q')][','('n'|'0'|'+oo'|'-oo')]]

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

  [let (generator:Generator1.t) = Parser.generator1_of_string env "R:x+2y"]

  [let (texpr:Texpr1.t) = Parser.texpr1_of_string env "a %_i,? b +_f,0 c"]

  {3 Remarks}

  There is the possibility to parse directly from a lexing buffer, or from a
  string (from which one can generate a buffer with the function
  [Lexing.from_string].

  This module uses the underlying modules [Apron_lexer] and [Apron_parser].
*)

exception Error of string

let raise_error str = raise (Error str)

(*  **********************************************************************)
(** {2 Internal functions} *)
(*  **********************************************************************)

let rec check_list = function
  | (var1,_)::(((var2,_)::_) as l)->
      if (String.compare var1 var2)=0 then
	raise (Error (Format.sprintf "%s appears twice in an expression"
	  (if var1=""
	  then "Constant coefficient"
	  else ("Variable "^var1))))
      else
	check_list l
  | _ -> ()

let linexpr1_of_linexpr (env:Environment.t) (list:(string*Coeff.t) list)
  :
  Linexpr1.t
  =
  let list =
    List.sort
      (fun (var1,_) (var2,_) -> String.compare var1 var2)
      list
  in
  check_list list;
  let res = Linexpr1.make env in
  List.iter
    (begin fun (var,coeff) ->
      if var="" then
	Linexpr1.set_cst res coeff
      else
	Linexpr1.set_coeff res (Var.of_string var) coeff
    end)
    list
  ;
  res

let lincons1_of_lincons (env:Environment.t) (lincons:Lincons0.typ * (string*Coeff.t) list)
  :
  Lincons1.t
  =
  let (typ,list) = lincons in
  Lincons1.make (linexpr1_of_linexpr env list) typ

let generator1_of_generator (env:Environment.t) (generator:Generator0.typ * (string*Coeff.t) list)
  :
  Generator1.t
  =
  let (typ,list) = generator in
  Generator1.make (linexpr1_of_linexpr env list) typ

(*  **********************************************************************)
(** {2 Exported functions: using lexbuf} *)
(*  **********************************************************************)

let linexpr1_of_lexbuf (env:Environment.t) (lexbuf:Lexing.lexbuf)
  :
  Linexpr1.t
  =
  let x = Apron_parser.linexpr Apron_lexer.lex lexbuf in
  linexpr1_of_linexpr env x

let lincons1_of_lexbuf (env:Environment.t) (lexbuf:Lexing.lexbuf)
  :
  Lincons1.t
  =
  let x = Apron_parser.lincons Apron_lexer.lex lexbuf in
  lincons1_of_lincons env x

let generator1_of_lexbuf (env:Environment.t) (lexbuf:Lexing.lexbuf)
  :
  Generator1.t
  =
  let x = Apron_parser.generator Apron_lexer.lex lexbuf in
  generator1_of_generator env x

let texpr1expr_of_lexbuf (lexbuf:Lexing.lexbuf)
  :
  Texpr1.expr
  =
  Apron_parser.texpr Apron_lexer.lex lexbuf

let texpr1_of_lexbuf (env:Environment.t) (lexbuf:Lexing.lexbuf)
  :
  Texpr1.t
  =
  let x = texpr1expr_of_lexbuf lexbuf in
  Texpr1.of_expr env x

let tcons1_of_lexbuf (env:Environment.t) (lexbuf:Lexing.lexbuf)
  :
  Tcons1.t
  =
  let (t,x) = Apron_parser.tcons Apron_lexer.lex lexbuf in
  Tcons1.make (Texpr1.of_expr env x) t

(*  **********************************************************************)
(** {2 Exported functions: using strings} *)
(*  **********************************************************************)

let linexpr1_of_string (env:Environment.t) (str:string) : Linexpr1.t
  =
  try
    let lexbuf = Lexing.from_string str in
    try linexpr1_of_lexbuf env lexbuf
    with Parsing.Parse_error ->
      raise_error
      (Format.sprintf
	"Syntaxical error, characters %d-%d in expression %s"
	(Lexing.lexeme_start lexbuf)
	(Lexing.lexeme_end lexbuf)
	str)
    with Apron_lexer.Error (s,e) ->
      raise_error
      (Format.sprintf
	"Lexical error, characters %d-%d in expression %s"
	s e str)

let lincons1_of_string (env:Environment.t) (str:string) : Lincons1.t
  =
  try
    let lexbuf = Lexing.from_string str in
    try lincons1_of_lexbuf env lexbuf
    with Parsing.Parse_error ->
      raise_error
      (Format.sprintf
	"Syntaxical error, characters %d-%d in constraint %s"
	(Lexing.lexeme_start lexbuf)
	(Lexing.lexeme_end lexbuf)
	str)
    with Apron_lexer.Error (s,e) ->
      raise_error
      (Format.sprintf
	"Lexical error, characters %d-%d in constraint %s"
	s e str)

let generator1_of_string (env:Environment.t) (str:string) : Generator1.t
  =
  try
    let lexbuf = Lexing.from_string str in
    try
      generator1_of_lexbuf env lexbuf
    with Parsing.Parse_error ->
      raise_error
      (Format.sprintf
	"Syntaxical error, characters %d-%d in generator %s"
	(Lexing.lexeme_start lexbuf)
	(Lexing.lexeme_end lexbuf)
	str)
    with Apron_lexer.Error (s,e) ->
      raise_error
      (Format.sprintf
	"Lexical error, characters %d-%d in generator %s"
	s e str)

let texpr1expr_of_string (str:string) : Texpr1.expr
  =
  try
    let lexbuf = Lexing.from_string str in
    try texpr1expr_of_lexbuf lexbuf
    with Parsing.Parse_error ->
      raise_error
      (Format.sprintf
	"Syntaxical error, characters %d-%d in expression %s"
	(Lexing.lexeme_start lexbuf)
	(Lexing.lexeme_end lexbuf)
	str)
    with Apron_lexer.Error (s,e) ->
      raise_error
      (Format.sprintf
	"Lexical error, characters %d-%d in expression %s"
	s e str)

let texpr1_of_string (env:Environment.t) (str:string) : Texpr1.t
  =
  let expr = texpr1expr_of_string str in
  Texpr1.of_expr env expr

let tcons1_of_string (env:Environment.t) (str:string) : Tcons1.t
  =
  try
    let lexbuf = Lexing.from_string str in
    try tcons1_of_lexbuf env lexbuf
    with Parsing.Parse_error ->
      raise_error
      (Format.sprintf
	"Syntaxical error, characters %d-%d in constraint %s"
	(Lexing.lexeme_start lexbuf)
	(Lexing.lexeme_end lexbuf)
	str)
    with Apron_lexer.Error (s,e) ->
      raise_error
      (Format.sprintf
	"Lexical error, characters %d-%d in constraint %s"
	s e str)

let lincons1_of_lstring (env:Environment.t) (lstr:string list)
  :
  Lincons1.earray
  =
  let length = List.length lstr in
  let res = Lincons1.array_make env length in
  let i = ref (-1) in
  List.iter
    (fun str ->
      incr i;
      let x = lincons1_of_string env str in
      Lincons1.array_set res !i x
    )
    lstr
  ;
  res

let generator1_of_lstring (env:Environment.t) (lstr:string list)
  :
  Generator1.earray
  =
  let length = List.length lstr in
  let res = Generator1.array_make env length in
  let i = ref (-1) in
  List.iter
    (fun str ->
      incr i;
      let x = generator1_of_string env str in
      Generator1.array_set res !i x
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
