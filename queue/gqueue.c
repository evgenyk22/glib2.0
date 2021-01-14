#include <glib.h>
#include <stdio.h>

void createsetq () {
    GQueue* gq = g_queue_new();
    g_queue_push_tail (gq,"Alice");
    g_queue_push_tail(gq,"Bob");
    g_queue_push_tail(gq,"Fred");
    printf("first in line %s \n",(char*)g_queue_peek_head(gq));
    printf("last in line %s \n",(char*)g_queue_peek_tail(gq));
    printf("Queue length %d \n" , g_queue_get_length(gq));
    g_queue_pop_head(gq);
    printf ("Now the first %s\n" , (char*)g_queue_peek_head(gq));
    g_queue_push_head(gq,"Big Jim");
    printf ("Look sm1 cutting the queue %s\n" , (char*)g_queue_peek_head(gq));
    int fredpos = g_queue_index(gq,"Fred");
    printf ("Fred position is still %d\n" , fredpos);
    g_queue_remove(gq,"Bob");
    fredpos = g_queue_index(gq,"Fred");
    printf ("now Fred position is  %d\n" , fredpos);

    g_queue_free (gq);
}

typedef struct 
{
    /* data */
    gchar* name;
    gint   prio;
} task;

task* taskfab (const gchar* name , gint prio) {
   task* newtask = g_new0(task,1);
   newtask->name = (gchar*)g_string_new(name);
   newtask->prio = prio;
   return newtask;
}

gboolean cmpsort (gpointer left , gpointer right) {
    return ((task*)right)->prio - ((task*)left)->prio  ;
}

void prnt (gpointer data) {
    printf( "name %s \n",((task*)data)->name );
}

void strfree (gpointer data) {
    g_string_free( (GString*)(((task*)data)->name),TRUE);
}

void taskQueue () {
    GQueue* tskq = g_queue_new();
    g_queue_push_tail (tskq,taskfab("meditation", 3));   
    g_queue_push_tail (tskq,taskfab("wake up", 0));
    g_queue_push_tail (tskq,taskfab("po4esat sobaken", 1));
    g_queue_push_tail (tskq,taskfab("pogyljat sobaken", 1));
    printf("currently task queue is: \n");
    g_queue_foreach(tskq,(GFunc)prnt,NULL);
    g_queue_sort(tskq,(GCompareFunc)cmpsort,NULL);
    printf("after sort task queue is: \n");
    g_queue_foreach(tskq,(GFunc)prnt,NULL);
    g_queue_foreach(tskq,(GFunc)strfree,NULL);
    g_queue_free(tskq);
}