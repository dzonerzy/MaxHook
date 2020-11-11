#pragma once

#define MHOOK_CALLBACK __stdcall
#define MHOOK_PUBLIC extern "C"

#define MHOOK_LLU unsigned long long 
#define MHOOK_BOOL bool
#define MHOOK_VPTR void *
#define MHOOK_VOID void

typedef void* MHInvocationContext;
typedef void (MHOOK_CALLBACK* HookCallback)(MHInvocationContext* ic);

// Initialization
MHOOK_PUBLIC MHOOK_VOID mhook_init();
MHOOK_PUBLIC MHOOK_VOID mhook_deinit();

// Hooks 
MHOOK_PUBLIC MHOOK_LLU mhook_add(void* ptr, HookCallback onEnter, HookCallback onLeave);
MHOOK_PUBLIC MHOOK_BOOL mhook_remove(unsigned long hookId);
MHOOK_PUBLIC MHOOK_BOOL mhook_enable(unsigned long long hookId);
MHOOK_PUBLIC MHOOK_BOOL mhook_disable(unsigned long long hookId);
MHOOK_PUBLIC MHOOK_BOOL mhook_is_enable(unsigned long long hookId);

// Utils
MHOOK_PUBLIC MHOOK_VOID mhook_set_parameter(MHInvocationContext* ctx, int n, void* value);
MHOOK_PUBLIC MHOOK_VPTR mhook_get_parameter(MHInvocationContext* ctx, int n);
MHOOK_PUBLIC MHOOK_VOID mhook_replace_return_value(MHInvocationContext* ctx, void* value);
MHOOK_PUBLIC MHOOK_VPTR mhook_get_return_address(MHInvocationContext* ctx);