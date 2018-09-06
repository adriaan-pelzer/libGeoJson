#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <json.h>

#include "geoJson.h"

coord_t *geoJson_get_coord ( struct json_object *coordinates ) {
    coord_t *rc = NULL, *coords = NULL;
    struct json_object *lat = NULL, *lon = NULL;

    if ( ( lat = json_object_array_get_idx ( coordinates, 0 ) ) == NULL || ! json_object_is_type ( lat, json_type_double ) )
        goto over;

    if ( ( lon = json_object_array_get_idx ( coordinates, 1 ) ) == NULL || ! json_object_is_type ( lon, json_type_double ) )
        goto over;

    if ( ( coords = malloc ( sizeof ( coord_t ) ) ) == NULL )
        goto over;

    coords->lat = json_object_get_double ( lat );
    coords->lon = json_object_get_double ( lon );

    rc = coords;
over:
    if ( rc == NULL && coords != NULL )
        free ( coords );

    return rc;
};

void geoJson_coord_list_free ( coord_list_t *coord_list ) {
    size_t i = 0;

    if ( coord_list ) {
        for ( i = 0; i < coord_list->len; i++ )
            free ( coord_list->coords[i] );
        free ( coord_list );
    }
};

coord_list_t *geoJson_get_coord_list ( struct json_object *coordinates ) {
    coord_list_t *rc = NULL, *coord_list = NULL;
    size_t i = 0, coordinates_len = 0;

    if ( ! json_object_is_type ( coordinates, json_type_array ) )
        goto over;

    coordinates_len = json_object_array_length ( coordinates );

    if ( ( coord_list = calloc ( 1, sizeof ( coord_list_t ) ) ) == NULL )
        goto over;

    if ( ( coord_list->coords = calloc ( 1, coordinates_len * sizeof ( coord_t * ) ) ) == NULL )
        goto over;

    coord_list->len = coordinates_len;

    for ( i = 0; i < coordinates_len; i++ ) {
        struct json_object *coords = NULL;

        if ( ( coords = json_object_array_get_idx ( coordinates, i ) ) == NULL )
            goto over;

        if ( ( coord_list->coords[i] = geoJson_get_coord ( coords ) ) == NULL )
            goto over;
    }

    rc = coord_list;
over:
    if ( rc == NULL && coord_list != NULL )
        geoJson_coord_list_free ( coord_list );   

    return rc;
};

void geoJson_coord_list_list_free ( coord_list_list_t *coord_list_list ) {
    size_t i = 0;

    if ( coord_list_list ) {
        for ( i = 0; i < coord_list_list->len; i++ )
            geoJson_coord_list_free ( coord_list_list->coord_lists[i] );
        free ( coord_list_list );
    }
};

coord_list_list_t *geoJson_get_coord_list_list ( struct json_object *coordinates ) {
    coord_list_list_t *rc = NULL, *coord_list_list = NULL;
    size_t i = 0, coordinates_len = 0;

    if ( ! json_object_is_type ( coordinates, json_type_array ) )
        goto over;

    coordinates_len = json_object_array_length ( coordinates );

    if ( ( coord_list_list = calloc ( 1, sizeof ( coord_list_list_t ) ) ) == NULL )
        goto over;

    if ( ( coord_list_list->coord_lists = calloc ( 1, coordinates_len * sizeof ( coord_list_t * ) ) ) == NULL )
        goto over;

    coord_list_list->len = coordinates_len;

    for ( i = 0; i < coordinates_len; i++ ) {
        struct json_object *coords = NULL;

        if ( ( coords = json_object_array_get_idx ( coordinates, i ) ) == NULL )
            goto over;

        if ( ( coord_list_list->coord_lists[i] = geoJson_get_coord_list ( coords ) ) == NULL )
            goto over;
    }

    rc = coord_list_list;
over:
    if ( rc == NULL && coord_list_list != NULL )
        geoJson_coord_list_list_free ( coord_list_list );   

    return rc;
};

