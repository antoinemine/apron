#! /usr/bin/env python
# -*- coding: iso-8859-15 -*-

import os
import math

nb = 4

filename = "InterLog%d.spl" % nb

f = open(filename, 'w+')

C1 = 0
C2 = 0.5
C = C2 - C1

f.write ("var x : real, y : real ; \n\
begin \n\
assume x >= %f and x <= %f; \n" % (C1,C2))

for i in range (0,nb):
  x1 = C1 + (i)*(C/nb);
  x2 = C1 + (i+1)*(C/nb);
  if (i < nb -1 ): f.write ("if (x<=%f) then \n" % (x2))
  else : f.write ("else \n")
  f.write ("\
  y = x - (x*x)/2 + (x*x*x)/3 - (x*x*x*x)/4; \n\
  ")
  if (i < nb - 2 ):
    f.write ("else \n")


for i in range (1,nb):
    f.write("endif;\n")

#f.write ("z = y1*y1 + y2*y2;\n")

f.write ("end")

f.close()
