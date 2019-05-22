/**
 * @file
 */
#if ENABLE_LAZY_DEEP_CLONE
#include "libbirch/LazyMemo.hpp"

#include "libbirch/SwapClone.hpp"
#include "libbirch/SwapContext.hpp"

libbirch::LazyMemo::LazyMemo() {
  //
}

libbirch::LazyMemo::LazyMemo(LazyMemo* parent) {
  assert(parent);
  m.copy(parent->m);
}

libbirch::LazyMemo::~LazyMemo() {
  //
}

libbirch::LazyAny* libbirch::LazyMemo::get(LazyAny* o) {
  assert(o->isFrozen());
  LazyAny* prev = nullptr;
  LazyAny* next = o;
  l.keep();
  do {
    prev = next;
    next = m.get(prev, prev);
  } while (next != prev && this != next->getContext());
  if (this != next->getContext()) {
    next = copy(next);
  } else if (next->isFrozen()) {
    next = next->getForward();
  }
  l.unkeep();
  return next;
}

libbirch::LazyAny* libbirch::LazyMemo::pull(LazyAny* o) {
  assert(o->isFrozen());
  LazyAny* prev = nullptr;
  LazyAny* next = o;
  l.share();
  do {
    prev = next;
    next = m.get(prev, prev);
  } while (next != prev && this != next->getContext());
  if (this == next->getContext() && next->isFrozen()) {
    next = next->pullForward();
  }
  l.unshare();
  return next;
}

libbirch::LazyAny* libbirch::LazyMemo::finish(LazyAny* o) {
  assert(o->isFrozen());
  LazyAny* prev = nullptr;
  LazyAny* next = o;
  //l.keep();
  while (next != prev && this != next->getContext()) {
    prev = next;
    next = m.get(prev, prev);
  }
  if (this != next->getContext()) {
    next = copy(next);
  }
  //l.unkeep();
  return next;
}

libbirch::LazyAny* libbirch::LazyMemo::copy(LazyAny* o) {
  /* for a lazy deep clone there is no risk of infinite recursion, but
   * there may be thread contention if two threads access the same object
   * and both trigger a lazy clone simultaneously; in this case multiple
   * new objects may be made but only one thread can be successful in
   * inserting an object into the map; a shared pointer is used to
   * destroy any additional objects */
  assert(o->isFrozen());
  SwapClone swapClone(true);
  SwapContext swapContext(this);
  if (o->isUniquelyReachable()) {
    /* don't need to record this in the memo, as it will not be encountered
     * again */
    return o->clone_();
  } else {
    /* clone and record in the memo, using a shared pointer to ensure that
     * the clone is collected if another thread beats this one */
    SharedPtr<LazyAny> cloned = o->clone_();
    return m.put(o, cloned.get());
  }
}

#endif
