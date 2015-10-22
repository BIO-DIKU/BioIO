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

#include <iostream>

TEST_CASE("read 9 fasta files", "[fasta_reader]") {
  try {
    FastaReader reader("../test/fasta_files/test21.fasta");

    REQUIRE(reader.hasNextEntry() == true);
  }
  catch (FastaException e) {
    REQUIRE(e.errorCode == 1);
  }
}