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

#ifndef BIOIO_FASTA_READER_H_
#define BIOIO_FASTA_READER_H_

#include <string>
#include <memory>
#include <fstream>
#include <exception>

#include <BioIO/seq_entry.h>

/**
 * @brief Exception thrown by Fasta class
 */
class FastaException : public std::exception {
  public:
    FastaException(std::string &msg): exceptionMsg(msg) {}
    FastaException(std::string &&msg): exceptionMsg(msg) {}
    FastaException(const FastaException &e): exceptionMsg(e.exceptionMsg) {}
    const std::string exceptionMsg;
};

class FastaReader
{
  public:
    FastaReader(const FastaReader&) = delete;
    FastaReader& operator=(const FastaReader&) = delete;

    FastaReader(const std::string& filePath, int errorToleranceLevel = 0);
    FastaReader(FastaReader&& other);

    virtual ~FastaReader();

    std::unique_ptr<SeqEntry> nextEntry();

  private:
    std::ifstream m_inputStream;
    std::string m_nextHeader;
    int m_errorToleranceLevel;
};

#endif  // BIOIO_FASTA_READER_H_