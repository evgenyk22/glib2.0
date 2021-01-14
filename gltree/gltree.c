#include <glib.h>
#include <stdio.h>

void k_destr (gpointer node) {
    printf("destroed key %s\n",(char*)node);
}

void v_destr (gpointer node) {
    printf("destroed value %s\n",(char*)node);
}

gboolean iterateall (gpointer key , gpointer value , gpointer data) {
    printf("key '%s' val '%s' \n",(char*)key , (char*)value);
    return FALSE;
}

gboolean iteratesome (gpointer key , gpointer value , gpointer data) {
    return TRUE;
}

void gbtreecins () {

    GTree* bt = g_tree_new_full((GCompareDataFunc)g_ascii_strcasecmp,NULL,(GDestroyNotify)k_destr,(GDestroyNotify)v_destr);
    g_tree_insert (bt,"c","Chicago");
    g_tree_insert(bt,"b","Boston");
    g_tree_insert(bt,"d","Detroit");

    printf ("Data in b is '%s' \n", (char*)g_tree_lookup(bt,"b") );
    gpointer key;
    gpointer data;
    gpointer* key_f = &key;
    gpointer* data_f = &data;
    gboolean found = g_tree_lookup_extended(bt , "b", (gpointer*)key_f , (gpointer*) data_f);
    printf ("extended search find key:'%s'  val:'%s' ",(char*)key,(char*)data);
    g_tree_foreach(bt,(GTraverseFunc)iterateall,NULL);
    g_tree_replace(bt,"b","Boulder");
    g_tree_foreach(bt,(GTraverseFunc)iterateall,NULL);
    printf("stealing key '%s' \n","b");
    g_tree_steal(bt,"b");
    g_tree_foreach(bt,(GTraverseFunc)iterateall,NULL);
    g_tree_destroy(bt); 
}

