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

#include "catch.hpp"

#include <BioIO/bioio.h>

#include <memory>
#include <string>

TEST_CASE("read 9 fasta files", "[fasta_reader]") {
  std::string filename1 = "test/fasta_files/test1.fasta";
  std::string filename2 = "test/fasta_files/test2.fasta";
  std::string filename3 = "test/fasta_files/test3.fasta";
  std::string filename4 = "test/fasta_files/test4.fasta";
  std::string filename5 = "test/fasta_files/test5.fasta";
  std::string filename6 = "test/fasta_files/test6.fasta";
  std::string filename7 = "test/fasta_files/test7.fasta";
  std::string filename8 = "test/fasta_files/test8.fasta";
  std::string filename9 = "test/fasta_files/test9.fasta";

  SECTION("Test with test1.fasta") {
    FastaReader reader(filename1);

    REQUIRE(reader.hasNextEntry());

    auto entry1 = reader.nextEntry();
    REQUIRE(entry1->name() == "1 K#Bacteria;P#Proteobacteria");
    REQUIRE(entry1->seq() == "ATCGUatcgu");

    REQUIRE(reader.hasNextEntry());

    auto entry2 = reader.nextEntry();
    REQUIRE(entry2->name() == "1 K#Bacteria;P#Proteobacteria");
    REQUIRE(entry2->seq() == "atcgu");

    REQUIRE_FALSE(reader.hasNextEntry());
  }

  SECTION("Test with test2.fasta") {
    FastaReader reader(filename2);

    REQUIRE(reader.hasNextEntry());

    auto entry1 = reader.nextEntry();
    REQUIRE(entry1->name() == "1");
    REQUIRE(entry1->seq() == "atcgATCG");

    REQUIRE(reader.hasNextEntry());

    auto entry2 = reader.nextEntry();
    REQUIRE(entry2->name() == "2");
    REQUIRE(entry2->seq() == "atcg");

    REQUIRE_FALSE(reader.hasNextEntry());
  }

  SECTION("Test with test3.fasta") {
    FastaReader reader(filename3);

    REQUIRE(reader.hasNextEntry());

    auto entry1 = reader.nextEntry();
    REQUIRE(entry1->name() == "1>2");
    REQUIRE(entry1->seq() == "AT>CG");

    REQUIRE_FALSE(reader.hasNextEntry());
  }

  SECTION("Test with test4.fasta") {
    FastaReader reader(filename4);

    REQUIRE(reader.hasNextEntry());

    try {
      reader.nextEntry();
      FAIL("reader did not throw expected exception");
    }
    catch (FastaException& e) {
      REQUIRE(e.errorCode == 3);
    }
  }

  SECTION("Test with test5.fasta") {
    FastaReader reader(filename5);

    REQUIRE(reader.hasNextEntry());

    try {
      reader.nextEntry();
      FAIL("reader.nextEntry() did not throw expected exception");
    }
    catch (FastaException& e) {
      REQUIRE(e.errorCode == 4);
    }
  }

  SECTION("Test with test6.fasta") {
    try {
      FastaReader reader(filename6);
      FAIL("reader did not throw expected exception");
    }
    catch (FastaException& e) {
      REQUIRE(e.errorCode == 2);
    }
  }

  SECTION("Test with test7.fasta") {
    FastaReader reader(filename7);

    REQUIRE(reader.hasNextEntry());

    try {
      reader.nextEntry();
      FAIL("reader.nextEntry() did not throw expected exception");
    }
    catch (FastaException& e) {
      REQUIRE(e.errorCode == 4);
    }
  }

  SECTION("Test with test8.fasta") {
    try {
      FastaReader reader(filename8);
      FAIL("reader did not throw expected exception");
    }
    catch (FastaException& e) {
      REQUIRE(e.errorCode == 5);
    }
  }

  SECTION("Test with test9.fasta") {
    FastaReader reader(filename9);

    REQUIRE(reader.hasNextEntry());

    try {
      reader.nextEntry();
      FAIL("reader.nextEntry() did not throw expected exception");
    }
    catch (FastaException& e) {
      REQUIRE(e.errorCode == 4);
    }
  }
}

#include <string>
#include <memory>
#include <fstream>
#include <exception>
#include <iostream>
#include <algorithm>

TEST_CASE("fasta file with windows line-endings", "[fasta_reader]") {
  /*std::ifstream inputStream;
  inputStream.open("../test/fasta_files/test2_cr.fasta", std::ifstream::in);
  std::string str;
  while(std::getline(inputStream, str)) {
    std::cout << str << std::endl;
  }
  std::cout << std::isspace('\r') << std::isspace('\n') << std::endl;
  inputStream.close();*/

  SECTION("Test file test2_cr.fasta") {
    std::string filepath = "test/fasta_files/test2_cr.fasta";
    CAPTURE(filepath);
    FastaReader reader(filepath, FastaReader::IgnoreContentBeforeFirstHeader);

    REQUIRE(reader.hasNextEntry());

    auto entry1 = reader.nextEntry();
    REQUIRE(entry1->name() == "1");
    REQUIRE(entry1->seq() == "atcgATCG");

    REQUIRE(reader.hasNextEntry());

    auto entry2 = reader.nextEntry();
    REQUIRE(entry2->name() == "2");
    REQUIRE(entry2->seq() == "atcg");

    REQUIRE_FALSE(reader.hasNextEntry());
  }
}

TEST_CASE("fasta reader exceptions", "[fasta_reader]") {
  SECTION("Test with non-existing file") {
    try {
      FastaReader reader("sdbvuoysgvo.873fg23");

      FAIL("Reader did not throw expected exception");
    }
    catch (FastaException& e) {
      REQUIRE(e.errorCode == 1);
    }
  }
}