#! /usr/bin/env python
# -*- coding: iso-8859-15 -*-

import os
import random

random.seed()
nbNS = random.randint(2, 100)
a = -5.
b = 5.

filename = "join_ub_%d.spl" % nbNS
f = open(filename, 'w+')
f.write ("var x : real")
for i in range (1,nbNS):
    f.write (",t%d : real" % i)
f.write (";\nbegin\n")
for i in range (1,nbNS):
    f.write ("assume t%d >= -1 and t%d <= 1; \n" % (i,i));

# generate 3 random linear expressions
for k in range (1,5):
    l = random.randint(1, nbNS)
    exp = "%f " % (a + (b-a) * random.random())
    for i in range (1,l):
	j = random.randint(1,l-1)
	coeff = a + (b-a) * random.random()
	exp += "+ %f*t%d" % (coeff,j)
    if (k == 1):
	exp1 = exp
    if (k == 2):
	exp2 = exp
    if (k == 3):
	exp3 = exp
    if (k == 4):
	exp4 = exp
    
f.write ("if (%s>=0) then \n\
x = %s; \n\
if (%s>=0) then\n\
x = %s;\n\
else\n\
skip;\n\
endif;\n\
else\n\
skip;\n\
endif;\n" % (exp1,exp2,exp3,exp4))

f.write ("end\n")
f.close()
