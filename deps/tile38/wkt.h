/*
 * Copyright (c) 2016, Josh Baker <joshbaker77@gmail.com>.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *  * Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *  * Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *  * Neither the name of Redis nor the names of its contributors may be used
 *    to endorse or promote products derived from this software without
 *    specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef WKT_H_
#define WKT_H_

#include <stddef.h>
#include <stdint.h>
#include <stdint.h>

#if defined(__cplusplus)
extern "C" {
#endif

struct wktGeometry;
typedef struct wktGeometry wktGeometry;

typedef enum {
    WKT_UNKNOWN            = 0,
    WKT_POINT              = 1,
    WKT_LINESTRING         = 2,
    WKT_POLYGON            = 3,
    WKT_MULTIPOINT         = 4,
    WKT_MULTILINESTRING    = 5,
    WKT_MULTIPOLYGON       = 6,
    WKT_GEOMETRYCOLLECTION = 7,
} wktType;

typedef enum {
    WKT_ERRNONE  =  0, // No error
    WKT_ERRUNKN  = -1, // Unknown error
    WKT_ERRMEM   = -2, // Out of memory
    WKT_ERRINPUT = -3, // Invalid input
} wktErr;

typedef struct {
    double x, y;
} wktPoint;

typedef struct {
    wktPoint *points;
    int size;
} wktLineString, wktMultiPoint;

typedef struct {
    wktLineString *lineStrings;
    int size;
} wktPolygon, wktMultiLineString;

typedef struct {
    wktPolygon *polygons;
    int size;
} wktMultiPolygon;


typedef struct {
    wktGeometry **geometries;
    int size;
} wktGeometryCollection;


// wktGeometry is a simple geometry type + 16 byte union. This should cover
// all of the basic WKT types and keep the memory footprint decently low.
struct wktGeometry{
    wktType type;
    union {
        wktPoint point;
        wktLineString lineString;
        wktMultiPoint multiPoint;
        wktPolygon polygon;
        wktMultiLineString multiLineString;
        wktMultiPolygon multiPolygon;
        wktGeometryCollection geometryCollection;
    } v;
};

// wktParse parses the input and fills the wkt object. wktFree must be called
// to release the wkt from memory.
wktErr wktParse(const char *input, wktGeometry **wkt);

// wtkFree releases a wktGeometry object.
void wktFree(wktGeometry *wkt);

// wktErrText returns a string representation of the error.
const char *wktErrText(wktErr err);

// wtkText returns a string represention of the geometry. The returned string
// must be freed when it's no longer needed.
char *wktText(wktGeometry *wkt);


#if defined(__cplusplus)
}
#endif
#endif /* WKT_H_ */
