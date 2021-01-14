#include "glib.h"
#include "stdio.h"

void dumparray( const GArray* vec) {
    int i;
    printf("The array holds : \n");
    for ( i = 0 ; i < vec->len ; i++ ) {
        printf ("%d ," , g_array_index(vec , int , i ) );     
    }
    printf ("\n");
} 

void createarray () {
    GArray* a = g_array_new (FALSE,FALSE , sizeof(char*));
    char* first = "hello" , *second = "there" , *third = "world";
    g_array_append_val (a,first);
    g_array_append_val (a,second);
    g_array_append_val (a,third);
    printf ("there is %d items in array" , a->len);
    printf ("first %s\n " , g_array_index(a,char* , 0));
    printf ("second %s\n" , g_array_index(a,char* ,1));
    printf ("Third %s\n" , g_array_index(a,char*,2));
    g_array_remove_index(a,1);
    printf ("there is %d items in array" , a->len);
    g_array_free (a,FALSE);

    GArray* b = g_array_sized_new (TRUE,TRUE,sizeof(int), 16);
    printf ("Len hidden %d\n" , b->len);
    printf ("Zeroed therefore index 2 is %d\n " , g_array_index (b,int,2));
    g_array_free (b,FALSE);

    GArray* c = g_array_new (FALSE,FALSE,sizeof(char));
    char* str = g_strdup ("hello");
    g_array_append_val (c,str);
    printf ("first elem %d\n",g_array_index(c,char,0));
    g_array_free (c,TRUE); 
}

gboolean intcompare (gpointer arg1,gpointer arg2) {
    gint *a1 = (gint*)arg1;
    gint *a2 = (gint*)arg2;
    return *a1 - *a2;  
}
void garappend () {
    GArray* a = g_array_new(FALSE,FALSE,sizeof(int));
    gint x[2] = {1,2};
    gint y[2] = {3,4};
    g_array_append_vals(a,&x,2);
    g_array_prepend_vals(a,&y,2);
    dumparray(a); 
    printf("rem first 2 items \2");
    //g_array_remove_range(a,0,2);
    g_array_remove_index_fast(a,0); 
    dumparray(a);  
    printf ("Let's append 5 items and sort \n");
    gint z [5] = {99,98,96,1,23};
    g_array_append_vals(a,&z,5);
    g_array_sort (a,(GCompareFunc)intcompare);
    dumparray(a);
    g_array_free(a,FALSE);
}

void gptrarray () {
    GPtrArray* a = g_ptr_array_new();
    g_ptr_array_add(a,g_strdup("hello "));
    g_ptr_array_add(a,g_strdup("again "));
    g_ptr_array_add(a,g_strdup("world "));
    g_ptr_array_add(a,g_strdup("hi "));
    g_ptr_array_foreach (a,(GFunc)printf,NULL);
    printf("\n");
    g_ptr_array_free(a,TRUE);
}



