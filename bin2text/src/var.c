/* var.c
 *
 * Defines the variable data structure and associated routines implementation
 * used by bin2text
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



#include "var.h"

struct var *lookup_var = NULL;
size_t * max_tuple_indexes = NULL;
size_t * min_tuple_indexes = NULL;

void init_lookup_var(const size_t nvar,
                     const size_t k)
{
  lookup_var = (struct var*) calloc(sizeof(struct var), nvar);

  max_tuple_indexes = (size_t*) calloc(sizeof(size_t), k*nvar);
  min_tuple_indexes = (size_t*) calloc(sizeof(size_t), k*nvar);

  for(size_t i=0; i < nvar; ++i)
  {
    lookup_var[i].tuple_indexes_max = &max_tuple_indexes[i*k];
    lookup_var[i].tuple_indexes_min = &min_tuple_indexes[i*k];
  }
}

void delete_lookup_var()
{
  free(max_tuple_indexes);
  free(min_tuple_indexes);
  free(lookup_var);
}

void fill_vars(const uint64_t n_tuples,
               const size_t d,
               const size_t k,
               const size_t nvar)
{
  // Get count
  for(size_t i_tuple=0; i_tuple < n_tuples; ++i_tuple)
    for(size_t i=0; i < d; ++i)
    {
      var_t var_index = lookup_tuple[i_tuple].values[i];
      ++lookup_var[var_index].count;
    }
  // Compute AVG
  for(size_t i_tuple=0; i_tuple < n_tuples; ++i_tuple)
    for(size_t i=0; i < d; ++i)
    {
      var_t var_index = lookup_tuple[i_tuple].values[i];
      lookup_var[var_index].avg += lookup_tuple[i_tuple].avg/lookup_var[var_index].count; // TODO: overflow risk
    }

  size_t beg = 0, end = n_tuples-1;
  size_t nvar_to_be_processed = nvar * 2 *k;
  while (nvar_to_be_processed 
    && beg <= end)
  {
    for(size_t i=0; i < d; ++i)
    {
      var_t var_index = lookup_tuple[beg].values[i];
      if(lookup_var[var_index].size_tuple_min < k)
      {
        lookup_var[var_index].tuple_indexes_min[lookup_var[var_index].size_tuple_min++] = beg;
        --nvar_to_be_processed;
      }
    }

    for(size_t i=0; i < d; ++i)
    {
      var_t var_index = lookup_tuple[end].values[i];
      if(lookup_var[var_index].size_tuple_max < k)
      {
        lookup_var[var_index].tuple_indexes_max[lookup_var[var_index].size_tuple_max++] = end;
        --nvar_to_be_processed;
      }
    }
    ++beg;
    --end;
  }
}

void write_var_avg_and_participation(FILE* out3,
                                     const size_t nvar,
                                     const size_t d,
                                     const size_t k)
{

  for(var_t i=0; i < (var_t)nvar; ++i)
  {
    struct var * var = &lookup_var[i];
    //if(var->count == 0) continue;
    fprintf(out3, "%d\t%.10lf\n", i, var->avg);
    for(size_t tindex = 0; tindex < var->size_tuple_max; ++tindex)
      write_tuple_to_file(out3, &lookup_tuple[var->tuple_indexes_max[tindex]], d);

    if(var->size_tuple_max + var->size_tuple_min > 2*k)
    for(size_t tindex = 0; tindex < var->size_tuple_min; ++tindex)
      write_tuple_to_file(out3, &lookup_tuple[var->tuple_indexes_min[tindex]], d);
  }
}
