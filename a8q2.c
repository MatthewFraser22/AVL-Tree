
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "avl.h"
#include "markstats.h"

int main(int argc, char* args[]) {
  char infilename1[40] = "marks.txt.1";
  char infilename2[40] = "marks.txt.2";
  char outfilename[40] = "report.txt";
  if (argc > 1) {
    if (argc >= 2) strcpy(infilename1, args[1]);
    if (argc >= 3) strcpy(infilename2, args[2]);
    if (argc >= 4) strcpy(outfilename, args[3]);
  }  
  
  printf("\nLoad data from file %s\n", infilename1);
  MARKS ds1 = {0};
  import_data(&ds1, infilename1);
  display_marks(&ds1);
  
  printf("\nLoad data from file %s\n", infilename2);
  MARKS ds2 = {0};
  import_data(&ds2, infilename2); 
  display_marks(&ds2);

  printf("\nMerge\n");
  merge_data(&ds1, &ds2);
  clean_marks(&ds2);
  display_marks(&ds1);

  printf("\nWrite report to file %s\n", outfilename);
  report_data(&ds1, outfilename);    
  clean_marks(&ds1);
  
  printf("\nRead file %s\n", outfilename);
  FILE *fp =fopen(outfilename, "r");
  if ( fp == NULL ) {
    printf("output file does not exist\n");
    return 0;
  }
  char buf[100];
  while( !feof(fp) ) {
     fgets(buf, sizeof(buf), fp);
     printf("%s", buf);
     buf[0] = 0;
  }
  fclose(fp);
  
  return 0;
}

/*
gcc avl.c markstats.c a8q2.c -o a8q2
a8q2

Load data from file marks.txt.1
grade report
Bodnar         93.6
Chabot         80.4
Costa          45.1
Dabu           74.4
Giblett        59.1
Hatch          66.5
Myrie          76.7
Smith          60.1
Suglio         85.7
Sun            67.7

statistics summary
count          10
mean           70.9
stddev         13.5
Load data from file marks.txt.2
grade report
Ali            88.0
Allison        67.7
Eccles         77.8
He             85.7
Koreck         77.4
Lamont         98.1
Parr           92.5
Pereira        80.3
Peters         82.3
Wang           98.1

statistics summary
count          10
mean           84.8
stddev         9.2
Merge
grade report
Ali            88.0
Allison        67.7
Bodnar         93.6
Chabot         80.4
Costa          45.1
Dabu           74.4
Eccles         77.8
Giblett        59.1
Hatch          66.5
He             85.7
Koreck         77.4
Lamont         98.1
Myrie          76.7
Parr           92.5
Pereira        80.3
Peters         82.3
Smith          60.1
Suglio         85.7
Sun            67.7
Wang           98.1

statistics summary
count          20
mean           77.9
stddev         13.5
Write report to file report.txt
The contents of file report.txt
grade report
Ali            88.0   A
Allison        67.7   C
Bodnar         93.6   A
Chabot         80.4   B
Costa          45.1   F
Dabu           74.4   B
Eccles         77.8   B
Giblett        59.1   D
Hatch          66.5   C
He             85.7   A
Koreck         77.4   B
Lamont         98.1   A
Myrie          76.7   B
Parr           92.5   A
Pereira        80.3   B
Peters         82.3   B
Smith          60.1   C
Suglio         85.7   A
Sun            67.7   C
Wang           98.1   A

simple statistics summary
count          20
mean           77.9
stddev         13.5

*/
