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

#include <sstream>
#include <memory>

SeqReader::SeqReader(const std::string& file) :
    read_buffer_(new ReadBuffer(SeqReader::kBufferSize ,file))
{
}

SeqReader::SeqReader(std::unique_ptr<ReadBuffer>& read_buffer) :
  read_buffer_(std::move(read_buffer))
{
}

std::unique_ptr<SeqReader> SeqReader::CreateReader(const std::string &file) {
  char c;
  std::unique_ptr<ReadBuffer> read_buffer(new ReadBuffer(SeqReader::kBufferSize, file));

  while ((c = read_buffer->NextChar())) {
    if (!isalpha(c)) {
      continue;
    }

    read_buffer->Reset();

    if (c == '>') {
      return std::unique_ptr<SeqReader>(new FastaReader(read_buffer));
    } else if (c == '@') {
      return std::unique_ptr<SeqReader>(new FastqReader(read_buffer));
    } else {
      break;
    }
  }

  std::string msg = "Error: File not in FASTA or FASTQ format";

  throw SeqReaderException(msg);
}
