#ifndef WHEELSON_FIRMWARE_SKELETONTRACER_H
#define WHEELSON_FIRMWARE_SKELETONTRACER_H

/*
 SkeletonTracer.h
 Trace skeletonization result into polylines

MIT License

Copyright (c) 2020 Lingdong Huang

Permission is hereby granted, free of charge, to any person obtaining a copy
		of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
		to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
		copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
		copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
		AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.

 Rewritten as a C++ class by CircuitMess, 2021.

 */

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <math.h>
#include <string>
#include <climits>
#include <sstream>

//================================
// ENUMS
//================================
#define HORIZONTAL 1
#define VERTICAL 2

//================================
// PARAMS
//================================
#define CHUNK_SIZE 5           // the chunk size
//#define SAVE_RECTS 1            // additionally save bounding rects of chunks (for visualization)
#define MAX_ITER 999            // maximum number of iterations



//================================
// DATASTRUCTURES
//================================

struct point_t {
	int x;
	int y;
	struct point_t *next;
};

struct polyline_t {
	point_t *head;
	point_t *tail;
	struct polyline_t *prev;
	struct polyline_t *next;
	int size;
};


struct rect_t {
	int x;
	int y;
	int w;
	int h;
	struct rect_t *next;
};

struct rects_t {
	rect_t *head;
	rect_t *tail;
};


class SkeletonTracer {
public:
	//================================
	// GLOBALS
	//================================
	uint16_t *im; // the image
	int W;     // width
	int H;     // height

	SkeletonTracer();
	~SkeletonTracer();

	//================================
	// RASTER SKELETONIZATION
	//================================
	// Binary image thinning (skeletonization) in-place.
	// Implements Zhang-Suen algorithm.
	// http://agcggs680.pbworks.com/f/Zhan-Suen_algorithm.pdf
	void thinning_zs();

	/**Trace skeleton from thinning result.
	 * Algorithm:
	 * 1. if chunk size is small enough, reach recursive bottom and turn it into segments
	 * 2. attempt to split the chunk into 2 smaller chunks, either horizontall or vertically;
	 *    find the best "seam" to carve along, and avoid possible degenerate cases
	 * 3. recurse on each chunk, and merge their segments
	 *
	 * @param x       left of   chunk
	 * @param y       top of    chunk
	 * @param w       width of  chunk
	 * @param h       height of chunk
	 * @param iter    current iteration
	 * @return        an array of polylines
	*/
	polyline_t *trace_skeleton(int x, int y, int w, int h, int iter);
	void destroy_polylines(polyline_t *q);


private:
	rects_t rects;
	//================================
	// DATASTRUCTURE IMPLEMENTATION
	//================================

	polyline_t *new_polyline();

	std::string print_polyline(polyline_t *q);

	std::string print_polylines(polyline_t *q);


	void reverse_polyline(polyline_t *q);

	void cat_tail_polyline(polyline_t *q0, polyline_t *q1);

	void cat_head_polyline(polyline_t *q0, polyline_t *q1);

	void add_point_to_polyline(polyline_t *q, int x, int y);

	polyline_t *prepend_polyline(polyline_t *q0, polyline_t *q1);


	std::string print_rects();


	void destroy_rects();

	void add_rect(int x, int y, int w, int h);


	bool thinning_zs_iteration(int iter);;


	//================================
	// MAIN ALGORITHM
	//================================

	// check if a region has any white pixel
	int not_empty(int x, int y, int w, int h);

	/**merge ith fragment of second chunk to first chunk
	 * @param c0   fragments from  first  chunk
	 * @param c1i  ith fragment of second chunk
	 * @param sx   (x or y) coordinate of the seam
	 * @param isv  is vertical, not horizontal?
	 * @param mode 2-bit flag,
	 *             MSB = is matching the left (not right) end of the fragment from first  chunk
	 *             LSB = is matching the right (not left) end of the fragment from second chunk
	 * @return     matching successful?
	 */
	int merge_impl(polyline_t *c0, polyline_t *c1i, int sx, int isv, int mode);

	/**merge fragments from two chunks
	 * @param c0   fragments from first  chunk
	 * @param c1   fragments from second chunk
	 * @param sx   (x or y) coordinate of the seam
	 * @param dr   merge direction, HORIZONTAL or VERTICAL?
	 */
	polyline_t *merge_frags(polyline_t *c0, polyline_t *c1, int sx, int dr);

	/**recursive bottom: turn chunk into polyline fragments;
	 * look around on 4 edges of the chunk, and identify the "outgoing" pixels;
	 * add segments connecting these pixels to center of chunk;
	 * apply heuristics to adjust center of chunk
	 *
	 * @param x    left of   chunk
	 * @param y    top of    chunk
	 * @param w    width of  chunk
	 * @param h    height of chunk
	 * @return     the polyline fragments
	 */
	polyline_t *chunk_to_frags(int x, int y, int w, int h);


	//================================
	// GUI/IO
	//================================
	void print_bitmap();
	uint16_t *trace(uint16_t *img, int w, int h);

};


#endif //WHEELSON_FIRMWARE_SKELETONTRACER_H
