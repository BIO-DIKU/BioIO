#!/usr/bin/env gnuplot

# Copyright (C) 2015 BIO-DIKU.

# Create a plot for comparing benchmarks of FASTA/FASTQ reading and writing with
# seqtk, Python, and BioIO.

# Example data below (benchmark.dat):

# File BioIO seqtk kseq Python
# Short 92.719564 9.440293 24.070178 81.315654
# Long 67.578663 4.264639 14.40807 15.275339

set terminal png
set output 'benchmark.png'
set termopt noenhanced
set title "Benchmark"
set key autotitle column box samplen 1
set style data histogram
set style fill solid 0.5 border 0.5
set grid ytics
set ytics scale 0
set xtics scale 0 rotate by -45
set lmargin 15
set ylabel "Seconds"
set logscale y
set xtics scale 0 rotate by -30 font "Arial,9"
plot 'benchmark.dat' using 2:xtic(1) lt 1 lc rgb "#fcaf61", \
                  '' using 3 lt 2 lc rgb "#5bd7b3", \
		  '' using 4 lt 3 lc rgb "#a8988b", \
		  '' using 5 lt 3 lc rgb "#e8e354"
