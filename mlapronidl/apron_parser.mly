/* $Id$ */

/* Syntaxical analysis to convert strings to objects. */

%{
(* This file is part of the APRON Library, released under LGPL license.
   Please read the COPYING file packaged in the distribution  *)

type scalar = (float,Mpqf.t,unit) Common.t
type coeff = (float,Mpqf.t,unit) Common.t * (float,Mpqf.t,unit) Common.t
let scalar_neg = function
  | Common.D(x) -> Common.D(-. x)
  | Common.MPQ(x) -> Common.MPQ(Mpqf.neg x)
  | _ -> failwith ""

let coeff_neg (neginf,sup) = (sup,neginf)

let rec sub acc = function
  | [] -> acc
  | (var,coeff)::l ->
      let nacc = (var,coeff_neg coeff)::acc in
      sub nacc l
%}

/* \section{Lexems} %======================================================== */

%token TK_EOF

%token TK_VERTEX TK_RAY TK_LINE TK_RAYMOD TK_LINEMOD

%token TK_SUPEG TK_INFEG TK_SUP TK_INF TK_EG TK_DISEG TK_MOD

%token TK_LBRACKET TK_RBRACKET TK_SEMICOLON TK_LPAR TK_RPAR

%token <(Texpr1.typ * Texpr1.round)> TK_MUL
%token <(Texpr1.typ * Texpr1.round)> TK_ADD
%token <(Texpr1.typ * Texpr1.round)> TK_SUB
%token <(Texpr1.typ * Texpr1.round)> TK_DIV
%token <(Texpr1.typ * Texpr1.round)> TK_MODULO
%token <(Texpr1.typ * Texpr1.round)> TK_CAST
%token <(Texpr1.typ * Texpr1.round)> TK_SQRT

%token <Mpqf.t> TK_MPQF
%token <float> TK_FLOAT
%token <string> TK_VAR


%start lincons lingen linexpr
%start tcons texpr

%type <Common.constyp * (string *  ((float,Mpqf.t,unit) Common.t * (float,Mpqf.t,unit) Common.t)) list * Mpqf.t option> lincons
%type <Common.gentyp * (string *  ((float,Mpqf.t,unit) Common.t * (float,Mpqf.t,unit) Common.t)) list> lingen
%type <(string *  ((float,Mpqf.t,unit) Common.t * (float,Mpqf.t,unit) Common.t)) list> linexpr

%type <( ((float,Mpqf.t,unit) Common.t * (float,Mpqf.t,unit) Common.t),string) Texpr1.gexpr> texpr
%type <Common.constyp * ( ((float,Mpqf.t,unit) Common.t * (float,Mpqf.t,unit) Common.t),string) Texpr1.gexpr * Mpqf.t option> tcons

%%

/* \section{Rules} %========================================================= */

lincons:
  linexpr0 TK_EG linexpr0 TK_EOF { (Common.EQ, sub $1 $3, None) }
| linexpr0 TK_EG linexpr0 TK_MOD mpq0 TK_EOF { (Common.EQMOD, sub $1 $3, Some $5) }
| linexpr0 TK_DISEG linexpr0 TK_EOF { failwith "!= not yet supported" }
| linexpr0 TK_SUP linexpr0 TK_EOF { (Common.SUP, sub $1 $3, None) }
| linexpr0 TK_SUPEG linexpr0 TK_EOF { (Common.SUPEQ, sub $1 $3, None) }
| linexpr0 TK_INFEG linexpr0 TK_EOF { (Common.SUPEQ, sub $3 $1, None) }
| linexpr0 TK_INF linexpr0 TK_EOF { (Common.SUP, sub $3 $1, None) }

lingen:
  TK_VERTEX linexpr0 TK_EOF { (Common.VERTEX,$2) }
| TK_RAY linexpr0 TK_EOF { (Common.RAY,$2) }
| TK_LINE linexpr0 TK_EOF { (Common.LINE,$2) }
| TK_RAYMOD linexpr0 TK_EOF { (Common.RAYMOD,$2) }
| TK_LINEMOD linexpr0 TK_EOF { (Common.LINEMOD,$2) }

linexpr:
  linexpr0 TK_EOF { $1 }

linexpr0:
  linexpr0 TK_ADD term
    { $3::$1 }