void geoJson_coord_list_list_list_free ( coord_list_list_list_t *coord_list_list_list ) {
    size_t i = 0;

    if ( coord_list_list_list ) {
        for ( i = 0; i < coord_list_list_list->len; i++ )
            geoJson_coord_list_list_free ( coord_list_list_list->coord_list_lists[i] );
        free ( coord_list_list_list );
    }
};

coord_list_list_list_t *geoJson_get_coord_list_list_list ( struct json_object *coordinates ) {
    coord_list_list_list_t *rc = NULL, *coord_list_list_list = NULL;
    size_t i = 0, coordinates_len = 0;

    if ( ! json_object_is_type ( coordinates, json_type_array ) )
        goto over;

    coordinates_len = json_object_array_length ( coordinates );

    if ( ( coord_list_list_list = calloc ( 1, sizeof ( coord_list_list_list_t ) ) ) == NULL )
        goto over;

    if ( ( coord_list_list_list->coord_list_lists = calloc ( 1, coordinates_len * sizeof ( coord_list_list_t * ) ) ) == NULL )
        goto over;

    coord_list_list_list->len = coordinates_len;

    for ( i = 0; i < coordinates_len; i++ ) {
        struct json_object *coords = NULL;

        if ( ( coords = json_object_array_get_idx ( coordinates, i ) ) == NULL )
            goto over;

        if ( ( coord_list_list_list->coord_list_lists[i] = geoJson_get_coord_list_list ( coords ) ) == NULL )
            goto over;
    }

    rc = coord_list_list_list;
over:
    if ( rc == NULL && coord_list_list_list != NULL )
        geoJson_coord_list_list_list_free ( coord_list_list_list );   

    return rc;
};

void *geoJson_get_coordinates ( geotype_t geometry_type, struct json_object *geometry ) {
    void *rc = NULL, *coordset = NULL;
    struct json_object *coordinates = NULL;

    if ( ( coordinates = json_object_object_get ( geometry, "coordinates" ) ) == NULL || ! json_object_is_type ( coordinates, json_type_array ) )
        goto over;

    switch ( geometry_type ) {
        case GEO_POINT:
            if ( ( coordset = (void *) geoJson_get_coord ( coordinates ) ) == NULL )
                goto over;
            break;
        case GEO_MULTIPOINT:
        case GEO_LINESTRING:
            if ( ( coordset = (void *) geoJson_get_coord_list ( coordinates ) ) == NULL )
                goto over;
            break;
        case GEO_MULTILINESTRING:
        case GEO_POLYGON:
            if ( ( coordset = (void *) geoJson_get_coord_list_list ( coordinates ) ) == NULL )
                goto over;
            break;
        case GEO_MULTIPOLYGON:
            if ( ( coordset = (void *) geoJson_get_coord_list_list_list ( coordinates ) ) == NULL )
                goto over;
            break;
        case GEO_GEOMETRYCOLLECTION:
            fprintf ( stderr, "GeometryCollection gemoetry type not currently supported\n" );
            goto over;
        default:
            fprintf ( stderr, "Unknown geometry type: %d\n", geometry_type );
            goto over;
    }

    rc = coordset;
over:
    if ( rc == NULL && coordset != NULL )
        switch ( geometry_type ) {
            case GEO_POINT:
                free ( (coord_t *) coordset );
                break;
            case GEO_MULTIPOINT:
            case GEO_LINESTRING:
                geoJson_coord_list_free ( (coord_list_t *) coordset );
                break;
            case GEO_MULTILINESTRING:
            case GEO_POLYGON:
                geoJson_coord_list_list_free ( (coord_list_list_t *) coordset );
                break;
            case GEO_MULTIPOLYGON:
                geoJson_coord_list_list_list_free ( (coord_list_list_list_t *) coordset );
                break;
            case GEO_GEOMETRYCOLLECTION:
                fprintf ( stderr, "GeometryCollection gemoetry type not currently supported\n" );
                break;
            default:
                break;
        }

    return rc;
};

