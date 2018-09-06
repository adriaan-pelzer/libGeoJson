#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <geoJson.h>

int main ( int argc, char **argv ) {
    int rc = EXIT_FAILURE;
    char *input_filename, *output_filename, *features_filename;
    struct json_object *json_o = NULL, *json_features = NULL;
    feature_list_t *feature_list = NULL;
    //size_t i = 0, j = 0;

    if ( argc < 4 ) {
        fprintf ( stderr, "Usage: %s infile outfile features_file\n", argv[0] );
        goto over;
    }

    input_filename = argv[1];
    output_filename = argv[2];
    features_filename = argv[3];

    if ( ( json_o = geoJson_json_from_file ( input_filename ) ) == NULL ) {
        fprintf ( stderr, "Cannot get json object from file %s: %s\n", input_filename, strerror ( errno ) );
        goto over;
    }

    if ( geoJson_json_to_file ( json_o, output_filename ) != EXIT_SUCCESS ) {
        fprintf ( stderr, "Cannot write json object to file %s: %s\n", output_filename, strerror ( errno ) );
        goto over;
    }

    if ( ( feature_list = geoJson_feature_list_from_json ( json_o ) ) == NULL ) {
        fprintf ( stderr, "Cannot get feature list from json: %s\n", strerror ( errno ) );
        goto over;
    }

    if ( ( json_features = geoJson_feature_list_to_json ( feature_list ) ) == NULL ) {
        fprintf ( stderr, "Cannot get json from feature list: %s\n", strerror ( errno ) );
        goto over;
    }

    if ( geoJson_json_to_file ( json_features, features_filename ) != EXIT_SUCCESS ) {
        fprintf ( stderr, "Cannot json json object to file %s: %s\n", features_filename, strerror ( errno ) );
        goto over;
    }

    /*for ( i = 0; i < feature_list->len; i++ ) {
        printf ( "----------------------------\n" );
        for ( j = 0; j < feature_list->features[i].property_len; j++ )
            printf ( "%s: %s\n", feature_list->features[i].properties[j].key, feature_list->features[i].properties[j].val );
    }*/

    rc = EXIT_SUCCESS;
over:
    if ( json_o )
        geoJson_json_free ( json_o );

    if ( feature_list )
        geoJson_feature_list_free ( feature_list );

    return rc;
};

