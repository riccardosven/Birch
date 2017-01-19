/**
 * @file
 */
#pragma once

#include "bi/primitive/possibly.hpp"

#include <cstdlib>
#include <set>
#include <map>
#include <list>

namespace bi {
/**
 * Partially ordered set.
 *
 * @tparam T Value type.
 * @tparam Compare Comparison functor.
 */
template<class T, class Compare>
class poset {
public:
  /**
   * Constructor.
   */
  poset();

  /**
   * Does the set contain an equivalent value?
   */
  bool contains(T v);

  /**
   * Get the equivalent value.
   */
  T get(T v);

  /**
   * Find the most-specific definite match(es) as well as more-specific
   * possible matche(es).
   *
   * @tparam Comparable Type comparable to value type.
   * @tparam Container Container type with push_back() function.
   *
   * @param v The value.
   * @param[out] definites Container to hold most-specific definite
   * match(es).
   * @param[out] possibles Container to hold more-specific possible
   * match(es).
   */
  template<class Comparable, class Container>
  void match(Comparable v, Container& definites, Container& possibles);

  /**
   * Insert vertex.
   *
   * @param v Value at the vertex.
   */
  void insert(T v);

  /**
   * Output dot graph. Useful for diagnostic purposes.
   */
  void dot();

private:
  /**
   * Add vertex.
   *
   * @param v Value at the vertex.
   */
  void add_vertex(T v);

  /**
   * Add edge.
   *
   * @param u Source vertex index.
   * @param v Destination vertex index.
   */
  void add_edge(T u, T v);

  /**
   * Remove edge.
   *
   * @param u Source vertex index.
   * @param v Destination vertex index.
   */
  void remove_edge(T u, T v);

  /**
   * Sub-operation for match to find most-specific definite match.
   *
   * @return True if a definite match was made in the subgraph.
   */
  template<class Comparable, class Container>
  bool match_definites(T u, Comparable v, Container& definites);

  /**
   * Sub-operation for match to find most-specific definite match.
   */
  template<class Comparable, class Container>
  bool match_possibles(T u, Comparable v, Container& possibles);

  /*
   * Sub-operations for insert.
   */
  void forward(T v);
  void forward(T u, T v);
  void backward(T v);
  void backward(T u, T v);
  void reduce();  // transitive reduction
  void reduce(T u);

  /*
   * Sub-operations for dot().
   */
  void dot(T u);

  /**
   * Vertex colours.
   */
  std::map<T,int> colours;

  /**
   * Forward and backward edges.
   */
  std::multimap<T,T> forwards, backwards;

  /**
   * Roots and leaves.
   */
  std::set<T> roots, leaves;

  /**
   * Comparison operator.
   */
  Compare compare;

  /**
   * Current colour.
   */
  int colour;
};
}

#include <iostream>
#include <cassert>

template<class T, class Compare>
bi::poset<T,Compare>::poset() :
    colour(0) {
  //
}

template<class T, class Compare>
bool bi::poset<T,Compare>::contains(T v) {
  std::list<T> definites, possibles;
  match(v, definites, possibles);
  return definites.size() == 1 && definites.front() <= v
      && v <= definites.front();
}

template<class T, class Compare>
T bi::poset<T,Compare>::get(T v) {
  /* pre-condition */
  assert(contains(v));

  std::list<T> definites, possibles;
  match(v, definites, possibles);
  return definites.front();
}

template<class T, class Compare>
template<class Comparable, class Container>
void bi::poset<T,Compare>::match(Comparable v, Container& definites,
    Container& possibles) {
  ++colour;
  for (auto iter = roots.begin(); iter != roots.end(); ++iter) {
    match_definites(*iter, v, definites);
  }

  ++colour;
  for (auto iter = roots.begin(); iter != roots.end(); ++iter) {
    match_possibles(*iter, v, possibles);
  }
}

template<class T, class Compare>
void bi::poset<T,Compare>::insert(T v) {
  add_vertex(v);
  forward(v);
  backward(v);
  reduce();
}

template<class T, class Compare>
void bi::poset<T,Compare>::add_vertex(T v) {
  /* pre-condition */
  assert(!contains(v));

  colours.insert(std::make_pair(v, colour));
  roots.insert(v);
  leaves.insert(v);
}

template<class T, class Compare>
void bi::poset<T,Compare>::add_edge(T u, T v) {
  /* pre-condition */
  assert(u != v);

  forwards.insert(std::make_pair(u, v));
  backwards.insert(std::make_pair(v, u));
  leaves.erase(u);
  roots.erase(v);
}

template<class T, class Compare>
void bi::poset<T,Compare>::remove_edge(T u, T v) {
  /* remove forward edge */
  auto range1 = forwards.equal_range(u);
  bool found1 = false;
  for (auto iter1 = range1.first; !found1 && iter1 != range1.second;
      ++iter1) {
    if (iter1->second == v) {
      forwards.erase(iter1);
      found1 = true;
    }
  }

  /* remove backward edge */
  auto range2 = backwards.equal_range(v);
  bool found2 = false;
  for (auto iter2 = range2.first; !found2 && iter2 != range2.second;
      ++iter2) {
    if (iter2->second == u) {
      backwards.erase(iter2);
      found2 = true;
    }
  }

  /* update roots and leaves if necessary */
  if (forwards.count(u) == 0) {
    leaves.insert(u);
  }
  if (backwards.count(v) == 0) {
    roots.insert(v);
  }
}

