/**
 * @file
 */
#pragma once

#include "bi/expression/Expression.hpp"

namespace bi {
/**
 * Object with generic type parameters.
 *
 * @ingroup common
 */
template<class Target>
class TypeParameterised {
public:
  /**
   * Constructor.
   *
   * @param typeParams Generic type parameters.
   */
  TypeParameterised(Expression* typeParams);

  /**
   * Destructor.
   */
  virtual ~TypeParameterised() = 0;

  /**
   * Does this class have generic type parameters?
   */
  bool isGeneric() const;

  /**
   * Have type arguments been bound to all type parameters?
   */
  bool isBound() const;

  /**
   * Is this an instantiation of a generic class? Equivalent to
   * `isGeneric() && isBound()`.
   */
  bool isInstantiation() const;

  /**
   * Bind type arguments to the type parameters.
   */
  void bind(Type* typeArgs);

  /**
   * Add an instantiation.
   */
  void addInstantiation(Target* o);

  /**
   * Get the instantiation, if any, that exactly matches the given generic
   * type arguments. Returns `nullptr` if not such instantiation exists.
   */
  Target* getInstantiation(const Type* typeArgs);

  /**
   * Generic type parameters.
   */
  Expression* typeParams;

  /**
   * Instantiations of this object.
   */
  std::list<Target*> instantiations;

  /**
   * Is this an explicit instantiation of a generic object?
   */
  bool isExplicit;
};
}
