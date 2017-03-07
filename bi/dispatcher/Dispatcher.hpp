/**
 * @file
 */
#pragma once

#include "bi/common/Named.hpp"
#include "bi/common/Numbered.hpp"
#include "bi/common/Mangled.hpp"
#include "bi/common/Parenthesised.hpp"
#include "bi/common/Scoped.hpp"
#include "bi/expression/FuncParameter.hpp"
#include "bi/primitive/poset.hpp"
#include "bi/primitive/possibly.hpp"

namespace bi {
/**
 * Dispatcher for runtime resolution of a function call.
 *
 * @ingroup compiler_expression
 */
class Dispatcher : public Named,
    public Numbered,
    public Mangled,
    public Parenthesised,
    public Scoped {
public:
  /**
   * Constructor.
   *
   * @param name Name.
   * @param mangled Mangled name associated with the pattern of this
   * dispatcher.
   * @param parent Parent dispatcher.
   */
  Dispatcher(shared_ptr<Name> name, shared_ptr<Name> mangled,
      Dispatcher* parent = nullptr);

  /**
   * Destructor.
   */
  virtual ~Dispatcher();

  /**
   * Insert a function into the dispatcher. The function pattern must match
   * that of the dispatcher.
   */
  void insert(FuncParameter* func);

  virtual Dispatcher* accept(Cloner* visitor) const;
  virtual Dispatcher* accept(Modifier* visitor);
  virtual void accept(Visitor* visitor) const;

  /*
   * Comparison operators.
   */
  bool operator==(const Dispatcher& o) const;

  /**
   * Functions handled by this dispatcher.
   */
  poset<FuncParameter*,bi::possibly> funcs;

  /**
   * Parent dispatcher.
   */
  Dispatcher* parent;

  /**
   * Parameter types.
   */
  std::list<Type*> paramTypes;

  /**
   * Result type.
   */
  unique_ptr<Type> type;

private:
  /**
   * Update the variant type of @p o2 with the type of @p o1.
   */
  void update(Expression* o1, Expression* o2);
};
}