int geoJson_add_feature_to_list ( struct json_object *feature, feature_list_t *feature_list ) {
    int rc = EXIT_FAILURE;
    struct json_object *properties = NULL, *name = NULL, *geometry = NULL, *type = NULL;
    char **excluded_properties = (char *[]){ "id", "timestamp", "user", "uid", "changeset", "source", "version" };
    size_t excluded_properties_len = 7, i = 0;
    
    if ( ( properties = json_object_object_get ( feature, "properties" ) ) != NULL && json_object_is_type ( properties, json_type_object ) ) {
        if ( ( name = json_object_object_get ( properties, "name" ) ) != NULL && json_object_is_type ( name, json_type_string ) ) {
            feature_t *curr_feat;

            feature_list->len++;

            if ( ( feature_list->features = realloc ( feature_list->features, feature_list->len * sizeof ( feature_t ) ) ) == NULL )
                goto over;

            curr_feat = &feature_list->features[feature_list->len - 1];

            memset ( curr_feat, 0, sizeof ( feature_t ) );

            {
                json_object_object_foreach ( properties, key, val ) {
                    {
                        char skip = 0;

                        for ( i = 0; i < excluded_properties_len; i++ )
                            if ( ! strncmp ( key, excluded_properties[i], strlen ( key ) ) ) {
                                skip = 1;
                                break;
                            }

                        if ( ! skip ) {
                            curr_feat->property_len++;

                            if ( ( curr_feat->properties = realloc ( curr_feat->properties, curr_feat->property_len * sizeof ( property_t ) ) ) == NULL )
                                goto over;

                            curr_feat->properties[curr_feat->property_len - 1].key = strndup ( key, strlen ( key ) );
                            curr_feat->properties[curr_feat->property_len - 1].val = strndup ( json_object_get_string ( val ), json_object_get_string_len ( val ) );
                        }
                    }
                }
            }

            if ( ( geometry = json_object_object_get ( feature, "geometry" ) ) != NULL && json_object_is_type ( geometry, json_type_object ) ) {
                if ( ( type = json_object_object_get ( geometry, "type" ) ) != NULL && json_object_is_type ( type, json_type_string ) ) {
                    if ( ! strncmp ( json_object_get_string ( type ), "Point", 5 ) ) {
                        curr_feat->geometry_type = GEO_POINT;
                    } else if ( ! strncmp ( json_object_get_string ( type ), "MultiPoint", 10 ) ) {
                        curr_feat->geometry_type = GEO_MULTIPOINT;
                    } else if ( ! strncmp ( json_object_get_string ( type ), "LineString", 10 ) ) {
                        curr_feat->geometry_type = GEO_LINESTRING;
                    } else if ( ! strncmp ( json_object_get_string ( type ), "MultiLineString", 10 ) ) {
                        curr_feat->geometry_type = GEO_MULTILINESTRING;
                    } else if ( ! strncmp ( json_object_get_string ( type ), "Polygon", 10 ) ) {
                        curr_feat->geometry_type = GEO_POLYGON;
                    } else if ( ! strncmp ( json_object_get_string ( type ), "MultiPolygon", 10 ) ) {
                        curr_feat->geometry_type = GEO_MULTIPOLYGON;
                    } else if ( ! strncmp ( json_object_get_string ( type ), "GeometryCollection", 10 ) ) {
                        fprintf ( stderr, "GeometryCollection gemoetry type not currently supported\n" );
                        goto over;
                    } else {
                        fprintf ( stderr, "Unknown geometry type: %s\n", json_object_get_string ( type ) );
                        goto over;
                    }

                    if ( ( curr_feat->coordset = geoJson_get_coordinates ( curr_feat->geometry_type, geometry ) ) == NULL )
                        goto over;
                }
            }
        }
    }

    rc = EXIT_SUCCESS;
over:
    return rc;
};

feature_list_t *geoJson_feature_list_from_json ( struct json_object *json_o ) {
    feature_list_t *rc = NULL, *feature_list = NULL;
    struct json_object *features = NULL;
    size_t i = 0, features_len = 0;

    if ( ( feature_list = calloc ( 1, sizeof ( feature_list_t ) ) ) == NULL )
        goto over;

    if ( ( features = json_object_object_get ( json_o, "features" ) ) == NULL || ! json_object_is_type ( features, json_type_array ) )
        goto over;

    features_len = (size_t) json_object_array_length ( features );

    for ( i = 0; i < features_len; i++ ) {
        struct json_object *feature = NULL;
        
        if ( ( feature = json_object_array_get_idx ( features, i ) ) == NULL )
            goto over;

        if ( geoJson_add_feature_to_list ( feature, feature_list ) == EXIT_FAILURE )
            goto over;
    }

    rc = feature_list;
over:
    if ( rc == NULL && feature_list != NULL )
        geoJson_feature_list_free ( feature_list );

    return rc;
}

