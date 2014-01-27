% Lecture 10
% CprE 308
% February 3, 2013

# Intro

## Today's Topics
 - Review

# Review

## Threads
 - Need to have multi-programming
    - Many processes executing in parallel
 - Why not use multiple processes?
    - Process creation expensive
    - Each process needs memory, lots of state
    - We don't need all that...
 - Multiple "threads" of control within a single process
 - Threads share process address space

## Process vs. Threads
![](img/process_v_thread.png)


## The Thread Model

| Per process Items | Per thread items |
|-------------------|------------------|
| Address space | Program counter |
| Global variables | Registers |
| Open files | Stack |
| Child processes | State |
| Pending alarms | |
| Signals and signal handlers | |
| Accounting information | |

## Threads
 - Threads = Parallelism within the same process

 - Well the following benefit from multiple threads?
    - Multiplying huge matrices on a single processor
    - The same problem on multiple processors
    - UNIX Shell

# Implementing Threads

## Threads Standards
 - The POSIX standard describes general thread behavior, and the functions which control threads
    - Individual operating systems are allowed some freedom in how threads are implemented, and hence how they behave
 - POSIX 1003.4a -> 1003.1c
    - variants
 - Microsoft
    - Win32

## Create a pthread
 - Use `pthread_create()` to add a new thread of control to the current process.

```c
int pthread_create(
  // The function writes t he thread id in tid
  pthread_t *tid,
  // The address of a thread attribute object is passed using tattr
  const pthread _attr_t *tattr,
  // The thread runs a function passed in start_routine
  void* (* start_routine) (void*),
  // The start routine's arguments are passed with arg
  void *arg);
```

## Creating a Thread
```c
start_servers() {
  pthread_t thread;
  int i;
  for (i = 0; i < nr_of_server_threads; i++)
    pthread_create(&thread, // thread ID
      0, // default attributes
      server, // start_routine
      argument); // argument
}

void *server(void *arg) {
  while(1) {
    /* get and handle request */
  }
}
```

## Complications
```c
func(int r_in, int r_out, int l_in, int l_out) {
  pthread_t in_thread, out_thread;

  pthread_create(&in_thread,
    0,
    incoming,
    r_in, l_out);  // Can't do this ...
  pthread_create(&out_thread,
    0,
    outgoing,
    l_in, r_out); // Can't do this ...
  /* How do we wait till they're done? */
}
```

## Multiple Arguments
```c
typedef struct {
  int first, second;
} two_ints_t;

func(int r_in, int r_out, int l_in, int l_out) {
  pthread_t in_thread, out_thread;
  two_ints_t in={r_in, l_out}, out={l_in, r_out};
  pthread_create(&in_thread,
    0,
    incoming,
    &in);						
  ...
}
```

## Thread attributes
Some POSIX thread (pthread) attributes include:

 - A thread may have local or global scope of contention
    - That is, it may compete with all threads in the system for CPU time, or it may compete only with threads in the same task (process)
 - A thread has a priority for scheduling
    - Threads may use several scheduling methods, some of which use priority
 - A thread may be detached
    - Only non-detached threads may be joined
    - `join` is to wait as thread is to process

## The threat attribute object
  
