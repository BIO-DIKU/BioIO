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

#include <string>
#include <memory>
#include "catch.hpp"
#include <BioIO/bioio.h>

TEST_CASE("SeqReader with FASTA file", "[seq_reader]") {
  std::string file = "test/fasta_files/test1.fasta";

  auto reader = SeqReader::CreateReader(file);

  SECTION("First entry is read OK") {
    REQUIRE(reader->HasNextEntry());

    auto entry1 = reader->NextEntry();
    REQUIRE(entry1->name() == "1 K#Bacteria;P#Proteobacteria");
    REQUIRE(entry1->seq() == "ATCGUatcgu");

    REQUIRE(reader->HasNextEntry());
  }
}

TEST_CASE("SeqReader with FASTQ file", "[seq_reader]") {
  std::string file = "test/fastq_files/test1.fastq";

  auto reader = SeqReader::CreateReader(file);

  SECTION("First entry is read OK") {
    static const uint8_t scores[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    const std::vector<uint8_t> v(scores, scores + sizeof(scores) / sizeof(scores[0]));

    REQUIRE(reader->HasNextEntry());

    auto entry1 = reader->NextEntry();
    REQUIRE(entry1->name() == "test1");
    REQUIRE(entry1->seq() == "ATCGUatcgu");
    REQUIRE(entry1->scores() == v);

    REQUIRE(reader->HasNextEntry());
  }
}

TEST_CASE("SeqReader with bad file format", "[seq_reader]") {
  std::string file = "temp_file";

  remove(file.c_str());

  std::ofstream output;
  output.open(file);
  output << " " << std::endl;
  output << " A" << std::endl;
  output.close();

  SECTION("Throws exception") {
    try {
      auto reader = SeqReader::CreateReader(file);
      FAIL("SeqReader constructor did not throw expected exception");
    }

    catch (SeqReaderException& e) {
      REQUIRE(e.exceptionMsg == "Error: File not in FASTA or FASTQ format");
    }
  }

  remove(file.c_str());
}
