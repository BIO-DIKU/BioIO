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

#ifndef BIOIO_SEQ_READER_H_
#define BIOIO_SEQ_READER_H_

#include <string>
#include <memory>
#include <fstream>
#include <exception>
#include <iostream>

#include <BioIO/seq_entry.h>
#include <BioIO/read_buffer.h>

/**
 * @brief Exception class for SeqReader class.
 *
 * @example
 *   std::string msg = "Exception message";
 *   throw SeqReaderException(msg);
 *
 * @example
 *   throw SeqReaderException("Exception message");
 */
class SeqReaderException : public std::exception {
 public:
  SeqReaderException(std::string &msg) :
    exceptionMsg(msg)
  {}

  SeqReaderException(const SeqReaderException &e) :
    exceptionMsg(e.exceptionMsg)
  {}

  virtual const char* what() const throw() { return exceptionMsg.c_str(); }

  const std::string exceptionMsg;
};

/**
 * @brief Class for identifying the type of a given sequence file as either a
 * FASTA or FASTQ format and return a unique_ptr inheriting from FastaReader or
 * FastqReader, respectively.
 */
class SeqReader
{
 public:
  SeqReader(const std::string &file);

  ~SeqReader();

  virtual bool HasNextEntry() = 0;

 private:

  /*
   * Size of custom buffer used to determine file type.
   */
  static const auto kBufferSize = 640 * 1024;

  /*
   * File read buffer.
   */
  ReadBuffer read_buffer_;

  /*
   * Determine the file type and return this.
   */
  std::unique_ptr<SeqReader> DetermineFileType(const std::string &file);
};

#endif  // BIOIO_SEQ_READER_H_
