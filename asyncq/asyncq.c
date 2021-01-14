#include "stdio.h"
#include "stdlib.h"
#include "glib.h"
#include "sys/time.h"

typedef struct {
    GAsyncQueue* q;
    GThread* th;
} TH_STR;

#define MAX_SIZE 200

char* gettm (char* buf) {
    if ( NULL == buf) {
        buf = g_new0 (char,MAX_SIZE);
    }
    struct  timeval tv;
    gettimeofday(&tv,NULL);
    snprintf(buf,MAX_SIZE,"%010ld.%06ld",tv.tv_sec,tv.tv_usec);
    return buf;
}

void* thread (void* p) {
    TH_STR* ths;
    gboolean stop = FALSE;
    if (p == NULL) {
        printf ("struct should be initialized\n");
        g_thread_exit (NULL);
        return NULL;
    }
    ths = (TH_STR*)p;
    g_async_queue_ref(ths->q);
    while (FALSE == stop) {
        void* pv = g_async_queue_pop(ths->q);
        if ( NULL != pv ) {
            char* data = (char*)pv;
            char* t = gettm (NULL);
            printf("got msg %s in %s\n",data,t);

            if ('Q' == data[0] ) {
                printf ("Closed queue in %s\n",t);
                stop = TRUE;    
            }
            g_free(t);
            g_free(pv);
        }
    }
    g_async_queue_unref(ths->q);
    g_print("thread exit\n");
    g_thread_exit (NULL);
    return NULL;
}

gboolean send_msg (GAsyncQueue* q ,char* msg) {
    
    if ( NULL == q ) {
        g_print("error could be null");
        return FALSE;
    } 
    char* ts = gettm(NULL);
    char* buf = g_strdup_printf("%s [%s]" , msg,ts);
    g_async_queue_push(q,buf);
    return TRUE; 
}
void print_msg_tmstm (char* msg) {
    char* tstmp = gettm(NULL);
    printf ("%s %s\n",msg,tstmp);
    g_free(tstmp); 
}

int trytosync () {
    TH_STR my_th; 
    my_th.q = g_async_queue_new ();
    g_async_queue_ref(my_th.q);

    my_th.th = g_thread_new("TH1" , thread , &my_th);
    send_msg(my_th.q,"a");
    send_msg(my_th.q,"b");
    send_msg(my_th.q , "c");
    send_msg(my_th.q , "d");
    print_msg_tmstm("stop at ");
    send_msg(my_th.q , "Q");
    g_async_queue_unref(my_th.q);
    g_thread_join(my_th.th);
    print_msg_tmstm("join thread at: ");
}

gboolean tmo_clbk (gpointer data) {
    print_msg_tmstm((char*) data );
    return TRUE;
}

void main_l() {
    print_msg_tmstm("started\n");
    int timeout = 5;
    GMainLoop* mainloop = NULL;
    mainloop = g_main_loop_new(NULL,FALSE);
    g_timeout_add_seconds (timeout,tmo_clbk,"timeout callback");
    g_main_loop_run(mainloop);
}