#pragma once

#include <QString>

class Location
{
  public:
    Location() = default;
    Location(const QString& city_, const QString& country_, double lat_, double lng_) :
      city(city_), country(country_), lat(lat_), lng(lng_)
    {}

    bool operator==(const Location& other) const
    {
      return city == other.city && country == other.country && lat == other.lat && lng == other.lng;
    }
    bool operator!=(const Location& other) const { return !operator==(other); }

    QString city;
    QString country;
    double lat = 0;
    double lng = 0;
};
