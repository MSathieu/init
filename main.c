#include <__/syscall.h>
#include <capability.h>
#include <spawn.h>
#include <unistd.h>

int main(void) {
  if (getpid() != 1) {
    return 1;
  }
  spawn_process("ipcd");
  grant_capability(CAP_NAMESPACE_KERNEL, CAP_KERNEL_GET_CAPS);
  start_process();
  spawn_process("fbd");
  grant_capability(CAP_NAMESPACE_KERNEL, CAP_KERNEL_GET_CAPS);
  grant_capability(CAP_NAMESPACE_KERNEL, CAP_KERNEL_GET_FB_INFO);
  grant_capability(CAP_NAMESPACE_SERVERS, CAP_IPCD_REGISTER);
  uintptr_t fb_phys_addr = _syscall(_SYSCALL_GET_FB_INFO, 0, 0, 0, 0, 0);
  size_t height = _syscall(_SYSCALL_GET_FB_INFO, 2, 0, 0, 0, 0);
  size_t pitch = _syscall(_SYSCALL_GET_FB_INFO, 3, 0, 0, 0, 0);
  map_memory(fb_phys_addr, height * pitch);
  start_process();
  spawn_process("kbdd");
  grant_capability(CAP_NAMESPACE_KERNEL, CAP_KERNEL_GET_CAPS);
  grant_capability(CAP_NAMESPACE_SERVERS, CAP_IPCD_REGISTER);
  start_process();
  spawn_process("ps2d");
  grant_capability(CAP_NAMESPACE_SERVERS, CAP_KBDD_SEND_KEYPRESS);
  grant_ioport(0x60);
  grant_ioport(0x64);
  register_irq(1);
  start_process();
  spawn_process("ttyd");
  grant_capability(CAP_NAMESPACE_KERNEL, CAP_KERNEL_GET_FB_INFO);
  grant_capability(CAP_NAMESPACE_DRIVERS, CAP_FBD_DRAW);
  start_process();
}
