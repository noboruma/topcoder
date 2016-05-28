/* run.h
 *
 * defines entry point for main routines used in bin2text
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

// Paul Draghicescu
// pdraghicescu@pnri.org
//

#ifndef RUN_H 
#define RUN_H

#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#define TUP_BUF_SIZE 10000

typedef struct option {
    uint32_t n, k;
    double b, s_std;
    int32_t s_option;
    char in_file1[50];
    char in_file2[50];
} option_t;

int32_t get_header(FILE     * const __restrict fbin,
                   uint32_t * const __restrict d,
                   uint32_t * const __restrict n_vars,
                   uint64_t * const __restrict n_tuples,
                   double   * const __restrict average);

int32_t run(option_t *opt);

#endif

