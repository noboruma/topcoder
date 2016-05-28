#include <stdlib.h>
#include <stdio.h>

struct header
{
  int dummy;
  int d;
  int n_vars;
  unsigned long n_tups;
  double avg;
};

struct tuple
{
  int d[8];
  double avg;
};

void write_header(FILE* fhead, struct header *h)
{
  fwrite(&h->dummy, sizeof(int), 1, fhead);
  fwrite(&h->d, sizeof(int), 1, fhead);
  fwrite(&h->n_vars, sizeof(int), 1, fhead);
  fwrite(&h->n_tups, sizeof(unsigned long), 1, fhead);
  fwrite(&h->avg, sizeof(double), 1, fhead);
}

void write_tuples(FILE* ft, struct header *h, struct tuple * t)
{
  for(size_t i=0; i < h->n_tups; ++i)
  {
    for(size_t d=0; d < h->d; ++d)
      fwrite(&t[i].d[d], sizeof(int), 1, ft);
    fwrite(&t[i].avg, sizeof(double), 1, ft);
  }
}

int main(int argc, const char **argv)
{
  FILE * fbin1 = fopen("./f1bin.bin", "wb");
  FILE * fbin2 = fopen("./f2bin.bin", "wb");

  struct header h = {0, 3, 5, 5, 3.141596};
  struct tuple *t = (struct tuple*)calloc(sizeof(struct tuple) , h.n_tups);

  t[0].d[0] = 0;
  t[0].d[1] = 1;
  t[0].d[2] = 2;
  t[0].avg = 3.1411;

  t[1].d[0] = 3;
  t[1].d[1] = 1;
  t[1].d[2] = 0;
  t[1].avg = 3.1415;

  t[2].d[0] = 0;
  t[2].d[1] = 4;
  t[2].d[2] = 1;
  t[2].avg = 3.1413;

  t[3].d[0] = 2;
  t[3].d[1] = 1;
  t[3].d[2] = 0;
  t[3].avg = 3.1414;

  t[4].d[0] = 0;
  t[4].d[1] = 2;
  t[4].d[2] = 3;
  t[4].avg = 3.1410;

  write_header(fbin1, &h);
  write_tuples(fbin2, &h, t);

  fclose(fbin1);
  fclose(fbin2);
  
  return 0;
}
