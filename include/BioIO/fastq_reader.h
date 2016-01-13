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

#ifndef BIOIO_FASTQ_READER_H_
#define BIOIO_FASTQ_READER_H_

#include <string>
#include <memory>
#include <fstream>
#include <exception>
#include <iostream>

#include <BioIO/seq_entry.h>
#include <BioIO/seq_reader.h>
#include <BioIO/read_buffer.h>

/**
 * @brief Exception class for FastqReader class.
 *
 * @example
 *   std::string msg = "Exception message";
 *   throw FastqReaderException(msg);
 *
 * @example
 *   throw FastqReaderException("Exception message");
 */
class FastqReaderException : public std::exception
{
 public:
  FastqReaderException(std::string &msg) :
    exceptionMsg(msg)
  {}

  FastqReaderException(const FastqReaderException &e) :
    exceptionMsg(e.exceptionMsg)
  {}

  virtual const char* what() const throw() { return exceptionMsg.c_str(); }

  const std::string exceptionMsg;
};

/**
 * Reader for FASTQ files. Assumes that FASTQ entries are unwrapped - that is all
 * entries consists of exactly 4 lines. Also, we assume there are no whitespace
 * in the sequences.
 *
 * Inherits from SeqReader in order to automatically determine the file format.
 */
class FastqReader : public SeqReader
{
 public:
  FastqReader(const std::string &file);
  FastqReader(const std::string &file, const int encoding);

  ~FastqReader();

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
   * Default FASTQ score encoding.
   */
  static const auto kDefaultEncoding = 33;

  /*
   * Size of custom buffer used to read from a FASTQ file a chunk of data this size
   */
  static const auto kBufferSize = 640 * 1024;

  /*
   * Size of sequence name buffer used to temporary store the sequence name while
   * parsing a FASTQ file. The size of this buffer is determined by kMaxNameSize,
   * which must be larger than the longest name or undefined things will happen.
  */
  static const auto kMaxNameSize = 1024;

  /*
   * Size of sequence buffer used to temporary store the sequence while parsing
   * a FASTQ file. The size of this buffer is determined by kMaxSeqSize, which
   * must be larger than the longest sequence or undefined things will happen.
   */
  static const auto kMaxSeqSize = 4096;

  /*
   * Size of scores buffer used to temporary store the scores while parsing
   * a FASTQ file. The size of this buffer is determined by kMaxScoresSize, which
   * must be larger than the longest sequence or undefined things will happen.
   */
  static const auto kMaxScoresSize = 4096;

  /*
   * Temporary file reading buffer.
   */
  ReadBuffer read_buffer_;

  /*
   * FASTQ score encoding.
   */
  size_t encoding_;

  /*
   * Temporary buffer for collecting sequence name.
   */
  char *name_buffer_;

  /*
   * Temporary buffer for collecting sequence.
   */
  char *seq_buffer_;

  /*
   * Temporary buffer for collecting scores.
   */
  char *scores_buffer_;

  /*
   * Get the next FASTQ header in the buffer.
   */
  void GetName(std::unique_ptr<SeqEntry> &seq_entry);

  /*
   * Get the next FASTQ sequence in the buffer.
   */
  void GetSeq(std::unique_ptr<SeqEntry> &seq_entry);

  /*
   * Get the next FASTQ scores in the buffer.
   */
  void GetScores(std::unique_ptr<SeqEntry> &seq_entry);

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

#endif  // BIOIO_FASTQ_READER_H_
