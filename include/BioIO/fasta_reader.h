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
#include <iostream>

#include <BioIO/seq_entry.h>

/**
 * @brief Exception thrown by Fasta class
 */
class FastaException : public std::exception {
  public:
    FastaException(const std::string& msg, int code = 0) :
      exceptionMsg(msg), errorCode(code) {}
    FastaException(const FastaException& e) :
      exceptionMsg(e.exceptionMsg), errorCode(e.errorCode) {}
    FastaException(FastaException&& e) :
      exceptionMsg(std::move(e.exceptionMsg)), errorCode(e.errorCode) {}

    virtual const char* what() const throw() { return exceptionMsg.c_str(); }

    const std::string exceptionMsg;
    int errorCode; // Mostly used for tests
};

class FastaReader
{
  public:
    static const uint8_t ignore_content_before_first_header = 0x01;

  public:
    FastaReader(const FastaReader&) = delete;
    FastaReader& operator=(const FastaReader&) = delete;
    FastaReader(FastaReader&&) = delete;

    FastaReader(const std::string& file_path, int errorToleranceLevel = 0);

    virtual ~FastaReader();

    std::unique_ptr<SeqEntry> nextEntry();

    bool hasNextEntry() const;

  private:
    static const size_t MAX_LINE = 100000;
    char* result;
    char input_line[MAX_LINE];
    FILE * fp;
    std::unique_ptr<SeqEntry> seqPtr;
    int           error_tolerance_flags_;

    std::istream& windowsSafeGetLine(std::istream& is, std::string& str);
};

#endif  // BIOIO_FASTA_READER_H_
