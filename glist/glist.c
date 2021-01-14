#include "glib.h"
#include "stdio.h"

void addItems2List () {
    GSList* l = NULL;
    printf ("\nCurr list size %d \n",g_slist_length(l));
    l = g_slist_append(l,"apples");
    l = g_slist_append(l,"bananas");
    printf ("\nCurr list size %d \n",g_slist_length(l));
    g_slist_free(l);
}

void addremtest () {
    GSList* list = NULL;
    list = g_slist_append(list,"apples");
    list = g_slist_prepend(list,"banana");
    list = g_slist_append(list,"carrot");
    printf ("current items in list %d\n" , g_slist_length(list));
    list = g_slist_remove(list,"carrot");
    printf ("current items in list %d\n" , g_slist_length(list));
    g_slist_free(list);
}

void removedup () {
    GSList* list = NULL;
    list = g_slist_append(list,"apples");
    list = g_slist_prepend(list,"banana");
    list = g_slist_prepend(list,"carrot");
    list = g_slist_prepend(list,"banana");
    list = g_slist_prepend(list,"banana");
    printf ("current items in list %d\n" , g_slist_length(list));
    list = g_slist_remove_all (list,"banana");
    printf ("current items in list %d\n" , g_slist_length(list));
    g_slist_free (list);
}

void showItems () {
    GSList* list = NULL;
    list = g_slist_append(list,"apples");
    list = g_slist_prepend(list,"banana");
    list = g_slist_prepend(list,"carrot");
    list = g_slist_prepend(list,"banana");
    list = g_slist_prepend(list,"banana");
    printf ("the last items is '%s'\n" , (char*)g_slist_last(list)->data);
    printf ("index 2 item is %s\n", (char*)g_slist_nth(list,2)->data);
    printf ("other way to see item 1 is %s\n",(char*)g_slist_nth_data(list,3));
    printf ("element after first is %s\n",(char*)g_slist_next(list)->data);
    g_slist_free (list);    
}

typedef struct {
	  char* name;
	  int shoe_size;
} Person;

void freeIterator (gpointer item ) {
    g_free(((Person*)item)->name);
    g_free(item);
}
int showUData () {
	 GSList* list = NULL;
	 GSList* iterator = NULL;
     Person* tom = g_new(Person,1);
	 tom->name = g_strdup("Tom");
	 tom->shoe_size = 12;
	 list = g_slist_append(list, tom);
	 Person* fred = g_new(Person, 1); // allocate memory for one Person struct
	 fred->name = g_strdup("Fred");
	 fred->shoe_size = 11;
     list = g_slist_prepend(list, fred);
     Person* ivan = g_new(Person,1);
     ivan->name= g_strdup("Ivan");
     ivan->shoe_size=9;
     list = g_slist_prepend(list,ivan);

     for ( iterator = list ; iterator != NULL ; iterator = iterator->next )
     {
        printf ("name %s person shoe size %d \n",((Person*)iterator->data)->name ,((Person*)iterator->data)->shoe_size);         
     }
     g_slist_foreach(list,(GFunc)freeIterator,NULL);
     /*
	 printf("Tom's shoe size is '%d'\n", ((Person*)list->data)->shoe_size);
     printf("The last Person's name is '%s' %d\n", ((Person*)g_slist_last(list)->data)->name,((Person*)g_slist_last(list)->data)->shoe_size);
     */
     g_slist_free(list);
     
     //g_free(ivan);
	 //g_free(tom);
	 //g_free(fred);
     return 0;
}

void strIter (gpointer data ) {
    printf ("%s \n",(char*)data);
}

gint mycompare (gpointer s1 , gpointer s2) {
    return g_ascii_strcasecmp(s1,s2);
}

gint myfind (gconstpointer item) {
    return (g_ascii_strcasecmp(item , "Chicago"));
}

void trysort () {
    GSList* list = NULL;
    list = g_slist_append(NULL,"Chicago");
    list = g_slist_append(list,"Boston");
    list = g_slist_append(list,"Arona");
    list = g_slist_append(list,"Glazgo");

    list = g_slist_sort(list,(GCompareFunc)mycompare);
    g_slist_foreach(list,(GFunc)strIter,NULL);

    GSList* item = g_slist_find (list,"Glazgo");
    printf ("\n  after extensive search find %s  \n",(item == NULL)?("null"):((char*)item->data)); 
    item = g_slist_find_custom (list,NULL,(GCompareFunc)myfind);
    printf ("\n  after extensive search find %s  \n",(item == NULL)?("null"):((char*)item->data)); 
    list = g_slist_insert_sorted(list,"Denver",(GCompareFunc)mycompare);
    g_slist_foreach(list,(GFunc)strIter,NULL);
    g_slist_free(list);
}
