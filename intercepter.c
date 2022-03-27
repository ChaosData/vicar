#define _GNU_SOURCE
#include <stdint.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>
#include <signal.h>
#include <asm/unistd.h>

#include <sys/prctl.h>

#include <stdio.h>

#include "./syktest/print_syscall.h"

#define PR_SET_SYSCALL_USER_DISPATCH 59
#define PR_SYS_DISPATCH_OFF 0
#define PR_SYS_DISPATCH_ON 1
#define SYSCALL_DISPATCH_FILTER_ALLOW 0
#define SYSCALL_DISPATCH_FILTER_BLOCK 1

static const uint8_t selector = SYSCALL_DISPATCH_FILTER_BLOCK;

typedef struct pages {
  uint8_t guard1[4096];
  struct {
    uint8_t write[4096];
    uint8_t rt_sigreturn[4096];
    uint8_t exit[4096];
  } syscalls;
  uint8_t guard2[4096];
} pages_t;

#define SA_RESTORER 0x04000000
struct real_sigaction {
  void (*_sa_sigaction)(int,siginfo_t*,void*);
  unsigned long _sa_flags;
  void (*_sa_restorer)(void);
  sigset_t _sa_mask;
};

static pages_t* pages = NULL;
static int remote_fd = 0;

void stdout_write(char const* buf, size_t len) {
  void(*_stdout_write)(char const*,size_t) = (void(*)(char const*,size_t))&pages->syscalls.write;
  (*_stdout_write)(buf,len);
}

void stdout_print(char const* str) {
  stdout_write(str, strlen(str));
}

void exit_wrapper(int code, void* _arg) {
  void(*_exit)(int) = (void(*)(int))&pages->syscalls.exit;
  (*_exit)(code);
}

static void syscall_handler(int sig, siginfo_t *info, void *ucontext) {
  volatile int foo = 0;
  //stdout_print("got a syscall\n");

  ucontext_t *context = (ucontext_t *)ucontext;
  greg_t *regs = context->uc_mcontext.gregs;
  void* fpregs = context->uc_mcontext.fpregs;

  char buf[256];
  snprintf(buf, sizeof(buf), "fpregs: %p, foo: %p\n", fpregs, &foo);
  stdout_print(buf);

  uint64_t rip = regs[REG_RIP];
  uint64_t rax = regs[REG_RAX];
  uint64_t rdi = regs[REG_RDI];
  uint64_t rsi = regs[REG_RSI];
  uint64_t rdx = regs[REG_RDX];
  uint64_t r10 = regs[REG_R10];
  uint64_t r8 = regs[REG_R8];
  uint64_t r9 = regs[REG_R9];
  uint64_t *rsp = (uint64_t *)regs[REG_RSP];

  stdout_print("syscall: ");
  print_syscall(rax, &stdout_print);
  snprintf(buf, sizeof(buf), "rip: 0x%lx, rax: 0x%lx, rdi: 0x%lx, rsi: 0x%lx, rdx: 0x%lx, \n"
                             "r10: 0x%lx, r8: 0x%lx, r9: 0x%lx, rsp: %p\n",
           rip, rax, rdi, rsi, rdx, r10, r8, r9, rsp);
  stdout_print(buf);
  regs[REG_EFL] = 0;
  regs[REG_RAX] = 42;

  // if this function returns, it goes into an injected rt_sigreturn syscall
  // added by glibc/the kernel. so we need to restore the entire context
  // ourselves, which is tricky. the sanest way to do this is to write all
  // of caught register values out to the signal stack, and then one by one,
  // pop them into the actual registers, with the final part being writing
  // rip to the address _after_ the real stack, updating rsp to that address,
  // and returning.
  // or not. we'll still run out of signal stack. we'd need to do
  // SA_SIGINFO|SA_RESTART and then rt_sigprocmask anyway. and the manual copy
  // would be more work.
}

