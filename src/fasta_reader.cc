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

FastaReader::FastaReader(const std::string& filePath, int errorToleranceFlags) :
  m_errorToleranceFlags(errorToleranceFlags)
{
  m_inputStream.open(filePath, std::ifstream::in);

  if (!m_inputStream.good()) {
    std::string msg("FASTA file not found or not readable: " + filePath);
    throw FastaException(msg, 1);
  }

  // Read until eof or the first header is located
  while (std::getline(m_inputStream, m_nextHeader)) {
    if (m_nextHeader.empty()) continue;  // Ignore empty lines

    if (m_nextHeader.at(0) == '>') {
      break;
    } else if (m_errorToleranceFlags & IgnoreContentBeforeFirstHeader) {
      //continue;
    } else {
      std::string msg("Bad FASTA format: Contents before first header. Line: \"" +
        m_nextHeader + "\"");
      throw FastaException(msg, 2);
    }
  }

  if(m_nextHeader.at(0) != '>') {
    std::string msg("Bad FASTA format: No header found.");
    throw FastaException(msg, 5);
  }
  
  m_nextHeader = m_nextHeader.substr(1);
}

FastaReader::~FastaReader() {
  m_inputStream.close();
}

std::unique_ptr<SeqEntry> FastaReader::nextEntry() {
  std::unique_ptr<SeqEntry> seqPtr(new SeqEntry());
  std::stringstream ss;

  if (m_nextHeader.empty()) {
    std::string msg("Bad FASTA format: Empty header.");
    throw FastaException(msg, 3);
  }

  seqPtr->name() = m_nextHeader;

  while (std::getline(m_inputStream, m_nextHeader)) {
    if (m_nextHeader.length() == 0) continue;  // Ignore empty lines

    if (m_nextHeader.at(0) == '>') {
      m_nextHeader = m_nextHeader.substr(1);
      break;
    } else {
      ss << m_nextHeader;
    }
  }

  seqPtr->seq() = ss.str();

  seqPtr->seq().erase(
    std::remove_if(
      seqPtr->seq().begin(),
      seqPtr->seq().end(),
      [](char ch) {
        return std::isspace<char>(
          ch,
          std::locale::classic()
        );
      }
    ),
    seqPtr->seq().end()
  );

  if (seqPtr->seq().empty()) {
    std::string msg("Bad FASTA format: Missing sequence for header \"" + m_nextHeader + "\"");
    throw FastaException(msg, 4);
  }

  return seqPtr;
}

bool FastaReader::hasNextEntry() const {
  return !m_inputStream.eof();
}
