#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <hook.h> // <MaxHook.h>

void MHOOK_CALLBACK onEnter(MHInvocationContext* ctx) {
	printf("Called Beep(%d, %d);\n", mhook_get_parameter(ctx, 0), mhook_get_parameter(ctx, 1));
	printf("Return address => 0x%p\n", mhook_get_return_address(ctx));
}

void MHOOK_CALLBACK onLeave(MHInvocationContext* ctx) {
	mhook_replace_return_value(ctx, 1337);
	printf("Leave\n");
}

int main() {
	mhook_init();
	unsigned long long hookId = mhook_add(Beep, onEnter, onLeave);
	if (hookId > 0) {
		int ret = Beep(100, 1000);
		if (mhook_remove(hookId))
			printf("Hook removed\n");
		printf("Beep return: %d\n", ret);
		// This won't be triggered by the hook
		Beep(100, 1000);
	}
	mhook_deinit();
}