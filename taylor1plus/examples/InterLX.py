#! /usr/bin/env python
# -*- coding: iso-8859-15 -*-

import os

nb = 300

filename = "InterL%d.spl" % nb
f = open(filename, 'w+')
f.write ("var x : real, y : real; \n\
begin \n\
assume x >= -1 and x <= 1; \n\
if (x>=0) then \n\
y = 10*x; \n\
else\n")

for i in range (1,nb):
  f.write ("if (x>=-%d/%d) then \n\
	y = (10+(10/%d)*%d)*x; \n\
	else \n\
	" % (i,nb,nb,i))

f.write ("y = 20*x;\n")

for i in range (0,nb):
    f.write("endif;\n")

f.write ("assume y == 1;")
f.write ("end")


for i in range (1,nb):
    print ("%d" % i)

f.close()
