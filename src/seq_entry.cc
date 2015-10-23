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

#include <BioIO/seq_entry.h>

#include <ostream>

SeqEntry::SeqEntry(SeqEntry::SeqType sequence_type) :
  type_(sequence_type)
{}

SeqEntry::SeqEntry(const std::string& name,
                   const std::string& sequence,
                   const std::vector<uint8_t>& scores,
                   SeqEntry::SeqType sequence_type) :
  name_(name),
  seq_(sequence),
  scores_(scores),
  type_(sequence_type)
{}

SeqEntry::SeqEntry(const SeqEntry& other) :
  name_(other.name_),
  seq_(other.seq_),
  scores_(other.scores_),
  type_(other.type_)
{}

SeqEntry::SeqEntry(SeqEntry&& other) :
  name_(std::move(other.name_)),
  seq_(std::move(other.seq_)),
  scores_(std::move(other.scores_)),
  type_(std::move(other.type_))
{}

SeqEntry::~SeqEntry() {
}

SeqEntry& SeqEntry::operator=(const SeqEntry& other) {
  if(this != &other) {
    name_ = other.name_;
    seq_ = other.seq_;
    scores_ = other.scores_;
    type_ = other.type_;
  }
  return *this;
}

SeqEntry SeqEntry::SubSeq(size_t i, size_t len) const {
  if (!scores_.empty()) {
    std::vector<uint8_t>::const_iterator first = scores_.begin() + i;
    std::vector<uint8_t>::const_iterator last = first + len;
    std::vector<uint8_t> newScores(first, last);

    return SeqEntry(name_, seq_.substr(i, len), newScores, type_);
  } else {
    return SeqEntry(name_, seq_.substr(i, len), scores_, type_);
  }
}

std::string& SeqEntry::name() {
  return name_;
}

const std::string& SeqEntry::name() const {
  return name_;
}

std::string& SeqEntry::seq() {
  return seq_;
}

const std::string& SeqEntry::seq() const {
  return seq_;
}

std::vector<uint8_t>& SeqEntry::scores() {
  return scores_;
}

const std::vector<uint8_t>& SeqEntry::scores() const {
  return scores_;
}

SeqEntry::SeqType SeqEntry::type() const {
  return type_;
}

void SeqEntry::set_name(const std::string& name) {
  name_ = name;
}

void SeqEntry::set_seq(const std::string& sequence) {
  seq_ = sequence;
}

void SeqEntry::set_scores(const std::vector<uint8_t>& scores) {
  scores_ = scores;
}

void SeqEntry::set_type(SeqType type) {
  type_ = type;
}

std::ostream& operator<< (std::ostream& o, const SeqEntry& sequence) {
  o << '>' << sequence.name_ << std::endl;
  return o << sequence.seq_ << std::endl;
}