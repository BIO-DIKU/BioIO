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

#ifndef BIOIO_READ_BUFFER_H_
#define BIOIO_READ_BUFFER_H_

#include <string>
#include <fstream>
#include <iostream>

/**
 * @brief Exception class for ReadBuffer class.
 *
 * @example
 *   std::string msg = "Exception message";
 *   throw ReadBufferException(msg);
 *
 * @example
 *   throw ReadBufferException("Exception message");
 */
class ReadBufferException : public std::exception {
 public:
  ReadBufferException(std::string &msg) :
    exceptionMsg(msg)
  {}

  ReadBufferException(const ReadBufferException &e) :
    exceptionMsg(e.exceptionMsg)
  {}

  virtual const char* what() const throw() { return exceptionMsg.c_str(); }

  const std::string exceptionMsg;
};

class ReadBuffer
{
 public:
  ReadBuffer(const size_t size, const std::string &file);

  ~ReadBuffer();

  /*
   * Get the next char from the read buffer.
   */
  char NextChar();

  /*
   * Get the previous char from the read buffer.
   */
  char PrevChar();

  /*
   * Rewind the ReadBuffer with len chars.
   */
  void Rewind(size_t len);

  /*
   * Return wether end-of-file is reached.
   */
  bool Eof();

 private:

  /*
   * Size of read buffer.
   */
  const size_t buffer_size_;

  /*
   * Input stream of file being read.
   */
  std::ifstream input_stream_;

  /*
   * Read buffer.
   */
  char *buffer_;

  /*
   * Current position in buffer being read.
   */
  size_t buffer_pos_;

  /*
   * Size of file being read.
   */
  int file_size_;

  /*
   * Get the file size from the istream and store in file_size_.
   */
  void GetFileSize();

  /*
   * Read data from the input stream into the buffer.
   */
  void LoadBuffer();
};

#endif  // BIOIO_READ_BUFFER_H_
