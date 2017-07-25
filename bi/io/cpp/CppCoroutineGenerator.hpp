/**
 * @file
 */
#pragma once

#include "bi/io/cpp/CppBaseGenerator.hpp"
#include "bi/visitor/Gatherer.hpp"

namespace bi {
/**
 * C++ code generator for coroutines.
 *
 * @ingroup compiler_io
 */
class CppCoroutineGenerator: public CppBaseGenerator {
public:
  CppCoroutineGenerator(std::ostream& base,
      const int level = 0, const bool header = false);

  using indentable_ostream::visit;

  virtual void visit(const Coroutine* o);
  virtual void visit(const Return* o);
  virtual void visit(const Yield* o);
  virtual void visit(const Identifier<LocalVariable>* o);
  virtual void visit(const LocalVariable* o);

protected:
  void genSwitch();
  void genEnd();

  /*
   * Gatherers for important objects.
   */
  Gatherer<Yield> yields;
  Gatherer<Parameter> parameters;
  Gatherer<LocalVariable> locals;

  /**
   * Current state index.
   */
  int state;
};
}
