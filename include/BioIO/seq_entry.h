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

#ifndef BIOIO_SEQ_ENTRY_H_
#define BIOIO_SEQ_ENTRY_H_

#include <string>
#include <vector>
#include <ostream>

/**
 * // TODO
 */
class SeqEntry {
  public:
    enum class SeqType {
      nucleotide,
      protein
    };

    /**
     * Default constructor.
     * Sequence Type defaults to nucleotide.
     */
    SeqEntry(SeqEntry::SeqType sequence_type = SeqEntry::SeqType::nucleotide);

    /**
     * Constructor.
     * @param Name of sequence (note this is not the comment)
     * @param Sequence
     * @param Scores (implemented as vector, which is subject to change)
     * @param Sequence Type (see enum class SeqType)
     */
    SeqEntry(const std::string& name,
             const std::string& sequence,
             const std::vector<uint8_t>& scores,
             SeqEntry::SeqType sequence_type);

    /**
     * Copy constructor.
     */
    SeqEntry(const SeqEntry& other);

    /**
     * Move constructor.
     */
    SeqEntry(SeqEntry&& other);

    virtual ~SeqEntry();

    /**
     * Copy operator.
     */
    SeqEntry& operator=(const SeqEntry& other);

    /*
     * Returns a SeqEntry containing the (consecutive) subsequence of the
     * SeqEntry object it is called on, starting at the given index and of
     * the given length.
     */
    SeqEntry SubSeq(size_t i, size_t len) const;

    /**
     * Reverse sequence and scores.
     */
    void reverse();

    /**
     * @return Reference to name
     */
    std::string& name();

    /**
     * @return Reference to const name
     */
    const std::string& name() const;

    /**
     * @return Reference to sequence
     */
    std::string& seq();

    /**
     * @return Reference to const sequence
     */
    const std::string& seq() const;

    /**
     * @return Reference to vector of scores
     */
    std::vector<uint8_t>& scores();

    /**
     * @return Reference to const vector of scores
     */
    const std::vector<uint8_t>& scores() const;

    /**
     * @return Reference to sequence type
     */
    SeqEntry::SeqType type() const;

    /**
     * @param Sequence name
     */
    void set_name(const std::string& name);

    /**
     * @param Sequence
     */
    void set_seq(const std::string& sequence);

    /**
     * @param Sequence scores
     */
    void set_scores(const std::vector<uint8_t>& scores);

    /**
     * @param Sequence type
     */
    void set_type(SeqType type);

    /**
     * // TODO
     */
     friend std::ostream& operator<< (std::ostream& o, const SeqEntry& sequence);

  private:
    std::string name_;
    std::string seq_;
    std::vector<uint8_t> scores_;
    SeqType type_;
};

#endif // BIOIO_SEQ_ENTRY_H_
