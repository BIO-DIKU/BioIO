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

FastaReader::FastaReader(const std::string& filePath, const size_t buffer_size)
{
  input_stream.open(filePath, std::ifstream::in);

  if (!input_stream.good()) {
    std::string msg("FASTA file not found or not readable: " + filePath);
    throw FastaException(msg, 1);
  }

  input_stream.seekg(0, std::ios::end);
  input_len       = input_stream.tellg();
  buffer_pos      = 0;
  buffer.resize(buffer_size);
  BufferFlushAndFill();
  header_pos      = buffer.find_first_of('>');
  if(header_pos==std::string::npos){
    std::string msg("Bad FASTA format: No header found.");
    throw FastaException(msg, 2);
  }
}

//>hhhhhh\nssss\nsssssss\n>hhhhhh\nssssssss\n>hhhhhhh\nsssssssssss
//                |                |                |                | 
//

FastaReader::~FastaReader() {
  input_stream.close();
}

//Always assumes that header_pos points to a position in buffer with a '>'
std::unique_ptr<SeqEntry> FastaReader::nextEntry() {
  std::unique_ptr<SeqEntry> ret(new SeqEntry());
  //std::stringstream ss;

  //Extract header
  size_t next_seq_pos     = buffer.find_first_of('\n',header_pos);
  ret->name()             = buffer.substr(header_pos+1, next_seq_pos-header_pos-2);

  //Extract sequence
  size_t next_header_pos  = buffer.find_first_of('>',next_seq_pos+1);
  size_t seq_length_rem   = (next_header_pos==std::string::npos) ? std::string::npos : next_header_pos-next_seq_pos;
  //ss<<buffer.substr(next_seq_pos, seq_length_rem);
  ret->seq()+=buffer.substr(next_seq_pos, seq_length_rem);
  while (seq_length_rem==std::string::npos){
    buffer_pos += buffer.size();
    if(buffer_pos>=input_len)
      break;
    BufferFlushAndFill();
    next_seq_pos     = 0;
    next_header_pos  = buffer.find_first_of('>',0);
    seq_length_rem   = (next_header_pos==std::string::npos) ? std::string::npos : next_header_pos-next_seq_pos;
    //ss<<buffer.substr(next_seq_pos, seq_length_rem);
    ret->seq()+=buffer.substr(next_seq_pos, seq_length_rem);
  }
  //ret->seq() = ss.str();
  ret->seq().erase(
    std::remove_if(
      ret->seq().begin(),
      ret->seq().end(),
      [](char ch) {
        return std::isspace<char>( ch, std::locale::classic() );
      }
    ),
    ret->seq().end()
  );

  header_pos = next_header_pos;


  //if (m_nextHeader.empty()) {
  //  std::string msg("Bad FASTA format: Empty header.");
  //  throw FastaException(msg, 3);
  //}

  //seqPtr->name() = m_nextHeader;

  //while (windowsSafeGetLine(input_stream, m_nextHeader)) {
  //  if (m_nextHeader.length() == 0) continue;  // Ignore empty lines

  //  if (m_nextHeader.at(0) == '>') {
  //    m_nextHeader = m_nextHeader.substr(1);
  //    break;
  //  } else {
  //    ss << m_nextHeader;
  //  }
  //}

  //seqPtr->seq() = ss.str();
  //seqPtr->seq().erase(
  //  std::remove_if(
  //    seqPtr->seq().begin(),
  //    seqPtr->seq().end(),
  //    [](char ch) {
  //      return std::isspace<char>( ch, std::locale::classic() );
  //    }
  //  ),
  //  seqPtr->seq().end()
  //);

  if (ret->seq().empty()) {
    std::string msg("Bad FASTA format: Missing sequence for header \"" + ret->name() + "\"");
    throw FastaException(msg, 4);
  }

  return ret;
}

bool FastaReader::hasNextEntry() const {
  return header_pos!=std::string::npos;
}


// Assumes buffer_pos has been set
void FastaReader::BufferFlushAndFill()
{
  input_stream.seekg(buffer_pos, std::ios::beg);
  size_t remaining = std::min( buffer.size(), input_len-buffer_pos );
  buffer.resize(remaining);
  input_stream.read(&buffer[0], remaining);

}

