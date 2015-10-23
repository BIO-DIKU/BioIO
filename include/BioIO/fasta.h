#ifndef FASTA_H_
#define FASTA_H_

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

/**
 * @brief A sequence entry container.
 */
struct FastaEntry{
  /** Header info contained between '>' character and end-of-line */
  std::string header;
  /** Sequence info is all printable characters between two header-lines or a
   * header-line and eof. */
  std::string sequence;
};

/**
 * @brief Exception thrown by Fasta class
 */
class FastaException: public std::exception {
  public:
    FastaException(std::string &msg): exceptionMsg(msg) {}
    FastaException(std::string &&msg): exceptionMsg(msg) {}
    FastaException(const FastaException &e): exceptionMsg(e.exceptionMsg) {}
    const std::string exceptionMsg;
};

/**
 * @brief Class for parsing fasta-files
 */
class Fasta {
  public:
    /**
     * @brief Set up a fasta-file for reading from the specified file.
     * Throws an exception if the first entry has empty header.
     */
    Fasta(const std::string &filePath);

    /**
     * @brief Read the next entry from the fasta-file.
     * Blank lines are ignored.
     * The behavior of this function is undefined if hasNextEntry() is false.
     * An exception is thrown if an empty header or sequence is encountered.
     * Empty spaces are not stripped from either sequences or headers.
     */
    FastaEntry nextEntry();

    /**
     * @brief Checks if there are any more entries in the Fasta file.
     */
    bool hasNextEntry();

  private:
    std::ifstream inputStream;
    std::string nextHeader;
};

#endif   // FASTA_H_
