/*
 * Copyright (C) 2015 BIO-DIKU.
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation; either version 2 of the License, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc., 51
 * Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA
 *
 * http://www.gnu.org/copyleft/gpl.html
 */

#include <BioIO/fasta_reader.h>
#include <BioIO/read_buffer.h>

#include <sstream>
#include <algorithm>
#include <iostream>
#include <string>

FastaReader::FastaReader(const std::string &file) :
  read_buffer_(kBufferSize, file)
{}

FastaReader::~FastaReader()
{}

std::unique_ptr<SeqEntry> FastaReader::NextEntry() {
  std::unique_ptr<SeqEntry> seq_entry(new SeqEntry());

  GetName(seq_entry);
  GetSeq(seq_entry);

  if (seq_entry->name().empty()) {
    std::string msg = "Error: missing sequence name";
    throw FastaReaderException(msg);
  }

  return seq_entry;
}

bool FastaReader::HasNextEntry() {
  return !read_buffer_.Eof();
}

void FastaReader::GetName(std::unique_ptr<SeqEntry> &seq_entry) {
  std::string name = "";
  char        c;

  while ((c = read_buffer_.NextChar()) && (c != '>')) {
    // std::string err_msg = ""
    //
    // throw FastaReaderException(msg);
  }

  while ((c = read_buffer_.NextChar()) && !isendl(c)) {
    name += c;
  }

  seq_entry->set_name(name);
}

void FastaReader::GetSeq(std::unique_ptr<SeqEntry> &seq_entry) {
  std::string seq = "";
  char        c;

  while ((c = read_buffer_.NextChar())) {
    if (c == '>' && isendl(read_buffer_.PrevChar())) {
      read_buffer_.Rewind(1);
      break;
    }

    if (isseq(c)) {
      seq += c;
    }
  }

  seq_entry->set_seq(seq);
}
