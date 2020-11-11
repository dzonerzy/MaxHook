// MaxHook.cpp : Definisce le funzioni per la libreria statica.
//
#include "framework.h"
#include "hook.h"

static void mhook_listener_iface_init(gpointer g_iface, gpointer iface_data);

#define MHOOK_TYPE_LISTENER (mhook_listener_get_type ())
G_DECLARE_FINAL_TYPE(MHListener, mhook_listener, MH, LISTENER, GObject)
G_DEFINE_TYPE_EXTENDED(MHListener, mhook_listener, G_TYPE_OBJECT, 0, G_IMPLEMENT_INTERFACE(GUM_TYPE_INVOCATION_LISTENER, mhook_listener_iface_init))


static void mhook_listener_on_enter(GumInvocationListener* listener, GumInvocationContext* ic) {
    MHListener* self = MH_LISTENER(listener);
    unsigned long long hookId = GUM_IC_GET_FUNC_DATA(ic, unsigned int);
    Hook* hook = mhook_get_hook(hookId);
    if(hook && hook->enabled)
        hook->onEnter(ic);
}


static void mhook_listener_on_leave(GumInvocationListener* listener, GumInvocationContext* ic) {
    MHListener* self = MH_LISTENER(listener);
    unsigned long long hookId = GUM_IC_GET_FUNC_DATA(ic, unsigned int);
    Hook* hook = mhook_get_hook(hookId);
    if(hook && hook->enabled)
        hook->onLeave(ic);
}

static void mhook_listener_class_init(MHListener* klass) {
    (void)MH_IS_LISTENER(klass);
#ifndef _MSC_VER
    (void) glib_autoptr_cleanup_ExampleListener;
#endif
}

static void mhook_listener_iface_init(gpointer g_iface, gpointer iface_data) {
    GumInvocationListenerInterface* iface = (GumInvocationListenerInterface *)g_iface;
    iface->on_enter = mhook_listener_on_enter;
    iface->on_leave = mhook_listener_on_leave;
}

static void mhook_listener_init(MHListener* self) {

}

void mhook_initialize_hook(Hook* hook) {
    hook->interceptor = gum_interceptor_obtain();
    hook->listener = (GumInvocationListener *)g_object_new(MHOOK_TYPE_LISTENER, NULL);
    gum_interceptor_begin_transaction(hook->interceptor);
    gum_interceptor_attach(hook->interceptor, GSIZE_TO_POINTER(hook->ptr), hook->listener, GSIZE_TO_POINTER(hook->hookId));
    gum_interceptor_end_transaction(hook->interceptor);
}

Hook* mhook_get_last_hook() {
    Hook* hook = HookHead;
    while (hook) {
        if (hook->next == NULL)
            return hook;
        else
            hook = hook->next;
    }
    return NULL;
}

void mhook_append_hook(Hook** hook) {
    if (HookHead != NULL) {
        Hook* lastest = mhook_get_last_hook();
        lastest->next = (*hook);
        (*hook)->prev = lastest;
        (*hook)->next = NULL;
    }
    else {
        HookHead = *hook;
    }
}

MHOOK_EXPORTED(Hook*) mhook_get_hook(unsigned long long hookId) {
    Hook* hook = HookHead;
    while (hook) {
        if (hook->hookId == hookId) {
            return hook;
        }
        else {
            hook = hook->next;
        }
    }
    return NULL;
}

MHOOK_EXPORTED(unsigned long long) mhook_add(void * ptr, HookCallback onEnter, HookCallback onLeave) {
    Hook* hook = (Hook*)malloc(sizeof(Hook));
    Hook* tmp = NULL;
    if (hook) {
        memset(hook, 0, sizeof(Hook));
        hook->hookId = mhook_get_hash("%llu%llu%llu", ptr, onEnter, onLeave);
        tmp = mhook_get_hook(hook->hookId);
        if (tmp == NULL) {
            hook->onEnter = onEnter;
            hook->onLeave = onLeave;
            hook->ptr = ptr;
            hook->enabled = true;
            mhook_append_hook(&hook);
            mhook_initialize_hook(hook);
            return hook->hookId;
        }
        return tmp->hookId;
    }
    return -1;
}

MHOOK_EXPORTED(bool) mhook_remove(unsigned long hookId) {
    Hook* hook = mhook_get_hook(hookId);
    if (hook) {
        if (hook->next == NULL && hook->prev == NULL) {
            HookHead = NULL;
        }
        else if (hook->next == NULL) {
            hook->prev->next = NULL;
        }
        else if (hook->prev == NULL) {
            HookHead = hook->next;
        }
        else {
            Hook* prev = hook->prev;
            Hook* next = hook->next;
            prev->next = next;
            next->prev = prev;
        }
        gum_interceptor_detach(hook->interceptor, hook->listener);
        g_object_unref(hook->listener);
        g_object_unref(hook->interceptor);
        free(hook);
        return true;
    }
    return false;
}

unsigned int mhook_get_hash(const char* fmt, ...)
{
    char buffer[1024] = { '\0' };
    va_list va_alist;
    va_start(va_alist, fmt);
    vsprintf_s(buffer, 1024, fmt, va_alist);
    va_end(va_alist);

    unsigned int h = 3323198485ul;
    for (int i = 0; i < strlen(buffer); i++) {
        h ^= buffer[i];
        h *= 0x5bd1e995;
        h ^= h >> 15;
    }
    return h;
}

MHOOK_EXPORTED(void) mhook_set_parameter(MHInvocationContext* ctx, int n, void* value) {
    gum_invocation_context_replace_nth_argument(ctx, n, value);
}

MHOOK_EXPORTED(void*) mhook_get_parameter(MHInvocationContext* ctx, int n) {
    return gum_invocation_context_get_nth_argument(ctx, n);
}

MHOOK_EXPORTED(void) mhook_replace_return_value(MHInvocationContext* ctx, void * value) {
    gum_invocation_context_replace_return_value(ctx, value);
}

MHOOK_EXPORTED(void) mhook_init() {
    gum_init_embedded();
}

MHOOK_EXPORTED(void) mhook_deinit() {
    gum_deinit_embedded();
}

MHOOK_EXPORTED(bool) mhook_enable(unsigned long long hookId) {
    Hook* hook = mhook_get_hook(hookId);
    if (hook) {
        hook->enabled = true;
        return true;
    }
    return false;
}

MHOOK_EXPORTED(bool) mhook_disable(unsigned long long hookId) {
    Hook* hook = mhook_get_hook(hookId);
    if (hook) {
        hook->enabled = false;
        return true;
    }
    return false;
}

MHOOK_EXPORTED(bool) mhook_is_enable(unsigned long long hookId) {
    Hook* hook = mhook_get_hook(hookId);
    if (hook) {
        return hook->enabled;
    }
    return false;
}

MHOOK_EXPORTED(void*) mhook_get_return_address(MHInvocationContext* ctx) {
    return gum_invocation_context_get_return_address(ctx);
}

Hook* HookHead;