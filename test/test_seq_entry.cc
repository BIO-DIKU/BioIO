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

#include <ostream>
#include <vector>
#include <sstream>
#include <BioIO/bioio.h>

TEST_CASE("sequences can be constructed, copied and moved", "[sequence]") {
  SeqEntry t1;
  SeqEntry t2 = SeqEntry("Name", "Sequence", {}, SeqEntry::SeqType::protein);

  REQUIRE(t1.name() == "");
  REQUIRE(t1.seq() == "");
  REQUIRE(t1.scores().empty());
  REQUIRE(t1.type() == SeqEntry::SeqType::nucleotide);

  REQUIRE(t2.name() == "Name");
  REQUIRE(t2.seq() == "Sequence");
  REQUIRE(t2.scores().empty());
  REQUIRE(t2.type() == SeqEntry::SeqType::protein);

  SECTION("Copy SeqEntry 2 to SeqEntry 1") {
    t1 = t2;

    REQUIRE(t1.name() == "Name");
    REQUIRE(t1.seq() == "Sequence");
    REQUIRE(t1.scores().empty());
    REQUIRE(t1.type() == SeqEntry::SeqType::protein);
  }

  SECTION("Use copy contructor for new SeqEntry") {
    SeqEntry t3(t2);

    REQUIRE(t3.name() == "Name");
    REQUIRE(t3.seq() == "Sequence");
    REQUIRE(t3.scores().empty());
    REQUIRE(t3.type() == SeqEntry::SeqType::protein);
  }

  SECTION("Use move contructor for new SeqEntry") {
    SeqEntry t4 = std::move(t2);

    REQUIRE(t4.name() == "Name");
    REQUIRE(t4.seq() == "Sequence");
    REQUIRE(t4.scores().empty());
    REQUIRE(t4.type() == SeqEntry::SeqType::protein);

    REQUIRE(t2.name() == "");
    REQUIRE(t2.seq() == "");
    REQUIRE(t2.scores().empty());
    REQUIRE(t2.type() == SeqEntry::SeqType::protein);
  }
}

TEST_CASE("subsequences can be extracted from sequences", "[sequence]") {
  SECTION("With empty scores") {
    SeqEntry t1 = SeqEntry("Name", "Sequence", {}, SeqEntry::SeqType::nucleotide);

    REQUIRE(t1.name() == "Name");
    REQUIRE(t1.seq() == "Sequence");
    REQUIRE(t1.scores().empty());
    REQUIRE(t1.type() == SeqEntry::SeqType::nucleotide);

    SeqEntry t2 = t1.SubSeq(2, 4);

    // t1 stays unchanged
    REQUIRE(t1.name() == "Name");
    REQUIRE(t1.seq() == "Sequence");
    REQUIRE(t1.scores().empty());
    REQUIRE(t1.type() == SeqEntry::SeqType::nucleotide);

    // t2 contains expected subsequence of t1 and they are otherwise identical
    REQUIRE(t2.name() == "Name");
    REQUIRE(t2.seq() == "quen");
    REQUIRE(t2.scores().empty());
    REQUIRE(t2.type() == SeqEntry::SeqType::nucleotide);
  }

  SECTION("With empty scores") {
    static const uint8_t scores[] = {0,1,2,3,4,5,6,7};
    const std::vector<uint8_t> v(scores, scores + sizeof(scores) / sizeof(scores[0]));

    SeqEntry t1 = SeqEntry("Name", "Sequence", v, SeqEntry::SeqType::nucleotide);

    REQUIRE(t1.name() == "Name");
    REQUIRE(t1.seq() == "Sequence");
    REQUIRE(t1.scores() == v);
    REQUIRE(t1.type() == SeqEntry::SeqType::nucleotide);

    SeqEntry t2 = t1.SubSeq(2, 4);

    // t1 stays unchanged
    REQUIRE(t1.name() == "Name");
    REQUIRE(t1.seq() == "Sequence");
    REQUIRE(t1.scores() == v);
    REQUIRE(t1.type() == SeqEntry::SeqType::nucleotide);

    std::vector<uint8_t>::const_iterator first = v.begin() + 2;
    std::vector<uint8_t>::const_iterator last = first + 4;
    const std::vector<uint8_t> newVec(first, last);

    // t2 contains expected subsequence of t1 and they are otherwise identical
    REQUIRE(t2.name() == "Name");
    REQUIRE(t2.seq() == "quen");
    REQUIRE(t2.scores() == newVec);
    REQUIRE(t2.type() == SeqEntry::SeqType::nucleotide);
  }
}

TEST_CASE("write SeqEntry", "[sequence]") {
  SeqEntry t1 = SeqEntry("Name", "Sequence", {}, SeqEntry::SeqType::nucleotide);

  std::stringstream ss;

  ss << t1;

  REQUIRE(ss.str() == ">Name\nSequence");
}


void test_reverse(SeqEntry& entry, std::string& exp_name, std::vector<uint8_t> exp_scores) {
  std::string oldName = entry.name();
  SeqEntry::SeqType oldType = entry.type();

  entry.reverse();

  // Name and type should stay the same
  REQUIRE(entry.name() == oldName);
  REQUIRE(entry.type() == oldType);

  // Sequence and scores should be as expected
  REQUIRE(entry.seq() == exp_name);
  REQUIRE(entry.scores() == exp_scores);

}

TEST_CASE("Reverse SeqEntry", "[sequence]") {
  SECTION("Reverse entry with empty scores") {
    SeqEntry s1 = SeqEntry("Name", "Sequence", {}, SeqEntry::SeqType::nucleotide);
    std::string s("ecneuqeS");
    std::vector<uint8_t> v;

    test_reverse(s1, s, v);
  }

  SECTION("Reverse entry with scores") {
    // Initialize scores
    static const uint8_t scores[] = {0,1,2,3,4,5,6,7};
    const std::vector<uint8_t> v(scores, scores + sizeof(scores) / sizeof(scores[0]));

    SeqEntry s1 = SeqEntry("Name", "Sequence", v, SeqEntry::SeqType::nucleotide);

    // Expected
    std::string s("ecneuqeS");
    static const uint8_t exp_scores[] = {7,6,5,4,3,2,1,0};
    const std::vector<uint8_t> v2(exp_scores, exp_scores + sizeof(exp_scores)
                                  / sizeof(exp_scores[0]));

    test_reverse(s1, s, v2);
  }

  SECTION("Reverse entry with empty sequence") {
    SeqEntry s1 = SeqEntry("Name", "", {}, SeqEntry::SeqType::nucleotide);
    std::string s;
    std::vector<uint8_t> v;

    test_reverse(s1, s, v);
  }
}

TEST_CASE("SeqEntry.Size()", "[sequence]") {
  SECTION("With uninitialized sequence") {
    SeqEntry s1 = SeqEntry();
    REQUIRE(s1.Size() == 0);
  }

  SECTION("With empty sequence") {
    SeqEntry s2 = SeqEntry();
    s2.set_seq("");
    REQUIRE(s2.Size() == 0);
  }

  SECTION("With non-empty sequence") {
    SeqEntry s3 = SeqEntry();
    s3.set_seq("ATCG");
    REQUIRE(s3.Size() == 4);
  }
}