| linexpr0 TK_SUB term
    { let (var,coeff) = $3 in (var,coeff_neg coeff)::$1 }
| term { [$1] }
term:
  coeff id { ($2,$1) }
| coeff TK_MUL id { ($3,$1) }
| coeff { ("",$1) }
| id { ($1, let s = Common.D(1.0) in (scalar_neg s,s)) }
| TK_SUB id { ($2,let s = Common.D(-1.0) in (scalar_neg s,s)) }

tcons:
  tcons0 TK_EOF { $1 }

tcons0:
  texpr0 TK_EG texpr0 { (Common.EQ, (Texpr1.Binop (Texpr1.Sub,$1,$3,Texpr1.Real,Texpr1.Rnd)), None) }
| texpr0 TK_EG texpr0 TK_MOD mpq0 { (Common.EQMOD, (Texpr1.Binop (Texpr1.Sub,$1,$3,Texpr1.Real,Texpr1.Rnd)),Some $5) }
| texpr0 TK_DISEG texpr0 { failwith "!= not yet supported" }
| texpr0 TK_SUP texpr0 { (Common.SUP, (Texpr1.Binop (Texpr1.Sub,$1,$3,Texpr1.Real,Texpr1.Rnd)),None) }
| texpr0 TK_SUPEG texpr0 { (Common.SUPEQ, (Texpr1.Binop (Texpr1.Sub,$1,$3,Texpr1.Real,Texpr1.Rnd)),None) }
| texpr0 TK_INFEG texpr0 { (Common.SUPEQ, (Texpr1.Binop (Texpr1.Sub,$3,$1,Texpr1.Real,Texpr1.Rnd)),None) }
| texpr0 TK_INF texpr0 { (Common.SUP, (Texpr1.Binop (Texpr1.Sub,$3,$1,Texpr1.Real,Texpr1.Rnd)),None) }

texpr:
  texpr0 TK_EOF { $1 }

texpr0:
  texpr0 TK_ADD texpr0_1
    { let (t,r) = $2 in Texpr1.Binop(Texpr1.Add,$1,$3,t,r) }
| texpr0 TK_SUB texpr0_1
    { let (t,r) = $2 in Texpr1.Binop(Texpr1.Sub,$1,$3,t,r) }
| texpr0_1
    { $1 }

texpr0_1:
  texpr0_1 TK_MUL texpr0_2
    { let (t,r) = $2 in Texpr1.Binop(Texpr1.Mul,$1,$3,t,r) }
| texpr0_1 TK_DIV texpr0_2
    { let (t,r) = $2 in Texpr1.Binop(Texpr1.Div,$1,$3,t,r) }
| texpr0_1 TK_MODULO texpr0_2
    { let (t,r) = $2 in Texpr1.Binop(Texpr1.Div,$1,$3,t,r) }
| texpr0_2
    { $1 }
texpr0_2:
  TK_SUB texpr0_2
    { let (t,r) = $1 in Texpr1.Unop(Texpr1.Neg,$2,t,r) }
| texpr0_3
    { $1 }
texpr0_3:
  TK_CAST texpr0_3
    { let (t,r) = $1 in Texpr1.Unop(Texpr1.Cast,$2,t,r) }
| TK_SQRT texpr0_3
    { let (t,r) = $1 in Texpr1.Unop(Texpr1.Sqrt,$2,t,r) }
| TK_LPAR texpr0 TK_RPAR
    { $2 }
| coeff0
    { Texpr1.Cst($1) }
| id
    { Texpr1.DimVar($1) }

id:
  TK_VAR { $1 }

float0:
  TK_FLOAT { $1 }
float:
  float0 { $1 }
| TK_SUB float0 { -. $2 }

mpq0:
  TK_MPQF { $1 }
mpq:
  mpq0 { $1 }
| TK_SUB mpq0 { Mpqf.neg $2  }

scalar0:
  float0 { Common.D $1 }
| mpq0 { Common.MPQ $1 }
scalar:
  float { Common.D $1 }
| mpq { Common.MPQ $1 }

coeff0:
  scalar0 { ($1,$1) }
| TK_LBRACKET scalar TK_SEMICOLON scalar TK_RBRACKET
    { (scalar_neg $2,$4) }
coeff:
  coeff0 { $1 }
| TK_SUB coeff0 { coeff_neg $2 }
