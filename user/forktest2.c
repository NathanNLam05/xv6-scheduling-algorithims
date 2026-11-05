// Test that fork fails gracefully.
// Tiny executable so that the limit can be filling the proc table.

#include "kernel/fcntl.h" // if your user program needs the O_* flags
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
//#include <cstdio>
struct procstats {
  int arrival;
  int first_run;
  int duration;
  int completion;
  int context_switches;
};

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
    //struct procstats *results = malloc(sizeof(struct procstats));
    struct procstats results; // = malloc(sizeof(struct procstats));
    childstats(pid, &results);

    int fd = open("logfile.txt", O_CREATE | O_WRONLY);
    if (fd >= 0) {
      fprintf(
        fd,
        "arrival=%d first_run=%d duration=%d completion=%d context_switches=%d\n",
        results.arrival, results.first_run, results.duration,
        results.completion, results.context_switches);
      close(fd);
    }
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
