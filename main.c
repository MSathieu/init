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
  grant_capability(CAP_NAMESPACE_KERNEL, CAP_KERNEL_GET_FB_INFO);
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
}
