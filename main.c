#include <stdlib.h>
#include <unistd.h>

int main(void) {
  if (getpid() != 1) {
    return 1;
  }
}
