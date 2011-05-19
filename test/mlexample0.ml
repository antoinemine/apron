(** Test the special case of abstract values with dimensionality 0,0 *)

open Format;;
open Apron;;

let print_array print_elt fmt array =
  if array=[||] then begin
    fprintf fmt "[||]"
  end
  else begin
    fprintf fmt "[|@[";
    let first = ref true in
    Array.iter
      (begin fun e ->
	if !first then first := false else fprintf fmt ";@ ";
	print_elt fmt e
      end)
      array
    ;
    fprintf fmt "@]|]";
  end
;;

let string_of_dim dim = "x"^(string_of_int dim);;

let man1 = Box.manager_alloc ();;
let man2 = Oct.manager_alloc ();;
let man3 = Polka.manager_alloc_loose ();;
let man4 = Ppl.manager_alloc_loose ();;
let man5 = Ppl.manager_alloc_grid ();;
let man6 = PolkaGrid.manager_alloc man3 man5;;

let f man name =
  printf "man=%s, %s@.  @[<v>" (Manager.get_library man) name;
  let a = match name with
    | "bottom" -> Abstract0.bottom man 0 0
    | "top" -> Abstract0.top man 0 0
    | _ -> failwith ""
  in
  let a2 = match name with
    | "bottom" -> Abstract0.bottom man 0 2
    | "top" ->
	let tcons = [|
	  Lincons0.make
	    (Linexpr0.of_list
	      (Some 2)
	      [((Coeff.s_of_int 1),0)]
	      None)
	    Lincons0.EQ;
	  Lincons0.make
	    (Linexpr0.of_list
	      (Some 2)
	      [((Coeff.s_of_int 1),1)]
	      None)
	    Lincons0.EQ
	|]
	in
	let a0 = Abstract0.of_lincons_array man 0 2 tcons in
	let tcons = [|
	  Lincons0.make
	    (Linexpr0.of_list
	      (Some 2)
	      [((Coeff.s_of_int 1),0)]
	      (Some(Coeff.s_of_int 2)))
	    Lincons0.EQ;
	  Lincons0.make
	    (Linexpr0.of_list
	      (Some 2)
	      [((Coeff.s_of_int 1),1)]
	      (Some(Coeff.s_of_int 2)))
	    Lincons0.EQ
	|]
	in
	let a1 = Abstract0.of_lincons_array man 0 2 tcons in
	Abstract0.join_with man a0 a1;
	a0
    | _ -> failwith ""
  in

  let linexpr = Linexpr0.make None in
  Linexpr0.set_cst linexpr (Coeff.s_of_int (-2));
  let lincons = Lincons0.make linexpr Lincons0.SUPEQ in
  let texpr = Texpr0.cst (Coeff.s_of_int (-2)) in
  let tcons = Tcons0.make texpr Lincons0.SUPEQ in

  printf "a = %a@ " (Abstract0.print string_of_dim) a;

  printf "is_bottom = %b@ " (Abstract0.is_bottom man a);
  printf "is_top = %b@ " (Abstract0.is_top man a);

  printf "to_box = %a@ "
    (print_array Interval.print)
    (Abstract0.to_box man a);
  printf "to_lincons_array = %a@ "
    (print_array (Lincons0.print string_of_dim))
    (Abstract0.to_lincons_array man a);
  printf "to_tcons_array = %a@ "
    (print_array (Tcons0.print string_of_dim))
    (Abstract0.to_tcons_array man a);

  if man!=(Obj.magic man6) then
    printf "to_generator_array = %a@ "
      (print_array (Generator0.print string_of_dim))
      (Abstract0.to_generator_array man a);

  printf "bound_linexpr = %a@ "
    Interval.print
    (Abstract0.bound_linexpr man a linexpr);
  printf "bound_texpr = %a@ "
    Interval.print
    (Abstract0.bound_texpr man a texpr);

  printf "sat_lincons = %b@ "
    (Abstract0.sat_lincons man a lincons);
  printf "sat_tcons = %b@ "
    (Abstract0.sat_tcons man a tcons);

  let x = Abstract0.add_dimensions man a ({Dim.dim=[|0;0|];Dim.intdim=0;Dim.realdim=2}) false
  in
  printf "add_dimensions = %a (x)@ "
    (Abstract0.print string_of_dim)
    x
  ;
  printf "sat_lincons(x) = %b@ "
    (Abstract0.sat_lincons man x lincons);
  printf "sat_tcons(x) = %b@ "
    (Abstract0.sat_tcons man x tcons);

  printf "a2 = %a@ " (Abstract0.print string_of_dim) a2;
  printf "remove_dimensions(a2) = %a@ "
    (Abstract0.print string_of_dim)
    (Abstract0.remove_dimensions man a2 ({Dim.dim=[|0;1|];Dim.intdim=0;Dim.realdim=2}))
  ;

  printf "@]@.";
    ()
;;

let test_widening_strict man =
  let var_x = Var.of_string "x" in
  let var_y = Var.of_string "y" in
  let var_z = Var.of_string "z" in
  let env = Environment.make
    [|var_x|]
    [|var_y; var_z|]
  in
  let tab1 = Parser.lincons1_of_lstring
    env
    ["-x+8>=0";"-y>=0";"-5*z+4>0"; "z>=0";"x>=0"]
  in
  let tab2 = Parser.lincons1_of_lstring
    env
    ["-x+8>=0";"-y>=0";"-5*z+4>=0"; "z>=0";"x>=0"]
  in
  let abs1 = Abstract1.of_lincons_array man env tab1 in
  let abs2 = Abstract1.of_lincons_array man env tab2 in
  let abs3 = Abstract1.widening man abs1 abs2 in
  printf "widening @[<v>%a@ %a@ = %a@]@."
    Abstract1.print abs1
    Abstract1.print abs2
    Abstract1.print abs3
  ;
  ()
;;

test_widening_strict man1;;

let _ =
  f man1 "bottom";
  f man2 "bottom";
  f man3 "bottom";
  f man4 "bottom";
  f man5 "bottom";
  f man6 "bottom";
  f man1 "top";
  f man2 "top";
  f man3 "top";
  f man4 "top";
  f man5 "top";
  f man6 "top";
  ()
;;

let f man =
  let abs1 = Abstract0.bottom man 1 1 in
  let abs2 = Abstract0.top man 0 0 in
  begin try
    printf "hash bottom = %i@." (Hashtbl.hash abs1)
  with _ as exn ->
    printf "hash bottom = exception %s@." (Printexc.to_string exn)
  end;
  begin try
    printf "hash top = %i@." (Hashtbl.hash abs2)
  with _ as exn ->
    printf "hash top = exception %s@." (Printexc.to_string exn)
  end;
  begin try
    printf "compare bottom top = %i@." (Pervasives.compare abs1 abs2)
  with _ as exn ->
    printf "compare bottom top = exception %s@." (Printexc.to_string exn)
  end;
  ()
;;
  f man1;;
  f man2;;
  f man3;;
  f man4;;
  f man5;;
  f man6;;
  f man1;;
  f man2;;
  f man3;;
  f man4;;
  f man5;;
  f man6;;
