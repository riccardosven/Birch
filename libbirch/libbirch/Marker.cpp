/**
 * @file
 */
#include "libbirch/Marker.hpp"

void libbirch::Marker::visit(Any* o) {
  if (!(o->f.exchangeOr(MARKED) & MARKED)) {
    o->f.maskAnd(~(POSSIBLE_ROOT|BUFFERED|SCANNED|REACHED|COLLECTED));
    o->accept_(*this);
  }
}