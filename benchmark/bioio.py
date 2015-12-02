#!/usr/bin/env python

# Usage: fasta_reader.py <file>

import regex
import sys

def read_fasta(fname):
    entry_pat = regex.compile(">(.*)\n([^>]*)")
    with open(fname) as fasta_file:
        for m in entry_pat.finditer(fasta_file.read()):
            yield (m.group(1), m.group(2).replace(" ", "").replace("\n",""))

fasta_file = read_fasta(sys.argv[1])
for name, seq in fasta_file:
    print '>', name
    print seq
