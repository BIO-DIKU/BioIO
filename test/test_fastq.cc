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
#include <BioIO/fastq_reader.h>

#include <memory>
#include <string>

#include <iostream>

static std::string folder = "fasta_test_files/fastq/";

static std::string filename1 = folder + "test1.fastq";

TEST_CASE("Load properly formatted file", "[fastq_reader]") {
  FastqReader reader(filename1);

  auto entry1 = reader.nextEntry();

  REQUIRE(entry1->name() == "fastq1");
  REQUIRE(entry1->scores().size() == entry1->seq().size());
  REQUIRE(entry1->seq() == "GATTACA");

  REQUIRE_FALSE(reader.hasNextEntry());
}

TEST_CASE("Properly raise file not found exception", "[fastq_reader]") {
  try {
    FastqReader reader("SRR121511.fastq");

    FAIL("FastaReader did not throw exception");
  } catch(FastaException e) {
    REQUIRE(e.errorCode  == 1);
  }
}
