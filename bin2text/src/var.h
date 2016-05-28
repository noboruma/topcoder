/* var.h
 *
 * Defines the variable data structure and associated routines
 * used in bin2text.
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


#ifndef VAR_H__
#define VAR_H__

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <float.h>
#include <string.h>

#include "tuple.h"

struct var
{
  size_t count;
  double avg;
  size_t *tuple_indexes_max;
  size_t size_tuple_max;
  size_t *tuple_indexes_min;
  size_t size_tuple_min;
};

typedef int32_t var_t;

extern struct var *lookup_var;
/*
 * Initialize lookup variable table containing tuples' variable information
 */
void init_lookup_var(const size_t nvar,
                     const size_t k);

/*
 * Fill information of variables according to the tuples where they are defined
 */
void fill_vars(const uint64_t n_tuples,
               const size_t d,
               const size_t k,
               const size_t nvar);
/*
 * Free spaces
 */
void delete_lookup_var();

/*
 * Output a list of variable containing for each variable its own
 * average as well as the list of tuples where the variable is
 */
void write_var_avg_and_participation(FILE* out3,
                                     const size_t nvar,
                                     const size_t d,
                                     const size_t k);

#endif
