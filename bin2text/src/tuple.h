/* tuple.h
 *
 * Defines the tuple data structure and associated routines.
 *
 * MIT License
 * 
 * Copyright (c) 2016 Thomas Legris, Edmund Higham
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
**/


#ifndef TUPLE_H__
#define TUPLE_H__

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <float.h>
#include <string.h>
#include <assert.h>
#include <stdbool.h>
#include <math.h>

typedef int32_t var_t;

//__attribute__((packed)) //TODO: to be considered
struct tuple
{
  __attribute__((aligned(16)))
  var_t *values;
  double avg;
};

extern struct tuple *lookup_tuple;

/*
 * Initialize the global lookup table cotnaining tuples' information
 */
void init_lookup_tuple(const uint64_t ntuples,
                       const size_t d);

/*
 * Read a binary file and fill the a lookup table of tuples
 */
void fill_tuples(FILE * const __restrict fbin,
                 const size_t d,
                 const uint64_t n_tuples);

/*
 * Write a tuple entry to file (tuple values + score average)
 */
void write_tuple_to_file(FILE * const __restrict file,
                         const struct tuple * const __restrict _tuple,
                         const size_t d);
/*
 * Sort tuples in place
 */
void sort_tuples_inplace(struct tuple * arr,
                         const uint64_t n_tuples);
/*
 * Free memory
 */
void delete_lookup_tuple();



uint64_t count_tuples_bin_cutoff(FILE * const __restrict out5,
                                 size_t d,
                                 double cutoff,
                                 uint64_t n_tuples,
                                 double average,
                                 double std,
                                 bool lookup_tuple_sorted);

// calculate standard deviation, min and max delta
double calculate_std_bin(double average,
                         uint32_t d,
                         const uint64_t n_tuples,
                         bool lookup_tuple_sorted);

void write_n_tuples_hi(FILE * const __restrict out1,
                       const struct tuple * const __restrict tuples,
                       uint32_t n,
                       const uint32_t d,
                       const uint64_t n_tuples);


void write_n_tuples_lo(FILE * const __restrict out2,
                       const struct tuple * const __restrict tuples,
                       uint32_t n,
                       const uint32_t d,
                       const uint64_t n_tuples);

void write_scoring_histogram(FILE * const __restrict out4,
                             const uint64_t ntuples,
                             const double b, 
                             const double min,
                             const double max);


#endif

