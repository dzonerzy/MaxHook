#pragma once
#include <Windows.h>
#ifdef _WIN64
#include "frida-gum-64.h"
#else
#include "frida-gum-32.h"
#endif

#define MHOOK_EXPORTED(t) __declspec(dllexport) t
#define MHOOK __stdcall

typedef struct _MHListener MHListener;
typedef struct _Hook Hook;
typedef void* MHInvocationContext;
typedef void (MHOOK* HookCallback)(MHInvocationContext* ic);

struct _MHListener
{
    GObject parent;
};

struct _Hook {
    GumInterceptor* interceptor;
    GumInvocationListener* listener;
    Hook* next;
    Hook* prev;
    unsigned long long hookId;
    void* ptr;
    HookCallback onEnter;
    HookCallback onLeave;
    bool enabled;
};

void mhook_initialize_hook(Hook* hook);
void mhook_append_hook(Hook** hook);
unsigned int mhook_get_hash(const char* fmt, ...);
MHOOK_EXPORTED(Hook*) mhook_get_hook(unsigned long long hookId);
MHOOK_EXPORTED(void) mhook_set_parameter(MHInvocationContext* ctx, int n, void * value);
MHOOK_EXPORTED(void*) mhook_get_parameter(MHInvocationContext* ctx, int n);
MHOOK_EXPORTED(void) mhook_replace_return_value(MHInvocationContext* ctx, void * value);
MHOOK_EXPORTED(unsigned long long) mhook_add(void* ptr, HookCallback onEnter, HookCallback onLeave);
MHOOK_EXPORTED(bool) mhook_remove(unsigned long hookId);
MHOOK_EXPORTED(void) mhook_init();
MHOOK_EXPORTED(void) mhook_deinit();
MHOOK_EXPORTED(bool) mhook_enable(unsigned long long hookId);
MHOOK_EXPORTED(bool) mhook_disable(unsigned long long hookId);
MHOOK_EXPORTED(bool) mhook_is_enable(unsigned long long hookId);
MHOOK_EXPORTED(void*) mhook_get_return_address(MHInvocationContext* ctx);


extern Hook* HookHead;