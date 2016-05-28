/* tuple.c
 *
 * Defines the tuple data structure and associated routines implementation
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


#include "tuple.h"

struct tuple *lookup_tuple = NULL;
var_t *tuple_values = NULL;

void init_lookup_tuple(const uint64_t ntuples,
                       const size_t d)
{
  lookup_tuple = (struct tuple*) calloc(sizeof(struct tuple), ntuples);
  tuple_values = (var_t*) calloc(sizeof(var_t), ntuples*d);

  for(size_t i=0; i < ntuples; ++i)
    lookup_tuple[i].values = &tuple_values[i*d];
}

void delete_lookup_tuple()
{
  free(lookup_tuple);
  free(tuple_values);
}

int32_t compare_tuples (const void * a, const void * b)
{
  double xx = ((struct tuple*)a)->avg;
  double yy = ((struct tuple*)b)->avg;
  return (xx > yy) - (xx < yy);
}

void sort_tuples_inplace(struct tuple * arr, const uint64_t n_tuples)
{
  // std
  qsort(arr, n_tuples, sizeof(struct tuple), compare_tuples);
}

union double_var_t
{
  var_t v[2];
  double d;
};

#define BUF_SIZE 4096
void fill_tuples(FILE *  const __restrict fbin,
                 const size_t d,
                 const uint64_t n_tuples)
{
  const size_t tuplesize = d + (sizeof(double)/sizeof(var_t));
  const size_t bufsize = BUF_SIZE * tuplesize;

  var_t buffer[bufsize];
  size_t n;
  size_t i_tuple = 0;
  while ((n = fread(buffer, sizeof(var_t), bufsize,  fbin)))
  {
    assert(i_tuple <= n_tuples);
    while(n)
    {
      n -= tuplesize;
      for(size_t i=0; i < d; ++i)
        lookup_tuple[i_tuple].values[i] = buffer[n+i];

      union double_var_t avg;
      avg.v[0] = buffer[n+d];
      avg.v[1] = buffer[n+d+1];
      lookup_tuple[i_tuple].avg = avg.d;

      ++i_tuple;
    }
  }
}
#undef BUF_SIZE

void write_tuple_to_file(FILE * const __restrict file,
                         const struct tuple * const __restrict _tuple,
                         const size_t d)
{
  const var_t * const value = _tuple->values;

  // There are minimum 2 values per tuple 
  fprintf(file, "%d\t%d\t", value[0], value[1]);

  // any others
  for(size_t i = 2; i < d; ++i)
    fprintf(file, "%d\t", value[i]);

  fprintf(file, "%.10lf\n", _tuple->avg);
}


#ifdef DELTA_PREC
#define PRINT_PRC_D "%.10Lf%c"
#else
#define PRINT_PRC_D "%.10lf%c"
#endif
uint64_t count_tuples_bin_cutoff(FILE * const __restrict fout5,
                                 size_t d,
                                 double cutoff,
                                 uint64_t n_tuples,
                                 double average,
                                 double std,
                                 bool lookup_tuple_sorted) 
{
  double next;
  uint64_t c = 0;
  if(lookup_tuple_sorted)
  {
    if(d%2)
    {
      size_t i_tuple = 0;
      while(i_tuple < n_tuples && ((next = lookup_tuple[i_tuple].avg) <= cutoff))
      {
        for (size_t i = 0; i < d; i++)
          fprintf(fout5,"%d\t",lookup_tuple[i_tuple].values[i]);
        fprintf(fout5, PRINT_PRC_D,next,'\t');
        fprintf(fout5, PRINT_PRC_D,fabs(next-average)/std, '\n');
        c++;
        ++i_tuple;
      }
    }
    else 
    {
      size_t i_tuple = n_tuples - 1;
      while(i_tuple < n_tuples  // unsigned, will never be lower than 0
            && ((next = lookup_tuple[i_tuple].avg) >= cutoff))
      {
        fprintf(fout5, "%d\t%d\t", lookup_tuple[i_tuple].values[0], lookup_tuple[i_tuple].values[1]);
        for (size_t i = 2; i < d; ++i)
          fprintf(fout5,"%d\t",lookup_tuple[i_tuple].values[i]);
        fprintf(fout5, PRINT_PRC_D,next,'\t');
        fprintf(fout5, PRINT_PRC_D,fabs(next-average)/std, '\n');
        c++;
        --i_tuple;
      }
    }
  }
  else
    for(size_t i_tuple=0; i_tuple < n_tuples; ++i_tuple)
    {
      next = lookup_tuple[i_tuple].avg;
      // if next score is in the tail
      if ((!(d%2) && (next >= cutoff)) || ((d%2) && (next <= cutoff))) 
      {
        fprintf(fout5, "%d\t%d\t", lookup_tuple[i_tuple].values[0], lookup_tuple[i_tuple].values[1]);
        for (size_t i = 2; i < d; ++i)
          fprintf(fout5,"%d\t",lookup_tuple[i_tuple].values[i]);
        fprintf(fout5, PRINT_PRC_D,next,'\t');
        fprintf(fout5, PRINT_PRC_D,fabs(next-average)/std, '\n');
        c++;
      }
    }
  return c;
}
#undef DELTA_PREC

// calculate standard deviation, min and max delta
double calculate_std_bin(double average,
                         uint32_t d,
                         const uint64_t n_tuples,
                         bool lookup_tuple_sorted) 
{
  double next;
  double sum = 0.0;
  double result;
  double min = DBL_MAX;
  double max = -DBL_MAX;
  // TODO: hard code types for different values of d
  if(!lookup_tuple_sorted)
  {
    for(size_t i_tuple=0; i_tuple < n_tuples; ++i_tuple)
    {
      next = lookup_tuple[i_tuple].avg;
      if (next < min)
        min = next;
      if (next > max)
        max = next;
      sum += pow(next-average, 2);
      //c++;
    }
  }
  else
  {
    for(size_t i_tuple=0; i_tuple < n_tuples; ++i_tuple)
    {
      next = lookup_tuple[i_tuple].avg;
      sum += pow(next-average, 2);
    }
    min = lookup_tuple[0].avg;
    max = lookup_tuple[n_tuples-1].avg;
  }
  fprintf(stderr, "Min Delta:\t\t");
  fprintf(stderr, PRINT_PRC_D,min, '\n');
  fprintf(stderr, "Max Delta:\t\t");
  fprintf(stderr, PRINT_PRC_D,max, '\n');

  result = sum/n_tuples;
  return sqrtl(result);
}

void write_n_tuples_lo(FILE * const __restrict file,
                       const struct tuple * const __restrict tuples,
                       uint32_t n,
                       const uint32_t d,
                       const uint64_t n_tuples)
{
  if ((uint64_t)n > n_tuples) n = (uint32_t)n_tuples;
  for (uint32_t i = 0; i < n; ++i)
      write_tuple_to_file(file, &tuples[i], d);
}

void write_n_tuples_hi(FILE * const __restrict file,
                       const struct tuple * const __restrict tuples,
                       uint32_t n,
                       const uint32_t d,
                       const uint64_t n_tuples)
{
  if ((uint64_t)n > n_tuples) n = (uint32_t)n_tuples;
  const uint64_t last_idx = n_tuples - (uint64_t)n;
  uint64_t idx = n_tuples;
  while (idx > last_idx)
      write_tuple_to_file(file, &tuples[--idx], d);
}

void write_scoring_histogram(FILE * const __restrict out4,
                             const uint64_t ntuples,
                             const double b, 
                             const double min,
                             const double max)
{
  assert(b != 0);

  double tmp_line_num = (double)floor((max-min)/b)+1;
  size_t line_num = (size_t) tmp_line_num;

#ifdef DELTA_PREC
  fprintf(out4, "%.10Lf\t%.10Lf\n", min, max);
#else
  fprintf(out4, "%.10lf\t%.10lf\n", min, max);
#endif
  size_t i_tuples=0;
  double max_bound = min;
  for(size_t i=0; i < line_num; ++i)
  {
    max_bound += b;
    uint64_t counter = 0;
    // ntuples
    while(i_tuples < ntuples && lookup_tuple[i_tuples].avg < max_bound)
    {
      if(fabs(max_bound - lookup_tuple[i_tuples].avg) < FLT_EPSILON)
        break;
      ++counter;
      ++i_tuples;
    }
    fprintf(out4, "%ld\n", counter);
  }
}
