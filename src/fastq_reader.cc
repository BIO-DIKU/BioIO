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

FastqReader::FastqReader(const std::string& filePath, int errorToleranceFlags) :
  m_errorToleranceFlags(errorToleranceFlags)
{
  m_inputStream.open(filePath, std::ifstream::in);

  if (!m_inputStream.good()) {
    std::string msg("FASTQ file not found or not readable: " + filePath);
    throw FastaException(msg, 1);
  }
}

FastqReader::~FastqReader() {
  m_inputStream.close();
}

std::unique_ptr<SeqEntry> FastqReader::nextEntry() {
  std::unique_ptr<SeqEntry> seqPtr(new SeqEntry());

  if(!windowsSafeGetLine(m_inputStream, seqPtr->name())) {
    std::string msg("FASTQ file ended unexpectedly when attempting to read header.");
    throw FastaException(msg, 2);
  }
  if (seqPtr->name().at(0) != '@') {
    std::string msg("Bad FASTQ format: Expected header, found: " + seqPtr->name());
    throw FastaException(msg, 3);
  }

  if(!windowsSafeGetLine(m_inputStream, seqPtr->seq())) {
    std::string msg("FASTQ file ended unexpectedly when attempting to read sequence.");
    throw FastaException(msg, 4);
  }

  std::string comment;
  if(!windowsSafeGetLine(m_inputStream, comment)) {
    std::string msg("FASTQ file ended unexpectedly when attempting to read comment.");
    throw FastaException(msg, 5);
  }
  if (seqPtr->name().at(0) != '+') {
    std::string msg("Bad FASTQ format: Expected comment, found: " + comment);
    throw FastaException(msg, 6);
  }

  std::string quality;
  if(!windowsSafeGetLine(m_inputStream, quality)) {
    std::string msg("FASTQ file ended unexpectedly when attempting to read sequence quality.");
    throw FastaException(msg, 7);
  }

  const size_t seqSize = seqPtr->seq().size();
  if(quality.size() != seqSize) {
    std::string msg("Bad FASTQ format: Seqence and sequence quality does not have the same length.");
    throw FastaException(msg, 8);
  }

  seqPtr->scores().reserve(seqSize);
  for(size_t i = 0; i < seqSize; ++i) {
    seqPtr->scores()[i] = static_cast<uint8_t>(quality[i]) - 33;
  }

  return seqPtr;
}

bool FastqReader::hasNextEntry() const {
  return !m_inputStream.eof();
}

std::istream& FastqReader::windowsSafeGetLine(std::istream& is, std::string& str) {
  std::getline(is, str);

  // handle evil windows line endings
  if (str.back() == '\r')
    str.pop_back();

  return is;
}