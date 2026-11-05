// Test that fork fails gracefully.
// Tiny executable so that the limit can be filling the proc table.

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

#define NUM_PROCESS 4

void
print(const char *s)
{
  write(1, s, strlen(s));
}

void
forktest3(void)
{
    int pids[NUM_PROCESS];
    int workload[NUM_PROCESS] = {10000000, 1000, 500000000, 0};
    for (int n = 0; n < NUM_PROCESS; n++) {
      pids[n] = fork();
      if (pids[n] < 0) {
        print("Fork Failed\n");
        break;
      }
      if (pids[n] == 0) {
        for (int m = 0; m < workload[n]; m++) {}
        print("Child done\n");
        exit(0);
      }
    }
    for (int i = 0; i < NUM_PROCESS; i++) {
      wait(&pids[i]);
    }
    return;
}

int
main(void)
{
  forktest3();
  exit(0);
}
