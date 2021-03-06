/**
 * @file
 */
#pragma once

#include "src/common/Location.hpp"

namespace birch {
/**
 * Object with a location within a file being parsed.
 *
 * @ingroup common
 */
class Located {
public:
  /**
   * Constructor.
   *
   * @param loc Location.
   */
  Located(Location* loc = nullptr);

  /**
   * Destructor.
   */
  virtual ~Located() = 0;

  /**
   * Location.
   */
  Location* loc;
};
}
