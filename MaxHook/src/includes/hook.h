#pragma once
#include <Windows.h>
#ifdef _WIN64
#include "frida-gum-64.h"
#else
#include "frida-gum-32.h"
#endif

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
};

void mhook_initialize_hook(Hook* hook);
void mhook_append_hook(Hook** hook);
unsigned int mhook_get_hash(const char* fmt, ...);
Hook* mhook_get_last_hook();
__declspec(dllexport)  Hook* mhook_get_hook(unsigned long long hookId);
__declspec(dllexport) void mhook_set_parameter(MHInvocationContext* ctx, int n, void * value);
__declspec(dllexport) void* mhook_get_parameter(MHInvocationContext* ctx, int n);
__declspec(dllexport) void mhook_replace_return_value(MHInvocationContext* ctx, void * value);
__declspec(dllexport) unsigned long long  mhook_add(void* ptr, HookCallback onEnter, HookCallback onLeave);
__declspec(dllexport) bool mhook_remove(unsigned long hookId);
__declspec(dllexport) void mhook_init();
__declspec(dllexport) void mhook_deinit();

extern Hook* HookHead;