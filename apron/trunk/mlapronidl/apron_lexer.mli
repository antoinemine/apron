(* This file is part of the APRON Library, released under LGPL license
   with an exception allowing the redistribution of statically linked
   executables.

   Please read the COPYING file packaged in the distribution  *)

(** Lexical analysis of expressions, constraints, generators *)

exception Error of int * int
val lex: Lexing.lexbuf -> Apron_parser.token 