void setup_harness() {
  remote_fd = socket(AF_INET, SOCK_STREAM, 0);

  struct sockaddr_in addr = {
    .sin_family = AF_INET,
    .sin_port = htons(9988),
    .sin_addr = (struct in_addr){
      .s_addr = 0x0100007f
    }
  };

  if(connect(remote_fd, (struct sockaddr*)&addr, sizeof(struct sockaddr_in)) != 0) {
    perror("connect");
    exit(1);
  }

  pages = (pages_t*)mmap(NULL, sizeof(pages_t), PROT_NONE, MAP_SHARED|MAP_ANONYMOUS, -1, 0);
  if (pages == (void*)-1) {
    perror("mmap");
    exit(1);
  }

  if(mprotect(&pages->syscalls, sizeof(pages->syscalls), PROT_READ|PROT_WRITE) != 0) {
    perror("mprotect1");
    exit(1);
  }

  //note: this is not a sandbox, code could just jump to the syscall instruction w/ arbitrary values
  char const write_shellcode[] = ""
    "\x48\x89\xf2" // mov rdx, rsi // len
    "\x48\x89\xfe" // mov rsi, rdi // data
    "\x48\x31\xc0\x48\xff\xc0" // xor rax, rax; inc rax // syscall 0x1
    "\x48\x89\xc7" // mov rdi, rax // fd (1: stdout)
    "\x0f\x05" // syscall
    "\xc3" // ret
  ;

  char const rt_sigreturn_shellcode[] = ""
    "\x48\xc7\xc0\x0f\x00\x00\x00" // mov rax, 0xf
    "\x0f\x05" // syscall
  ;

  char const exit_shellcode[] = ""
    "\x48\xc7\xc0\x3c\x00\x00\x00" // mov rax, 60
    "\x0f\x05" // syscall
  ;

  memset(&pages->syscalls.write, '\xf4', 4096);
  memcpy(&pages->syscalls.write, write_shellcode, sizeof(write_shellcode)-1);

  memset(&pages->syscalls.rt_sigreturn, '\xf4', 4096);
  memcpy(&pages->syscalls.rt_sigreturn, rt_sigreturn_shellcode, sizeof(rt_sigreturn_shellcode)-1);

  memset(&pages->syscalls.exit, '\xf4', 4096);
  memcpy(&pages->syscalls.exit, exit_shellcode, sizeof(exit_shellcode)-1);

  if(mprotect(&pages->syscalls, sizeof(pages->syscalls), PROT_READ|PROT_EXEC) != 0) {
    perror("mprotect2");
    exit(1);
  }

  // we can't use the glibc sigaction b/c it injects its own garbage
  // rt_sigreturn syscall that we can't trust
  /*
  struct sigaction act;
  sigset_t mask;
  memset(&act, 0, sizeof(act));
  sigemptyset(&mask);

  act.sa_sigaction = syscall_handler;
  act.sa_flags = SA_SIGINFO | SA_NODEFER;
  act.sa_mask = mask;

  if (sigaction(SIGSYS, &act, NULL) != 0) {
    perror("sigaction");
    exit(1);
  }
  */

  struct real_sigaction rs = {0};
  sigset_t mask;
  sigemptyset(&mask);

  rs._sa_sigaction = syscall_handler;
  rs._sa_flags = SA_SIGINFO | SA_NODEFER | SA_RESTORER;
  rs._sa_mask = mask;
  rs._sa_restorer = (void(*)(void))&pages->syscalls.rt_sigreturn;

  if (syscall(__NR_rt_sigaction, SIGSYS, &rs, NULL, 8) != 0) {
    perror("sigaction");
    exit(1);
  }

  on_exit(exit_wrapper, NULL);

  //printf("sizeof(sigset_t): %lu\n", sizeof(sigset_t));
  //printf("sizeof(__sigset_t): %lu\n", sizeof(__sigset_t));
  //printf("__NSIG_BYTES: %lu\n", __NSIG_BYTES);

  //uint8_t selector = SYSCALL_DISPATCH_FILTER_BLOCK;
  prctl(PR_SET_SYSCALL_USER_DISPATCH, PR_SYS_DISPATCH_ON, &pages->syscalls, sizeof(pages->syscalls), &selector);
}

int main(int argc, char** argv) {
  setup_harness();

  stdout_print("hello world\n");
  //puts("hello");
  int r = write(1, "hello\n", 6);
  char buf[128];
  snprintf(buf, sizeof(buf), "r: %d\n", r);
  stdout_print(buf);

  return 0;
}
