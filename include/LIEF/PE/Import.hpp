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
#ifndef LIEF_PE_IMPORT_H_
#define LIEF_PE_IMPORT_H_

#include <string>
#include <iostream>

#include "LIEF/Object.hpp"
#include "LIEF/types.hpp"
#include "LIEF/visibility.h"

#include "LIEF/PE/type_traits.hpp"

namespace LIEF {
namespace PE {
class Parser;
class Builder;
struct pe_import;

class LIEF_API Import : public Object {

  friend class Parser;
  friend class Builder;

  public:
  Import(const pe_import *import);
  Import(const std::string& name);
  Import();
  virtual ~Import();

  Import(const Import& other);
  Import& operator=(Import other);
  void swap(Import& other);

  uint32_t forwarder_chain() const;
  uint32_t timedatestamp() const;


  //! @brief Return a vector of @link PE::ImportEntry Import entries @endlink
  it_import_entries       entries();
  it_const_import_entries entries() const;

  //! @brief Return the relative virtual address of the import address table (`IAT`)
  //
  //! @warning
  //! This address could be change when re-building the binary
  //!
  uint32_t import_address_table_rva() const;

  //! @brief Return the relative virtual address of the import lookup table
  //!
  //! @warning
  //! This address could be change when re-building the binary
  //!
  uint32_t  import_lookup_table_rva() const;

  //! @brief Return the Function's RVA from the import address table (`IAT`)
  //!
  //! @warning
  //! This address could be change when re-building the binary
  //!
  uint32_t get_function_rva_from_iat(const std::string& function) const;

  //! @brief Return the imported function with the given name
  ImportEntry&       get_entry(const std::string& name);
  const ImportEntry& get_entry(const std::string& name) const;

  //! @brief Return the library's name
  //!
  //! e.g. `kernel32.dll`
  const std::string& name() const;
  void               name(const std::string& name);

  //! @brief Return the @link PE::DataDirectory Data directory@endlink associated.
  //! It should be the one at index PE::DATA_DIRECTORY::IMPORT_TABLE
  //!
  DataDirectory&       directory();
  const DataDirectory& directory() const;

  //! @brief Return the @link PE::DataDirectory Data directory@endlink associated.
  //! It should be the one at index PE::DATA_DIRECTORY::IAT
  //!
  DataDirectory&       iat_directory();
  const DataDirectory& iat_directory() const;

  //! @brief Add a function
  ImportEntry& add_entry(const ImportEntry& entry);

  //! @brief Add a function from name
  ImportEntry& add_entry(const std::string& name);

  void import_lookup_table_rva(uint32_t rva);
  void import_address_table_rva(uint32_t rva);

  virtual void accept(Visitor& visitor) const override;

  bool operator==(const Import& rhs) const;
  bool operator!=(const Import& rhs) const;

  LIEF_API friend std::ostream& operator<<(std::ostream& os, const Import& entry);

  private:
  import_entries_t entries_;
  DataDirectory*   directory_{nullptr};
  DataDirectory*   iat_directory_{nullptr};
  uint32_t         import_lookup_table_RVA_;
  uint32_t         timedatestamp_;
  uint32_t         forwarder_chain_;
  uint32_t         name_RVA_;
  uint32_t         import_address_table_RVA_;
  std::string      name_;
  PE_TYPE          type_;
};

}
}

#endif
