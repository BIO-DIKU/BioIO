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

#ifndef BIOIO_FASTA_READER_H_
#define BIOIO_FASTA_READER_H_

#include <string>
#include <memory>
#include <fstream>
#include <exception>
#include <iostream>

#include <BioIO/seq_entry.h>
#include <BioIO/read_buffer.h>

static const size_t kBufferSize = 640 * 1024;

class FastaReader
{
 public:
  FastaReader(const std::string &file);

  ~FastaReader();

  /*
   * Return next sequence entry.
   */
  std::unique_ptr<SeqEntry> NextEntry();

  /*
   * Tells if more sequence entries can be found.
   */
  bool HasNextEntry();

 private:

  ReadBuffer read_buffer_;

  /*
   * Get the next FASTA header in the buffer.
   */
  void GetName(std::unique_ptr<SeqEntry> &seq_entry);

  /*
   * Get the next FASTA sequence in the buffer.
   */
  void GetSeq(std::unique_ptr<SeqEntry> &seq_entry);

  /*
   * Return true on \n or \r.
   */
  void isendl(const char);
};

#endif  // BIOIO_FASTA_READER_H_
