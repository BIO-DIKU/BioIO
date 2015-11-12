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

#include <fstream>
#include <BioIO/read_buffer.h>

ReadBuffer::ReadBuffer(const size_t buffer_size, const std::string &file) :
  buffer_size_(buffer_size),
  input_stream_(),
  buffer_(new char[buffer_size]),
  file_pos_(0),
  file_len_()
{
  input_stream_.rdbuf()->pubsetbuf(0, 0);
  input_stream_.open(file, std::ifstream::in);

  if (!input_stream_.good()) {
    std::string msg("File not found or not readable: " + file);
    // throw
  }

  GetFileSize();
  LoadBuffer();
}

ReadBuffer::~ReadBuffer() {
  input_stream_.close();
}

void ReadBuffer::GetFileSize() {
  input_stream_.seekg(0, input_stream_.end);
  file_len_ = input_stream_.tellg();
  input_stream_.seekg(0, input_stream_.beg);
}

void ReadBuffer::LoadBuffer() {
  input_stream_.read(buffer_, buffer_size_);

  file_pos_ = 0;
}

char ReadBuffer::GetChar() {
  if (file_pos_ == buffer_size_) {
    LoadBuffer();
  }

  if (file_len_-- == 0)
    return '\0';

  return buffer_[file_pos_++];
}
