#! /usr/bin/env python
# -*- coding: iso-8859-15 -*-

import os
import math

nb = 25

filename = "stolfi%d.spl" % nb

f = open(filename, 'w+')

C1 = -2
C2 = 2
C = C2 - C1

f.write ("var x:real, y:real, z:real, h:real ; \n\
begin \n\
assume x >= %f and x <= %f; \n" % (C1,C2))
f.write ("h = %f; \n" %((C2-C1)*1.0/nb))

for i in range (1,nb):
  f.write ("if (-x<=%d*h - %d) then \n" % (i,C2))
  f.write ("\
  y = sqrt(x*x - x + 0.5) / sqrt(x*x + 0.5) ; \n\
  z = sqrt(y*y - y + 0.5) / sqrt(y*y + 0.5) ; \n\
  ")
  f.write ("else \n")

f.write ("\
	  y = sqrt(x*x - x + 0.5) / sqrt(x*x + 0.5) ; \n\
  z = sqrt(y*y - y + 0.5) / sqrt(y*y + 0.5) ; \n\
	    ")

for i in range (1,nb):
    f.write("endif;\n")

#f.write ("z = y1*y1 + y2*y2;\n")

f.write ("end")

f.close()
