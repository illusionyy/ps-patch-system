#include <ps5/kernel.h>
#include "../shared/notify.h"

// int kernel_copyout(unsigned long kaddr, void *uaddr, unsigned long len)
// int32_t kernel_copyin(const void *udaddr, intptr_t kaddr, size_t len)

int main(void)
{
    // TODO: only for demo, make this a pattern
    const uint32_t bapm = 0;
    kernel_copyin(&bapm, KERNEL_ADDRESS_TEXT_BASE + (0xffffffff80850fb3 - 0xffffffff80210000), sizeof(bapm));
    notify("bapm 0x%08x\n", bapm);
    return 0;
}
