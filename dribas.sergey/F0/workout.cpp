#include "workout.hpp"
#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>
#include <locale>
#include <stdexcept>
#include <cmath>
#include <numbers>
#include <streamGuard.hpp>
#include <iomanip>

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/time_facet.hpp>
#include <boost/optional.hpp>
#include <boost/date_time/posix_time/posix_time_io.hpp>

namespace
{
  double toRad(double degree)
  {
    return degree * std::acos(-1.0) / 180.0;
  }
  double calculateDistance(double lat1, double lon1, double lat2, double lon2)
  {
    const double kEarthRadiusKm = 6371.0;
    double dLat = toRad(lat2 - lat1);
    double dLon = toRad(lon2 - lon1);
    double sinDLat2 = std::sin(dLat / 2);
    double sinDLon2 = std::sin(dLon / 2);
    double cosLat1 = std::cos(toRad(lat1));
    double cosLat2 = std::cos(toRad(lat2));
    double a = sinDLat2 * sinDLat2;
    a += cosLat1 * cosLat2 * sinDLon2 * sinDLon2;
    double c = 2 * std::atan2(std::sqrt(a), std::sqrt(1 - a));
    double distance = kEarthRadiusKm * c;
    return distance;
  }
}

namespace dribas
{
  workout parseGpx(std::istream& gpxStream)
  {
    workout result;
    boost::property_tree::ptree pt;
    try {
      boost::property_tree::read_xml(gpxStream, pt);
    } catch (const boost::property_tree::xml_parser_error& e) {
      std::cerr << "XML error: " << e.what() << '\n';
      return result;
    }

    result.name = pt.get< std::string >("gpx.trk.name", "Unknown workout");

    std::vector< int > heartRates;
    std::vector< int > cadences;
    boost::posix_time::ptime startTimePt;
    boost::posix_time::ptime endTimePt;
    bool firstPoint = true;

    double prevLat = 0.0;
    double prevLon = 0.0;

    try {
      for (const auto& trkptV : pt.get_child("gpx.trk.trkseg")) {
        if (trkptV.first == "trkpt") {
          std::string timeStr = trkptV.second.get< std::string >("time");
          size_t dotPos = timeStr.find('.');
          if (dotPos != std::string::npos) {
            timeStr = timeStr.substr(0, dotPos);
          }
          if (!timeStr.empty() && timeStr.back() == 'Z') {
            timeStr.pop_back();
          }
          try {
            std::stringstream timeSs(timeStr);
            timeSs.imbue(std::locale(std::locale::classic(), new boost::posix_time::time_input_facet("%Y-%m-%dT%H:%M:%S")));
            boost::posix_time::ptime currentTimestamp;
            timeSs >> currentTimestamp;
            if (timeSs.fail() || currentTimestamp.is_special()) {
              throw std::runtime_error("Time parse failed: " + timeStr);
            }
            if (firstPoint) {
              startTimePt = currentTimestamp;
            }
            endTimePt = currentTimestamp;
          } catch (const std::exception& e) {
            std::cerr << "Time error: '" << timeStr << "' - " << e.what() << '\n';
          }

          double currentLat = 0.0;
          double currentLon = 0.0;

          try {
            currentLat = trkptV.second.get< double >("<xmlattr>.lat");
            currentLon = trkptV.second.get< double >("<xmlattr>.lon");
          } catch (const boost::bad_lexical_cast& e) {
            std::cerr << "Coord error: " << e.what() << '\n';
            firstPoint = true;
            continue;
          } catch (const boost::property_tree::ptree_bad_path& e) {
            std::cerr << "No coords in trkpt: " << e.what() << '\n';
            firstPoint = true;
            continue;
          }

          if (!firstPoint) {
            result.distance += calculateDistance(prevLat, prevLon, currentLat, currentLon);
          } else {
            firstPoint = false;
          }
          prevLat = currentLat;
          prevLon = currentLon;

          boost::optional< std::string >hrStrOpt = trkptV.second.get_optional< std::string >("extensions.ns3:TrackPointExtension.ns3:hr");
          if (hrStrOpt) {
            try {
              int hr = std::stoi(*hrStrOpt);
              heartRates.push_back(hr);
            } catch (const std::invalid_argument&) {
              std::cerr << "HR not number: '" << *hrStrOpt << "'\n";
            } catch (const std::out_of_range&) {
              std::cerr << "HR out of range: '" << *hrStrOpt << "'\n";
            }
          }

          boost::optional < std::string > cadStrOpt = trkptV.second.get_optional < std::string > ("extensions.ns3:TrackPointExtension.ns3:cad");
          if (cadStrOpt) {
            try {
              int cad = std::stoi(*cadStrOpt);
              cadences.push_back(cad);
            } catch (const std::invalid_argument&) {
              std::cerr << "Cadence not number: '" << *cadStrOpt << "'\n";
            } catch (const std::out_of_range&) {
              std::cerr << "Cadence out of range: '" << *cadStrOpt << "'\n";
            }
          }
        }
      }
    } catch (const boost::property_tree::ptree_bad_path& e) {
      std::cerr << "GPX path missing:" << e.what() << '\n';
    }
    if (!heartRates.empty()) {
      long long sumHeartRates = std::accumulate(heartRates.begin(), heartRates.end(), 0LL);
      result.avgHeart = static_cast< int >(sumHeartRates / heartRates.size());
      result.maxHeart = *std::max_element(heartRates.begin(), heartRates.end());
    }

    if (!cadences.empty()) {
      long long sumCadences = std::accumulate(cadences.begin(), cadences.end(), 0LL);
      result.cadence = static_cast< int >(sumCadences / cadences.size());
    }

    boost::posix_time::ptime epoch(boost::gregorian::date(1970, 1, 1));
    if (!startTimePt.is_special()) {
      boost::posix_time::time_duration diff = startTimePt - epoch;
      result.timeStart = diff.total_seconds();
    }
    if (!endTimePt.is_special()) {
      boost::posix_time::time_duration diff = endTimePt - epoch;
      result.timeEnd = diff.total_seconds();
    }

    if (result.distance > 0 && result.timeEnd > result.timeStart) {
      long long durationSeconds = result.timeEnd - result.timeStart;
      double durationMinutes = static_cast < double > (durationSeconds) / 60.0;
      result.avgPaceMinPerKm = durationMinutes / result.distance;
    } else {
      result.avgPaceMinPerKm = 0.0;
    }

    return result;
  }

