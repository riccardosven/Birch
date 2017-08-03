/**
 * @file
 *
 * Standard headers that should be included for all C++ files generated by
 * the compiler.
 */
#pragma once

#include "bi/lib/Span.hpp"
#include "bi/lib/Index.hpp"
#include "bi/lib/Range.hpp"
#include "bi/lib/Frame.hpp"
#include "bi/lib/View.hpp"
#include "bi/lib/Array.hpp"
#include "bi/lib/Markable.hpp"
#include "bi/lib/Object.hpp"
#include "bi/lib/Pointer.hpp"
#include "bi/lib/FiberState.hpp"
#include "bi/lib/Fiber.hpp"

#include "boost/optional.hpp"

#include <getopt.h>
#include <gc.h>

#include <utility>
#include <functional>
#include <string>
#include <cstdlib>
#include <cassert>

namespace bi {
/**
 * Left tilde (`<~`) operator
 */
template<class Left, class Right>
void left_tilde_(Left& left, const Right& right) {
  left = right->simulate_();
}

/**
 * Right tilde (`~>`) operator
 */
template<class Left, class Right>
auto right_tilde_(const Left& left, const Right& right) {
  return right->observe_(left);
}

/**
 * Tilde (`~`) operator
 */
template<class Left, class Right>
auto tilde_(Left& left, const Right& right) {
  assert(left->isUninitialized_());
  if (!left->isMissing_()) {
    right_tilde_(left->value_(), right);
  }
  left = right;
}

/**
 * Make a span.
 *
 * @ingroup library
 *
 * @param length Length.
 */
inline auto make_span(const size_t length) {
  return Span<mutable_value,1,mutable_value>(length);
}

/**
 * Make an index.
 *
 * @ingroup library
 *
 * @param index Index.
 */
inline auto make_index(const ptrdiff_t i) {
  return Index<mutable_value>(i);
}

/**
 * Make a range.
 *
 * @ingroup library
 *
 * @param start First index.
 * @param end Last index.
 */
inline auto make_range(const ptrdiff_t start, const ptrdiff_t end) {
  return Range<mutable_value,mutable_value,1>(start, end - start + 1);
}

/**
 * Make a frame, no arguments.
 *
 * @ingroup library
 */
inline auto make_frame() {
  return EmptyFrame();
}

/**
 * Make a frame, single argument.
 *
 * @ingroup library
 */
template<size_t length_value, ptrdiff_t stride_value, size_t lead_value>
auto make_frame(const Span<length_value,stride_value,lead_value>& arg) {
  auto tail = make_frame();
  auto head = arg;
  return NonemptyFrame<decltype(tail),decltype(head)>(tail, head);
}

/**
 * Make a frame, single argument.
 *
 * @ingroup library
 */
inline auto make_frame(const size_t arg) {
  return make_frame(make_span(arg));
}

/**
 * Make a frame, multiple arguments.
 *
 * @ingroup library
 */
template<size_t length_value, ptrdiff_t stride_value, size_t lead_value,
    class ... Args>
auto make_frame(const Span<length_value,stride_value,lead_value>& arg,
    Args ... args) {
  return make_frame(make_frame(arg), args...);
}

/**
 * Make a frame, multiple arguments.
 *
 * @ingroup library
 */
template<class ... Args>
auto make_frame(const size_t arg, Args ... args) {
  return make_frame(make_frame(arg), args...);
}

/**
 * Make a frame, tail and single argument.
 *
 * @ingroup library
 */
template<class Tail, class Head, size_t length_value, ptrdiff_t stride_value,
size_t lead_value>
auto make_frame(const NonemptyFrame<Tail,Head>& frame,
    const Span<length_value,stride_value,lead_value>& arg) {
  auto tail = frame;
  auto head = arg;
  return NonemptyFrame<decltype(tail),decltype(head)>(tail, head);
}

/**
 * Make a frame, tail and single argument.
 *
 * @ingroup library
 */
template<class Tail, class Head>
auto make_frame(const NonemptyFrame<Tail,Head>& frame, const size_t arg) {
  return make_frame(frame, make_span(arg));
}

/**
 * Make a frame, tail and multiple arguments.
 *
 * @ingroup library
 */
template<class Tail, class Head, class Arg, class ... Args>
auto make_frame(const NonemptyFrame<Tail,Head>& tail, const Arg& arg,
    Args ... args) {
  return make_frame(make_frame(tail, arg), args...);
}

/**
 * Make a frame, tail and multiple arguments.
 *
 * @ingroup library
 */
template<class Tail, class Head, class ... Args>
auto make_frame(const NonemptyFrame<Tail,Head>& tail, const size_t arg,
    Args ... args) {
  return make_frame(make_frame(tail, make_span(arg)), args...);
}

/**
 * Make a view, no arguments.
 *
 * @ingroup library
 */
inline auto make_view() {
  return EmptyView();
}

/**
 * Make a view, single argument.
 *
 * @ingroup library
 */
template<ptrdiff_t offset_value, size_t length_value, ptrdiff_t stride_value>
auto make_view(const Range<offset_value,length_value,stride_value>& arg) {
  auto tail = make_view();
  auto head = arg;
  return NonemptyView<decltype(tail),decltype(head)>(tail, head);
}

/**
 * Make a view, single argument.
 *
 * @ingroup library
 */
template<ptrdiff_t offset_value>
auto make_view(const Index<offset_value>& arg) {
  auto tail = make_view();
  auto head = arg;
  return NonemptyView<decltype(tail),decltype(head)>(tail, head);
}

/**
 * Make a view, single argument.
 *
 * @ingroup library
 */
inline auto make_view(const ptrdiff_t arg) {
  return make_view(make_index(arg));
}

/**
 * Make a view, multiple arguments.
 *
 * @ingroup library
 */
template<ptrdiff_t offset_value, size_t length_value, ptrdiff_t stride_value,
    class ... Args>
auto make_view(const Range<offset_value,length_value,stride_value>& arg,
    Args ... args) {
  return make_view(make_view(arg), args...);
}

/**
 * Make a view, multiple arguments.
 *
 * @ingroup library
 */
template<ptrdiff_t offset_value, class ... Args>
auto make_view(const Index<offset_value>& arg, Args ... args) {
  return make_view(make_view(arg), args...);
}

/**
 * Make a view, multiple arguments.
 *
 * @ingroup library
 */
template<class ... Args>
auto make_view(const ptrdiff_t arg, Args ... args) {
  return make_view(make_index(arg), args...);
}

/**
 * Make a view, tail plus single argument.
 *
 * @ingroup library
 */
template<class Tail, class Head, ptrdiff_t offset_value, size_t length_value,
    ptrdiff_t stride_value>
auto make_view(const NonemptyView<Tail,Head>& view,
    const Range<offset_value,length_value,stride_value>& arg) {
  auto tail = view;
  auto head = arg;
  return NonemptyView<decltype(tail),decltype(head)>(tail, head);
}

/**
 * Make a view, tail plus single argument.
 *
 * @ingroup library
 */
template<class Tail, class Head, ptrdiff_t offset_value>
auto make_view(const NonemptyView<Tail,Head>& view,
    const Index<offset_value>& arg) {
  auto tail = view;
  auto head = arg;
  return NonemptyView<decltype(tail),decltype(head)>(tail, head);
}

/**
 * Make a view, tail plus single argument.
 *
 * @ingroup library
 */
template<class Tail, class Head>
auto make_view(const NonemptyView<Tail,Head>& view, const ptrdiff_t arg) {
  return make_view(view, make_index(arg));
}

/**
 * Make a view, tail plus multiple arguments.
 *
 * @ingroup library
 */
template<class Tail, class Head, class Arg, class ... Args>
auto make_view(const NonemptyView<Tail,Head>& tail, Arg arg, Args ... args) {
  return make_view(make_view(tail, arg), args...);
}

/**
 * Make an array.
 *
 * @ingroup library
 *
 * @tparam Type Value type.
 * @tparam Frame Frame type.
 *
 * @param frame Frame.
 *
 * @return The array.
 */
template<class Type, class Frame = EmptyFrame>
auto make_array(const Frame& frame = EmptyFrame()) {
  return Array<Type,Frame>(frame);
}

/**
 * Make an object.
 *
 * @ingroup library
 *
 * @tparam Type Value type.
 * @tparam Args Argument types.
 *
 * @param args Constructor arguments.
 *
 * @return Pointer to the object.
 */
template<class Type, class... Args>
Pointer<Type> make_object(Args... args) {
  auto raw = new (GC_MALLOC(sizeof(Type))) Type(args...);
  return Pointer<Type>(raw);
}

/**
 * Shallow-copy an object.
 *
 * @ingroup library
 *
 * @tparam Type Value type.
 *
 * @param o The object to copy.
 *
 * @return Pointer to the new object.
 */
template<class Type>
Type* copy_object(Type* o) {
  return new (GC_MALLOC(sizeof(Type))) Type(*o);
}
}