template<class T, class Compare>
template<class Comparable, class Container>
bool bi::poset<T,Compare>::match_definites(T u, Comparable v,
    Container& definites) {
  bool deeper = false;
  if (colours[u] < colour) {
    /* not visited yet */
    colours[u] = colour;
    if (compare(v, u) == definite) {
      /* this vertex matches, check if any vertices in the subgraph match
       * more-specifically */
      auto range = forwards.equal_range(u);
      for (auto iter = range.first; iter != range.second; ++iter) {
        deeper = match_definites(iter->second, v, definites) || deeper;
        // ^ do the || in this order to prevent short circuit
      }
      if (!deeper) {
        /* no more-specific matches in the subgraph beneath this vertex, so
         * this is the most-specific match */
        definites.push_back(u);
        deeper = true;
      }
    }
  }
  return deeper;
}

template<class T, class Compare>
template<class Comparable, class Container>
bool bi::poset<T,Compare>::match_possibles(T u, Comparable v,
    Container& possibles) {
  bool deeper = false;
  if (colours[u] < colour) {
    /* not visited yet */
    colours[u] = colour;
    if (compare(v, u) != untrue) {
      /* this vertex matcher, check if any vertices in the subgraph match
       * more-specifically */
      auto range = forwards.equal_range(u);
      for (auto iter = range.first; iter != range.second; ++iter) {
        deeper = match_possibles(iter->second, v, possibles) || deeper;
        // ^ do the || in this order to prevent short circuit
      }
      if (!deeper) {
        /* no more-specific matches in the subgraph beneath this vertex, so
         * this is the most-specific match */
        possibles.push_back(u);
        deeper = true;
      }
    }
  }
  return deeper;
}

template<class T, class Compare>
void bi::poset<T,Compare>::forward(T v) {
  ++colour;
  auto roots1 = roots;  // local copy as may be modified during iteration
  for (auto iter = roots1.begin(); iter != roots1.end(); ++iter) {
    if (*iter != v) {
      forward(*iter, v);
    }
  }
}

template<class T, class Compare>
void bi::poset<T,Compare>::forward(T u, T v) {
  if (colours[u] < colour) {
    colours[u] = colour;
    if (compare(u, v) == definite) {
      add_edge(v, u);
    } else {
      /* local copy of forward edges, as may change */
      std::list<T> forwards1;
      auto range = forwards.equal_range(u);
      for (auto iter = range.first; iter != range.second; ++iter) {
        forwards1.push_back(iter->second);
      }
      for (auto iter = forwards1.begin(); iter != forwards1.end(); ++iter) {
        forward(*iter, v);
      }
    }
  }
}

template<class T, class Compare>
void bi::poset<T,Compare>::backward(T v) {
  ++colour;
  auto leaves1 = leaves;  // local copy as may be modified during iteration
  for (auto iter = leaves1.begin(); iter != leaves1.end(); ++iter) {
    if (*iter != v) {
      backward(*iter, v);
    }
  }
}

template<class T, class Compare>
void bi::poset<T,Compare>::backward(T u, T v) {
  if (colours[u] < colour) {
    colours[u] = colour;
    if (compare(v, u) == definite) {
      add_edge(u, v);
    } else {
      /* local copy of backward edges, as may change */
      std::list<T> backwards1;
      auto range = backwards.equal_range(u);
      for (auto iter = range.first; iter != range.second; ++iter) {
        backwards1.push_back(iter->second);
      }
      for (auto iter = backwards1.begin(); iter != backwards1.end(); ++iter) {
        backward(*iter, v);
      }
    }
  }
}

template<class T, class Compare>
void bi::poset<T,Compare>::reduce() {
  auto roots1 = roots;  // local copy as may change
  for (auto iter = roots1.begin(); iter != roots1.end(); ++iter) {
    reduce(*iter);
  }
}

template<class T, class Compare>
void bi::poset<T,Compare>::reduce(T u) {
  int colour1 = ++colour;

  /* local copy of forward edges, as may change */
  std::list<T> forwards1;
  auto range = forwards.equal_range(u);
  for (auto iter = range.first; iter != range.second; ++iter) {
    forwards1.push_back(iter->second);
  }

  /* depth first search discovery */
  for (auto iter = forwards1.begin(); iter != forwards1.end(); ++iter) {
    if (colours[*iter] < colour1) {
      colours[*iter] = colour1;
    }
    reduce(*iter);
  }

  /* remove edges for children that were rediscovered */
  for (auto iter = forwards1.begin(); iter != forwards1.end(); ++iter) {
    if (colours[*iter] > colour1) {  // rediscovered
      remove_edge(u, *iter);
    }
  }
}

template<class T, class Compare>
void bi::poset<T,Compare>::dot() {
  ++colour;
  std::cout << "digraph {" << std::endl;
  auto iter = roots.begin();
  while (iter != roots.end()) {
    dot(*iter);
    ++iter;
  }
  std::cout << "}" << std::endl;
}

template<class T, class Compare>
void bi::poset<T,Compare>::dot(T u) {
  if (colours[u] != colour) {
    colours[u] = colour;
    std::cout << "\"" << u << "\"" << std::endl;
    auto range = forwards.equal_range(u);
    for (auto iter = range.first; iter != range.second; ++iter) {
      std::cout << "\"" << u << "\" -> \"" << *iter << "\"" << std::endl;
      dot(*iter);
      ++iter;
    }
  }
}
