(* $Id$ *)

(* This file is part of the APRON Library, released under LGPL license
   with an exception allowing the redistribution of statically linked
   executables.

   Please read the COPYING file packaged in the distribution  *)

(** Lexical analysis of expressions, constraints, generators *)

{
open Apron_parser
open Lexing
exception Error of int * int

let attributes_of_string (pos:int) lexbuf =
  let str = lexeme lexbuf in
  let length = (String.length str) - pos in
  let suffix = String.sub str pos length in
  let index = ref 0 in
  if length=0 then
    (Texpr1.Real, Texpr1.Rnd)
  else begin
    if String.get suffix !index <> '_' || length = 1 then
      raise (Error(lexeme_start lexbuf, lexeme_end lexbuf))
    ;
    incr index;
    let round_typ =
      match String.get suffix !index with
      | 'i' -> Texpr1.Int
      | 'f' -> Texpr1.Single
      | 'd' -> Texpr1.Double
      | 'l' -> Texpr1.Extended
      | 'q' -> Texpr1.Quad
      | _ -> raise (Error(lexeme_start lexbuf, lexeme_end lexbuf))
    in
    if length = 2 then
      (round_typ, Texpr1.Rnd)
    else begin
      incr index;
      if (String.get suffix !index) <> ',' || length = 3 then
	raise (Error(lexeme_start lexbuf, lexeme_end lexbuf))
      ;
      incr index;
      let round_dir =
	match String.sub suffix !index (length - !index) with
	| "n" -> Texpr1.Near
	| "0" -> Texpr1.Zero
	| "+oo" -> Texpr1.Up
	| "-oo" -> Texpr1.Down
	| "?" -> Texpr1.Rnd
	|  _ -> raise (Error(lexeme_start lexbuf, lexeme_end lexbuf))
      in
      (round_typ, round_dir)
    end
  end

}

rule lex = parse
  eof    { TK_EOF }
| [' ' '\t' '\n'] +  { lex lexbuf }

| "["    { TK_LBRACKET }
| "]"    { TK_RBRACKET }
| "("    { TK_LPAR }
| ")"    { TK_RPAR }
| ";"    { TK_SEMICOLON }

| "mod"  { TK_MOD }
| "V:"   { TK_VERTEX }
| "R:"   { TK_RAY }
| "L:"   { TK_LINE }
| "RM:"  { TK_RAYMOD }
| "LM:"  { TK_LINEMOD }

(* Arithmetic operations *)
| "+"(('_'['i''f''d''l''q'])(','("n"|"0"|"+oo"|"-oo"|"?"))?)?
    { TK_ADD(attributes_of_string 1 lexbuf) }
| "-"(('_'['i''f''d''l''q'])(','("n"|"0"|"+oo"|"-oo"|"?"))?)?
    { TK_SUB(attributes_of_string 1 lexbuf) }
| "*"(('_'['i''f''d''l''q'])(','("n"|"0"|"+oo"|"-oo"|"?"))?)?
    { TK_MUL(attributes_of_string 1 lexbuf) }
| "/"(('_'['i''f''d''l''q'])(','("n"|"0"|"+oo"|"-oo"|"?"))?)?
    { TK_DIV(attributes_of_string 1 lexbuf) }
| "%"(('_'['i''f''d''l''q'])(','("n"|"0"|"+oo"|"-oo"|"?"))?)?
    { TK_MODULO(attributes_of_string 1 lexbuf) }
| ("^"|"**")(('_'['i''f''d''l''q'])(','("n"|"0"|"+oo"|"-oo"|"?"))?)?
    { TK_POW(attributes_of_string 1 lexbuf) }
| "cast"(('_'['i''f''d''l''q'])(','("n"|"0"|"+oo"|"-oo"|"?"))?)?
    { TK_CAST(attributes_of_string 4 lexbuf) }
| "sqrt"(('_'['i''f''d''l''q'])(','("n"|"0"|"+oo"|"-oo"|"?"))?)?
    { TK_SQRT(attributes_of_string 4 lexbuf) }

(* Boolean operations *)
| ">"  { TK_SUP }
| "<"   { TK_INF }
| ">="  { TK_SUPEG }
| "<="   { TK_INFEG }
| "="   { TK_EG }
| "!="   { TK_DISEG }
| (['0'-'9'])+ ('/'['0'-'9']+)?
  {
    let str = lexeme lexbuf in
    TK_MPQF(Mpqf.of_string str)
  }
| ['0'-'9']* ('.' ['0'-'9']+) (['e' 'E'] ['+' '-']? ['0'-'9']+)?
  {
    let str = lexeme lexbuf in
    TK_FLOAT(float_of_string str)
  }
(* Identifiers *)
| ['A'-'Z' 'a'-'z' '_']
    ( ['_' 'A'-'Z' 'a'-'z' '0'-'9' '.'] ) *
    ( ['\''] ) *
    { TK_VAR (lexeme lexbuf) }
| _ { raise (Error(lexeme_start lexbuf, lexeme_end lexbuf)) }
