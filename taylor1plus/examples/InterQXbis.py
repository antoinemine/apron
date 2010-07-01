#! /usr/bin/env python
# -*- coding: iso-8859-15 -*-

import os

nb = 300

filename = "InterQ%dbis.spl" % nb

f = open(filename, 'w+')
f.write ("var x : real, y : real, z : real; \n\
begin \n\
assume x >= 0 and x <= 10; \n\
y = x*x - x; \n\
if (y>=0) then \n\
y = x/10; \n\
else\n")

for i in range (1,nb):
  f.write ("if (y >= -%d/%d) then \n\
  y = x*x + %f; \n\
  else \n\
  " % (i,4*nb,1+(1./nb)*i))

f.write ("y = x*x + 2;\n")

for i in range (0,nb):
    f.write("endif;\n")

f.write ("end")

f.close()
