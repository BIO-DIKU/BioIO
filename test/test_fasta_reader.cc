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

TEST_CASE("FastaReader w. OK entries", "[fasta_reader]") {
  std::string file = "test/fasta_files/test1.fasta";

  FastaReader reader(file);

  SECTION("First entry is read OK") {
    REQUIRE(reader.HasNextEntry());

    auto entry1 = reader.NextEntry();
    REQUIRE(entry1->name() == "1 K#Bacteria;P#Proteobacteria");
    REQUIRE(entry1->seq() == "ATCGUatcgu");

    REQUIRE(reader.HasNextEntry());
  }

reader.NextEntry();

  SECTION("Second entry is read OK") {
    REQUIRE(reader.HasNextEntry());

    auto entry2 = reader.NextEntry();
    REQUIRE(entry2->name() == "1 K#Bacteria;P#Proteobacteria");
    REQUIRE(entry2->seq() == "atcgu");

    REQUIRE_FALSE(reader.HasNextEntry());
  }
}

TEST_CASE("FastaReader w. OK entries with extra whitespace", "[fasta_reader]") {
  std::string file = "test/fasta_files/test2.fasta";
  FastaReader reader(file);

  SECTION("First entry is read OK and whitespace is stripped") {
    REQUIRE(reader.HasNextEntry());

    auto entry1 = reader.NextEntry();
    REQUIRE(entry1->name() == "1");
    REQUIRE(entry1->seq() == "atcgATCG");

    REQUIRE(reader.HasNextEntry());
  }

  reader.NextEntry();

  SECTION("Second entry is read OK and whitespace is stripped") {
    REQUIRE(reader.HasNextEntry());

    auto entry2 = reader.NextEntry();
    REQUIRE(entry2->name() == "2");
    REQUIRE(entry2->seq() == "atcg");

    REQUIRE_FALSE(reader.HasNextEntry());
  }
}

TEST_CASE("FastaReader w. OK entries with internal >", "[fasta_reader]") {
  std::string file = "test/fasta_files/test3.fasta";
  FastaReader reader(file);

  SECTION("First entry with extra > is read OK") {
    REQUIRE(reader.HasNextEntry());

    auto entry1 = reader.NextEntry();
    REQUIRE(entry1->name() == "1>2");
    REQUIRE(entry1->seq() == "AT>CG");

    REQUIRE(reader.HasNextEntry());
  }

  reader.NextEntry();

  SECTION("Second entry with extra > is read OK") {
    REQUIRE(reader.HasNextEntry());

    auto entry2 = reader.NextEntry();
    REQUIRE(entry2->name() == "3>4");
    REQUIRE(entry2->seq() == "cg>ta");

    REQUIRE_FALSE(reader.HasNextEntry());
  }
}

TEST_CASE("FastaReader w. only sequence throws", "[fasta_reader]") {
  std::string file = "test/fasta_files/test4.fasta";
  FastaReader reader(file);

  REQUIRE(reader.HasNextEntry());

  try {
    reader.NextEntry();
    FAIL("reader.NextEntry() did not throw expected exception");
  }

  catch (FastaReaderException& e) {
    REQUIRE(e.exceptionMsg == "Error: missing sequence name");
  }
}

TEST_CASE("FastaReader w. only sequence name throws", "[fasta_reader]") {
  std::string file = "test/fasta_files/test5.fasta";
  FastaReader reader(file);

  REQUIRE(reader.HasNextEntry());

  try {
    reader.NextEntry();
    FAIL("reader.NextEntry() did not throw expected exception");
  }

  catch (FastaReaderException& e) {
    REQUIRE(e.exceptionMsg == "Error: missing sequence");
  }
}

TEST_CASE("FastaReader w. missing header throws", "[fasta_reader]") {
  std::string file = "test/fasta_files/test6.fasta";
  FastaReader reader(file);

  REQUIRE(reader.HasNextEntry());

  try {
    reader.NextEntry();
    FAIL("reader.NextEntry() did not throw expected exception");
  }

  catch (FastaReaderException& e) {
    REQUIRE(e.exceptionMsg == "Error: File not in FASTA format");
  }
}

TEST_CASE("FastaReader w. missing sequence throws", "[fasta_reader]") {
  std::string file = "test/fasta_files/test7.fasta";
  FastaReader reader(file);

  REQUIRE(reader.HasNextEntry());

  try {
    reader.NextEntry();
    FAIL("reader.NextEntry() did not throw expected exception");
  }

  catch (FastaReaderException& e) {
    REQUIRE(e.exceptionMsg == "Error: missing sequence");
  }
}

TEST_CASE("FastaReader w. empty file throws", "[fasta_reader]") {
  std::string file = "test/fasta_files/test8.fasta";
  FastaReader reader(file);

  REQUIRE(reader.HasNextEntry());

  try {
    reader.NextEntry();
    FAIL("reader.NextEntry() did not throw expected exception");
  }

  catch (FastaReaderException& e) {
    REQUIRE(e.exceptionMsg == "Error: missing sequence name");
  }
}

TEST_CASE("FastaReader w. OK name and space only seq throws", "[fasta_reader]") {
  std::string file = "test/fasta_files/test9.fasta";
  FastaReader reader(file);

  REQUIRE(reader.HasNextEntry());

  try {
    reader.NextEntry();
    FAIL("reader.NextEntry() did not throw expected exception");
  }

  catch (FastaReaderException& e) {
    REQUIRE(e.exceptionMsg == "Error: missing sequence");
  }
}

TEST_CASE("FastaReader w. Windows line-endings are OK", "[fasta_reader]") {
  std::string file = "test/fasta_files/test10.fasta";
  FastaReader reader(file);

  SECTION("First entry is read OK and whitespace is stripped") {
    REQUIRE(reader.HasNextEntry());

    auto entry1 = reader.NextEntry();
    REQUIRE(entry1->name() == "1");
    REQUIRE(entry1->seq() == "atcgATCG");

    REQUIRE(reader.HasNextEntry());
  }

  reader.NextEntry();

  SECTION("Second entry is read OK and whitespace is stripped") {
    REQUIRE(reader.HasNextEntry());

    auto entry2 = reader.NextEntry();
    REQUIRE(entry2->name() == "2");
    REQUIRE(entry2->seq() == "atcg");

    REQUIRE_FALSE(reader.HasNextEntry());
  }
}

TEST_CASE("FastaReader w. missing file throws", "[fasta_reader]") {
  try {
    FastaReader reader("blefh");

    FAIL("Reader did not throw expected exception");
  }
  // catch (FastaReaderException& e) {  // FIXME:(Martin) Figure out inheritage
  catch (ReadBufferException& e) {
    REQUIRE(e.exceptionMsg == "Error: File not found or not readable: blefh");
  }
}

TEST_CASE("FastaReader w. non-FASTA content throws", "[fasta_reader]") {
  std::string file = "test/fasta_files/test11.fasta";
  FastaReader reader(file);

  REQUIRE(reader.HasNextEntry());

  try {
    reader.NextEntry();

    FAIL("Reader did not throw expected exception");
  }
  catch (FastaReaderException& e) {
    REQUIRE(e.exceptionMsg == "Error: File not in FASTA format");
  }
}
