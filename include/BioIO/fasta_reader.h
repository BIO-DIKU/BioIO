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

/**
 * @brief Exception class for FastaReader class.
 *
 * @example
 *   std::string msg = "Exception message";
 *   throw FastaReaderException(msg);
 *
 * @example
 *   throw FastaReaderException("Exception message");
 */
class FastaReaderException : public std::exception {
 public:
  FastaReaderException(std::string &msg) :
    exceptionMsg(msg)
  {}

  FastaReaderException(const FastaReaderException &e) :
    exceptionMsg(e.exceptionMsg)
  {}

  virtual const char* what() const throw() { return exceptionMsg.c_str(); }

  const std::string exceptionMsg;
};

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

  /*
   * Size of custom buffer used to read from a FASTA file a chunk of data this size
   */
  static const auto kBufferSize = 640 * 1024;

  /*
   * Size of sequence name buffer used to temporary store the sequence name while
   * parsing a FASTA file. The size of this buffer is determined by kMaxNameSize,
   * which must be larger than the longest name or undefined things will happen.
  */
  static const auto kMaxNameSize = 1024;

  /*
   * Size of sequence buffer used to temporary store the sequence  while parsing
   * a FASTA file. The size of this buffer is determined by kMaxSeqSize, which
   * must be larger than the longest sequence or undefined things will happen.
   */
  static const auto kMaxSeqSize = 300000000;

  /*
   * Temporary file reading buffer.
   */
  ReadBuffer read_buffer_;

  /*
   * Temporary buffer for collecting a read name.
   */
  char *name_buffer_;

  /*
   * Temporary buffer for collecting a read sequence.
   */
  char *seq_buffer_;

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
  inline bool isendl(const char c) {
    return (c == '\n') || (c == '\r');
  }

  /*
   * Return true if valid sequence, that is printable chars except whitespace.
   */
  inline bool isseq(const char c) {
    return (c >= 33 && c <= 126);
  }
};

#endif  // BIOIO_FASTA_READER_H_
