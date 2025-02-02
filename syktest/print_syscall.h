
static void print_syscall(int id, void(*print_func)(char const*)) {
  switch (id) {
    case 0: { (*print_func)("read\n"); break; }
    case 1: { (*print_func)("write\n"); break; }
    case 2: { (*print_func)("open\n"); break; }
    case 3: { (*print_func)("close\n"); break; }
    case 4: { (*print_func)("stat\n"); break; }
    case 5: { (*print_func)("fstat\n"); break; }
    case 6: { (*print_func)("lstat\n"); break; }
    case 7: { (*print_func)("poll\n"); break; }
    case 8: { (*print_func)("lseek\n"); break; }
    case 9: { (*print_func)("mmap\n"); break; }
    case 10: { (*print_func)("mprotect\n"); break; }
    case 11: { (*print_func)("munmap\n"); break; }
    case 12: { (*print_func)("brk\n"); break; }
    case 13: { (*print_func)("rt_sigaction\n"); break; }
    case 14: { (*print_func)("rt_sigprocmask\n"); break; }
    case 15: { (*print_func)("rt_sigreturn\n"); break; }
    case 16: { (*print_func)("ioctl\n"); break; }
    case 17: { (*print_func)("pread64\n"); break; }
    case 18: { (*print_func)("pwrite64\n"); break; }
    case 19: { (*print_func)("readv\n"); break; }
    case 20: { (*print_func)("writev\n"); break; }
    case 22: { (*print_func)("pipe\n"); break; }
    case 23: { (*print_func)("select\n"); break; }
    case 24: { (*print_func)("sched_yield\n"); break; }
    case 25: { (*print_func)("mremap\n"); break; }
    case 26: { (*print_func)("msync\n"); break; }
    case 27: { (*print_func)("mincore\n"); break; }
    case 28: { (*print_func)("madvise\n"); break; }
    case 32: { (*print_func)("dup\n"); break; }
    case 33: { (*print_func)("dup2\n"); break; }
    case 34: { (*print_func)("pause\n"); break; }
    case 35: { (*print_func)("nanosleep\n"); break; }
    case 36: { (*print_func)("getitimer\n"); break; }
    case 37: { (*print_func)("alarm\n"); break; }
    case 38: { (*print_func)("setitimer\n"); break; }
    case 39: { (*print_func)("getpid\n"); break; }
    case 40: { (*print_func)("sendfile\n"); break; }
    case 56: { (*print_func)("clone\n"); break; }
    case 60: { (*print_func)("exit\n"); break; }
    case 61: { (*print_func)("wait4\n"); break; }
    case 63: { (*print_func)("uname\n"); break; }
    case 72: { (*print_func)("fcntl\n"); break; }
    case 73: { (*print_func)("flock\n"); break; }
    case 74: { (*print_func)("fsync\n"); break; }
    case 75: { (*print_func)("fdatasync\n"); break; }
    case 76: { (*print_func)("truncate\n"); break; }
    case 77: { (*print_func)("ftruncate\n"); break; }
    case 78: { (*print_func)("getdents\n"); break; }
    case 79: { (*print_func)("getcwd\n"); break; }
    case 80: { (*print_func)("chdir\n"); break; }
    case 81: { (*print_func)("fchdir\n"); break; }
    case 82: { (*print_func)("rename\n"); break; }
    case 83: { (*print_func)("mkdir\n"); break; }
    case 84: { (*print_func)("rmdir\n"); break; }
    case 85: { (*print_func)("creat\n"); break; }
    case 86: { (*print_func)("link\n"); break; }
    case 87: { (*print_func)("unlink\n"); break; }
    case 88: { (*print_func)("symlink\n"); break; }
    case 89: { (*print_func)("readlink\n"); break; }
    case 90: { (*print_func)("chmod\n"); break; }
    case 91: { (*print_func)("fchmod\n"); break; }
    case 92: { (*print_func)("chown\n"); break; }
    case 93: { (*print_func)("fchown\n"); break; }
    case 94: { (*print_func)("lchown\n"); break; }
    case 97: { (*print_func)("getrlimit\n"); break; }
    case 98: { (*print_func)("getrusage\n"); break; }
    case 99: { (*print_func)("sysinfo\n"); break; }
    case 100: { (*print_func)("times\n"); break; }
    case 101: { (*print_func)("ptrace\n"); break; }
    case 102: { (*print_func)("getuid\n"); break; }
    case 104: { (*print_func)("getgid\n"); break; }
    case 105: { (*print_func)("setuid\n"); break; }
    case 106: { (*print_func)("setgid\n"); break; }
    case 107: { (*print_func)("geteuid\n"); break; }
    case 108: { (*print_func)("getegid\n"); break; }
    case 109: { (*print_func)("setpgid\n"); break; }
    case 111: { (*print_func)("getpgrp\n"); break; }
    case 113: { (*print_func)("setreuid\n"); break; }
    case 114: { (*print_func)("setregid\n"); break; }
    case 115: { (*print_func)("getgroups\n"); break; }
    case 116: { (*print_func)("setgroups\n"); break; }
    case 117: { (*print_func)("setresuid\n"); break; }
    case 118: { (*print_func)("getresuid\n"); break; }
    case 119: { (*print_func)("setresgid\n"); break; }
    case 120: { (*print_func)("getresgid\n"); break; }
    case 121: { (*print_func)("getpgid\n"); break; }
    case 122: { (*print_func)("setfsuid\n"); break; }
    case 123: { (*print_func)("setfsgid\n"); break; }
    case 125: { (*print_func)("capget\n"); break; }
    case 126: { (*print_func)("capset\n"); break; }
    case 127: { (*print_func)("rt_sigpending\n"); break; }
    case 128: { (*print_func)("rt_sigtimedwait\n"); break; }
    case 129: { (*print_func)("rt_sigqueueinfo\n"); break; }
    case 130: { (*print_func)("rt_sigsuspend\n"); break; }
    case 131: { (*print_func)("sigaltstack\n"); break; }
    case 132: { (*print_func)("utime\n"); break; }
    case 133: { (*print_func)("mknod\n"); break; }
    case 134: { (*print_func)("uselib\n"); break; }
    case 135: { (*print_func)("personality\n"); break; }
    case 136: { (*print_func)("ustat\n"); break; }
    case 137: { (*print_func)("statfs\n"); break; }
    case 138: { (*print_func)("fstatfs\n"); break; }
    case 139: { (*print_func)("sysfs\n"); break; }
    case 140: { (*print_func)("getpriority\n"); break; }
    case 141: { (*print_func)("setpriority\n"); break; }
    case 142: { (*print_func)("sched_setparam\n"); break; }
    case 143: { (*print_func)("sched_getparam\n"); break; }
    case 144: { (*print_func)("sched_setscheduler\n"); break; }
    case 145: { (*print_func)("sched_getscheduler\n"); break; }
    case 148: { (*print_func)("sched_rr_get_interval\n"); break; }
    case 149: { (*print_func)("mlock\n"); break; }
    case 150: { (*print_func)("munlock\n"); break; }
    case 151: { (*print_func)("mlockall\n"); break; }
    case 152: { (*print_func)("munlockall\n"); break; }
    case 154: { (*print_func)("modify_ldt\n"); break; }
    case 155: { (*print_func)("pivot_root\n"); break; }
    case 160: { (*print_func)("setrlimit\n"); break; }
    case 161: { (*print_func)("chroot\n"); break; }
    case 162: { (*print_func)("sync\n"); break; }
    case 163: { (*print_func)("acct\n"); break; }
    case 172: { (*print_func)("iopl\n"); break; }
    case 173: { (*print_func)("ioperm\n"); break; }
    case 175: { (*print_func)("init_module\n"); break; }
    case 176: { (*print_func)("delete_module\n"); break; }
    case 179: { (*print_func)("quotactl\n"); break; }
    case 186: { (*print_func)("gettid\n"); break; }
    case 187: { (*print_func)("readahead\n"); break; }
    case 200: { (*print_func)("tkill\n"); break; }
    case 201: { (*print_func)("time\n"); break; }
    case 203: { (*print_func)("sched_setaffinity\n"); break; }
    case 204: { (*print_func)("sched_getaffinity\n"); break; }
    case 205: { (*print_func)("set_thread_area\n"); break; }
    case 211: { (*print_func)("get_thread_area\n"); break; }
    case 212: { (*print_func)("lookup_dcookie\n"); break; }
    case 213: { (*print_func)("epoll_create\n"); break; }
    case 216: { (*print_func)("remap_file_pages\n"); break; }
    case 217: { (*print_func)("getdents64\n"); break; }
    case 218: { (*print_func)("set_tid_address\n"); break; }
    case 219: { (*print_func)("restart_syscall\n"); break; }
    case 221: { (*print_func)("fadvise64\n"); break; }
    case 222: { (*print_func)("timer_create\n"); break; }
    case 223: { (*print_func)("timer_settime\n"); break; }
    case 224: { (*print_func)("timer_gettime\n"); break; }
    case 225: { (*print_func)("timer_getoverrun\n"); break; }
    case 226: { (*print_func)("timer_delete\n"); break; }
    case 227: { (*print_func)("clock_settime\n"); break; }
    case 228: { (*print_func)("clock_gettime\n"); break; }
    case 229: { (*print_func)("clock_getres\n"); break; }
    case 230: { (*print_func)("clock_nanosleep\n"); break; }
    case 231: { (*print_func)("exit_group\n"); break; }
    case 232: { (*print_func)("epoll_wait\n"); break; }
    case 233: { (*print_func)("epoll_ctl\n"); break; }
    case 234: { (*print_func)("tgkill\n"); break; }
    case 235: { (*print_func)("utimes\n"); break; }
    case 237: { (*print_func)("mbind\n"); break; }
    case 238: { (*print_func)("set_mempolicy\n"); break; }
    case 239: { (*print_func)("get_mempolicy\n"); break; }
    case 240: { (*print_func)("mq_open\n"); break; }
    case 241: { (*print_func)("mq_unlink\n"); break; }
    case 242: { (*print_func)("mq_timedsend\n"); break; }
    case 243: { (*print_func)("mq_timedreceive\n"); break; }
    case 244: { (*print_func)("mq_notify\n"); break; }
    case 245: { (*print_func)("mq_getsetattr\n"); break; }
    case 246: { (*print_func)("kexec_load\n"); break; }
    case 247: { (*print_func)("waitid\n"); break; }
    case 251: { (*print_func)("ioprio_set\n"); break; }
    case 252: { (*print_func)("ioprio_get\n"); break; }
    case 256: { (*print_func)("migrate_pages\n"); break; }
    case 257: { (*print_func)("openat\n"); break; }
    case 258: { (*print_func)("mkdirat\n"); break; }
    case 259: { (*print_func)("mknodat\n"); break; }
    case 260: { (*print_func)("fchownat\n"); break; }
    case 261: { (*print_func)("futimesat\n"); break; }
    case 262: { (*print_func)("newfstatat\n"); break; }
    case 263: { (*print_func)("unlinkat\n"); break; }
    case 264: { (*print_func)("renameat\n"); break; }
    case 265: { (*print_func)("linkat\n"); break; }
    case 266: { (*print_func)("symlinkat\n"); break; }
    case 267: { (*print_func)("readlinkat\n"); break; }
    case 268: { (*print_func)("fchmodat\n"); break; }
    case 269: { (*print_func)("faccessat\n"); break; }
    case 270: { (*print_func)("pselect6\n"); break; }
    case 271: { (*print_func)("ppoll\n"); break; }
    case 275: { (*print_func)("splice\n"); break; }
    case 276: { (*print_func)("tee\n"); break; }
    case 277: { (*print_func)("sync_file_range\n"); break; }
    case 278: { (*print_func)("vmsplice\n"); break; }
    case 279: { (*print_func)("move_pages\n"); break; }
    case 280: { (*print_func)("utimensat\n"); break; }
    case 281: { (*print_func)("epoll_pwait\n"); break; }
    case 282: { (*print_func)("signalfd\n"); break; }
    case 283: { (*print_func)("timerfd_create\n"); break; }
    case 284: { (*print_func)("eventfd\n"); break; }
    case 285: { (*print_func)("fallocate\n"); break; }
    case 286: { (*print_func)("timerfd_settime\n"); break; }
    case 287: { (*print_func)("timerfd_gettime\n"); break; }
    case 289: { (*print_func)("signalfd4\n"); break; }
    case 290: { (*print_func)("eventfd2\n"); break; }
    case 291: { (*print_func)("epoll_create1\n"); break; }
    case 292: { (*print_func)("dup3\n"); break; }
    case 293: { (*print_func)("pipe2\n"); break; }
    case 295: { (*print_func)("preadv\n"); break; }
    case 296: { (*print_func)("pwritev\n"); break; }
    case 297: { (*print_func)("rt_tgsigqueueinfo\n"); break; }
    case 302: { (*print_func)("prlimit64\n"); break; }
    case 303: { (*print_func)("name_to_handle_at\n"); break; }
    case 304: { (*print_func)("open_by_handle_at\n"); break; }
    case 305: { (*print_func)("clock_adjtime\n"); break; }
    case 306: { (*print_func)("syncfs\n"); break; }
    case 310: { (*print_func)("process_vm_readv\n"); break; }
    case 311: { (*print_func)("process_vm_writev\n"); break; }
    case 312: { (*print_func)("kcmp\n"); break; }
    case 313: { (*print_func)("finit_module\n"); break; }
    case 314: { (*print_func)("sched_setattr\n"); break; }
    case 315: { (*print_func)("sched_getattr\n"); break; }
    case 316: { (*print_func)("renameat2\n"); break; }
    case 318: { (*print_func)("getrandom\n"); break; }
    case 319: { (*print_func)("memfd_create\n"); break; }
    case 324: { (*print_func)("membarrier\n"); break; }
    case 325: { (*print_func)("mlock2\n"); break; }
    case 326: { (*print_func)("copy_file_range\n"); break; }
    case 327: { (*print_func)("preadv2\n"); break; }
    case 328: { (*print_func)("pwritev2\n"); break; }
    case 329: { (*print_func)("pkey_mprotect\n"); break; }
    case 330: { (*print_func)("pkey_alloc\n"); break; }
    case 331: { (*print_func)("pkey_free\n"); break; }
    case 332: { (*print_func)("statx\n"); break; }
    case 334: { (*print_func)("rseq\n"); break; }
    case 435: { (*print_func)("clone3\n"); break; }

    default: {
      char buf[128];
      snprintf(buf, sizeof(buf), "unknown syscall id: %d\n", id);
      (*print_func)(buf);
    }
  }
}

