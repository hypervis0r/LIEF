/* Copyright 2017 - 2021 R. Thomas
 * Copyright 2017 - 2021 Quarkslab
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef LIEF_ELF_PARSER_H_
#define LIEF_ELF_PARSER_H_

#include "LIEF/visibility.h"
#include "LIEF/utils.hpp"

#include "LIEF/Abstract/Parser.hpp"

#include "LIEF/ELF/enums.hpp"

struct Profiler;

namespace LIEF {
class BinaryStream;

namespace OAT {
class Parser;
}
namespace ELF {

class Section;
class Binary;


//! Class which parse an ELF file and transform into a ELF::Binary
class LIEF_API Parser : public LIEF::Parser {
  friend class OAT::Parser;
  public:
  friend struct ::Profiler;

  static constexpr uint32_t NB_MAX_SYMBOLS         = 1000000;
  static constexpr uint32_t DELTA_NB_SYMBOLS       = 3000;
  static constexpr uint32_t NB_MAX_BUCKETS         = NB_MAX_SYMBOLS;
  static constexpr uint32_t NB_MAX_CHAINS          = 1000000;
  static constexpr uint32_t NB_MAX_SECTION         = 10000;
  static constexpr uint32_t NB_MAX_SEGMENTS        = 10000;
  static constexpr uint32_t NB_MAX_RELOCATIONS     = 3000000;
  static constexpr uint32_t NB_MAX_DYNAMIC_ENTRIES = 1000;
  static constexpr uint32_t NB_MAX_MASKWORD        = 512;
  static constexpr uint32_t MAX_NOTE_DESCRIPTION   = 1_MB;
  static constexpr uint32_t MAX_SECTION_SIZE       = 300_MB;
  static constexpr uint32_t MAX_SEGMENT_SIZE       = MAX_SECTION_SIZE;


  //! Parse an ELF file an return a LIEF::ELF::Binary object
  //!
  //! For weird binaries (e.g. sectionless) you can choose which method use to count dynamic symbols
  //!
  //! @param[in] file Path to the ELF binary
  //! @param[in] count_mtd Method used to count dynamic symbols. Default: LIEF::ELF::DYNSYM_COUNT_METHODS::COUNT_AUTO
  //!
  //! @return LIEF::ELF::Binary
  static std::unique_ptr<Binary> parse(const std::string& file, DYNSYM_COUNT_METHODS count_mtd = DYNSYM_COUNT_METHODS::COUNT_AUTO);

  //! Parse the given raw data as an ELF binary and return a LIEF::ELF::Binary object
  //!
  //! For weird binaries (e.g. sectionless) you can choose which method use to count dynamic symbols
  //!
  //! @param[in] data Raw ELF
  //! @param[in] name Binary name (optional)
  //! @param[in] count_mtd Method used to count dynamic symbols. Default: LIEF::ELF::DYNSYM_COUNT_METHODS::COUNT_AUTO
  //!
  //! @return LIEF::ELF::Binary
  static std::unique_ptr<Binary> parse(const std::vector<uint8_t>& data, const std::string& name = "", DYNSYM_COUNT_METHODS count_mtd = DYNSYM_COUNT_METHODS::COUNT_AUTO);

  Parser& operator=(const Parser&) = delete;
  Parser(const Parser&)            = delete;

  private:
  Parser();
  Parser(const std::string& file, DYNSYM_COUNT_METHODS count_mtd = DYNSYM_COUNT_METHODS::COUNT_AUTO, Binary* output = nullptr);
  Parser(const std::vector<uint8_t>& data, const std::string& name, DYNSYM_COUNT_METHODS count_mtd = DYNSYM_COUNT_METHODS::COUNT_AUTO, Binary* output = nullptr);
  ~Parser();

  void init(const std::string& name = "");

  bool should_swap() const;

  // map, dynamic_symbol.version <----> symbol_version
  // symbol_version comes from symbol_version table
  void link_symbol_version();

  template<typename ELF_T>
  void parse_binary();

  template<typename ELF_T>
  bool parse_header();

  //! Parse binary's Section
  //!
  //! Parse sections by using the ``e_shoff`` field as offset
  template<typename ELF_T>
  void parse_sections();

  //! Parse binary's segments
  //!
  //! Parse segment by using the ``e_phoff`` field as offset
  template<typename ELF_T>
  void parse_segments();

  //! Return offset of the dynamic string table
  uint64_t get_dynamic_string_table() const;

  uint64_t get_dynamic_string_table_from_segments() const;

  uint64_t get_dynamic_string_table_from_sections() const;

  //! Return the number of dynamic symbols using the given method
  template<typename ELF_T>
  uint32_t get_numberof_dynamic_symbols(DYNSYM_COUNT_METHODS mtd) const;

  //! Count based on hash table (reliable)
  template<typename ELF_T>
  uint32_t nb_dynsym_hash() const;

  //! Count based on SYSV hash table
  template<typename ELF_T>
  uint32_t nb_dynsym_sysv_hash() const;

  //! Count based on GNU hash table
  template<typename ELF_T>
  uint32_t nb_dynsym_gnu_hash() const;

  //! Count based on sections (not very reliable)
  template<typename ELF_T>
  uint32_t nb_dynsym_section() const;

  //! Count based on PLT/GOT relocations (very reliable but not accurate)
  template<typename ELF_T>
  uint32_t nb_dynsym_relocations() const;

  template<typename ELF_T>
  void parse_dynamic_entries(uint64_t offset, uint64_t size);

  template<typename ELF_T>
  void parse_dynamic_symbols(uint64_t offset);

  //! Parse static Symbol
  //!
  //! Parser find Symbols offset by using the file offset attribute of the
  //! ELF_SECTION_TYPES::SHT_SYMTAB Section.
  //!
  //! The number of symbols is taken from the `information` attribute in the section header.
  //!
  //! The section containing symbols name is found with the `link` attribute.
  template<typename ELF_T>
  void parse_static_symbols(uint64_t offset, uint32_t nbSymbols, const Section* string_section);

  //! Parse Dynamic relocations
  //!
  //! It use DT_REL/DT_RELA dynamic entries to parse it
  template<typename ELF_T, typename REL_T>
  void parse_dynamic_relocations(uint64_t relocations_offset, uint64_t size);

  //! Parse `.plt.got`/`got` relocations
  //!
  //! For:
  //! * ELF32 it uses **DT_JMPREL** and **DT_PLTRELSZ**
  //! * ELF64 it uses **DT_PLTREL** and **DT_PLTRELSZ**
  template<typename ELF_T, typename REL_T>
  void parse_pltgot_relocations(uint64_t offset, uint64_t size);


  //! Parse relocations using LIEF::ELF::Section.
  //!
  //! Parser::parse_dynamic_relocations and Parser::parse_pltgot_relocations
  //! use parse relocations by using LIEF::ELF::Segment. This method parse relocations
  //! that are not reachable through segments (For example Object file).
  template<typename ELF_T, typename REL_T>
  void parse_section_relocations(const Section& section);

  //! Parse SymbolVersionRequirement
  //!
  //! We use the virtual address stored in the
  //! DYNAMIC_TAGS::DT_VERNEED entry to get the offset.
  //! and DYNAMIC_TAGS::DT_VERNEEDNUM to get the number of entries
  template<typename ELF_T>
  void parse_symbol_version_requirement(uint64_t offset, uint32_t nb_entries);


  //! Parse SymbolVersionDefinition.
  //!
  //! We use the virtual address stored in
  //! the DYNAMIC_TAGS::DT_VERDEF DT_VERDEF entry to get the offset.
  //! DYNAMIC_TAGS::DT_VERDEFNUM gives the number of entries
  template<typename ELF_T>
  void parse_symbol_version_definition(uint64_t offset, uint32_t nb_entries);


  //! Parse @link SymbolVersion Symbol version @endlink.
  //!
  //! We use the virtual address stored in the
  //! DYNAMIC_TAGS::DT_VERSYM entry to parse it.
  //!
  //! @see http://dev.gentoo.org/~solar/elf/symbol-versioning
  void parse_symbol_version(uint64_t symbol_version_offset);

  //! Parse Symbols's GNU hash
  //!
  //! @see https://blogs.oracle.com/ali/entry/gnu_hash_elf_sections
  template<typename ELF_T>
  void parse_symbol_gnu_hash(uint64_t offset);

  //! Parse Note (.gnu.note)
  void parse_notes(uint64_t offset, uint64_t size);

  //! Parse Symbols's SYSV hash
  void parse_symbol_sysv_hash(uint64_t offset);

  void parse_overlay();

  template<typename ELF_T, typename REL_T>
  uint32_t max_relocation_index(uint64_t relocations_offset, uint64_t size) const;

  std::unique_ptr<BinaryStream> stream_;
  Binary*                       binary_{nullptr};
  ELF_CLASS                     type_;
  DYNSYM_COUNT_METHODS          count_mtd_;
};




} // namespace ELF
} // namespace LIEF
#endif
