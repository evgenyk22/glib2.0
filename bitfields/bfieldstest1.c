#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "bitfield.h"
#include "bitfield-internals.h"

/* Testing bfsub(), bfcat(), bfshift() and bfshift_ip() */

int test1()
{
	srand((unsigned)time(NULL));
	unsigned int i;		//counter
	unsigned int len = 80;
	char *msg = "Testing bfsub(), bfcat(), bfshift() and bfshift_ip()";
	char *failed = "[FAIL]";
	char *passed = "[PASS]";
	unsigned int dots = len - strlen(msg) - 6;	/* 6 is the length of pass/fail string */
	printf("%s", msg);
	for (i = 0; i < dots; i++)
		printf(".");
	struct bitfield *input = bfnew(len);
	for (i = 0; i < len; i++)
		if (rand() % 2)
			BITSET(input, i);
	unsigned int point, cmp;
	/* try all possible points of division */
	for (point = 1; point < (len - 1); point++) {
		/* divide a bitfield into two subfields */
		struct bitfield *chunk1 = bfsub(input, 0, point);
		struct bitfield *chunk2 = bfsub(input, point, input->size);
		/* concatenate them back into one and compare to the original */
		struct bitfield *output = bfcat(chunk1, chunk2);
		cmp = bfcmp(input, output, NULL);
		bfdel(output);
		if (cmp != 0) {
			printf("%s\n", failed);
			return 1;
		}
		/* shift a bitfield using bfshift() */
		struct bitfield *shifted = bfshift(input, point);
		/* shift a bitfield by swapping chunks */
		struct bitfield *swapped = bfcat(chunk2, chunk1);
		bfdel(chunk1);
		bfdel(chunk2);
		cmp = bfcmp(shifted, swapped, NULL);
		bfdel(swapped);
		if (cmp != 0) {
			printf("%s\n", failed);
			return 1;
		}
		/* shift it back and compare to the original bitfield */
		bfshift_ip(shifted, -point);
		cmp = bfcmp(input, shifted, NULL);
		bfdel(shifted);
		if (cmp != 0) {
			printf("%s\n", failed);
			bfdel(input);
			return 1;
		}
	}
	printf("%s\n", passed);
	bfdel(input);
	return 0;
} 