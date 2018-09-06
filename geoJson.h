#include <json.h>

#ifndef _GEOJSON_H_
#define _GEOJSON_H_

typedef enum geotype {
    GEO_POINT,
    GEO_MULTIPOINT,
    GEO_LINESTRING,
    GEO_MULTILINESTRING,
    GEO_POLYGON,
    GEO_MULTIPOLYGON,
    GEO_GEOMETRYCOLLECTION
} geotype_t;

typedef struct coord {
    long double lat;
    long double lon;
} coord_t;

typedef struct coord_list {
    size_t len;
    coord_t **coords;
} coord_list_t;

typedef struct coord_list_list {
    size_t len;
    coord_list_t **coord_lists;
} coord_list_list_t;

typedef struct coord_list_list_list {
    size_t len;
    coord_list_list_t **coord_list_lists;
} coord_list_list_list_t;

typedef struct property {
    char *key;
    char *val;
} property_t;

typedef struct feature {
    size_t property_len;
    property_t *properties;
    geotype_t geometry_type;
    void *coordset;
} feature_t;

typedef struct feature_list {
    size_t len;
    feature_t *features;
} feature_list_t;

feature_list_t *geoJson_feature_list_from_json ( struct json_object *json_o );
struct json_object *geoJson_feature_list_to_json ( feature_list_t *feature_list );
struct json_object *geoJson_json_from_file ( const char *filename );
int geoJson_json_to_file ( struct json_object *json_o, const char *filename );
void geoJson_json_free ( struct json_object *json_o );
void geoJson_feature_list_free ( feature_list_t *feature_list );

#endif
