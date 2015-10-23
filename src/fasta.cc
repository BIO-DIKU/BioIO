#include <BioIO/fasta.h>

#include <string>
#include <vector>

Fasta::Fasta(const std::string &filePath): 
  inputStream(filePath.c_str(), std::ifstream::in) 
{
  if (!inputStream) {
    std::string msg("FASTA file not found or not readable: " + filePath);
    throw FastaException(msg);
  }

  // Read until eof or the first header is located
  std::string line;
  while (std::getline(inputStream, line)) {
    if (line.empty()) continue;            // Ignore empty lines

    if (line.at(0) == '>') {
      nextHeader = line.substr(1);
      break;
    } else {
      std::string msg("Bad FASTA format: Contents before first header. Line: \"" + line + "\"");
      throw FastaException(msg);
    }
  }
}

FastaEntry Fasta::nextEntry() 
{
  std::string header = nextHeader;
  std::string sequence;
  nextHeader = "";

  if (header.empty()) {
    std::string msg("Bad FASTA format: Empty header.");
    throw FastaException(msg);
  }

  std::string line;

  while (std::getline(inputStream, line)) {
    if (line.length() == 0) continue;            // Ignore empty lines

    if (line.at(0) == '>') {
      nextHeader = line.substr(1);
      break;
    } else {
      sequence = sequence + line;
    }
  }

  if (sequence.empty()) {
    std::string msg("Bad FASTA format: Missing sequence for header \"" + header + "\"");
    throw FastaException(msg);
  }

  FastaEntry ret;
  ret.header   = header;
  ret.sequence = sequence;
  return ret;
}

bool Fasta::hasNextEntry() 
{
  return !inputStream.eof();
}
