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
#ifndef LIEF_PE_IMPORT_ENTRY_H_
#define LIEF_PE_IMPORT_ENTRY_H_
#include <string>
#include <iostream>

#include "LIEF/Object.hpp"
#include "LIEF/visibility.h"
#include "LIEF/Abstract/Symbol.hpp"

#include "LIEF/PE/enums.hpp"

namespace LIEF {
namespace PE {
class Parser;
class Builder;

class LIEF_API ImportEntry : public LIEF::Symbol {
  friend class Parser;
  friend class Builder;

  public:
  ImportEntry();
  ImportEntry(uint64_t data, const std::string& name = "");
  ImportEntry(const std::string& name);
  ImportEntry(const ImportEntry&);
  ImportEntry& operator=(const ImportEntry&);
  virtual ~ImportEntry();

  //! @brief ``True`` if ordinal is used
  bool is_ordinal() const;

  //! @brief ordinal value
  uint16_t ordinal() const;

  //! @see ImportEntry::data
  uint64_t hint_name_rva() const;

  //! @brief Index into the Export::entries
  uint16_t hint() const;

  //! @brief Value of the current entry in the Import Address Table.
  //! It should match the lookup table value
  uint64_t iat_value() const;

  //! @brief Import name if not ordinal
  //const std::string& name() const;

  //! @brief Raw value
  uint64_t data() const;

  //! @brief **Original** address of the entry in the Import Address Table
  uint64_t iat_address() const;


  //void name(const std::string& name);
  void data(uint64_t data);

  virtual void accept(Visitor& visitor) const override;

  bool operator==(const ImportEntry& rhs) const;
  bool operator!=(const ImportEntry& rhs) const;

  LIEF_API friend std::ostream& operator<<(std::ostream& os, const ImportEntry& entry);

  private:
  uint64_t    data_ = 0;
  uint16_t    hint_ = 0;
  uint64_t    iat_value_ = 0;
  uint64_t    rva_ = 0;
  PE_TYPE     type_ = PE_TYPE::PE32_PLUS;
};

}
}

#endif /* IMPORTENTRY_H_ */
