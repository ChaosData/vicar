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

type Define struct {
  Name string `json:"name"`
  Value string `json:"value"`
}

type TypeDef struct {
  Name string `json:"name"`
  Type string `json:"type"`
}

type Struct struct {
  Name string `json:"name"`
  Union bool `json:"union"`
  Attrs []Type `json:"attrs,omitempty"`
  Fields []Arg `json:"fields"`
}

type Template struct {
  Name string `json:"name"`
  Args []string `json:"args"`
  Type Type `json:"type,omitempty"`
  Struct Struct `json:"struct,omitempty"`
}

type Output struct {
  Includes []string `json:"includes"`
  Defines []Define `json:"defines"`
  TypeDefs []TypeDef `json:"typedefs"`
  Templates []Template `json:"templates"`
  Structs []Struct `json:"structs"`
  Syscalls []Syscall `json:"syscalls"`
}

func dumpStruct(s *ast.Struct) Struct {
  st := Struct{}
  st.Name = s.Name.Name
  st.Union = s.IsUnion

  if len(s.Attrs) > 0 {
    for _, a := range s.Attrs {
      attr := Type{}
      attr.Name = a.Ident
      if len(a.Args) > 0 {
        for _, aa := range a.Args {
          if aa.Ident != "" {
            attr.Args = append(attr.Args, aa.Ident)
          } else if aa.String != "" {
            attr.Args = append(attr.Args, aa.String)
          } else {
            attr.Args = append(attr.Args, fmt.Sprintf("%d", uint64(aa.Value)))
          }
        }
      }
      st.Attrs = append(st.Attrs, attr)
    }
  }
  for _, f := range s.Fields {
    field := Arg{}
    field.Name = f.Name.Name
    field.Type.Name = f.Type.Ident

    if len(f.Type.Args) > 0 {
      for _, a := range f.Type.Args {
        if a.Ident != "" {
          field.Type.Args = append(field.Type.Args, a.Ident)
        } else if a.String != "" {
          field.Type.Args = append(field.Type.Args, a.String)
        } else {
          field.Type.Args = append(field.Type.Args, fmt.Sprintf("%d", uint64(a.Value)))
        }
      }
    }
    st.Fields = append(st.Fields, field)
  }

  return st
}

func main() {
  data, _ := os.ReadFile("./sys.txt")

  desc := ast.Parse(data, "sys.txt", func(pos ast.Pos, msg string) {
    fmt.Printf("failed to parse: [%s] %+v\n", msg, pos)
  })

  output := Output{}

  desc.Walk(func(n ast.Node) {
    //fmt.Printf("n: [%T] %+v\n", n, n)
    if i, ok := n.(*ast.Include); ok {
      if i.File != nil {
        output.Includes = append(output.Includes, i.File.Value)
      }
    } else if d, ok := n.(*ast.Define); ok {
      def := Define{}
      def.Name = d.Name.Name
      if d.Value != nil {
        if d.Value.Ident != "" {
          def.Value = d.Value.Ident
        } else if d.Value.CExpr != "" {
          def.Value = d.Value.CExpr
        } else {
          def.Value = fmt.Sprintf("%d", d.Value.Value)
        }
      }
      output.Defines = append(output.Defines, def)
    } else if t, ok := n.(*ast.TypeDef); ok {
      if len(t.Args) > 0 {
        template := Template{}

      } else {
        typedef := TypeDef{}

      }

    } else if s, ok := n.(*ast.Struct); ok {
      st := dumpStruct(s)
      output.Structs = append(output.Structs, st)
    } else if c, ok := n.(*ast.Call); ok {
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
      output.Syscalls = append(output.Syscalls, s)
    }
  })

  jb, err := json.Marshal(output)
  if err != nil {
    fmt.Println(err)
  }
  fmt.Print(string(jb))
}
