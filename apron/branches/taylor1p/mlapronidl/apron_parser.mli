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

val lincons :
  (Lexing.lexbuf  -> token) -> Lexing.lexbuf -> Lincons0.typ * (string*Coeff.t) list
val generator :
  (Lexing.lexbuf  -> token) -> Lexing.lexbuf -> Generator0.typ * (string*Coeff.t) list
val linexpr :
  (Lexing.lexbuf  -> token) -> Lexing.lexbuf -> (string*Coeff.t) list
val tcons :
  (Lexing.lexbuf  -> token) -> Lexing.lexbuf -> Tcons0.typ * Texpr1.expr
val texpr :
  (Lexing.lexbuf  -> token) -> Lexing.lexbuf -> Texpr1.expr
