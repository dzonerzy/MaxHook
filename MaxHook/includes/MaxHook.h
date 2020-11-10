#pragma once
#define MHOOK __stdcall

typedef void* MHInvocationContext;
typedef void (MHOOK* HookCallback)(MHInvocationContext* ic);

extern "C" void mhook_set_parameter(MHInvocationContext* ctx, int n, void* value);
extern "C" void* mhook_get_parameter(MHInvocationContext* ctx, int n);
extern "C" void mhook_replace_return_value(MHInvocationContext* ctx, void* value);
extern "C" unsigned long long mhook_add(void* ptr, HookCallback onEnter, HookCallback onLeave);
extern "C" bool mhook_remove(unsigned long hookId);
extern "C" void mhook_init();
extern "C" void mhook_deinit();