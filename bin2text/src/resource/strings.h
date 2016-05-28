/* strings.h
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

#ifndef BIN2TEXT_RESOURCE_STRINGS_H
#define BIN2TEXT_RESOURCE_STRINGS_H

#include "../run.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/stat.h>

struct output_filenames
{
    const char * const out1_name;
    const char * const out2_name;
    const char * const out3_name;
    const char * const out4_name;
    const char * const out5_name; 
};

struct output_files
{
    FILE * out1;
    FILE * out2;
    FILE * out3;
    FILE * out4;
    FILE * out5; 
};

extern const struct output_filenames output_filenames;

/*
 * Open files needed for outputing 
 */
struct output_files open_and_check_output_files(const option_t * opt);

FILE * create_file_if_not_exists(const char * const __restrict filename);
FILE * open_file_exit_if_error(const char* file);
void close_file_exit_if_error(FILE* file, const char* message);
void exit_if_empty_file(FILE * file, const char * message);


#endif // BIN2TEXT_RESOURCE_STRINGS_H

