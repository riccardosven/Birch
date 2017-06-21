/**
 * @file
 */
#pragma once

#include "bi/expression/Expression.hpp"
#include "bi/expression/EmptyExpression.hpp"
#include "bi/primitive/unique_ptr.hpp"

namespace bi {
/**
 * Statement or expression with a default or initial value.
 *
 * @ingroup compiler_common
 */
class Valued {
public:
  /**
   * Constructor.
   *
   * @param value Value.
   */
  Valued(Expression* value = new EmptyExpression());

  /**
   * Destructor.
   */
  virtual ~Valued() = 0;

  /**
   * Condition.
   */
  unique_ptr<Expression> value;
};
}