CFLAGS=-g -Wall -fPIC -I/usr/local/include -I/usr/local/include/json-c
LIBS=-L/usr/local/lib -ljson-c

all: libGeoJson

test: testGeoJson
	./testGeoJson map.geojson dup.geojson features.geojson
	cat map.geojson | jq '.' > map.jq.geojson
	cat dup.geojson | jq '.' > dup.jq.geojson
	diff map.jq.geojson dup.jq.geojson
	rm dup.jq.geojson
	cat features.geojson | jq '.' > features.jq.geojson
	diff map.jq.geojson features.jq.geojson
	rm map.jq.geojson
	rm features.jq.geojson
	rm features.geojson
	rm dup.geojson

testGeoJson: Makefile geoJson.h testGeoJson.c
	gcc ${CFLAGS} -o testGeoJson testGeoJson.c ${LIBS} -lGeoJson

libGeoJson: Makefile geoJson.o geoJson.h
	gcc -shared -o libGeoJson.so.1.0 geoJson.o ${LIBS}

geoJson.o: Makefile geoJson.h geoJson.c
	gcc ${CFLAGS} -c geoJson.c -o geoJson.o

install:
	cp libGeoJson.so.1.0 /usr/local/lib
	ln -sf /usr/local/lib/libGeoJson.so.1.0 /usr/local/lib/libGeoJson.so.1
	ln -sf /usr/local/lib/libGeoJson.so.1.0 /usr/local/lib/libGeoJson.so
	ldconfig /usr/local/lib
	cp geoJson.h /usr/local/include/geoJson.h

clean:
	rm *.o; rm *.so*; rm core*; rm testGeoJson
