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
#include <cstdio>



FastaReader::FastaReader(const std::string& file_path, int error_tolerance_flags) :
  error_tolerance_flags_(error_tolerance_flags)
{
  fp = fopen(file_path.c_str(), "r");

  if (fp == NULL) {
     std::string msg("FASTA file not found or not readable: " + file_path);
     throw FastaException(msg, 1);
  }

  // Read until eof or the first header is located
  while ((result = fgets(input_line, MAX_LINE, fp)) != NULL) {
    if (input_line[0] == '\n') continue;  // Ignore empty lines

    if (input_line[0] == '>') {
      break;
    } else {
      throw FastaException("Bad format", 2);
    }
  }

  if (input_line[0] != '>' || input_line[1] == '\n') {
    std::string msg("Bad FASTA format: No header found.");
    throw FastaException(msg, 5);
  }

  seqPtr.reset(new SeqEntry());
  toLine(input_line);
  seqPtr->name() = input_line + 1;
}

FastaReader::~FastaReader() {
  fclose(fp);
}

std::unique_ptr<SeqEntry> FastaReader::nextEntry() {
  std::stringstream ss;
  std::unique_ptr<SeqEntry> retPtr = std::move(seqPtr);
  seqPtr.reset(new SeqEntry());

  while ((result = fgets(input_line, MAX_LINE, fp)) != NULL) {
    if (input_line[0] == '\n') continue;  // Ignore empty lines

    if (input_line[0] == '>') {
      toLine(input_line);
      seqPtr->name() = input_line + 1;
      break;
    } else {
      ss << input_line;
    }
  }

  retPtr->seq() = ss.str();

  retPtr->seq().erase(
    std::remove_if(
      retPtr->seq().begin(),
      retPtr->seq().end(),
      [](char ch) {
        return std::isspace<char>(ch, std::locale::classic());
    }), retPtr->seq().end());

  if (retPtr->seq().empty()) {
    std::string msg("Bad FASTA format: Missing sequence for header");
    throw FastaException(msg, 4);
  }

  return retPtr;
}

bool FastaReader::hasNextEntry() const {
  return result != NULL;
}

void FastaReader::toLine(char* cstr) {
    (*strchr(cstr, '\n')) = 0;
}
