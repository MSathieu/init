#include <__/syscall.h>
#include <capability.h>
#include <spawn.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

struct process {
  char* name;
  pid_t pid;
};

static struct process processes[] = {{"ipcd", 0}, {"fbd", 0}, {"kbdd", 0}, {"ps2d", 0}, {"ttyd", 0}};

static void spawn(const char* name) {
  for (size_t i = 0; i < sizeof(processes) / sizeof(struct process); i++) {
    if (!strcmp(processes[i].name, name)) {
      processes[i].pid = spawn_process(name);
      break;
    }
  }
  if (!strcmp(name, "fbd")) {
    grant_capability(CAP_NAMESPACE_KERNEL, CAP_KERNEL_GET_FB_INFO);
    grant_capability(CAP_NAMESPACE_SERVERS, CAP_IPCD_REGISTER);
    uintptr_t fb_phys_addr = _syscall(_SYSCALL_GET_FB_INFO, 0, 0, 0, 0, 0);
    size_t height = _syscall(_SYSCALL_GET_FB_INFO, 2, 0, 0, 0, 0);
    size_t pitch = _syscall(_SYSCALL_GET_FB_INFO, 3, 0, 0, 0, 0);
    map_memory(fb_phys_addr, height * pitch);
  } else if (!strcmp(name, "kbdd")) {
    grant_capability(CAP_NAMESPACE_SERVERS, CAP_IPCD_REGISTER);
    grant_capability(CAP_NAMESPACE_SERVERS, CAP_KBDD);
  } else if (!strcmp(name, "ps2d")) {
    grant_capability(CAP_NAMESPACE_SERVERS, CAP_KBDD_SEND_KEYPRESS);
    grant_ioport(0x60);
    grant_ioport(0x64);
    register_irq(1);
  } else if (!strcmp(name, "ttyd")) {
    grant_capability(CAP_NAMESPACE_DRIVERS, CAP_FBD_DRAW);
    grant_capability(CAP_NAMESPACE_SERVERS, CAP_KBDD_RECEIVE_EVENTS);
  }
  start_process();
}
int main(void) {
  if (getpid() != 1) {
    return 1;
  }
  spawn("ipcd");
  spawn("fbd");
  spawn("kbdd");
  spawn("ps2d");
  spawn("ttyd");
  while (1) {
    pid_t pid = wait(0);
    for (size_t i = 0; i < sizeof(processes) / sizeof(struct process); i++) {
      if (processes[i].pid == pid) {
        if (!strcmp(processes[i].name, "ipcd")) {
          return 1;
        }
        spawn(processes[i].name);
        break;
      }
    }
  }
}
