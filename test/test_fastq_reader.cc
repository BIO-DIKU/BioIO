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

TEST_CASE("FastqReader w. OK entries", "[fastq_reader]") {
  std::string file = "test/fastq_files/test1.fastq";
  FastqReader reader(file);

  SECTION("First entry is read OK") {
    static const uint8_t scores[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    const std::vector<uint8_t> v(scores, scores + sizeof(scores) / sizeof(scores[0]));

    REQUIRE(reader.HasNextEntry());

    auto entry1 = reader.NextEntry();
    REQUIRE(entry1->name() == "test1");
    REQUIRE(entry1->seq() == "ATCGUatcgu");
    REQUIRE(entry1->scores() == v);

    REQUIRE(reader.HasNextEntry());
  }

reader.NextEntry();

  SECTION("Second entry is read OK") {
    static const uint8_t scores[] = {36, 37, 38, 39, 40};
    const std::vector<uint8_t> v(scores, scores + sizeof(scores) / sizeof(scores[0]));
    REQUIRE(reader.HasNextEntry());

    auto entry2 = reader.NextEntry();
    REQUIRE(entry2->name() == "test2");
    REQUIRE(entry2->seq() == "natcg");
    REQUIRE(entry2->scores() == v);

    REQUIRE_FALSE(reader.HasNextEntry());
  }
}

TEST_CASE("FastqReader w. OK entries with extra whitespace", "[fastq_reader]") {
  std::string file = "test/fastq_files/test2.fastq";
  FastqReader reader(file);

  SECTION("First entry is read OK") {
    static const uint8_t scores[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    const std::vector<uint8_t> v(scores, scores + sizeof(scores) / sizeof(scores[0]));

    REQUIRE(reader.HasNextEntry());

    auto entry1 = reader.NextEntry();
    REQUIRE(entry1->name() == "test1");
    REQUIRE(entry1->seq() == "ATCGUatcgu");
    REQUIRE(entry1->scores() == v);

    REQUIRE(reader.HasNextEntry());
  }

reader.NextEntry();

  SECTION("Second entry is read OK") {
    static const uint8_t scores[] = {36, 37, 38, 39, 40};
    const std::vector<uint8_t> v(scores, scores + sizeof(scores) / sizeof(scores[0]));
    REQUIRE(reader.HasNextEntry());

    auto entry2 = reader.NextEntry();
    REQUIRE(entry2->name() == "test2");
    REQUIRE(entry2->seq() == "natcg");
    REQUIRE(entry2->scores() == v);

    REQUIRE_FALSE(reader.HasNextEntry());
  }
}

TEST_CASE("FastqReader w. OK entries with internal >", "[fastq_reader]") {
  std::string file = "test/fastq_files/test3.fastq";
  FastqReader reader(file);

  SECTION("First entry with extra @ is read OK") {
    static const uint8_t scores[] = {0, 1, 2, 3, 4, 31, 5, 6, 7, 8, 9};
    const std::vector<uint8_t> v(scores, scores + sizeof(scores) / sizeof(scores[0]));

    REQUIRE(reader.HasNextEntry());

    auto entry1 = reader.NextEntry();
    REQUIRE(entry1->name() == "te@st1");
    REQUIRE(entry1->seq() == "ATCGU@atcgu");
    REQUIRE(entry1->scores() == v);

    REQUIRE(reader.HasNextEntry());
  }

  reader.NextEntry();

  SECTION("Second entry with extra @ is read OK") {
    REQUIRE(reader.HasNextEntry());

    auto entry2 = reader.NextEntry();
    REQUIRE(entry2->name() == "te@st2");
    REQUIRE(entry2->seq() == "na@tcg");

    REQUIRE_FALSE(reader.HasNextEntry());
  }
}

TEST_CASE("FastqReader w/o name throws", "[fastq_reader]") {
  std::string file = "test/fastq_files/test4.fastq";
  FastqReader reader(file);

  REQUIRE(reader.HasNextEntry());

  try {
    reader.NextEntry();
    FAIL("reader.NextEntry() did not throw expected exception");
  }

  catch (FastqReaderException& e) {
    REQUIRE(e.exceptionMsg == "Error: missing sequence name");
  }
}

TEST_CASE("FastqReader w/o sequence throws", "[fastq_reader]") {
  std::string file = "test/fastq_files/test5.fastq";
  FastqReader reader(file);

  REQUIRE(reader.HasNextEntry());

  try {
    reader.NextEntry();
    FAIL("reader.NextEntry() did not throw expected exception");
  }

  catch (FastqReaderException& e) {
    REQUIRE(e.exceptionMsg == "Error: missing sequence");
  }
}

TEST_CASE("FastqReader w/o scores throws", "[fastq_reader]") {
  std::string file = "test/fastq_files/test6.fastq";
  FastqReader reader(file);

  REQUIRE(reader.HasNextEntry());

  try {
    reader.NextEntry();
    FAIL("reader.NextEntry() did not throw expected exception");
  }

  catch (FastqReaderException& e) {
    REQUIRE(e.exceptionMsg == "Error: missing scores");
  }
}

TEST_CASE("FastqReader w. missing header throws", "[fastq_reader]") {
  std::string file = "test/fastq_files/test7.fastq";
  FastqReader reader(file);

  REQUIRE(reader.HasNextEntry());

  try {
    reader.NextEntry();
    FAIL("reader.NextEntry() did not throw expected exception");
  }

  catch (FastqReaderException& e) {
    REQUIRE(e.exceptionMsg == "Error: File not in FASTQ format");
  }
}

// TEST_CASE("FastqReader w. missing sequence throws", "[fastq_reader]") {
//   std::string file = "test/fastq_files/test7.fastq";
//   FastqReader reader(file);
//
//   REQUIRE(reader.HasNextEntry());
//
//   try {
//     reader.NextEntry();
//     FAIL("reader.NextEntry() did not throw expected exception");
//   }
//
//   catch (FastqReaderException& e) {
//     REQUIRE(e.exceptionMsg == "Error: missing sequence");
//   }
// }
//
// TEST_CASE("FastqReader w. empty file throws", "[fastq_reader]") {
//   std::string file = "test/fastq_files/test8.fastq";
//   FastqReader reader(file);
//
//   REQUIRE(reader.HasNextEntry());
//
//   try {
//     reader.NextEntry();
//     FAIL("reader.NextEntry() did not throw expected exception");
//   }
//
//   catch (FastqReaderException& e) {
//     REQUIRE(e.exceptionMsg == "Error: missing sequence name");
//   }
// }
//
// TEST_CASE("FastqReader w. OK name and space only seq throws", "[fastq_reader]") {
//   std::string file = "test/fastq_files/test9.fastq";
//   FastqReader reader(file);
//
//   REQUIRE(reader.HasNextEntry());
//
//   try {
//     reader.NextEntry();
//     FAIL("reader.NextEntry() did not throw expected exception");
//   }
//
//   catch (FastqReaderException& e) {
//     REQUIRE(e.exceptionMsg == "Error: missing sequence");
//   }
// }
//
// TEST_CASE("FastqReader w. Windows line-endings are OK", "[fastq_reader]") {
//   std::string file = "test/fastq_files/test10.fastq";
//   FastqReader reader(file);
//
//   SECTION("First entry is read OK and whitespace is stripped") {
//     REQUIRE(reader.HasNextEntry());
//
//     auto entry1 = reader.NextEntry();
//     REQUIRE(entry1->name() == "1");
//     REQUIRE(entry1->seq() == "atcgATCG");
//
//     REQUIRE(reader.HasNextEntry());
//   }
//
//   reader.NextEntry();
//
//   SECTION("Second entry is read OK and whitespace is stripped") {
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
// TEST_CASE("FastqReader w. missing file throws", "[fastq_reader]") {
//   try {
//     FastqReader reader("blefh");
//
//     FAIL("Reader did not throw expected exception");
//   }
//   // catch (FastqReaderException& e) {  // FIXME:(Martin) Figure out inheritage
//   catch (ReadBufferException& e) {
//     REQUIRE(e.exceptionMsg == "Error: File not found or not readable: blefh");
//   }
// }
//
// TEST_CASE("FastqReader w. non-FASTA content throws", "[fastq_reader]") {
//   std::string file = "test/fastq_files/test11.fastq";
//   FastqReader reader(file);
//
//   REQUIRE(reader.HasNextEntry());
//
//   try {
//     reader.NextEntry();
//
//     FAIL("Reader did not throw expected exception");
//   }
//   catch (FastqReaderException& e) {
//     REQUIRE(e.exceptionMsg == "Error: File not in FASTA format");
//   }
// }
