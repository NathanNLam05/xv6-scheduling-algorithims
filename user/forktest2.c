// Test that fork fails gracefully.
// Tiny executable so that the limit can be filling the proc table.

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define N 1000

void
print(const char *s)
{
  write(1, s, strlen(s));
}

void
forktest2(void)
{
  int pid = fork();

  if (pid < 0) {
    print("Fork Failed\n");
    return;
  } else if (pid == 0) {
    print("Hello from Child!\n");
  } else {
    pause(10);
    childstats();
    wait(0);
    print("Hello from Parent!\n");
  }

  return;
}

int
main(void)
{
  forktest2();
  exit(0);
}
