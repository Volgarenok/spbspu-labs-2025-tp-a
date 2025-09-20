#ifndef FUNCTIONAL_HPP
#define FUNCTIONAL_HPP

#include "polygon.hpp"
#include <algorithm>
#include <numeric>
#include <string>

namespace nehvedovich
{
  struct AreaAccumulator;
  struct VerticesComparator;
  struct AreaComparator;
  struct CongruentPolygonsCounter;
  struct AreEqualPoints;

  using PolyVector = std::vector< Polygon >;
  using AreaIt = PolyVector::const_iterator;
  using AreaAlgo = AreaIt (*)(AreaIt, AreaIt, const AreaComparator &);
  using VertIt = PolyVector::const_iterator;
  using VertAlgo = VertIt (*)(VertIt, VertIt, const VerticesComparator &);

  AreaIt maxAreaElement(AreaIt first, AreaIt last, const AreaComparator &comp);
  AreaIt minAreaElement(AreaIt first, AreaIt last, const AreaComparator &comp);
  VertIt maxVertElement(VertIt first, VertIt last, const VerticesComparator &comp);
  VertIt minVertElement(VertIt first, VertIt last, const VerticesComparator &comp);

  void readPolygons(std::istream &, std::vector< Polygon > &);

  bool is_congruent_by_shift(const Polygon &candidate, const Polygon &reference, std::size_t shift);
  unsigned count_translation_congruent(const PolyVector &polygons, const Polygon &reference);

  struct AreaAccumulator
  {
    AreaAccumulator(const std::string &p):
      param(p)
    {}

    double operator()(double acc, const nehvedovich::Polygon &poly) const
    {
      if (param == "ODD")
      {
        if (poly.points.size() % 2 == 1)
        {
          acc += calcPolygonArea(poly);
        }
      }
      else if (param == "EVEN")
      {
        if (poly.points.size() % 2 == 0)
        {
          acc += calcPolygonArea(poly);
        }
      }
      else if (param == "MEAN")
      {
        acc += calcPolygonArea(poly);
      }
      else
      {
        int verticesCount = 0;
        try
        {
          verticesCount = std::stoi(param);
        }
        catch (...)
        {
          throw;
        }

        if (verticesCount > 0)
        {
          if (poly.points.size() == static_cast< std::size_t >(verticesCount))
          {
            acc += calcPolygonArea(poly);
          }
        }
      }
      return acc;
    }

    const std::string &param;
  };

  struct VerticesComparator
  {
    bool operator()(const Polygon &p1, const Polygon &p2) const
    {
      return p1.points.size() < p2.points.size();
    }
  };

  struct AreaComparator
  {
    bool operator()(const Polygon &p1, const Polygon &p2) const
    {
      return calcPolygonArea(p1) < calcPolygonArea(p2);
    }
  };

  struct ShapesAccumulator
  {
    ShapesAccumulator(const std::string &p):
      param(p)
    {}

    double operator()(double acc, const nehvedovich::Polygon &poly) const
    {
      if (param == "ODD")
      {
        if (poly.points.size() % 2 == 1)
        {
          acc++;
        }
      }
      else if (param == "EVEN")
      {
        if (poly.points.size() % 2 == 0)
        {
          acc++;
        }
      }
      else
      {
        int verticesCount = 0;
        try
        {
          verticesCount = std::stoi(param);
        }
        catch (...)
        {
          throw;
        }

        if (verticesCount > 0)
        {
          if (poly.points.size() == static_cast< std::size_t >(verticesCount))
          {
            acc++;
          }
        }
      }
      return acc;
    }
    const std::string &param;
  };

  struct RmEchoPredicate
  {
    RmEchoPredicate(const Polygon &poly):
      ref(poly)
    {}

    bool operator()(const Polygon &p1, const Polygon &p2)
    {
      return (p1 == p2) && (ref == p1);
    }

    const Polygon &ref;
  };

  struct TranslatePoint
  {
    TranslatePoint(int dx_, int dy_):
      dx(dx_),
      dy(dy_)
    {}

    Point operator()(const Point &p) const
    {
      return Point {p.x + dx, p.y + dy};
    }

    int dx, dy;
  };

  struct AnyOfShift
  {
    using PointVec = std::vector< Point >;

    AnyOfShift(const PointVec &candidate_, const PointVec &reference_):
      candidate(candidate_),
      reference(reference_)
    {}