struct json_object *geoJson_coord_to_json ( coord_t *coords ) {
    struct json_object *rc = NULL, *json_coords = NULL;

    if ( ( json_coords = json_object_new_array () ) == NULL )
        goto over;

    if ( json_object_array_add ( json_coords, json_object_new_double ( coords->lat ) ) != 0 )
        goto over;

    if ( json_object_array_add ( json_coords, json_object_new_double ( coords->lon ) ) != 0 )
        goto over;

    rc = json_coords;
over:
    if ( rc == NULL && json_coords != NULL )
        geoJson_json_free ( json_coords );

    return rc;
};

struct json_object *geoJson_coord_list_to_json ( coord_list_t *coord_list ) {
    struct json_object *rc = NULL, *json_coord_list = NULL;
    size_t i = 0;

    if ( ( json_coord_list = json_object_new_array () ) == NULL )
        goto over;

    for ( i = 0; i < coord_list->len; i++ ) {
        struct json_object *json_coords = NULL;

        if ( ( json_coords = geoJson_coord_to_json ( coord_list->coords[i] ) ) == NULL )
            goto over;

        if ( json_object_array_add ( json_coord_list, json_coords ) != 0 )
            goto over;
    }

    rc = json_coord_list;
over:
    if ( rc == NULL && json_coord_list != NULL )
        geoJson_json_free ( json_coord_list );

    return rc;
};

struct json_object *geoJson_coord_list_list_to_json ( coord_list_list_t *coord_list_list ) {
    struct json_object *rc = NULL, *json_coord_list_list = NULL;
    size_t i = 0;

    if ( ( json_coord_list_list = json_object_new_array () ) == NULL )
        goto over;

    for ( i = 0; i < coord_list_list->len; i++ ) {
        struct json_object *json_coord_list = NULL;

        if ( ( json_coord_list = geoJson_coord_list_to_json ( coord_list_list->coord_lists[i] ) ) == NULL )
            goto over;

        if ( json_object_array_add ( json_coord_list_list, json_coord_list ) != 0 )
            goto over;
    }

    rc = json_coord_list_list;
over:
    if ( rc == NULL && json_coord_list_list != NULL )
        geoJson_json_free ( json_coord_list_list );

    return rc;
};

struct json_object *geoJson_coord_list_list_list_to_json ( coord_list_list_list_t *coord_list_list_list ) {
    struct json_object *rc = NULL, *json_coord_list_list_list = NULL;
    size_t i = 0;

    if ( ( json_coord_list_list_list = json_object_new_array () ) == NULL )
        goto over;

    for ( i = 0; i < coord_list_list_list->len; i++ ) {
        struct json_object *json_coord_list_list = NULL;

        if ( ( json_coord_list_list = geoJson_coord_list_list_to_json ( coord_list_list_list->coord_list_lists[i] ) ) == NULL )
            goto over;

        if ( json_object_array_add ( json_coord_list_list_list, json_coord_list_list ) != 0 )
            goto over;
    }

    rc = json_coord_list_list_list;
over:
    if ( rc == NULL && json_coord_list_list_list != NULL )
        geoJson_json_free ( json_coord_list_list_list );

    return rc;
};

struct json_object *geoJson_geometry_from_feature ( feature_t feature ) {
    struct json_object *rc = NULL, *geometry = NULL;
    struct json_object *coordinates = NULL;

    if ( ( geometry = json_object_new_object () ) == NULL )
        goto over;

