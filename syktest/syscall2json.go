package main

import (
  "github.com/google/syzkaller/pkg/ast"
  "fmt"
  "os"
  //"reflect"
  "strings"
  "encoding/json"
)

type Type struct {
  Name string `json:"name"`
  Args []string `json:"args,omitempty"`
}

type Arg struct {
  Name string `json:"name"`
  Type Type `json:"type"`
}

type Syscall struct {
  Name string `json:"name"`
  Args []Arg `json:"args"`
  Ret *string `json:"ret,omitempty"`
}

func main() {
  data, _ := os.ReadFile("./sys.txt")

  desc := ast.Parse(data, "sys.txt", func(pos ast.Pos, msg string) {
    fmt.Printf("failed to parse: [%s] %+v\n", msg, pos)
  })

  var syscalls []Syscall

  desc.Walk(func(n ast.Node) {
    //fmt.Printf("n: [%T] %+v\n", n, n)
    if c, ok := n.(*ast.Call); ok {
      if strings.HasPrefix(c.Name.Name, "syz_") {
        return
      }
      if c.Name.Name != c.CallName {
        return
      }

      s := Syscall{}
      s.Name = c.CallName

      if c.Ret != nil {
        s.Ret = &c.Ret.Ident
      }

      for _, a := range c.Args {
        //fmt.Printf("%s %+v", a.Name.Name, a.Type)
        arg := Arg{}
        arg.Name = a.Name.Name
        arg.Type.Name = a.Type.Ident

        if len(a.Type.Args) > 0 {
          for _, aa := range a.Type.Args {
            arg.Type.Args = append(arg.Type.Args, aa.Ident)
          }
        }
        s.Args = append(s.Args, arg)
      }
      syscalls = append(syscalls, s)
    }
  })

  jb, err := json.Marshal(syscalls)
  if err != nil {
    fmt.Println(err)
  }
  fmt.Print(string(jb))
}
