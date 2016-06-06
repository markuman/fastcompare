#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <zlib.h>

long int thishash;
long long int n, count_a, idx;
long long int lines = 0, count_b = 0;

// binary_search because I dunno how to use bsearch with long long int correctly
long long int binary_search (long long int *array, long long int n, long long int value) {
    long long int from = 0, to = n;
    long long int indx = (from + to) / 2;
    while (from <= to) {
        if (array[indx] == value) {
            return indx;
        } else if (array[indx] < value) {
            from = indx + 1;
        } else {
            to = indx - 1;
        }
        indx = (from + to) / 2;
    }
    return -1;
}


struct vi {
	long long int value;
	long long int index;
	bool used;
};

long long int struct_binary_search (struct vi *array, long long int n, long long int value) {
    long long int from = 0, to = n;
    long long int indx = (from + to) / 2;
    while (from <= to) {
        if (array[indx].value == value) {
            return indx;
        } else if (array[indx].value < value) {
            from = indx + 1;
        } else {
            to = indx - 1;
        }
        indx = (from + to) / 2;
    }
    return -1;
}

int struct_compare (const void *v1, const void *v2) {
	struct vi *p1 = (struct vi *)v1;
	struct vi *p2 = (struct vi *)v2;
	if (p1->value < p2->value) {
		return -1;
	} else if (p1->value > p2->value) {
		return 1;
	} else {
		return 0;
	}
}

// compare for long long int
int compare (const void *a, const void *b) {
    const long long int *x = a, *y = b;
    if (*x > *y) {
        return 1;
    } else {
        return (*x < *y) ? -1 : 0;
	}
}

int main(int argc, char* argv[])
{
	if (argc >= 3) {
		FILE * fp;
		fp = fopen(argv[1], "r");
		if (fp == NULL) { exit(1); }
		FILE * fp2;
		fp2 = fopen(argv[2], "r");
		if (fp2 == NULL) { exit(1); }
		
		char * line = NULL;
		size_t len = 0;
		ssize_t read;
		
		// first determine number of lines for array allocation
		while ((n = fgetc (fp)) != EOF ) {
			if (n == '\n')
				lines++;
		}
		rewind(fp);
		// save number of lines
		count_a = lines;
		
		printf("start generating %lli hashes for file %s\n", lines, argv[1]);
		// build in-memory hashes for the first file
		//long long int *hashes;
		//hashes = malloc (sizeof(long long int) * lines + 1);
		struct vi* keyvalue = malloc (sizeof(struct vi) * lines + 1);
		lines = -1;
		while ((read = getline(&line, &len, fp)) != -1) {
			lines++;
			//hashes[lines] = crc32(0, (const void*)line, len);
			keyvalue[lines].value = crc32(0, (const void*)line, len);
			keyvalue[lines].index = lines;
			keyvalue[lines].used = false;
		}

		// sort to use bsearch
		//qsort(hashes, lines + 1, sizeof(long long int), compare);
		qsort(keyvalue, lines + 1, sizeof(struct vi), struct_compare);
		
		// 2nd file here
		char * line2 = NULL;
		size_t len2 = 0;
		ssize_t read2;
		printf("start comparing hashes with file %s\n", argv[2]);

		
		// build temporary new hash for every line
		// to compare with in-memory hash from 1st file.
		while ((read2 = getline(&line2, &len2, fp2)) != -1) {
			count_b++;
			// search 
			if (lines > -1) {
				thishash = crc32(0, (const void*)line2, len2);
				//idx = binary_search(hashes, count_a - 1, thishash);
				idx = struct_binary_search(keyvalue, count_a - 1, thishash);
				if (idx > -1) {
					// flag idx
					keyvalue[idx].used = true;
					lines--;
				} 
			} // we could break here, but then we don't know the number of lines from file 2.
		}
		
		printf("summary:\n File %s: %lli lines\n File %s: %lli lines\n", argv[1], count_a, argv[2], count_b);
		// if lines' value is >= 0, there are some lines left which are not
		// part of the 2nd file.
		if (lines >= 0) {
			printf(" File %s has %lli lines which are not in file %s\n", argv[1], lines + 1, argv[2]);
			// 4th argument will print all line numbers which are left
			if (argc == 4) {
				printf(" Following lines of %s are not included in %s: \n\n", argv[1], argv[2]);
				for (n = 0; n < count_a; n++) {
					if (keyvalue[n].used == false) { printf("%lli\n", keyvalue[n].index + 1); }
				}
			}
		// when lines' value is < 0, all lines of 1st file are included in the 2nd file.
		} else {
			printf("File %s is completly contained in file %s\n", argv[1], argv[2]);
		}
		free(keyvalue);
		// free(hashes);
	} else {
		printf("useage:\n\tfastcompare <file1> <file2>\n");
		printf("\tfastcompare <file1> <file2> verbose\n\n");
	}
    
}

