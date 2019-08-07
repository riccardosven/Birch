/**
 * @file
 */
#include "bi/type/Type.hpp"

#include "bi/type/TypeIterator.hpp"
#include "bi/type/TypeConstIterator.hpp"
#include "bi/exception/all.hpp"
#include "bi/visitor/all.hpp"

bi::Type::Type(Location* loc) :
    Located(loc) {
  //
}

bi::Type::~Type() {
  //
}

bool bi::Type::isEmpty() const {
  return false;
}

bool bi::Type::isValue() const {
  return false;
}

bool bi::Type::isBasic() const {
  return false;
}

bool bi::Type::isClass() const {
  return false;
}

bool bi::Type::isArray() const {
  return false;
}

bool bi::Type::isSequence() const {
  return false;
}

bool bi::Type::isList() const {
  return false;
}

bool bi::Type::isFunction() const {
  return false;
}

bool bi::Type::isFiber() const {
  return false;
}

bool bi::Type::isBinary() const {
  return false;
}

bool bi::Type::isMember() const {
  return false;
}

bool bi::Type::isOptional() const {
  return false;
}

bool bi::Type::isWeak() const {
  return false;
}

bool bi::Type::isGeneric() const {
  return false;
}

bool bi::Type::isBound() const {
  Gatherer<GenericType> generics;
  this->accept(&generics);
  for (auto o : generics) {
    if (o->target->type->isEmpty()) {
      return false;
    }
  }
  return true;
}

bi::Type* bi::Type::getLeft() const {
  assert(false);
  return nullptr;
}

bi::Type* bi::Type::getRight() const {
  assert(false);
  return nullptr;
}

bi::Class* bi::Type::getClass() const {
  assert(false);
  return nullptr;
}

bi::Basic* bi::Type::getBasic() const {
  assert(false);
  return nullptr;
}

int bi::Type::width() const {
  int result = 0;
  for (auto iter = begin(); iter != end(); ++iter) {
    ++result;
  }
  return result;
}

int bi::Type::depth() const {
  return 0;
}

bi::Type* bi::Type::unwrap() {
  return this;
}

const bi::Type* bi::Type::unwrap() const {
  return this;
}

bi::Type* bi::Type::canonical() {
  return this;
}

const bi::Type* bi::Type::canonical() const {
  return this;
}

bi::Type* bi::Type::element() {
  return this;
}

const bi::Type* bi::Type::element() const {
  return this;
}

void bi::Type::resolveConstructor(Argumented* o) {
  if (!o->args->isEmpty()) {
    throw ConstructorException(o);
  }
}

bi::TypeIterator bi::Type::begin() {
  if (isEmpty()) {
    return end();
  } else {
    return TypeIterator(this);
  }
}

bi::TypeConstIterator bi::Type::begin() const {
  if (isEmpty()) {
    return end();
  } else {
    return TypeConstIterator(this);
  }
}

bi::TypeIterator bi::Type::end() {
  return TypeIterator(nullptr);
}

bi::TypeConstIterator bi::Type::end() const {
  return TypeConstIterator(nullptr);
}

bool bi::Type::equals(const Type& o) const {
  return isConvertible(o) && o.isConvertible(*this);
}

bool bi::Type::isConvertible(const Type& o) const {
  return o.dispatchIsConvertible(*this);
}

bool bi::Type::isConvertible(const ArrayType& o) const {
  return false;
}

bool bi::Type::isConvertible(const BasicType& o) const {
  return false;
}

bool bi::Type::isConvertible(const BinaryType& o) const {
  return false;
}

bool bi::Type::isConvertible(const ClassType& o) const {
  return false;
}

bool bi::Type::isConvertible(const EmptyType& o) const {
  return false;
}

bool bi::Type::isConvertible(const FiberType& o) const {
  return false;
}

bool bi::Type::isConvertible(const FunctionType& o) const {
  return false;
}

bool bi::Type::isConvertible(const GenericType& o) const {
  return false;
}

bool bi::Type::isConvertible(const MemberType& o) const {
  return false;
}

bool bi::Type::isConvertible(const NilType& o) const {
  return false;
}

bool bi::Type::isConvertible(const OptionalType& o) const {
  return false;
}

bool bi::Type::isConvertible(const WeakType& o) const {
  return false;
}

bool bi::Type::isConvertible(const TupleType& o) const {
  return false;
}

bool bi::Type::isConvertible(const UnknownType& o) const {
  return false;
}

bool bi::Type::isConvertible(const TypeList& o) const {
  return false;
}

bool bi::Type::isAssignable(const Type& o) const {
  return o.dispatchIsAssignable(*this);
}

bool bi::Type::isAssignable(const ArrayType& o) const {
  return false;
}

bool bi::Type::isAssignable(const BasicType& o) const {
  return false;
}

bool bi::Type::isAssignable(const BinaryType& o) const {
  return false;
}

bool bi::Type::isAssignable(const ClassType& o) const {
  return false;
}

bool bi::Type::isAssignable(const EmptyType& o) const {
  return false;
}

bool bi::Type::isAssignable(const FiberType& o) const {
  return false;
}

bool bi::Type::isAssignable(const FunctionType& o) const {
  return false;
}

bool bi::Type::isAssignable(const GenericType& o) const {
  return false;
}

bool bi::Type::isAssignable(const MemberType& o) const {
  return false;
}

bool bi::Type::isAssignable(const NilType& o) const {
  return false;
}

bool bi::Type::isAssignable(const OptionalType& o) const {
  return false;
}

bool bi::Type::isAssignable(const WeakType& o) const {
  return false;
}

bool bi::Type::isAssignable(const TupleType& o) const {
  return false;
}

bool bi::Type::isAssignable(const UnknownType& o) const {
  return false;
}

bool bi::Type::isAssignable(const TypeList& o) const {
  return false;
}

bi::Type* bi::Type::common(const Type& o) const {
  return o.dispatchCommon(*this);
}

bi::Type* bi::Type::common(const ArrayType& o) const {
  return nullptr;
}

bi::Type* bi::Type::common(const BasicType& o) const {
  return nullptr;
}

bi::Type* bi::Type::common(const BinaryType& o) const {
  return nullptr;
}

bi::Type* bi::Type::common(const ClassType& o) const {
  return nullptr;
}

bi::Type* bi::Type::common(const EmptyType& o) const {
  return nullptr;
}

bi::Type* bi::Type::common(const FiberType& o) const {
  return nullptr;
}

bi::Type* bi::Type::common(const FunctionType& o) const {
  return nullptr;
}

bi::Type* bi::Type::common(const GenericType& o) const {
  return nullptr;
}

bi::Type* bi::Type::common(const MemberType& o) const {
  return nullptr;
}

bi::Type* bi::Type::common(const NilType& o) const {
  return nullptr;
}

bi::Type* bi::Type::common(const OptionalType& o) const {
  return nullptr;
}

bi::Type* bi::Type::common(const WeakType& o) const {
  return nullptr;
}

bi::Type* bi::Type::common(const TupleType& o) const {
  return nullptr;
}

bi::Type* bi::Type::common(const UnknownType& o) const {
  return nullptr;
}

bi::Type* bi::Type::common(const TypeList& o) const {
  return nullptr;
}
