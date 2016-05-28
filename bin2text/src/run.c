/* run.c
 *
 * Defines the main entry point to the routines in bin2text
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

#include <math.h>
#include <stdio.h>
#include <float.h>
#include "run.h"
#include "string.h"
#include <stdint.h>
#include <stdlib.h>
#include <math.h>
#include <assert.h>

#include <stdlib.h>
#include <stdio.h>

#include "tuple.h"
#include "var.h"

#include "resource/strings.h"


// read and store header information
int32_t get_header(FILE     * const __restrict fbin,
                   uint32_t * const __restrict d,
                   uint32_t * const __restrict n_vars,
                   uint64_t * const __restrict n_tuples,
                   double   * const __restrict average)
{
  size_t err;
  err = fread(d, 1, sizeof(int32_t), fbin);
  if (!err) {
    fprintf(stderr, "Error: couldn't read dummy 0 from file\n");
    return -1;
  }
  err = fread(d, 1, sizeof(int32_t), fbin);
  if (!err) {
    fprintf(stderr, "Error: couldn't read dimension from file\n");
    return -1;
  }
  err = fread(n_vars, 1, sizeof(int32_t), fbin);
  if (!err) {
    fprintf(stderr, "Error: couldn't read n_vars from file\n");
    return -1;
  }
  err = fread(n_tuples, 1, sizeof(uint64_t), fbin);
  if (!err) {
    fprintf(stderr, "Error: couldn't read n_tuples from file\n");
    return -1;
  }
  err = fread(average, 1, sizeof(double), fbin);
  if (!err) {
    fprintf(stderr, "Error: couldn't read average from file\n");
    return -1;
  }
  return 0;
}

void check_binary_files(FILE          ** const __restrict fbin1,
                        FILE          ** const __restrict fbin2,
                        const option_t * const __restrict opt,
                        uint32_t       * const __restrict d,
                        uint32_t       * const __restrict n_vars,
                        uint64_t       * const __restrict n_tuples,
                        double         * const __restrict average)
{
  // open output file, get dimension and other information from header
  *fbin1 = open_file_exit_if_error(opt->in_file1);
  *fbin2 = open_file_exit_if_error(opt->in_file2);

  exit_if_empty_file(*fbin1, "Error: empty binary file 1  \n");
  exit_if_empty_file(*fbin2, "Error: empty binary file 2  \n");

  fseek(*fbin1, 0L, SEEK_END);
  size_t sz = ftell(*fbin1);
  rewind(*fbin1);
  if(sz != 28)
  {
    fprintf(stderr, "Error: ill-form header\n");
    exit(-1);
  }

  get_header(*fbin1, d, n_vars, n_tuples, average);
  close_file_exit_if_error(*fbin1, "Error closing binary file 1.\n");
}

// main function 
int32_t run(option_t *opt) 
{
  FILE* fbin1;
  FILE* fbin2;
  double average = 0.0;
  double std = 0.0;
  double cutoff;
  uint32_t d, n_vars;
  double sign=1;
  uint64_t n_tuples, c;

  check_binary_files(&fbin1, &fbin2, opt, &d, &n_vars, &n_tuples, &average);
  const struct output_files output_files = open_and_check_output_files(opt);

  sign = pow(-1,d);
  fprintf(stderr, "Dimension:\t\t%d\n",d);
  fprintf(stderr, "Number of Variables:\t%d\n",n_vars);
  fprintf(stderr, "Number of Tuples:\t%ld\n",n_tuples);
  fprintf(stderr, "Average:\t\t%.10f\n",average);

  if(opt->n !=0 || opt->k != 0 || opt->s_option)
  {
    init_lookup_tuple(n_tuples, d);

    fill_tuples(fbin2, d,
                n_tuples);

    close_file_exit_if_error(fbin2, "Error closing binary file 1.\n");

    bool sorted_tuple = false;
    if(opt->k != 0 || opt->n != 0)
    {
      sort_tuples_inplace(lookup_tuple, n_tuples);
      sorted_tuple = true;
    }

    if (opt->n > 0) {
      write_n_tuples_hi(output_files.out1, lookup_tuple, opt->n, d, n_tuples);
      close_file_exit_if_error(output_files.out1, "Error: closing out1.txt failed");
      write_n_tuples_lo(output_files.out2,lookup_tuple, opt->n, d, n_tuples);
      close_file_exit_if_error(output_files.out2, "Error: closing out2.txt failed");
    }

    if(opt->k != 0)
    {
      init_lookup_var(n_vars, opt->k);
      fill_vars(n_tuples, d, opt->k, n_vars);
      write_var_avg_and_participation(output_files.out3,n_vars, d, opt->k);
      close_file_exit_if_error(output_files.out3, "Error: closing out3.txt failed");
      delete_lookup_var();
    }

    if(opt->b != 0)
    {
      write_scoring_histogram(output_files.out4,
                              n_tuples,
                              opt->b,
                              lookup_tuple[0].avg,
                              lookup_tuple[n_tuples-1].avg);
      close_file_exit_if_error(output_files.out4, "Error: closing out4.txt failed");
    }

    // if -s is passed, pass through the file once to calculate std, and again to create out5.txt
    if (opt->s_option) 
    {
      std = calculate_std_bin(average, d, n_tuples, sorted_tuple);
      cutoff = average + sign*std*opt->s_std;
      fprintf(stderr, "St. Dev.:\t\t%.10f\n",std);
      fprintf(stderr, "Cutoff:\t\t\t%.10f\n",cutoff);

      // count and print tuples above cutoff
      c = count_tuples_bin_cutoff(output_files.out5,d, cutoff, n_tuples, average, std, sorted_tuple);
      fprintf(stderr, "Tuples Above Cutoff:\t%ld\t(%.2f%%)\n", c, 100.0*c/n_tuples);
      close_file_exit_if_error(output_files.out5, "Error: closing out5.txt failed");
    }

    delete_lookup_tuple();
  }
  else if (fclose(fbin2) != 0) {
    fprintf(stderr, "Error closing binary file 2.\n");
    return -1;
  }

  return 0;
}

