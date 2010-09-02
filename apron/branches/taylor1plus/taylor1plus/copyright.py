#! /usr/bin/env python
# -*- coding: iso-8859-15 -*-

files = open('lsf.txt', 'r')
licence = open('licence.txt', 'r')
copyright = licence.read()

for file in files:
	f = open(file[:-1],'r+')
	src = f.read()
	f.seek(0)
	f.write(copyright)
	f.write(src)
	f.close()

licence.close()
files.close()
