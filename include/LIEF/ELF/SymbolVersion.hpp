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
#ifndef LIEF_ELF_SYMBOL_VERSION_H_
#define LIEF_ELF_SYMBOL_VERSION_H_
#include <iostream>

#include "LIEF/Object.hpp"
#include "LIEF/visibility.h"

namespace LIEF {
namespace ELF {
class Parser;
class SymbolVersionAux;

//! @brief Class which modelization of an entry defined in ``DT_VERSYM``
class LIEF_API SymbolVersion : public Object {
  friend class Parser;

  public:
  SymbolVersion(uint16_t value);
  SymbolVersion();

  //! Generate a *local* SymbolVersion
  static SymbolVersion local();

  //! Generate a *global* SymbolVersion
  static SymbolVersion global();

  virtual ~SymbolVersion();

  SymbolVersion& operator=(const SymbolVersion&);
  SymbolVersion(const SymbolVersion&);

  //! Value associated with the symbol
  //!
  //! If the given SymbolVersion hasn't Auxiliary version:
  //!
  //! * ``0`` means **Local**
  //! * ``1`` means **Global**
  uint16_t value() const;

  //! Whether or not the current SymbolVersion has an auxiliary one
  bool has_auxiliary_version() const;

  //! SymbolVersionAux associated with the current Version (if any)
  SymbolVersionAux&       symbol_version_auxiliary();
  const SymbolVersionAux& symbol_version_auxiliary() const;

  void value(uint16_t v);

  virtual void accept(Visitor& visitor) const override;

  bool operator==(const SymbolVersion& rhs) const;
  bool operator!=(const SymbolVersion& rhs) const;

  LIEF_API friend std::ostream& operator<<(std::ostream& os, const SymbolVersion& symv);

  private:
  uint16_t          value_;
  SymbolVersionAux* symbol_aux_{nullptr};
};
}
}
#endif
