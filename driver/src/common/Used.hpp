/**
 * @file
 */
#pragma once

namespace birch {
/**
 * Keeps track of the number of times a variable or function is used within
 * its scope.
 * 
 *
 * @ingroup common
 */
class Used {
public:
  /**
   * Constructor.
   */
  Used();

  /**
   * Destructor.
   */
  virtual ~Used() = 0;

  /**
   * Number of times used.
   */
  int useCount;
};
}
