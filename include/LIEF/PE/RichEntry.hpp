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
#ifndef LIEF_PE_RICH_ENTRY_H_
#define LIEF_PE_RICH_ENTRY_H_
#include <iostream>

#include "LIEF/Object.hpp"
#include "LIEF/visibility.h"

namespace LIEF {
namespace PE {

class LIEF_API RichEntry : public Object {
  public:

  RichEntry();
  RichEntry(uint16_t id, uint16_t build_id, uint32_t count);
  RichEntry(const RichEntry&);
  RichEntry& operator=(const RichEntry&);
  virtual ~RichEntry();

  //! @brief Entry type
  uint16_t id() const;

  //! @brief Builder number of the tool (if any)
  uint16_t build_id() const;

  //! @brief *Occurrence* count.
  uint32_t count() const;

  void id(uint16_t id);
  void build_id(uint16_t build_id);
  void count(uint32_t count);

  virtual void accept(Visitor& visitor) const override;

  bool operator==(const RichEntry& rhs) const;
  bool operator!=(const RichEntry& rhs) const;

  LIEF_API friend std::ostream& operator<<(std::ostream& os, const RichEntry& rich_entry);

  private:
  uint16_t id_;
  uint16_t build_id_;
  uint32_t count_;

};
}
}

#endif

