#include <sstream>
#include <iomanip>
#include <algorithm>
#include <functional>
#include <cstdlib>
#include "commands.hpp"
#include "utils.hpp"

namespace kazak {
  namespace commands {

    namespace {
      void printDouble(std::ostream & os, double val) {
        os << std::fixed << std::setprecision(1) << val << "\n";
      }

      bool isNumber(const std::string & s) {
        if (s.empty())
          return false;
        char * endptr = nullptr;
        std::strtol(s.c_str(), &endptr, 10);
        return (*endptr == '\0');
      }

      struct IsEven {
        bool operator()(const Polygon & p) const {
          return p.vertexCount() % 2 == 0;
        }
      };

      struct IsOdd {
        bool operator()(const Polygon & p) const {
          return p.vertexCount() % 2 == 1;
        }
      };

      struct AreaAdder {
        AreaAdder(double & acc, const std::function<bool(const Polygon &)> & cond)
          : acc_(acc), cond_(cond) {}

        void operator()(const Polygon & p) const {
          if (cond_(p)) {
            acc_ += p.area();
          }
        }

        double & acc_;
        const std::function<bool(const Polygon &)> & cond_;
      };

      struct AreaSum {
        AreaSum(double & acc, int & count)
          : acc_(acc), count_(count) {}

        void operator()(const Polygon & p) const {
          acc_ += p.area();
          ++count_;
        }

        double & acc_;
        int & count_;
      };
    }

    void commandArea(const std::vector<Polygon> & polygons,
                     const std::string & param,
                     std::ostream & os)
    {
      if (param == "EVEN") {
        double sum = 0.0;
        std::for_each(polygons.begin(), polygons.end(), AreaAdder(sum, IsEven()));
        printDouble(os, sum);
      } else if (param == "ODD") {
        double sum = 0.0;
        std::for_each(polygons.begin(), polygons.end(), AreaAdder(sum, IsOdd()));
        printDouble(os, sum);
      } else if (param == "MEAN") {
        double sum = 0.0;
        int count = 0;
        std::for_each(polygons.begin(), polygons.end(), AreaSum(sum, count));
        if (count == 0) {
          os << "<INVALID COMMAND>\n";
        } else {
          printDouble(os, sum / count);
        }
      } else if (isNumber(param)) {
        int target = std::atoi(param.c_str());
        if (target <= 0) {
          os << "<INVALID COMMAND>\n";
          return;
        }
        struct MatchVertexCount {
          MatchVertexCount(int v, double & s) : v_(v), s_(s) {}
          void operator()(const Polygon & p) const {
            if (static_cast<int>(p.vertexCount()) == v_) {
              s_ += p.area();
            }
          }
          int v_;
          double & s_;
        };
        double sum = 0.0;
        std::for_each(polygons.begin(), polygons.end(), MatchVertexCount(target, sum));
        printDouble(os, sum);
      } else {
        os << "<INVALID COMMAND>\n";
      }
    }

    void commandMax(const std::vector<Polygon> & polygons,
                    const std::string & param,
                    std::ostream & os)
    {
      if (polygons.empty()) {
        os << "<INVALID COMMAND>\n";
        return;
      }
      if (param == "AREA") {
        auto it = std::max_element(polygons.begin(), polygons.end(),
          std::bind(std::less<double>(),
                    std::bind(&Polygon::area, std::placeholders::_1),
                    std::bind(&Polygon::area, std::placeholders::_2)));
        printDouble(os, it->area());
      } else if (param == "VERTEXES") {
        auto it = std::max_element(polygons.begin(), polygons.end(),
          std::bind(std::less<size_t>(),
                    std::bind(&Polygon::vertexCount, std::placeholders::_1),
                    std::bind(&Polygon::vertexCount, std::placeholders::_2)));
        os << it->vertexCount() << "\n";
      } else {
        os << "<INVALID COMMAND>\n";
      }
    }

    void commandMin(const std::vector<Polygon> & polygons,
                    const std::string & param,
                    std::ostream & os)
    {
      if (polygons.empty()) {
        os << "<INVALID COMMAND>\n";
        return;
      }
      if (param == "AREA") {
        auto it = std::min_element(polygons.begin(), polygons.end(),
          std::bind(std::less<double>(),
                    std::bind(&Polygon::area, std::placeholders::_2),
                    std::bind(&Polygon::area, std::placeholders::_1)));
        printDouble(os, it->area());
      } else if (param == "VERTEXES") {
        auto it = std::min_element(polygons.begin(), polygons.end(),
          std::bind(std::less<size_t>(),
                    std::bind(&Polygon::vertexCount, std::placeholders::_2),
                    std::bind(&Polygon::vertexCount, std::placeholders::_1)));
        os << it->vertexCount() << "\n";
      } else {
        os << "<INVALID COMMAND>\n";
      }
    }

    void commandCount(const std::vector<Polygon> & polygons,
                      const std::string & param,
                      std::ostream & os)
    {
      if (param == "EVEN") {
        int count = std::count_if(polygons.begin(), polygons.end(), IsEven());
        os << count << "\n";
      } else if (param == "ODD") {
        int count = std::count_if(polygons.begin(), polygons.end(), IsOdd());
        os << count << "\n";
      } else if (isNumber(param)) {
        int target = std::atoi(param.c_str());
        if (target <= 0) {
          os << "<INVALID COMMAND>\n";
          return;
        }
        struct MatchVertex {
          MatchVertex(int v) : v_(v) {}
          bool operator()(const Polygon & p) const {
            return static_cast<int>(p.vertexCount()) == v_;
          }
          int v_;
        };
        int count = std::count_if(polygons.begin(), polygons.end(), MatchVertex(target));
        os << count << "\n";
      } else {
        os << "<INVALID COMMAND>\n";
      }
    }

    namespace {
      bool parsePolygonFromLine(const std::string & line, Polygon & poly) {
        return kazak::utils::parsePolygon(line, poly);
      }
    }

    void commandEcho(std::vector<Polygon> & polygons,
                     const std::string & line,
                     std::ostream & os)
    {
      Polygon target;
      if (!parsePolygonFromLine(line, target)) {
        os << "<INVALID COMMAND>\n";
        return;
      }
      int added = 0;
      for (size_t i = 0; i < polygons.size(); ++i) {
        if (polygons[i] == target) {
          polygons.insert(polygons.begin() + i + 1, target);
          ++added;
          ++i;
        }
      }
      os << added << "\n";
    }

    void commandInframe(const std::vector<Polygon> & polygons,
                        const std::string & line,
                        std::ostream & os)
    {
      Polygon target;
      if (!parsePolygonFromLine(line, target)) {
        os << "<INVALID COMMAND>\n";
        return;
      }
      if (polygons.empty()) {
        os << "<FALSE>\n";
        return;
      }
      Point minP, maxP;
      kazak::utils::boundingBox(polygons, minP, maxP);
      bool inside = kazak::utils::polygonInFrame(target, minP, maxP);
      os << (inside ? "<TRUE>\n" : "<FALSE>\n");
    }

  }
}

