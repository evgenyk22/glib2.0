#include <stdio.h>
#include <stdlib.h>
#include <glib.h>

#define MAX_SIZE 80

void freebuf (char** mem) {
    int i;
    for ( i = 0 ; i < 6 ; i++ ) {
        g_free(mem[i]);
    }
    g_free(mem);
}

void createshow_strvec () {

   char tempbuf[MAX_SIZE];
   char** stringbuf = NULL;
   int i;

   for ( i=0 ; i < 6 ; i++ ) {
       printf ("insert data\n");
       scanf ("%s",tempbuf);
       if (NULL == stringbuf) {
           stringbuf = (char**)g_new0(char** , 1)/*(char**)malloc(sizeof(char**))*/;
       } else {
           stringbuf = g_realloc(stringbuf,(sizeof(char*)*(i+1) )); /*realloc(stringbuf,(sizeof(char*)*(i+1) ))*/;
       }
       stringbuf[i] = g_new0(char , (strlen(tempbuf) + 1) );
       strncpy(stringbuf[i],tempbuf,(strlen(tempbuf) + 1));
   }
   for ( i=0 ; i < 6 ; i++ ) {
       printf ("%s\n", stringbuf[i]);
   }
   freebuf (stringbuf);
}