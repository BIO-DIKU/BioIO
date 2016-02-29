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
#include <iostream>
#include <string>

FastaReader::FastaReader(const std::string &file) :
    SeqReader(file),
    name_buffer_(new char[FastaReader::kMaxNameSize]),
    seq_buffer_(new char[FastaReader::kMaxSeqSize])
{}

FastaReader::FastaReader(std::unique_ptr<ReadBuffer>& read_buffer) :
  SeqReader(read_buffer),
  name_buffer_(new char[FastaReader::kMaxNameSize]),
  seq_buffer_(new char[FastaReader::kMaxSeqSize])
{}

FastaReader::~FastaReader() {
  delete[] name_buffer_;
  delete[] seq_buffer_;
}

std::unique_ptr<SeqEntry> FastaReader::NextEntry() {
  std::unique_ptr<SeqEntry> seq_entry(new SeqEntry());

  GetName(seq_entry);
  GetSeq(seq_entry);

  return seq_entry;
}

bool FastaReader::HasNextEntry() {
  return !read_buffer_->Eof();
}

void FastaReader::GetName(std::unique_ptr<SeqEntry> &seq_entry) {
  int  name_index = 0;
  char c;

  while ((c = read_buffer_->NextChar()) && (c != '>')) {
    if (isalpha(c)) {
      std::string msg = "Error: File not in FASTA format";

      throw FastaReaderException(msg);
    }
  }

  while ((c = read_buffer_->NextChar()) && !isendl(c)) {
    name_buffer_[name_index++] = c;
  }

  if (!name_index) {
    std::string msg = "Error: missing sequence name";
    throw FastaReaderException(msg);
  }

  seq_entry->set_name(std::string(name_buffer_, name_index));
}

void FastaReader::GetSeq(std::unique_ptr<SeqEntry> &seq_entry) {
  int  seq_index = 0;
  char c;

  while ((c = read_buffer_->NextChar())) {
    if (c == '>' && isendl(read_buffer_->PrevChar())) {
      read_buffer_->Rewind(1);
      break;
    }

    if (isseq(c)) {
      seq_buffer_[seq_index++] = c;
    }
  }

  if (!seq_index) {
    std::string msg = "Error: missing sequence";
    throw FastaReaderException(msg);
  }

  seq_entry->set_seq(std::string(seq_buffer_, seq_index));
}