    switch ( feature.geometry_type ) {
        case GEO_POINT:
            if ( json_object_object_add ( geometry, "type", json_object_new_string ( "Point" ) ) != 0 )
                goto over;

            if ( ( coordinates = geoJson_coord_to_json ( (coord_t *) feature.coordset ) ) == NULL )
                goto over;

            break;
        case GEO_MULTIPOINT:
            if ( json_object_object_add ( geometry, "type", json_object_new_string ( "MultiPoint" ) ) != 0 )
                goto over;

            if ( ( coordinates = geoJson_coord_list_to_json ( (coord_list_t *) feature.coordset ) ) == NULL )
                goto over;

            break;
        case GEO_LINESTRING:
            if ( json_object_object_add ( geometry, "type", json_object_new_string ( "LineString" ) ) != 0 )
                goto over;

            if ( ( coordinates = geoJson_coord_list_to_json ( (coord_list_t *) feature.coordset ) ) == NULL )
                goto over;

            break;
        case GEO_MULTILINESTRING:
            if ( json_object_object_add ( geometry, "type", json_object_new_string ( "MultiLineString" ) ) != 0 )
                goto over;

            if ( ( coordinates = geoJson_coord_list_list_to_json ( (coord_list_list_t *) feature.coordset ) ) == NULL )
                goto over;

            break;
        case GEO_POLYGON:
            if ( json_object_object_add ( geometry, "type", json_object_new_string ( "Polygon" ) ) != 0 )
                goto over;

            if ( ( coordinates = geoJson_coord_list_list_to_json ( (coord_list_list_t *) feature.coordset ) ) == NULL )
                goto over;

            break;
        case GEO_MULTIPOLYGON:
            if ( json_object_object_add ( geometry, "type", json_object_new_string ( "MultiPolygon" ) ) != 0 )
                goto over;

            if ( ( coordinates = geoJson_coord_list_list_list_to_json ( (coord_list_list_list_t *) feature.coordset ) ) == NULL )
                goto over;

            break;
        case GEO_GEOMETRYCOLLECTION:
            if ( json_object_object_add ( geometry, "type", json_object_new_string ( "GeometryCollection" ) ) != 0 )
                goto over;
        default:
            fprintf ( stderr, "Unkown geometry type: %d\n", feature.geometry_type );
            goto over;
    }

    if ( json_object_object_add ( geometry, "coordinates", coordinates ) != 0 )
        goto over;

    rc = geometry;
over:
    if ( rc == NULL && geometry != NULL )
        geoJson_json_free ( geometry );

    return rc;
};

int geoJson_add_feature_to_array ( feature_t feature, struct json_object *features ) {
    int rc = EXIT_FAILURE;
    struct json_object *json_feature = NULL, *json_properties = NULL, *json_geometry = NULL;
    size_t i = 0;

    if ( ( json_feature = json_object_new_object () ) == NULL )
        goto over;

    if ( json_object_object_add ( json_feature, "type", json_object_new_string ( "Feature" ) ) != 0 )
        goto over;

    if ( ( json_properties = json_object_new_object () ) == NULL )
        goto over;

    for ( i = 0; i < feature.property_len; i++ )
        if ( json_object_object_add ( json_properties, feature.properties[i].key, json_object_new_string ( feature.properties[i].val ) ) != 0 )
            goto over;

    if ( json_object_object_add ( json_feature, "properties", json_properties ) != 0 )
        goto over;

    if ( ( json_geometry = geoJson_geometry_from_feature ( feature ) ) == NULL )
        goto over;

    if ( json_object_object_add ( json_feature, "geometry", json_geometry ) != 0 )
        goto over;

    if ( json_object_array_add ( features, json_feature ) != 0 )
        goto over;

    rc = EXIT_SUCCESS;
over:
    if ( rc != EXIT_SUCCESS && json_feature != NULL )
        geoJson_json_free ( json_feature );

    return rc;
};

struct json_object *geoJson_feature_list_to_json_array ( feature_list_t *feature_list ) {
    struct json_object *rc = NULL, *features = NULL;
    size_t i = 0;

    if ( ( features = json_object_new_array () ) == NULL )
        goto over;

    for ( i = 0; i < feature_list->len; i++ )
        if ( geoJson_add_feature_to_array ( feature_list->features[i], features ) != EXIT_SUCCESS )
            goto over;

