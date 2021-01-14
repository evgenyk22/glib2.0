#include "stdio.h"
#include "stdlib.h"
#include "glib.h"
#include <glib-object.h>
#include <gio/gio.h>
#include "sys/time.h"

#define MAX_MSG_SIZE 100

char* tmo_msg(char* bmsg) {
    
    char* msg = g_new0 (char,MAX_MSG_SIZE);    
    
    struct timeval tv;
    gettimeofday (&tv,NULL);
    g_snprintf(msg,MAX_MSG_SIZE,"%s  %010ld.%06ld",bmsg,tv.tv_sec,tv.tv_usec);
    return msg;
}
gboolean tmoclbk (gpointer data) {
    static int count = 10;
    char* msg;

    if ( 0 == count ) {
        msg = tmo_msg("quit main loop");
        g_main_loop_quit ((GMainLoop*)data);
        g_print ("%s \n", msg);
        g_free(msg);
        return FALSE;
    }
    msg = tmo_msg(" ");
    g_print ("%s \n", msg);
    g_free(msg);

    --count;
    return TRUE;
}

void runmloop1 () {
    GMainLoop* ml;
    ml = g_main_loop_new(NULL,FALSE);
    g_timeout_add(100,tmoclbk,ml);
    g_main_loop_run(ml);
    g_main_loop_unref(ml);
}

void runloop2 () {
    GMainLoop* ml;
    GMainContext* ctx;
    GSource* src;
    int id;

    src = g_timeout_source_new(100);
    ctx = g_main_context_new();
    id = g_source_attach(src,ctx);
    ml = g_main_loop_new(ctx,FALSE);
    g_source_set_callback(src,tmoclbk,ml,NULL);
    g_main_loop_run(ml);
    g_main_loop_unref(ml);
}

gboolean prepare (GSource* src , gint* timeout_) {
    *timeout_ = 2000;
    return FALSE;
}

gboolean check (GSource* src) {
    return TRUE;
}

gboolean dispatch (GSource* src , GSourceFunc clbk , gpointer udata) {
    if ( FALSE == clbk(udata)) {
        return FALSE;
    }
    return TRUE;
}

void runloop3 () {
    GMainLoop* ml;
    GMainContext* ctx;
    GSource* src;
    int id;
    GSourceFuncs fn = {
       prepare,
       check,
       dispatch,
       NULL
    };
    src = g_source_new(&fn,sizeof(GSource));
    ctx = g_main_context_new();
    id = g_source_attach(src,ctx);
    ml = g_main_loop_new(ctx,FALSE);
    g_source_set_callback(src,tmoclbk,ml,NULL);
    g_main_loop_run(ml);
    g_main_loop_unref(ml);
}

typedef struct {
   GSource parent;
   GAsyncQueue* q;
   GDestroyNotify destroy_msg; 
} MsgQueueSrc;

typedef gboolean (*MsgSrcFunc) (gpointer msg , gpointer udata);

static gboolean msg_queue_src_prepare (GSource* src , 
                                       int *timeout_) 
{
    MsgQueueSrc* msgqs = (MsgQueueSrc*)src;
    return ( g_async_queue_length(msgqs->q) > 0 );
}

static gboolean msg_queue_src_dispatch ( GSource* src , 
                                         GSourceFunc callback , 
                                         gpointer udata) 
{
    MsgQueueSrc* msrc = (MsgQueueSrc*) src;
    gpointer msg;
    MsgSrcFunc sfunc = (MsgSrcFunc) callback;

    msg = g_async_queue_try_pop (msrc->q);
    
    if (NULL == msg) {
        return TRUE;
    }
    //g_print("try pop queue %d\n",GPOINTER_TO_INT(msg)); 
    if (sfunc == NULL) {
        //g_print("sfunc == NULL\n");
        if (msrc->destroy_msg != 0) {
            msrc->destroy_msg(msg);
        }
        return TRUE;
    }
    return sfunc (msg,udata);
}

static void msg_queue_src_finalize (GSource* src) 
{
    MsgQueueSrc* smsg = (MsgQueueSrc*)src;
    g_async_queue_unref (smsg->q);
}

static GSourceFuncs msgqsrcfuncs =  {
        msg_queue_src_prepare,
        NULL,
        msg_queue_src_dispatch,
        msg_queue_src_finalize,
        NULL,
        NULL
} ;

GSource* msgQSourceNew ( GAsyncQueue* q ,
                         GDestroyNotify destroy_msg
                        ) 
{
    GSource* source;
    MsgQueueSrc* msg_q_src;

    g_return_val_if_fail(q != NULL , NULL);
    source = g_source_new(&msgqsrcfuncs,sizeof(MsgQueueSrc));
    msg_q_src = (MsgQueueSrc*)source;
    g_source_set_name(source,"Msg queue source");
    msg_q_src->q = g_async_queue_ref(q);
    msg_q_src->destroy_msg  = destroy_msg;
    return source;
}                         

static gint destroy_cnt;
static void c_destroy (gpointer user_data) {
    destroy_cnt++;
}

gboolean source_cb(gpointer msg , gpointer udata) {
    guint* seen_i = (guint*) udata;
    
    g_assert_cmpuint(GPOINTER_TO_INT(msg),==,*seen_i);
    *seen_i = *seen_i + 1 ;
    return TRUE;
}
void testConstruct () {
    GSource* src = NULL;
    GAsyncQueue* q = NULL;
    gint i; 
    static gint seen_i = 1;
    GMainContext* main_context;

    destroy_cnt = 0;
    //seen_i = 1;
    q = g_async_queue_new_full(c_destroy);   
    src = msgQSourceNew(q,c_destroy);
    for ( i = 1 ; i <= 100 ; ++i ) {
        g_async_queue_push(q,GUINT_TO_POINTER(i));
    }
    main_context = g_main_context_new();
    g_source_set_callback(src,(GSourceFunc)source_cb,&seen_i,NULL);
    g_source_attach(src,main_context);
    while ( g_main_context_iteration (main_context,FALSE) );
    g_assert_cmpuint (destroy_cnt , == , 0);
    g_assert_cmpuint(seen_i,==,101);
    g_assert_cmpuint(g_async_queue_length(q), == , 0);

    g_main_context_unref(main_context);
    g_async_queue_unref(q);
    g_source_unref(src);
    //g_print("compleated , no errors\n");    
}