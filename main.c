#include <spawn.h>
#include <unistd.h>

int main(void) {
  if (getpid() != 1) {
    return 1;
  }
  spawn_process("ps2");
  start_process();
}
