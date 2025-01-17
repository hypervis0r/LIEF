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

#include <iomanip>
#include <sstream>

#include "logging.hpp"

#include "LIEF/ELF/hash.hpp"
#include "LIEF/ELF/EnumToString.hpp"
#include "LIEF/ELF/Note.hpp"
#include "LIEF/ELF/Binary.hpp"

#include "CoreAuxv.tcc"

namespace LIEF {
namespace ELF {

CoreAuxv::CoreAuxv(Note& note):
  NoteDetails::NoteDetails{note}
{}

CoreAuxv CoreAuxv::make(Note& note) {
  CoreAuxv pinfo(note);
  pinfo.parse();
  return pinfo;
}

CoreAuxv* CoreAuxv::clone() const {
  return new CoreAuxv(*this);
}


const CoreAuxv::val_context_t& CoreAuxv::values() const {
  return this->ctx_;
}


uint64_t CoreAuxv::get(LIEF::ELF::AUX_TYPE atype, bool* error) const {
  if (not this->has(atype)) {
    if (error != nullptr) {
      *error = true;
    }
    return 0;
  }

  if (error != nullptr) {
    *error = false;
  }
  return this->ctx_.at(atype);
}

bool CoreAuxv::has(LIEF::ELF::AUX_TYPE atype) const {
  return this->ctx_.find(atype) != std::end(this->ctx_);
}


void CoreAuxv::values(const val_context_t& ctx) {
  this->ctx_ = ctx;
  this->build();
}

bool CoreAuxv::set(LIEF::ELF::AUX_TYPE atype, uint64_t value) {
  this->ctx_[atype] = value;
  this->build();
  return true;
}

void CoreAuxv::accept(Visitor& visitor) const {
  visitor.visit(*this);
}

bool CoreAuxv::operator==(const CoreAuxv& rhs) const {
  size_t hash_lhs = Hash::hash(*this);
  size_t hash_rhs = Hash::hash(rhs);
  return hash_lhs == hash_rhs;
}

bool CoreAuxv::operator!=(const CoreAuxv& rhs) const {
  return not (*this == rhs);
}

uint64_t& CoreAuxv::operator[](LIEF::ELF::AUX_TYPE atype) {
  return this->ctx_[atype];
}

void CoreAuxv::dump(std::ostream& os) const {
  static constexpr size_t WIDTH = 16;
  os << std::left;

  os << std::setw(WIDTH) << std::setfill(' ') << "Auxiliary values: "<< std::dec << std::endl;
  for (auto&& val : this->ctx_) {
    os << std::setw(14) << std::setfill(' ') << to_string(val.first) << ": " << std::hex << std::showbase << val.second << std::endl;
  }
  os << std::endl;
}


void CoreAuxv::parse() {
  if (this->binary()->type() == ELF_CLASS::ELFCLASS64) {
    this->parse_<ELF64>();
  } else if (this->binary()->type() == ELF_CLASS::ELFCLASS32) {
    this->parse_<ELF32>();
  }
}

void CoreAuxv::build() {
  if (this->binary()->type() == ELF_CLASS::ELFCLASS64) {
    this->build_<ELF64>();
  } else if (this->binary()->type() == ELF_CLASS::ELFCLASS32) {
    this->build_<ELF32>();
  }
}


std::ostream& operator<<(std::ostream& os, const CoreAuxv& note) {
  note.dump(os);
  return os;
}


CoreAuxv::~CoreAuxv() = default;

} // namespace ELF
} // namespace LIEF
