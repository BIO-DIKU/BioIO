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

#include <BioIO/seq_reader.h>
#include <BioIO/fasta_reader.h>
#include <BioIO/fastq_reader.h>
#include <BioIO/read_buffer.h>

#include <sstream>
#include <iostream>
#include <string>

SeqReader::SeqReader(const std::string &file) :
  read_buffer_(SeqReader::kBufferSize, file)
{
  DetermineFileType();
}

SeqReader::~SeqReader()
{
  delete[] read_buffer_;
}

std::unique_ptr<SeqReader> SeqReader::DetermineFileType() {
  char c;

  while ((c = read_buffer_.NextChar())) {
    if (!isalpha(c)) {
      continue;
    }

    if (c == '>') {
      return std::unique_ptr<SeqReader> seq_reader(FastaReader reader(file));
    } else if (c == '@') {
      return std::unique_ptr<SeqReader> seq_reader(FastqReader reader(file));
    } else {
      break;
    }
  }

  std::string msg = "Error: File not in FASTA or FASTQ format";

  throw SeqReaderException(msg);
}
