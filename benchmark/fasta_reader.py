#!/usr/bin/env python

# Usage: fasta_reader.py <file>

import regex
import sys

def read_fasta(fname):
    ret = {}
    entry_pat = regex.compile(">(.*)\n([^>]*)")
    with open(fname) as fasta_file:
        for m in entry_pat.finditer(fasta_file.read()):
            ret[m.group(1)] = m.group(2).replace("\n","")
    return ret

fasta_file = read_fasta(sys.argv[1])
for header in fasta_file:
    print header
