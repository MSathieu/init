#include <spawn.h>
#include <unistd.h>

int main(void) {
  if (getpid() != 1) {
    return 1;
  }
  spawn_process("ps2");
  grant_ioport(0x60);
  grant_ioport(0x64);
  start_process();
}
