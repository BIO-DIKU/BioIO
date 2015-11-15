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

std::string file3 = "fasta_test_files/test3.fasta";
std::string file4 = "fasta_test_files/test4.fasta";
std::string file5 = "fasta_test_files/test5.fasta";
std::string file6 = "fasta_test_files/test6.fasta";
std::string file7 = "fasta_test_files/test7.fasta";
std::string file8 = "fasta_test_files/test8.fasta";
std::string file9 = "fasta_test_files/test9.fasta";

TEST_CASE("FastaReader w. OK entries", "[fasta_reader]") {
  std::string file = "fasta_test_files/test1.fasta";

  FastaReader reader(file);

  SECTION("First entry is read OK") {
    REQUIRE(reader.HasNextEntry());

    auto entry1 = reader.NextEntry();

    REQUIRE(entry1->name() == "1 K#Bacteria;P#Proteobacteria");
    REQUIRE(entry1->seq() == "ATCGUatcgu");
  }

  SECTION("Second entry is read OK") {
    REQUIRE(reader.HasNextEntry());

    auto entry2 = reader.NextEntry();
    REQUIRE(entry2->name() == "1 K#Bacteria;P#Proteobacteria");
    REQUIRE(entry2->seq() == "atcgu");

    REQUIRE_FALSE(reader.HasNextEntry());
  }
}

TEST_CASE("FastaReader w. OK entries with extra whitespace", "[fasta_reader]") {
  std::string file = "fasta_test_files/test2.fasta";
  FastaReader reader(file);

  SECTION("First entry is read OK and whitespace is stripped") {
    REQUIRE(reader.HasNextEntry());

    auto entry1 = reader.NextEntry();
    REQUIRE(entry1->name() == "1");
    REQUIRE(entry1->seq() == "atcgATCG");
  }

  SECTION("Second entry is read OK and whitespace is stripped") {
    REQUIRE(reader.HasNextEntry());

    auto entry2 = reader.NextEntry();
    REQUIRE(entry2->name() == "2");
    REQUIRE(entry2->seq() == "atcg");

    REQUIRE_FALSE(reader.HasNextEntry());
  }
}

TEST_CASE("FastaReader w. OK entries with internal >", "[fasta_reader]") {
  std::string file = "fasta_test_files/test3.fasta";
  FastaReader reader(file);

  SECTION("First entry with extra > is read OK") {
    REQUIRE(reader.HasNextEntry());

    auto entry1 = reader.NextEntry();
    REQUIRE(entry1->name() == "1>2");
    REQUIRE(entry1->seq() == "AT>CG");
  }

  SECTION("Second entry with extra > is read OK") {
    REQUIRE(reader.HasNextEntry());

    auto entry1 = reader.NextEntry();
    REQUIRE(entry1->name() == "3>4");
    REQUIRE(entry1->seq() == "cg>at");

    REQUIRE_FALSE(reader.HasNextEntry());
  }
}

//    REQUIRE_THROWS_AS(reader.NextEntry(), FastaReaderException);

//   SECTION("Test with test4.fasta") {
//     FastaReader reader(file4);
//
//     REQUIRE(reader.HasNextEntry());
//
//     try {
//       reader.NextEntry();
//       FAIL("reader did not throw expected exception");
//     }
//     catch (FastaException& e) {
//       REQUIRE(e.errorCode == 3);
//     }
//   }
//
//   SECTION("Test with test5.fasta") {
//     FastaReader reader(file5);
//
//     REQUIRE(reader.HasNextEntry());
//
//     try {
//       reader.NextEntry();
//       FAIL("reader.NextEntry() did not throw expected exception");
//     }
//     catch (FastaException& e) {
//       REQUIRE(e.errorCode == 4);
//     }
//   }
//
//   SECTION("Test with test6.fasta") {
//     try {
//       FastaReader reader(file6);
//       FAIL("reader did not throw expected exception");
//     }
//     catch (FastaException& e) {
//       REQUIRE(e.errorCode == 2);
//     }
//   }
//
//   SECTION("Test with test7.fasta") {
//     FastaReader reader(file7);
//
//     REQUIRE(reader.HasNextEntry());
//
//     try {
//       reader.NextEntry();
//       FAIL("reader.NextEntry() did not throw expected exception");
//     }
//     catch (FastaException& e) {
//       REQUIRE(e.errorCode == 4);
//     }
//   }
//
//   SECTION("Test with test8.fasta") {
//     try {
//       FastaReader reader(file8);
//       FAIL("reader did not throw expected exception");
//     }
//     catch (FastaException& e) {
//       REQUIRE(e.errorCode == 5);
//     }
//   }
//
//   SECTION("Test with test9.fasta") {
//     FastaReader reader(file9);
//
//     REQUIRE(reader.HasNextEntry());
//
//     try {
//       reader.NextEntry();
//       FAIL("reader.NextEntry() did not throw expected exception");
//     }
//     catch (FastaException& e) {
//       REQUIRE(e.errorCode == 4);
//     }
//   }
// }
//
// TEST_CASE("fasta file with windows line-endings", "[fasta_reader]") {
//   SECTION("Test file test2_cr.fasta") {
//     std::string filepath = "fasta_test_files/test2_cr.fasta";
//     CAPTURE(filepath);
//     FastaReader reader(filepath, FastaReader::ignore_content_before_first_header);
//
//     REQUIRE(reader.HasNextEntry());
//
//     auto entry1 = reader.NextEntry();
//     REQUIRE(entry1->name() == "1");
//     REQUIRE(entry1->seq() == "atcgATCG");
//
//     REQUIRE(reader.HasNextEntry());
//
//     auto entry2 = reader.NextEntry();
//     REQUIRE(entry2->name() == "2");
//     REQUIRE(entry2->seq() == "atcg");
//
//     REQUIRE_FALSE(reader.HasNextEntry());
//   }
// }
//
// TEST_CASE("fasta reader exceptions", "[fasta_reader]") {
//   SECTION("Test with non-existing file") {
//     try {
//       FastaReader reader("sdbvuoysgvo.873fg23");
//
//       FAIL("Reader did not throw expected exception");
//     }
//     catch (FastaException& e) {
//       REQUIRE(e.errorCode == 1);
//     }
//   }
// }
