/**
 * @file
 */
#pragma once

#include "libbirch/external.hpp"
#include "libbirch/internal.hpp"
#include "libbirch/Memo.hpp"

namespace libbirch {
/**
 * @internal
 * 
 * Copy a graph of unknown size.
 *
 * @ingroup libbirch
 */
class Copier {
public:
  void visit() {
    //
  }

  template<class Arg>
  void visit(Arg& arg) {
    //
  }

  template<class Arg, class... Args>
  void visit(Arg& arg, Args&... args) {
    visit(arg);
    visit(args...);
  }

  template<class... Args>
  void visit(std::tuple<Args...>& o) {
    std::apply([&](Args&... args) { return visit(args...); }, o);
  }

  template<class T>
  void visit(std::optional<T>& o) {
    if (o.has_value()) {
      visit(o.value());
    }
  }

  template<class T, class F>
  void visit(Array<T,F>& o);

  template<class T>
  void visit(Shared<T>& o);

  Any* visit(Any* o);

private:
  /**
   * Memo.
   */
  Memo m;
};
}

#include "libbirch/Array.hpp"
#include "libbirch/Shared.hpp"
#include "libbirch/Any.hpp"

template<class T, class F>
void libbirch::Copier::visit(Array<T,F>& o) {
  if (!is_value<T>::value) {
    auto iter = o.begin();
    auto last = o.end();
    for (; iter != last; ++iter) {
      visit(*iter);
    }
  }
}

template<class T>
void libbirch::Copier::visit(Shared<T>& o) {
  if (!o.b) {
    Any* w = o.load();
    Any* u = visit(w);
    T* v = static_cast<T*>(u);
    v->incShared_();
    o.store(v);
  }
}