    bool operator()(std::size_t shift) const
    {
      PointVec rotated(candidate.size());
      std::rotate_copy(candidate.begin(), candidate.begin() + shift, candidate.end(), rotated.begin());

      int dx = reference[0].x - rotated[0].x;
      int dy = reference[0].y - rotated[0].y;

      PointVec translated(rotated.size());
      std::transform(rotated.begin(), rotated.end(), translated.begin(), TranslatePoint(dx, dy));

      return std::equal(translated.begin(), translated.end(), reference.begin());
    }

    const PointVec &candidate;
    const PointVec &reference;
  };

  struct IsTranslationCongruent
  {
    IsTranslationCongruent(const Polygon &reference_):
      reference(reference_)
    {}

    bool operator()(const Polygon &poly) const
    {
      if (poly.points.size() != reference.points.size())
      {
        return false;
      }

      std::vector< std::size_t > shifts(reference.points.size());
      std::iota(shifts.begin(), shifts.end(), 0);

      if (std::any_of(shifts.begin(), shifts.end(), AnyOfShift(poly.points, reference.points)))
      {
        return true;
      }

      std::vector< Point > reversed(poly.points.rbegin(), poly.points.rend());
      if (std::any_of(shifts.begin(), shifts.end(), AnyOfShift(reversed, reference.points)))
      {
        return true;
      }

      return false;
    }

    const Polygon &reference;
  };

  struct AccumulatePoints
  {
    std::vector< Point > operator()(std::vector< Point > acc, const Polygon &poly) const;
  };

  struct CompareX
  {
    bool operator()(const Point &a, const Point &b) const;
  };

  struct CompareY
  {
    bool operator()(const Point &a, const Point &b) const;
  };

  struct BoundingBox
  {
    int minX, maxX, minY, maxY;
    BoundingBox(const std::vector< Polygon > &polygons);

    struct ContainsPoint
    {
      int minX, maxX, minY, maxY;
      ContainsPoint(int minX, int maxX, int minY, int maxY);
      bool operator()(const Point &p) const;
    };

    bool contains(const Polygon &poly) const;
  };

  struct PlusD
  {
    double operator()(double a, double b) const
    {
      return a + b;
    }
  };

  struct MinusD
  {
    double operator()(double a, double b) const
    {
      return a - b;
    }
  };

  struct MultD
  {
    double operator()(double a, double b) const
    {
      return a * b;
    }
  };

  template < typename T >
  struct BetweenClosed
  {
    T lo;
    T hi;

    explicit BetweenClosed(const T &lo_, const T &hi_):
      lo(lo_),
      hi(hi_)
    {}

    bool operator()(const T &v) const
    {
      return !(v < lo) && !(hi < v);
    }
  };

  template < typename T >
  struct LessThan
  {
    T bound;

    explicit LessThan(const T &b):
      bound(b)
    {}

    bool operator()(const T &v) const
    {
      return v < bound;
    }
  };

  template < typename T >
  struct EqualTo
  {
    T ref;

    explicit EqualTo(const T &r):
      ref(r)
    {}

    bool operator()(const T &v) const
    {
      return v == ref;
    }
  };

  struct Affine
  {
    double a;
    double b;

    Affine(double a_, double b_):
      a(a_),
      b(b_)
    {}

    double operator()(double x) const
    {
      return a * x + b;
    }
  };

  struct SubPoint
  {
    explicit SubPoint(const Point &base):
      base_(base)
    {}
    Point operator()(const Point &p) const
    {
      return Point {p.x - base_.x, p.y - base_.y};
    }
    Point base_;
  };

  struct NormalizePolygon
  {
    std::vector< Point > operator()(const Polygon &poly) const
    {
      if (poly.points.empty())
      {
        return std::vector< Point >();
      }

      std::vector< Point >::const_iterator minIt =
          std::min_element(poly.points.begin(), poly.points.end(), PointLess());
      std::vector< Point > rel(poly.points.size());
      std::transform(poly.points.begin(), poly.points.end(), rel.begin(), SubPoint(*minIt));
      std::sort(rel.begin(), rel.end(), PointLess());
      return rel;
    }
  };

  struct SameByTranslation
  {
    explicit SameByTranslation(const Polygon &ref):
      refKey_(NormalizePolygon()(ref))
    {}

    bool operator()(const Polygon &other) const
    {
      if (refKey_.empty() || other.points.size() != refKey_.size())
      {
        return false;
      }
      const std::vector< Point > key = NormalizePolygon()(other);
      return std::equal(refKey_.begin(), refKey_.end(), key.begin());
    }

  private:
    std::vector< Point > refKey_;
  };

}

#endif
