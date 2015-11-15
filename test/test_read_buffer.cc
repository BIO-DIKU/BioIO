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
#include <fstream>
#include "catch.hpp"
#include "BioIO/read_buffer.h"

using namespace std;

TEST_CASE("ReadBuffer", "[read_buffer]") {
  string file = "file";

  ofstream output;
  output.open(file);
  output << "fox"  << endl;
  output << "barz" << endl;
  output.close();

  string expected = "fox\nbarz\n";

  SECTION("NextChar with small buffer") {
    string result   = "";
    ReadBuffer rb(2, file);

    char c;

    while ((c = rb.NextChar())) {
      result += c;
    }

    REQUIRE(expected == result);
  }

  SECTION("NextChar with large buffer") {
    string result   = "";
    ReadBuffer rb(20, file);

    char c;

    while ((c = rb.NextChar())) {
      result += c;
    }

    REQUIRE(expected == result);
  }

  SECTION("PrevChar returns OK") {
    char c1, c2;

    ReadBuffer rb(20, file);

    REQUIRE_FALSE(rb.PrevChar());
    c1 = rb.NextChar();
    REQUIRE_FALSE(rb.PrevChar());
    c2 = rb.NextChar();
    REQUIRE(rb.PrevChar() == c1);
  }

  remove(file.c_str());
}
