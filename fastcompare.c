// gcc fastcompare.c -lz -O3 -faggressive-loop-optimizations
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>

//#define DEBUG
//#define numel(x)  (sizeof(x) / sizeof((x)[0]))

long int thishash;
long long int n;
long long int lines = 0;

int main(int argc, char* argv[])
{
	if (argc >= 3) {
		FILE * fp;
		char * line = NULL;
		size_t len = 0;
		ssize_t read;

		fp = fopen(argv[1], "r");
		
		// first determine number of lines for array allocation
		while ((n = fgetc (fp)) != EOF ) {
			if (n == '\n')
				lines++;
		}
		rewind(fp);
		
		printf("start generating %d hashes for file %s\n", lines, argv[1]);
		
		// build in-memory hashes for the first file
		long int hashes[lines], hashidx[lines];
		lines = -1;
		while ((read = getline(&line, &len, fp)) != -1) {
			lines++;  
			hashes[lines] = crc32(0, (const void*)line, len);
			hashidx[lines] = lines;			
		}
		//fclose(fp);
		
		#ifdef DEBUG
			for (n = 0; n <= lines; n++) {
				printf("%lli: %lX\n", n, hashes[n]);
			}
		#endif
		
		// 2nd file here
		FILE * fp2;
		char * line2 = NULL;
		size_t len2 = 0;
		ssize_t read2;
		printf("start comparing hashes with file %s\n", argv[2]);
		fp2 = fopen(argv[2], "r");
		
		// build temporary new hash for every line
		// to compare with in-memory hash from 1st file.
		while ((read2 = getline(&line2, &len2, fp2)) != -1) {
			thishash = crc32(0, (const void*)line2, len2);
			
			// search for thishash in hashes from 1st file
			for (n = 0; n <= lines; n++) {
				if (thishash == hashes[n]) {
					#ifdef DEBUG
						printf("%lX found in line %lli\n", thishash, hashidx[n]);
						printf("found after %d iterations\n", n);
					#endif
					
					// copy last element to current position and decrease
					// max iterator to reduce max iterator next round :)
					if (lines >= 0) {
						hashes[n] = hashes[lines];
						hashidx[n] = hashidx[lines];
						lines--;
					}
					break;
				}
			}
			
		}
		
		// if lines' value is >= 0, there are some lines left which are not
		// part of the 2nd file.
		if (lines >= 0) {
			printf("File %s has %d lines which are not in file %s\n", argv[1], lines + 1, argv[2]);
			// 4th argument will print all line numbers which are left
			if (argc == 4) {
				printf(" Following lines of %s are not included in %s: \n\n", argv[1], argv[2]);
				for (n = 0; n <= lines; n++) {printf("%d\n", hashidx[n] + 1);}
			}
		// when lines' value is < 0, all lines of 1st file are included in the 2nd file.
		} else {
			printf("File %s is completly contained in file %s\n", argv[1], argv[2]);
		}
	} else {
		printf("useage:\n\tfastcompare <file1> <file2>\n");
		printf("\tfastcompare <file1> <file2> verbose\n\n");
	}
    
}

