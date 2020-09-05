/**
 * @file
 */
#pragma once

#include "src/expression/all.hpp"
#include "src/statement/all.hpp"
#include "src/type/all.hpp"

namespace birch {
/**
 * Visitor.
 *
 * @ingroup visitor
 */
class Visitor {
public:
  /**
   * Destructor.
   */
  virtual ~Visitor();

  virtual void visit(const Package* o);
  virtual void visit(const File* o);
  virtual void visit(const Name* o);

  virtual void visit(const EmptyExpression* o);
  virtual void visit(const ExpressionList* o);
  virtual void visit(const Literal<bool>* o);
  virtual void visit(const Literal<int64_t>* o);
  virtual void visit(const Literal<double>* o);
  virtual void visit(const Literal<const char*>* o);
  virtual void visit(const Parentheses* o);
  virtual void visit(const Sequence* o);
  virtual void visit(const Cast* o);
  virtual void visit(const Call* o);
  virtual void visit(const BinaryCall* o);
  virtual void visit(const UnaryCall* o);
  virtual void visit(const Assign* o);
  virtual void visit(const Slice* o);
  virtual void visit(const Query* o);
  virtual void visit(const Get* o);
  virtual void visit(const GetReturn* o);
  virtual void visit(const Spin* o);
  virtual void visit(const LambdaFunction* o);
  virtual void visit(const Index* o);
  virtual void visit(const Span* o);
  virtual void visit(const Range* o);
  virtual void visit(const Member* o);
  virtual void visit(const Global* o);
  virtual void visit(const Super* o);
  virtual void visit(const This* o);
  virtual void visit(const Nil* o);
  virtual void visit(const Parameter* o);
  virtual void visit(const NamedExpression* o);

  virtual void visit(const EmptyStatement* o);
  virtual void visit(const Braces* o);
  virtual void visit(const StatementList* o);
  virtual void visit(const Assume* o);
  virtual void visit(const GlobalVariable* o);
  virtual void visit(const MemberVariable* o);
  virtual void visit(const LocalVariable* o);
  virtual void visit(const Function* o);
  virtual void visit(const Fiber* o);
  virtual void visit(const Program* o);
  virtual void visit(const MemberFunction* o);
  virtual void visit(const MemberFiber* o);
  virtual void visit(const BinaryOperator* o);
  virtual void visit(const UnaryOperator* o);
  virtual void visit(const AssignmentOperator* o);
  virtual void visit(const ConversionOperator* o);
  virtual void visit(const Class* o);
  virtual void visit(const Basic* o);
  virtual void visit(const Generic* o);
  virtual void visit(const ExpressionStatement* o);
  virtual void visit(const If* o);
  virtual void visit(const For* o);
  virtual void visit(const Parallel* o);
  virtual void visit(const While* o);
  virtual void visit(const DoWhile* o);
  virtual void visit(const Block* o);
  virtual void visit(const Assert* o);
  virtual void visit(const Return* o);
  virtual void visit(const Yield* o);
  virtual void visit(const Raw* o);

  virtual void visit(const EmptyType* o);
  virtual void visit(const TypeList* o);
  virtual void visit(const NamedType* o);
  virtual void visit(const MemberType* o);
  virtual void visit(const ArrayType* o);
  virtual void visit(const TupleType* o);
  virtual void visit(const FunctionType* o);
  virtual void visit(const FiberType* o);
  virtual void visit(const OptionalType* o);
};
}