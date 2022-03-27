import sys
import json

if len(sys.argv) < 2:
  print("usage: python3 {} <syscalls.json>".format(sys.argv[0]))
  sys.exit(1)

functemplate = '''
static void print_syscall(int id, void(*print_func)(char const*)) {{
  switch (id) {{
{}
    default: {{
      char buf[128];
      snprintf(buf, sizeof(buf), "unknown syscall id: %d\\n", id);
      (*print_func)(buf);
    }}
  }}
}}
'''

syscalls = json.loads(open(sys.argv[1],'r').read())
syscalls_inv = {}

for k,v in syscalls.items():
  if v != "???":
    syscalls_inv[int(v)] = k

cases = ""

for i in sorted(syscalls_inv.keys()):
  cases += '''\
    case {}: {{ (*print_func)("{}\\n"); break; }}
'''.format(i, syscalls_inv[i].split('__NR_')[1])

print(functemplate.format(cases))
