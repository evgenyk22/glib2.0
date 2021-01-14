#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include <limits.h>
#include <time.h>
#include <bitfield.h>
#include "bitfield-internals.h"
#ifdef RAND_MAX
#undef RAND_MAX
#define RAND_MAX UINT_MAX
#endif
#define INTS_SIZE  2
#define BIT_LEN    80

int int2bitfield ()
{
    u_int32_t val1[INTS_SIZE] = {0x55555555,0xaaaaaaaa};
	u_int32_t val2[INTS_SIZE] = {0xaaaaaaaa,0x55555555};
    int i;

    printf("Running logical AND over three bit arrays...\n");
	printf("Inputs:\n");
	struct bitfield *input1 =  uint32tobf(val1,sizeof(u_int32_t)*CHAR_BIT*INTS_SIZE);
	bfprint_msb(input1);
	printf("\n");
	struct bitfield *input2 = uint32tobf(val2,sizeof(u_int32_t)*CHAR_BIT*INTS_SIZE);
	bfprint_msb(input2);
	printf("\n");
	struct bitfield *output = bfxor(input1, input2);
	printf("Output:\n");
	bfprint_msb(output);
	printf("\n");
	printf("---\n");
    bfdel (input1);
    bfdel (input2);
	bfdel (output);
    
	struct bitfield * input3 = bfnew_ones  (BIT_LEN);
	bfprint_msb(input3);
	printf ("\n");
    struct bitfield * input4 = bfnew_quick (BIT_LEN);
	bfprint_msb(input4);
	printf ("\n");

	//compare
    int cmp;
	cmp = bfcmp (input3, input4,NULL);
	if (cmp) printf ("compare failed \n");

    bfsetall (input4);
	cmp = bfcmp (input3, input4,NULL);
	if (!cmp) printf ("compare succeded after bfsetall\n");
    
	
	bfprint_msb(input4);
	printf("\n");
    bfclearbit(input4, 79);
	bfclearbit(input4, 0);
	printf ("clear bit 709 , 0 \n"); 
    printf("\n");
	bfprint_msb(input4);
    printf("\n");
	if (BITGET(input4, 79) == 0) {
	    printf("%s\n", "bit zeroed");
	}
	for ( i = 9 ; i < 16 ; i++ ) {
        bftogglebit (input4,i);
	}
	bfprint_msb(input4);
    printf("\n");
	unsigned long *input_long = bf2long(input4);
	int bitnslots = BITNSLOTS(BIT_LEN);
	for ( i = 0 ; i < bitnslots ; i++ ) {
		printf ("%lx\n",input_long[i]);
	}
	free (input_long);

    //sub space
	struct bitfield * chunk0 =  bfsub (input4,0,40);
	struct bitfield * chunk1 =  bfsub (input4,40,80); 
	bfprint_msb(chunk0);
	printf("\n");
	bfprint_msb(chunk1);
	printf("\n");
	struct bitfield * output1 = bfcat (chunk0,chunk1);
    cmp = bfcmp (output1,input4,NULL);
	if (cmp != 0) {
		printf ("bfcmp (output1,input4) failed \n");  
	    bfprint_msb(input4);
	    printf("\n");
	    bfprint_msb(output1);
	    printf("\n");
	} else
	{
		printf ("Tested...\n");
	}


	bfdel (chunk0);
	bfdel (chunk1);
	//the same but inplace (prepare in place long array)
	//unsigned long *input_long1 = calloc (bitnslots,sizeof (unsigned long));	
	//bf2long_ip(input4,input_long1);

    
	/*
	int bitnslots = BITNSLOTS(len);
	unsigned long *input_long2 = malloc(bitnslots * sizeof(unsigned long));
	for (i = 0; i < bitnslots; i++)
		input_long2[i] = input->field[i]; 
	*/  
    bfdel (input3);
	bfdel (input4);
	return 0;
}