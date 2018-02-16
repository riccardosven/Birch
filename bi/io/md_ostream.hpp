/**
 * @file
 */
#pragma once

#include "bi/io/bih_ostream.hpp"

#include <list>
#include <vector>
#include <algorithm>

namespace bi {
/**
 * Output stream for Markdown files.
 *
 * @ingroup birch_io
 */
class md_ostream: public bih_ostream {
public:
  md_ostream(std::ostream& base);

  using bih_ostream::visit;

  virtual void visit(const Package* o);
  virtual void visit(const Name* o);

  virtual void visit(const Parameter* o);
  virtual void visit(const GlobalVariable* o);
  virtual void visit(const MemberVariable* o);
  virtual void visit(const Function* o);
  virtual void visit(const Fiber* o);
  virtual void visit(const Program* o);
  virtual void visit(const MemberFunction* o);
  virtual void visit(const MemberFiber* o);
  virtual void visit(const BinaryOperator* o);
  virtual void visit(const UnaryOperator* o);
  virtual void visit(const AssignmentOperator* o);
  virtual void visit(const ConversionOperator* o);
  virtual void visit(const Basic* o);
  virtual void visit(const Class* o);

  virtual void visit(const TypeList* o);
  virtual void visit(const ClassType* o);
  virtual void visit(const BasicType* o);
  virtual void visit(const ArrayType* o);
  virtual void visit(const TupleType* o);
  virtual void visit(const SequenceType* o);
  virtual void visit(const FunctionType* o);
  virtual void visit(const FiberType* o);
  virtual void visit(const OptionalType* o);
  virtual void visit(const PointerType* o);

private:
  void genHead(const std::string& name);

  template<class ObjectType, class RootType>
  void genBrief(const std::string& name, const RootType* root,
      const bool sort = false, const bool hideNoDoc = false);

  template<class ObjectType, class RootType>
  void genOneLine(const std::string& name, const RootType* root,
      const bool sort = false, const bool hideNoDoc = false);

  template<class ObjectType, class RootType>
  void genDetailed(const std::string& name, const RootType* root,
      const bool sort = false, const bool hideNoDoc = false);

  template<class ObjectType, class RootType>
  void genSections(const std::string& name, const RootType* root,
      const bool sort = false, const bool hideNoDoc = false);

  /**
   * Current section depth.
   */
  int depth;
};
}

#include "bi/visitor/Gatherer.hpp"
#include "bi/primitive/encode.hpp"

template<class ObjectType, class RootType>
void bi::md_ostream::genBrief(const std::string& name, const RootType* root,
    const bool sort, const bool hideNoDoc) {
  Gatherer<ObjectType> gatherer([&](const ObjectType* o) {
    return !hideNoDoc || !o->loc->doc.empty();
  });
  root->accept(&gatherer);

  std::vector<ObjectType*> sorted(gatherer.size());
  std::copy(gatherer.begin(), gatherer.end(), sorted.begin());
  if (sort) {
    std::stable_sort(sorted.begin(), sorted.end(),
        [](const ObjectType* o1, const ObjectType* o2) {
          return o1->name->str() < o2->name->str();
        });
  }

  if (sorted.size() > 0) {
    genHead(name);
    line("| Name | Brief description |");
    line("| --- | --- |");
    ++depth;
    for (auto o : sorted) {
      start("| ");
      middle('[' << o->name->str() << ']');
      middle("(#" << anchor(o->name->str()) << ')');
      finish(" | " << brief(o->loc->doc) << " |");
    }
    line("");
    --depth;
  }
}

template<class ObjectType, class RootType>
void bi::md_ostream::genOneLine(const std::string& name, const RootType* root,
    const bool sort, const bool hideNoDoc) {
  Gatherer<ObjectType> gatherer([&](const ObjectType* o) {
        return !hideNoDoc || !o->loc->doc.empty();
      });
  root->accept(&gatherer);

  std::vector<ObjectType*> sorted(gatherer.size());
  std::copy(gatherer.begin(), gatherer.end(), sorted.begin());
  if (sort) {
    std::stable_sort(sorted.begin(), sorted.end(),
        [](const ObjectType* o1, const ObjectType* o2) {
          return dynamic_cast<const Named*>(o1)->name->str() <
              dynamic_cast<const Named*>(o2)->name->str();
          // ^ dynamic_cast hack for when ObjectType == ConversionOperator,
          //   as no name.
        });
  }

  if (sorted.size() > 0) {
    genHead(name);
    line("| Name | Description |");
    line("| --- | --- |");
    ++depth;
    for (auto o : sorted) {
      line("| " << o << " | " << one_line(o->loc->doc) << " |");
    }
    line("");
    --depth;
  }
}

template<class ObjectType, class RootType>
void bi::md_ostream::genDetailed(const std::string& name,
    const RootType* root, const bool sort, const bool hideNoDoc) {
  Gatherer<ObjectType> gatherer([&](const ObjectType* o) {
    return !hideNoDoc || !o->loc->doc.empty();
  });
  root->accept(&gatherer);

  std::vector<ObjectType*> sorted(gatherer.size());
  std::copy(gatherer.begin(), gatherer.end(), sorted.begin());
  if (sort) {
    std::stable_sort(sorted.begin(), sorted.end(),
        [](const ObjectType* o1, const ObjectType* o2) {
          return o1->name->str() < o2->name->str();
        });
  }

  if (sorted.size() > 0) {
    genHead(name);
    ++depth;
    std::string name, desc;
    for (auto o : sorted) {
      if (o->name->str() != name) {
        /* heading for the first overload of this name */
        genHead(o->name->str());
        line("<a name=\"" << anchor(o->name->str()) << "\"></a>\n");
      }
      name = o->name->str();
      desc = quote(detailed(o->loc->doc), "    ");
      *this << o;
      line("");
      line(desc);
      line("");
    }
    --depth;
  }
}

template<class ObjectType, class RootType>
void bi::md_ostream::genSections(const std::string& name,
    const RootType* root, const bool sort, const bool hideNoDoc) {
  Gatherer<ObjectType> gatherer([&](const ObjectType* o) {
    return !hideNoDoc || !o->loc->doc.empty();
  });
  root->accept(&gatherer);

  std::vector<ObjectType*> sorted(gatherer.size());
  std::copy(gatherer.begin(), gatherer.end(), sorted.begin());
  if (sort) {
    std::stable_sort(sorted.begin(), sorted.end(),
        [](const ObjectType* o1, const ObjectType* o2) {
          return o1->name->str() < o2->name->str();
        });
  }

  if (sorted.size() > 0) {
    genHead(name);
    ++depth;
    for (auto o : sorted) {
      genHead(o->name->str());
      *this << o;
    }
    --depth;
  }
}
