
#include "types.h"
#include "param.h"
#include "memlayout.h"
#include "riscv.h"
#include "spinlock.h"
#include "proc.h"
#include "defs.h"
// scan children for zombies, takes in the address of a chidlstats struct
// (for now it just prints) each and then returns the number of children that are zombies
int
childstats(uint64 addr)
{
  struct proc *pp;
  int havekids, pid, numzombies;
  struct proc *p = myproc();

  acquire(&wait_lock);

  // Scan through table looking for children that are zombies
  havekids = 0;
  numzombies = 0;
  for (pp = proc; pp < &proc[NPROC]; pp++) {
    if (pp->parent == p) {
      // make sure the child isn't still in exit() or swtch().
      acquire(&pp->lock);

      havekids = 1;
      if (pp->state == ZOMBIE) {
        // Found one.
        pid = pp->pid;
        numzombies++;
        printf("hello, %d %d %d %d %d\n", pp->arrival, pp->first_run,
               pp->completion, pp->duration, pp->num_context_switches);

        //        if (addr != 0 && copyout(p->pagetable, p->sz, addr, (char *)&pp->xstate,
        //                                 sizeof(pp->xstate)) < 0) {
        //          release(&pp->lock);
        //          release(&wait_lock);
        //          //return -1;
        //        }
        pp->parent = 0;
        freeproc(pp);
        release(&pp->lock);
        release(&wait_lock);
        //return pid;
      }
      release(&pp->lock);
    }
  }
  return numzombies;

  //    // No point waiting if we don't have any children.
  //    if (!havekids || killed(p)) {
  //      release(&wait_lock);
  //      return -1;
  //    }
  //
  //    // Wait for a child to exit.
  //    sleep_prepare(p); //DOC: wait-sleep
  //    release(&wait_lock);
  //    sleep();
  //    acquire(&wait_lock);
}