    rc = features;
over:
    if ( rc == NULL && features != NULL )
        geoJson_json_free ( features );

    return rc;
};

struct json_object *geoJson_feature_list_to_json ( feature_list_t *feature_list ) {
    struct json_object *rc = NULL, *json_o = NULL;
    struct json_object *features = NULL;

    if ( ( json_o = json_object_new_object () ) == NULL )
        goto over;

    if ( json_object_object_add ( json_o, "type", json_object_new_string ( "FeatureCollection" ) ) != 0 )
        goto over;

    if ( ( features = geoJson_feature_list_to_json_array ( feature_list ) ) == NULL )
        goto over;

    if ( json_object_object_add ( json_o, "features", features ) != 0 )
        goto over;

    rc = json_o;
over:
    if ( rc == NULL && json_o != NULL )
        geoJson_json_free ( json_o );

    return rc;
}

char *geoJson_string_from_file ( const char *filename ) {
    char *rc = NULL;
    char *json_buffer = NULL;
    size_t json_buffer_size = 0;
    FILE *fp = NULL;

    if ( ( fp = fopen ( filename, "r" ) ) == NULL )
        goto over;

    while ( ! feof ( fp ) ) {
        if ( ( json_buffer = realloc ( json_buffer, json_buffer_size + 4096 ) ) == NULL )
            goto over;

        memset ( json_buffer + json_buffer_size, 0, 4096 );

        if ( fread ( json_buffer + json_buffer_size, 1, 4096, fp ) < 4096 && ! feof ( fp ) )
            goto over;

        json_buffer_size += 4096;
    }

    if ( json_buffer[json_buffer_size - 1] != '\0' ) {
        if ( ( json_buffer = realloc ( json_buffer, json_buffer_size + 1 ) ) == NULL )
            goto over;

        json_buffer[json_buffer_size] = '\0';
        json_buffer_size++;
    }

    rc = json_buffer;
over:
    if ( rc == NULL && json_buffer != NULL )
        free ( json_buffer );

    if ( fp )
        fclose ( fp );

    return rc;
};

int geoJson_string_to_file ( const char *string, const char *filename ) {
    int rc = EXIT_FAILURE;
    FILE *fp = NULL;

    if ( ( fp = fopen ( filename, "w" ) ) == NULL )
        goto over;

    if ( fwrite ( string, 1, strlen ( string ), fp ) < strlen ( string ) )
        goto over;

    rc = EXIT_SUCCESS;
over:
    if ( fp )
        fclose ( fp );

    return rc;
};

struct json_object *geoJson_json_from_file ( const char *filename ) {
    char *json_string = NULL;
    struct json_object *rc = NULL, *json_o = NULL;

    if ( ( json_string = geoJson_string_from_file ( filename ) ) == NULL )
        goto over;

    if ( ( json_o = json_tokener_parse ( json_string ) ) == NULL )
        goto over;

    rc = json_o;
over:
    if ( rc == NULL && json_o != NULL )
        geoJson_json_free ( json_o );

    if ( json_string )
        free ( json_string );

    return rc;
};

int geoJson_json_to_file ( struct json_object *json_o, const char *filename ) {
    int rc = EXIT_FAILURE;
    const char *json_buffer = json_object_to_json_string ( json_o );

    if ( geoJson_string_to_file ( json_buffer, filename ) == EXIT_FAILURE )
        goto over;

    rc = EXIT_SUCCESS;
over:
    return rc;
};

void geoJson_json_free ( struct json_object *json_o ) {
    json_object_put ( json_o );
};

void geoJson_feature_list_free ( feature_list_t *feature_list ) {
    size_t i = 0, j = 0;

    if ( feature_list ) {
        if ( feature_list->features ) {
            for ( i = 0; i < feature_list->len; i++ ) {
                for ( j = 0; j < feature_list->features[i].property_len; j++ ) {
                    free ( feature_list->features[i].properties[j].key );
                    free ( feature_list->features[i].properties[j].val );
                }
                free ( feature_list->features[i].properties );
            }
            free ( feature_list->features );
        }
        free ( feature_list );
    }
}
