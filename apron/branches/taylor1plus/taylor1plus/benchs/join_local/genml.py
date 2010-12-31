#! /usr/bin/env python
# -*- coding: iso-8859-15 -*-

import sys
import os
import random

random.seed()
#nbNS = random.randint(2, 100)
nbNS = 1+int(sys.argv[1])
a = -1.
b = 1.

filename = "join_ub_%d_%d.ml" % ((nbNS-1),int(sys.argv[2]))
f = open(filename, 'w+')
f.write ("open Apron;;\n")
f.write ("open Mpqf;;\n")
f.write ("open Format;;\n")

f.write ("let man = T1p.manager_alloc ();;\n")

for i in range (1,nbNS):
    f.write ("let var_t%d = Var.of_string \"t%d\";;\n" % (i,i))
f.write ("let var_x = Var.of_string \"x\";;\n")
f.write ("let env = Environment.make\n")
f.write ("[||]\n")
f.write ("[|")
for i in range (1,nbNS):
    f.write ("var_t%d;" % i);
f.write ("var_x|];;\n")

f.write("let abs = Abstract1.of_box man env [|var_t1")
for i in range (2,nbNS):
    f.write (";var_t%d" % i);
f.write("|]\n")
f.write("[|\n")
for i in range (1,nbNS):
    	f.write ("Interval.of_int (-1) 1;\n");
f.write("|];;\n")

# generate 3 random linear expressions
for k in range (1,3):
    #l = random.randint(1, nbNS)
    l = nbNS
    exp = "%f " % (a + (b-a) * random.random())
    for i in range (1,l):
	#j = random.randint(1,l-1)
	coeff = -1 + 2 * random.random()
	exp += "+ %f*t%d" % (coeff,i)
    if (k == 1):
	exp1 = exp
    if (k == 2):
	exp2 = exp

cons = ""
for i in range (1,nbNS):
    p = a + (b-a) * random.random()
    q = a + (b-a) * random.random()
    if (p <= q):
	cons += "\"t%d <= %f\"; \"t%d >= %f\";" % (i,q,i,p)
    else:
	cons += "\"t%d <= %f\"; \"t%d >= %f\";" % (i,p,i,q)

cons1 = ""
for i in range (1,nbNS):
    p = a + (b-a) * random.random()
    q = a + (b-a) * random.random()
    if (p <= q):
	cons1 += "\"t%d <= %f\"; \"t%d >= %f\";" % (i,q,i,p)
    else:
	cons1 += "\"t%d <= %f\"; \"t%d >= %f\";" % (i,p,i,q)

f.write("let exp1 = Parser.texpr1_of_string env \"%s\";;\n" % exp1)
f.write("let abs1 = Abstract1.assign_texpr man abs var_x exp1 None;;\n")

f.write("let exp2 = Parser.texpr1_of_string env \"%s\";;\n" % exp2)
f.write("let abs2 = Abstract1.assign_texpr man abs var_x exp2 None;;\n")

f.write("let tab = Parser.tcons1_of_lstring env [%s];;\n" % cons)
f.write("let abs3 = Abstract1.meet_tcons_array man abs1 tab;;\n")

f.write("let tab1 = Parser.tcons1_of_lstring env [%s];;\n" % cons1)
f.write("let abs4 = Abstract1.meet_tcons_array man abs2 tab1;;\n")

f.write("let abs5 = Abstract1.join man abs4 abs3;;\n")
f.write("let res1 = Abstract1.is_leq man abs3 abs5;;\n\
let res2 = Abstract1.is_leq man abs4 abs5;;\n\
\n\
printf \"\n%B and %B = %B\n\" res1 res2 (res1 && res2);;\n")
    
# generate 3 random linear expressions
for k in range (1,3):
    #l = random.randint(1, nbNS)
    l = nbNS
    exp = "%f " % (a + (b-a) * random.random())
    for i in range (1,l):
	#j = random.randint(1,l-1)
	coeff = -1 + 2 * random.random()
	exp += "+ %f*t%d" % (coeff,i)
    if (k == 1):
	exp1 = exp
    if (k == 2):
	exp2 = exp

cons = ""
for i in range (1,nbNS):
    p = a + (b-a) * random.random()
    q = a + (b-a) * random.random()
    if (p <= q):
	cons += "\"t%d <= %f\"; \"t%d >= %f\";" % (i,q,i,p)
    else:
	cons += "\"t%d <= %f\"; \"t%d >= %f\";" % (i,p,i,q)

cons1 = ""
for i in range (1,nbNS):
    p = a + (b-a) * random.random()
    q = a + (b-a) * random.random()
    if (p <= q):
	cons1 += "\"t%d <= %f\"; \"t%d >= %f\";" % (i,q,i,p)
    else:
	cons1 += "\"t%d <= %f\"; \"t%d >= %f\";" % (i,p,i,q)

f.write("let exp1b = Parser.texpr1_of_string env \"%s\";;\n" % exp1)
f.write("let abs1b = Abstract1.assign_texpr man abs var_x exp1b None;;\n")

f.write("let exp2b = Parser.texpr1_of_string env \"%s\";;\n" % exp2)
f.write("let abs2b = Abstract1.assign_texpr man abs var_x exp2b None;;\n")

f.write("let tabb = Parser.tcons1_of_lstring env [%s];;\n" % cons)
f.write("let abs3b = Abstract1.meet_tcons_array man abs1b tabb;;\n")

f.write("let tab1b = Parser.tcons1_of_lstring env [%s];;\n" % cons1)
f.write("let abs4b = Abstract1.meet_tcons_array man abs2b tab1b;;\n")

f.write("let abs5b = Abstract1.join man abs4b abs3b;;\n")
f.write("let res1b = Abstract1.is_leq man abs4b abs5b;;\n\
let res2b = Abstract1.is_leq man abs3b abs5b;;\n\
\n\
printf \"\n%B and %B = %B\n\" res1b res2b (res1b && res2b);;\n")
    
f.write("let abs6 = Abstract1.join man abs5 abs5b;;\n")
f.write("let res1bb = Abstract1.is_leq man abs5 abs6;;\n\
let res2bb = Abstract1.is_leq man abs5b abs6;;\n\
\n\
printf \"\n%B and %B = %B\n\" res1bb res2bb (res1bb && res2bb);;\n")
    
print nbNS-1
f.close()
