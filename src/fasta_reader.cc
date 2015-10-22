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

FastaReader::FastaReader(const std::string& filePath, int errorToleranceLevel) :
  m_errorToleranceLevel(errorToleranceLevel)
{
  m_inputStream.open(filePath, std::ifstream::in);

  if (!m_inputStream.good()) {
    std::string msg("FASTA file not found or not readable: " + filePath);
    throw FastaException(msg);
  }

  // Read until eof or the first header is located
  while (std::getline(inputStream, m_nextHeader)) {
    if (m_nextHeader.empty()) continue;  // Ignore empty lines

    if (m_nextHeader.at(0) == '>') {
      break;
    } else {
      std::string msg("Bad FASTA format: Contents before first header. Line: \"" +
        m_nextHeader + "\"");
      throw FastaException(msg);
    }
  }
}

FastaReader::FastaReader(FastaReader&& other) :
  m_inputStream(std::move(other.m_inputStream)),
  m_errorToleranceLevel(std::move(other.m_errorToleranceLevel))
{}

FastaReader::~FastaReader() {
  m_inputStream.close();
}

std::unique_ptr<SeqEntry> FastaReader::nextEntry() {
  std::unique_ptr<SeqEntry> seqPtr(new SeqEntry());
  

  return seqPtr;
}