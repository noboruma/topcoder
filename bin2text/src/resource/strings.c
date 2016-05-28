/* strings.c
 *
 * Defines common strings and io subroutines used in bin2text
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

#include "strings.h"
#include <sys/stat.h>
#include <stdio.h>

#define blksize  65536

const struct output_filenames output_filenames =
{
    .out1_name = "out1.txt",
    .out2_name = "out2.txt",
    .out3_name = "out3.txt",
    .out4_name = "out4.txt",
    .out5_name = "out5.txt"
};

struct output_files open_and_check_output_files(const option_t * opt)
{
  FILE *f1=NULL, *f2=NULL, *f3=NULL, *f4=NULL, *f5=NULL;

  if(opt->n > 0)
  {
    f1 = create_file_if_not_exists(output_filenames.out1_name);
    setvbuf(f1, NULL, _IOFBF, blksize);

    f2 = create_file_if_not_exists(output_filenames.out2_name);
    setvbuf(f2, NULL, _IOFBF, blksize);
  }
  if(opt->k > 0)
  {
    f3 = create_file_if_not_exists(output_filenames.out3_name);
    setvbuf(f3, NULL, _IOFBF, blksize);
  }

  if(opt->b != 0)
  {
    f4 = create_file_if_not_exists(output_filenames.out4_name);
    setvbuf(f4, NULL, _IOFBF, blksize);
  }

  if(opt->s_option)
  {
    f5 = create_file_if_not_exists(output_filenames.out5_name);
    setvbuf(f5, NULL, _IOFBF, blksize);
  }

  struct output_files res = {f1,f2,f3,f4,f5};
  return res;
}
#undef blksize

FILE * create_file_if_not_exists(const char * const __restrict filename)
{
  FILE * file = fopen(filename,"wx");
  if (file != NULL) return file;

  fprintf(stderr, "Could not create file %s\n", filename);
  fprintf(stderr, "Please verify it does not already exist ");
  fprintf(stderr, "and/or you have the correct write permissions.\n");
  exit(-1);
}

FILE * open_file_exit_if_error(const char* file)
{
  FILE * res = fopen(file, "rb");
  if (!res) {
    fprintf(stderr, "Error: couldn't open header file \"%s\".\n",file);
    exit(-1);
  }
  return res;
}

void close_file_exit_if_error(FILE* file , const char* msg)
{
  if (fclose(file) != 0) {
    fprintf(stderr, msg);
    exit(-1);
  }
}

void exit_if_empty_file(FILE * file, const char * message)
{
  rewind(file);
  fseek(file, 0L, SEEK_END);
  if (!ftell(file)) {
    fprintf(stderr, message);
    exit(-1);
  }
  rewind(file);
}