  std::istream& operator>>(std::istream& is, workout& w)
  {
    std::istream::sentry sentry(is);
    if (!sentry) {
      return is;
    }
    dribas::StreamGuard guard(is);
    is.unsetf(std::ios_base::skipws);
    w = parseGpx(is);
    return is;
  }

  std::ostream& operator<<(std::ostream& os, const workout& w)
  {
    dribas::StreamGuard guard(os);

    boost::posix_time::ptime startTimePt = boost::posix_time::from_time_t(w.timeStart);
    boost::posix_time::ptime endTimePt = boost::posix_time::from_time_t(w.timeEnd);

    std::string formattedStartTime = boost::posix_time::to_simple_string(startTimePt);
    std::string formattedEndTime = boost::posix_time::to_simple_string(endTimePt);

    long long durationSeconds = w.timeEnd - w.timeStart;
    long long hours = durationSeconds / 3600;
    long long minutes = (durationSeconds % 3600) / 60;
    long long seconds = durationSeconds % 60;

    int paceMinutes = static_cast< int >(w.avgPaceMinPerKm);
    int paceSeconds = static_cast< int >((w.avgPaceMinPerKm - paceMinutes) * 60.0);

    os << "Workout name: " << w.name << '\n';
    os << "Average heart rate: " << w.avgHeart << '\n';
    os << "Maximum heart rate: " << w.maxHeart << '\n';
    os << "Average cadence: " << w.cadence << '\n';
    os << "Distance: " << std::fixed << std::setprecision(2) << w.distance << " km" << '\n';
    os << "Average pace: " << paceMinutes << ":" << (paceSeconds < 10 ? "0" : "") << paceSeconds << " min/km" << '\n';
    os << "Start time: " << formattedStartTime << '\n';
    os << "End time: " << formattedEndTime << '\n';
    os << "Duration: " << hours << "h " << minutes << "m " << seconds << "s" << '\n';
    return os;
  }
}
