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

  [texpr ::= coeff | identifier | unop texpr | texpr binop texpr | '(' texpr
  ')']

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

  [let (generator:Generator1.t) = Parser.generator1_of_string env "R:x+2y"]

  [let (texpr:Texpr1.t) = Parser.texpr1_of_string env "a %_i,? b +_f,0 c"]

  {3 Remarks}

  There is the possibility to parse directly from a lexing buffer, or from a
  string (from which one can generate a buffer with the function
  [Lexing.from_string].

  This module uses the underlying modules [Apron_lexer] and [Apron_parser].
*)

(** {2 Interface} *)

exception Error of string
  (** Raised by conversion functions *)

val linexpr1_of_lexbuf : Environment.t -> Lexing.lexbuf -> Linexpr1.t
val lincons1_of_lexbuf : Environment.t -> Lexing.lexbuf -> Lincons1.t
val generator1_of_lexbuf : Environment.t -> Lexing.lexbuf -> Generator1.t
  (** Conversion from lexing buffers to resp. linear expressions, linear
    constraints and generators, defined on the given environment. *)

val texpr1expr_of_lexbuf : Lexing.lexbuf -> Texpr1.expr
val texpr1_of_lexbuf : Environment.t -> Lexing.lexbuf -> Texpr1.t
val tcons1_of_lexbuf : Environment.t -> Lexing.lexbuf -> Tcons1.t
  (** Conversion from lexing buffers to resp. tree expressions and
      constraints, defined on the given environment. *)

val linexpr1_of_string : Environment.t -> string -> Linexpr1.t
val lincons1_of_string : Environment.t -> string -> Lincons1.t
val generator1_of_string : Environment.t -> string -> Generator1.t
  (** Conversion from strings to resp. linear expressions, linear
    constraints and generators, defined on the given environment. *)

val texpr1expr_of_string : string -> Texpr1.expr
val texpr1_of_string : Environment.t -> string -> Texpr1.t
val tcons1_of_string : Environment.t -> string -> Tcons1.t
  (** Conversion from lexing buffers to resp. tree expressions and
      constraints, defined on the given environment. *)

val lincons1_of_lstring : Environment.t -> string list -> Lincons1.earray
val generator1_of_lstring : Environment.t -> string list -> Generator1.earray
  (** Conversion from lists of strings to array of resp. linear
      constraints and generators, defined on the given environment. *)

val tcons1_of_lstring : Environment.t -> string list -> Tcons1.earray
  (** Conversion from lists of strings to array of tree constraints. *)

val of_lstring : 'a Manager.t -> Environment.t -> string list -> 'a Abstract1.t
  (** Abstraction of lists of strings representing constraints to abstract
    values, on the abstract domain defined by the given manager. *)
