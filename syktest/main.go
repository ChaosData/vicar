package main

import (
  "github.com/google/syzkaller/pkg/ast"
  "fmt"
  "os"
  //"reflect"
  "strings"
)

func dumpStruct(s *ast.Struct, isStruct bool) {
  if isStruct {
    fmt.Printf("struct: %s (struct), union: %v\n", s.Name.Name, s.IsUnion)
  } else {
    fmt.Printf("struct: %s (typedef), union: %v\n", s.Name.Name, s.IsUnion)
  }
  if len(s.Attrs) > 0 {
    fmt.Printf("  attrs: ")
    for _, a := range s.Attrs {
      fmt.Printf("%s", a.Ident)
      if len(a.Args) > 0 {
        fmt.Printf("[")
        for _, aa := range a.Args {
          if aa.Ident != "" {
            fmt.Printf("%s, ", aa.Ident)
          } else if aa.String != "" {
            fmt.Printf("%s, ", aa.String)
          } else {
            fmt.Printf("%d, ", uint64(aa.Value))
          }
        }
        fmt.Printf("]")
      }
      fmt.Printf(", ")
    }
    fmt.Printf("\n")
  }
  fmt.Printf("  fields:\n")
  for _, f := range s.Fields {
    fmt.Printf("    name:%s, type:%s", f.Name.Name, f.Type.Ident)
    if len(f.Type.Args) > 0 {
      fmt.Printf("[")
      for _, a := range f.Type.Args {
        if a.Ident != "" {
          fmt.Printf("%s, ", a.Ident)
        } else if a.String != "" {
          fmt.Printf("%s, ", a.String)
        } else {
          fmt.Printf("%d, ", uint64(a.Value))
        }
      }
      fmt.Printf("]")
    }
    if len(f.Attrs) > 0 { //note: doesn't appear to be used
      fmt.Printf(", attrs: ")
      for _, a := range f.Attrs {
        fmt.Printf("%+v, ", a)
      }
    }
    fmt.Printf("\n")
  }
  //fmt.Printf("\n")
}

func main() {
  data, _ := os.ReadFile("./sys.txt")

  desc := ast.Parse(data, "sys.txt", func(pos ast.Pos, msg string) {
    fmt.Printf("failed to parse: [%s] %+v\n", msg, pos)
  })

  //data2 := ast.Format(desc)
  fmt.Println("success\n====")
  //fmt.Println(string(data2))

  desc.Walk(func(n ast.Node) {
    if c, ok := n.(*ast.Call); ok {
      return
      if strings.HasPrefix(c.Name.Name, "syz_") {
        return
      }
      if c.Name.Name != c.CallName {
        return
      }

      fmt.Printf("name: %s, callname: %s, nr: %d\n",  c.Name.Name, c.CallName, c.NR)
      if c.Ret != nil {
        fmt.Printf("  ret: %s\n", c.Ret.Ident)
      }
      fmt.Printf("  args: ")
      for _, a := range c.Args {
        //fmt.Printf("%s %+v", a.Name.Name, a.Type)
        fmt.Printf("name:%s type:%s", a.Name.Name, a.Type.Ident)
        if len(a.Type.Args) > 0 {
          fmt.Printf("[")
          for _, aa := range a.Type.Args {
            fmt.Printf("%s, ", aa.Ident)
          }
          fmt.Printf("]")
        }
        fmt.Printf(", ")
      }
      if len(c.Attrs) > 0 {
        fmt.Printf("\n  attrs: ")
        for _, a := range c.Attrs {
          fmt.Printf("%s, ", a.Ident)
        }
      }
      fmt.Printf("\n")
    } else if i, ok := n.(*ast.Include); ok {
      fmt.Printf("include:")
      if i.File != nil {
        fmt.Printf(" %s", i.File.Value)
      }
      fmt.Printf("\n")
    } else if d, ok := n.(*ast.Define); ok {
      fmt.Printf("define: %s", d.Name.Name)
      if d.Value != nil {
        if d.Value.Ident != "" {
          fmt.Printf(" %s", d.Value.Ident)
        } else if d.Value.CExpr != "" {
          fmt.Printf(" %s", d.Value.CExpr)
        } else {
          fmt.Printf(" %d", uint64(d.Value.Value))
        }
      }
      fmt.Printf("\n")
    } else if t, ok := n.(*ast.TypeDef); ok {
      fmt.Printf("typedef: %s", t.Name.Name)
      if len(t.Args) > 0 {
        fmt.Printf(" (template)\n")
        fmt.Printf("  args: ")
        for _, a := range t.Args {
          fmt.Printf("%s, ", a.Name)
        }
        fmt.Printf("\n")
      } else {
        fmt.Printf("(non-template) \n")
      }
      if t.Type != nil {
        fmt.Printf("  type: %s\n", t.Type.Ident)
      }
      if t.Struct != nil {
        dumpStruct(t.Struct, false)
      }
    } else if s, ok := n.(*ast.Struct); ok {
      dumpStruct(s, true)
    }
    //fmt.Printf("n: [%T] %+v\n", n, n)

  })
}
