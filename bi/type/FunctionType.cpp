/**
 * @file
 */
#include "bi/type/FunctionType.hpp"

#include "bi/visitor/all.hpp"

bi::FunctionType::FunctionType(Type* parens, Type* type,
    shared_ptr<Location> loc, const bool assignable) :
    Type(loc, assignable),
    parens(parens),
    type(type) {
  //
}

bi::FunctionType::~FunctionType() {
  //
}

bi::Type* bi::FunctionType::accept(Cloner* visitor) const {
  return visitor->clone(this);
}

bi::Type* bi::FunctionType::accept(Modifier* visitor) {
  return visitor->modify(this);
}

void bi::FunctionType::accept(Visitor* visitor) const {
  return visitor->visit(this);
}

bool bi::FunctionType::isFunction() const {
  return true;
}

bool bi::FunctionType::dispatchDefinitely(const Type& o) const {
  return o.definitely(*this);
}

bool bi::FunctionType::definitely(const FunctionType& o) const {
  return parens->definitely(*o.parens) && type->definitely(*o.type);
}

bool bi::FunctionType::definitely(const ParenthesesType& o) const {
  return definitely(*o.single);
}

bool bi::FunctionType::dispatchPossibly(const Type& o) const {
  return o.possibly(*this);
}

bool bi::FunctionType::possibly(const FunctionType& o) const {
  return parens->possibly(*o.parens) && type->possibly(*o.type);
}

bool bi::FunctionType::possibly(const ParenthesesType& o) const {
  return possibly(*o.single);
}