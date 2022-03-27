import sys
import json

if len(sys.argv) < 2:
  print("usage: python3 idmap.py <arch>")
  sys.exit(1)

arch = sys.argv[1]

consts = open('./sys.txt.const','r').read()

syscall_lines = [line for line in consts.split('\n') if line.startswith("__NR_")]

syscalls = {}

for s in syscall_lines:
  sp = s.split(' = ')
  vals = sp[1].split(', ')
  if ':' in vals[0]:
    syscalls[sp[0]] = {}
    valssp = vals[0].split(':')
    syscalls[sp[0]][valssp[0]] = valssp[1]
  else:
    syscalls[sp[0]] = {
      'i386': vals[0]
    }
  pairs = [val.split(':') for val in vals[1:]]
  for p in pairs:
    if len(p) == 2:
      syscalls[sp[0]][p[0]] = p[1]
    elif len(p) > 2:
      val = p[-1]
      arches = p[:-1]
      for a in arches:
        syscalls[sp[0]][a] = val

syscalls_arch = {}
for k, v in syscalls.items():
  #print(k + " " + str(v))
  if arch in v:
    syscalls_arch[k] = v[arch]
  else:
    if k == '__NR_clone3' and arch == 'amd64':
      syscalls_arch[k] = '435'
    else:
      syscalls_arch[k] = '???'

print(json.dumps(syscalls_arch))
