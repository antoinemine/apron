#! /usr/bin/env python
# -*- coding: iso-8859-15 -*-

import os
import math

nb = 10

filename = "InterSC%d.spl" % nb

f = open(filename, 'w+')

C1 = 0
C2 = 3.14/2
C = C2 - C1

f.write ("var x : real, y1 : real, y2 : real, z : real, w : real; \n\
begin \n\
assume x >= %f and x <= %f; \n" % (C1,C2))

for i in range (0,nb):
  x1 = C1 + (i)*(C/nb);
  x2 = C1 + (i+1)*(C/nb);
  s1 = math.sin(x1);
  s2 = math.sin(x2);
  c1 = math.cos(x1);
  c2 = math.cos(x2);
  sa = (s2 - s1) / (x2 - x1);
  sb = s1 - sa*x1;
  ca = (c2 - c1) / (x2 - x1);
  cb = c1 - ca*x1;
  if (i < nb -1 ): f.write ("if (x<=%f) then \n" % (x2))
  else : f.write ("else \n")
  f.write ("\
  y1 = %f*x + %f; \n\
  y2 = %f*x + %f; \n\
  z = y1*y1 + y2*y2;\n\
  w = 1 / z;\n\
  " % (sa,sb,ca,cb))
  if (i < nb - 2 ):
    f.write ("else \n")


for i in range (1,nb):
    f.write("endif;\n")

#f.write ("z = y1*y1 + y2*y2;\n")

f.write ("end")

f.close()
