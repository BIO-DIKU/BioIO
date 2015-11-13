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

#include <sstream>
#include <algorithm>
#include <iostream>

FastaReader::FastaReader(const std::string& file_path, int error_tolerance_flags)
    : error_tolerance_flags_(error_tolerance_flags) {
  input_stream_.open(file_path, std::ifstream::in);

  if (!input_stream_.good()) {
    std::string msg("FASTA file not found or not readable: " + file_path);
    throw FastaException(msg, 1);
  }

  // read and ignore until first occurring '>' character
  input_stream_.ignore(std::numeric_limits<std::streamsize>::max(), '>');

  // '>' character was not read if either stream fails or reaches EOF
  if (input_stream_.fail() || input_stream_.eof()) {
    std::string msg("Bad FASTA format: No header found.");
    throw FastaException(msg, 5);
  }

  // read first header
  getline(input_stream_, next_header_);
}

FastaReader::~FastaReader() {
  input_stream_.close();
}

std::unique_ptr<SeqEntry> FastaReader::nextEntry() {
  std::unique_ptr<SeqEntry> seqPtr(new SeqEntry());

  seqPtr->name() = next_header_;

  std::string seq_buf;

  // read until first next '>' character or EOF
  getline(input_stream_, seq_buf, '>');

  // read next sequence header
  getline(input_stream_, next_header_);

  seqPtr->seq() = seq_buf;

  seqPtr->seq().erase(
    std::remove_if(
      seqPtr->seq().begin(),
      seqPtr->seq().end(),
      [](char ch) {
        return ch == '\n' || ch == '\r' || ch == ' ';
      }), seqPtr->seq().end());

  if (seqPtr->seq().empty()) {
    std::string msg("Bad FASTA format: Missing sequence for header \""
                      + next_header_ + "\"");
    throw FastaException(msg, 4);
  }

  return seqPtr;
}

bool FastaReader::hasNextEntry() const {
  return !input_stream_.eof();
}

std::istream& FastaReader::windowsSafeGetLine(std::istream& is, std::string& str) {
  std::istream& ret = std::getline(is, str);

  // handle evil windows line endings
  if (str.back() == '\r')
    next_header_.pop_back();

  return ret;
}
