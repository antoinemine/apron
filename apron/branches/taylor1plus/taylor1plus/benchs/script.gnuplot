#set key box

#set terminal latex
#set output "comparaison_domains.tex"
#set term postscript portrait
#set output "mondessin.ps"
#set terminal postscript  enhanced
#set output "comparaison_domains.eps"
#set size 1,0.5

#set multiplot

set terminal gif 
set output "InterLX.gif"


set multiplot
#set size 0.5,0.5
#set origin 0,0
#set key top right
#set title "Output ranges"
#set xlabel "iterations"
#set ylabel "upper and lower bounds of S"
#set yrange [-20:20]
#set xrange [2:15]
#plot 'points_box.txt' using 1:4 t 'Box' with linespoints 18, \
#     'points_box.txt' using 1:5 notitle with linespoints 18
#replot 'points_oct.txt' using 1:4 t 'Octagon' with linespoints 7, \
#	'points_oct.txt' using 1:5 notitle with linespoints 7
#replot 'points_t1p.txt' using 1:4 t 'Taylor1+' with linespoints 43, \
#	'points_t1p.txt' using 1:5 notitle with linespoints 43
#replot 'points_ppl.txt' using 1:3 t 'PK/PPL' with linespoints 27, \
#	'points_ppl.txt' using 1:4 notitle with linespoints 27
#
#set size 0.5,0.5
#set origin 0.5,0
set title "Cputime"
set xlabel "segmentations"
set ylabel "time (s)"
set yrange [0:5]
set xrange [10:510]
plot 'interlinear_time.txt' using 1:3 title 'Box' w linespoints 18
replot 'interlinear_time.txt' u 1:2 t 'ConsTaylor1+' w linespoints 7
replot 'interlinear_time.txt' u 1:4 t 'Octagon' with linespoints 43
replot 'interlinear_time.txt' u 1:5 t 'Polka' with linespoints 27
replot 'interlinear_time.txt' u 1:6 t 'Ppl' with linespoints 25

#unset multiplot
#reset
#plot 'points_box.txt' using 1:($4+$5)/2:4:5 title 'box' with yerrorbars
#replot 'points_oct.txt' using 1:($4+$5)/2:4:5 title 'oct' with yerrorbars
#replot 'points_t1p.txt' using 1:($4+$5)/2:4:5 title 't1plus' with yerrorbars
#replot 'points_ppl.txt' using 1:($3+$4)/2:3:4 title 'ppl' with yerrorbars
