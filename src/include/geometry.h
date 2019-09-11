#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "welltris.h"

struct Geo
{
  Images           im;
  int              fromx, fromy;
  int              tox,toy;
  unsigned int     l,h;
};

struct GeoObject
{
  char             name[GEO_NAME];
  Geo              *geo;
};

extern Geo* get_geo_by_name(const char* name);


#endif
