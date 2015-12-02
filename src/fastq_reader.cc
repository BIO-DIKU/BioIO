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

#include <BioIO/fastq_reader.h>
#include <BioIO/read_buffer.h>

#include <sstream>
#include <iostream>
#include <string>

FastqReader::FastqReader(const std::string &file) :
  read_buffer_(FastqReader::kBufferSize, file),
  encoding_(kDefaultEncoding),
  name_buffer_(new char[FastqReader::kMaxNameSize]),
  seq_buffer_(new char[FastqReader::kMaxSeqSize]),
  scores_buffer_(new char[FastqReader::kMaxScoresSize])
{}

FastqReader::FastqReader(const std::string &file, const int encoding) :
  read_buffer_(FastqReader::kBufferSize, file),
  encoding_(encoding),
  name_buffer_(new char[FastqReader::kMaxNameSize]),
  seq_buffer_(new char[FastqReader::kMaxSeqSize]),
  scores_buffer_(new char[FastqReader::kMaxScoresSize])
{}

FastqReader::~FastqReader()
{
  delete[] name_buffer_;
  delete[] seq_buffer_;
  delete[] scores_buffer_;
}

std::unique_ptr<SeqEntry> FastqReader::NextEntry() {
  std::unique_ptr<SeqEntry> seq_entry(new SeqEntry());

  GetName(seq_entry);
  GetSeq(seq_entry);
  GetScores(seq_entry);

  return seq_entry;
}

bool FastqReader::HasNextEntry() {
  return !read_buffer_.Eof();
}

void FastqReader::GetName(std::unique_ptr<SeqEntry> &seq_entry) {
  int  name_index = 0;
  char c;

  while ((c = read_buffer_.NextChar()) && (c != '@')) {
    if (isalpha(c)) {
      std::string msg = "Error: File not in FASTQ format";

      throw FastqReaderException(msg);
    }
  }

  while ((c = read_buffer_.NextChar()) && !isendl(c)) {
    name_buffer_[name_index++] = c;
  }

  if (!name_index) {
    std::string msg = "Error: missing sequence name";
    throw FastqReaderException(msg);
  }

  seq_entry->set_name(std::string(name_buffer_, name_index));
}

void FastqReader::GetSeq(std::unique_ptr<SeqEntry> &seq_entry) {
  int  seq_index = 0;
  char c;

  while ((c = read_buffer_.NextChar()) && !isendl(c)) {
    seq_buffer_[seq_index++] = c;
  }

  if (!seq_index) {
    std::string msg = "Error: missing sequence";
    throw FastqReaderException(msg);
  }

  seq_entry->set_seq(std::string(seq_buffer_, seq_index));
}

void FastqReader::GetScores(std::unique_ptr<SeqEntry> &seq_entry) {
  size_t scores_index = 0;
  char c;

  // Skip comment line.
  while ((c = read_buffer_.NextChar()) && !isendl(c)) {}

  while ((c = read_buffer_.NextChar()) && !isendl(c)) {
    scores_buffer_[scores_index++] = c;
  }

  if (!scores_index) {
    std::string msg = "Error: missing scores";
    throw FastqReaderException(msg);
  }

  if (seq_entry->Size() != scores_index) {
    std::string msg = "Error: Sequence length != scores length: " +
                      std::to_string(seq_entry->Size()) + " != " +
                      std::to_string(scores_index);
    throw FastqReaderException(msg);
  }

  std::vector<uint8_t> scores(scores_index, 0);

  for (size_t i = 0; i < scores_index; ++i) {
    scores[i] = scores_buffer_[i] - encoding_;
  }

  seq_entry->set_scores(scores);
}
