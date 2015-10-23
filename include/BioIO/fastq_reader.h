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
#include <BioIO/fasta_reader.h>

class FastqReader
{
  public:
    FastqReader(const FastqReader&) = delete;
    FastqReader& operator=(const FastqReader&) = delete;
    FastqReader(FastqReader&&) = delete;

    FastqReader(const std::string& filePath, int errorToleranceLevel = 0);

    virtual ~FastqReader();

    std::unique_ptr<SeqEntry> nextEntry();

    bool hasNextEntry() const;

  private:
    std::istream& windowsSafeGetLine(std::istream& is, std::string& str);

  private:
    std::ifstream m_inputStream;
    int m_errorToleranceFlags;
};

#endif  // BIOIO_FASTQ_READER_H_