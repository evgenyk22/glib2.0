#include <stdio.h>
#include <glib.h>
//#include <qlibc.h>

GHashTable* table;

int create_hashtable_fromfile (const char* fname ) {

    FILE* fp;
    char line [1024];
        
    fp = fopen(fname,"r");
    table = g_hash_table_new (g_str_hash,g_str_equal);

    while(fgets (line,sizeof(line),fp )) {
        char* key;
        char* val;

        key = strtok(line,"\t");
        if (!key) continue; 
        val = strtok(NULL,"\t");
        if (!val) continue;

        char* old_key;
        //char* old_val;
        GList *string_list = NULL;

        if (g_hash_table_lookup_extended(table,key,(void*)&old_key,(void*)&string_list)) {
            //g_hash_table_insert (table,g_strdup(key),g_strdup(val));
            //g_free(old_key);
            //g_free(old_val);
            string_list = g_list_append (string_list, g_strdup(val) );
        }
        else {
            string_list = g_list_append (string_list, g_strdup(val));
            g_hash_table_insert (table,g_strdup(key),(gpointer)string_list);
        }
    }
    fclose(fp);
    return (EXIT_SUCCESS);
}

int destroyhash () {
    g_hash_table_destroy (table);
    return (EXIT_SUCCESS);
}

int remove_entry (const char* key) {
    char* old_key;
    char* old_value;

    if (g_hash_table_lookup_extended(table,key,(void*)&old_key,(void*)&old_value)) {
        g_hash_table_remove(table,key);
        g_free(old_key);
        g_free(old_value);           
    }
    return (EXIT_SUCCESS);
}

static void free_hash_table_entry (gpointer key , gpointer val , gpointer udata) {
    free (key);
    free (val);
}

void delete_all_entries () {
    g_hash_table_foreach (table,free_hash_table_entry,NULL);
    g_hash_table_destroy(table);
}

gboolean  free_hash_with_key (gpointer key , gpointer val , gpointer udata) {
    if ( !strcmp ( (char*)key , (char*)udata ) ) {
        g_free (key);
        g_free(val);
        return TRUE;
    }
    return FALSE;
}

void printlistelem (gpointer val ,  gpointer udata) {
        printf ("    data:%s\n" , (char*)val);
    }

void printelem (gpointer key , gpointer val , gpointer udata) {
    GList* stringList = (GList*) val;
    printf ("key:%s  \n",(char*)key  );
    g_list_foreach (stringList ,printlistelem , NULL);
}

void printhash () {
   g_hash_table_foreach (table,printelem,NULL); 
}


void createHash() {
    GHashTable* hash = g_hash_table_new (g_str_hash,g_str_equal);
    g_hash_table_insert (hash,"Virginia","Richmond");
    g_hash_table_insert (hash,"Texas","Austin");
    g_hash_table_insert (hash,"Ohio" , "Colombus");
    printf ("There are %d keys in hash" , g_hash_table_size(hash));
    printf ("The capital of Texas is %s \n", (char*)g_hash_table_lookup(hash,"Texas"));
    gboolean found = g_hash_table_remove (hash , "Virginia");
    printf ("Virginia was found and removed %s \n",(found)?("yes"):("no"));
    g_hash_table_destroy (hash);   
}

void intiterator (gpointer key , gpointer data , gpointer udata ) {
    printf (udata , *(gint*)key , data);
}

void intHash () {
    GHashTable* hash = g_hash_table_new (g_int_hash , g_int_equal);
    gint *key1 = g_new (gint,1);   
    gint *key2 = g_new (gint,1);
    gint *key3 = g_new (gint,1);
    *key1 = 1;
    *key2 = 4;
    *key3 = 9;
    g_hash_table_insert (hash,key1,"one");
    g_hash_table_insert (hash,key2,"four");
    g_hash_table_insert (hash,key3,"nine");
    g_hash_table_foreach(hash,(GHFunc)intiterator,"The square %d is %s \n");
    gint* key;
    char* res = NULL;
    gint** keyref = &key;
    char** resref = &res;
    gboolean fiind = g_hash_table_lookup_extended (hash,key2,(gpointer*)keyref,(gpointer*)resref);
    if (fiind) {
        printf ("find key %d , res %s\n" , *key , res);
    }
    g_hash_table_destroy(hash); 
}

void value_destroyed(gpointer data) {
    printf("Got value destroyed for key %d\n",GPOINTER_TO_INT(data));
}

gboolean finder (gpointer key , gpointer data , gpointer udata) {
   return ( GPOINTER_TO_INT(key) + GPOINTER_TO_INT(data) == 42 );
}
void pinthash () {
    GHashTable* hash = g_hash_table_new_full (g_direct_hash,g_direct_equal,NULL,(GDestroyNotify)value_destroyed);
    g_hash_table_insert (hash,GINT_TO_POINTER(36),GINT_TO_POINTER(2) );
    g_hash_table_insert (hash,GINT_TO_POINTER(33),GINT_TO_POINTER(4) );
    g_hash_table_insert (hash,GINT_TO_POINTER(36),GINT_TO_POINTER(6) );
    printf ("elements in hash %d\n",g_hash_table_size(hash));
    gpointer item_ptr = g_hash_table_find (hash,(GHRFunc)finder,NULL);
    if (item_ptr != NULL) {
        gint item = GPOINTER_TO_INT(item_ptr);
        printf ("find key %d data %d\n",item,42-item);
    }
    g_hash_table_destroy(hash);
    
}

void print (gpointer key , gpointer data , gpointer udata) {
    printf ("List of cities in State %s\n",(char*)key);
    g_slist_foreach ((GSList*)data , (GFunc)printf , NULL);
    printf("\n");
}

void destroy (gpointer key , gpointer data , gpointer udata) {
    g_slist_free(data);
}
void unordermultimap () {
    GHashTable* hash = g_hash_table_new (g_str_hash,g_str_equal);
    g_hash_table_insert (hash,"Texas" , g_slist_append(g_hash_table_lookup(hash,"Texas") , "Austin ") );
    g_hash_table_insert(hash , "Texas", g_slist_append( g_hash_table_lookup(hash,"Texas") , "Huston "));
    g_hash_table_insert(hash , "Virginia" , g_slist_append ( g_hash_table_lookup(hash,"Virginia"),"Richmound " ));
    g_hash_table_insert(hash , "Virginia" , g_slist_append ( g_hash_table_lookup(hash,"Virginia"),"KeysVille " ));
    g_hash_table_foreach (hash , print , NULL);
    g_hash_table_foreach (hash , destroy , NULL);
    g_hash_table_destroy (hash);
}

