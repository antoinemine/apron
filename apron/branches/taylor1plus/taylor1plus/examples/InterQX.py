#! /usr/bin/env python
# -*- coding: iso-8859-15 -*-

import os

nb = 250

filename = "InterQ%d.spl" % nb

f = open(filename, 'w+')
f.write ("var x : real, y : real; \n\
begin \n\
assume x >= -1 and x <= 1; \n\
if (x>=0) then \n\
y = 10*x*x; \n\
else\n")

for i in range (1,nb):
  f.write ("if (x>=-%d/%d) then \n\
  y = -%f*x*x; \n\
  else \n\
  " % (i,nb,(10*nb)+(10./nb)*i))

f.write ("y = -%f*x*x;\n" % (10*nb+10))

for i in range (0,nb):
    f.write("endif;\n")

f.write ("assume y == 1;\n")
f.write ("end")

f.close()
