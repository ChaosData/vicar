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
  //Type string `json:"type"`
  Type Type `json:"type"`
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
  Type *Type `json:"type,omitempty"`
  Struct *Struct `json:"struct,omitempty"`
}

type Output struct {
  Includes []string `json:"includes"`
  Defines []Define `json:"defines"`
  Templates []Template `json:"templates"`
  TypeDefs []TypeDef `json:"typedefs"`
  Structs []Struct `json:"structs"`
  Syscalls []Syscall `json:"syscalls"`
}

func flattenType(t *ast.Type, full bool) string {
  var _t string
  if t.Ident != "" {
    _t = t.Ident
  } else if t.String != "" {
    _t = t.String
  } else {
    _t = fmt.Sprintf("%d", uint64(t.Value))
  }

  for _, co := range t.Colon {
    _t += ":" + typeToString(dumpType(co))
  }

  if !full {
    return _t
  }

  if len(t.Args) > 0 {
    _t += "["
    for i, arg := range t.Args {
        _t += flattenType(arg, true)
      if i < len(t.Args)-1 {
        _t += ", "
      }
    }
    _t += "]"
  }

  return _t
}

func dumpType(t *ast.Type) Type {
  typ := Type{}

  typ.Name = flattenType(t, false)
  if len(t.Args) > 0 {
    for _, arg := range t.Args {
      _arg := flattenType(arg, true)
      typ.Args = append(typ.Args, _arg)
    }
  }
  return typ
}

func typeNameSimple(t Type) string {
  if t.Name == "" && len(t.Args) == 1 {
    return t.Args[0]
  }
  if len(t.Args) == 0 {
    return t.Name
  }
  return "<unknown>"
}

func typeToString(t Type) string {
  if len(t.Args) == 0 {
    return t.Name
  }
  if t.Name == "" {
    return t.Args[0]
  }

  return fmt.Sprintf("%s[%s]", t.Name, strings.Join(t.Args, ", "))

  /*ret := t.Name + "["
  for i, arg := range t.Args {
    ret += typeToString(arg)
    if i < len(t.Args) {
      ret += ", "
    }
  }
  ret += "]"
  return ret*/
}

func dumpStruct(s *ast.Struct) Struct {
  st := Struct{}
  st.Name = s.Name.Name
  st.Union = s.IsUnion

  if len(s.Attrs) > 0 {
    for _, a := range s.Attrs {
      attr := dumpType(a)
      st.Attrs = append(st.Attrs, attr)
    }
  }
  for _, f := range s.Fields {
    field := Arg{}
    field.Name = f.Name.Name
    field.Type = dumpType(f.Type)
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
/*
type Template struct {
  Name string `json:"name"`
  Args []string `json:"args"`
  Type Type `json:"type,omitempty"`
  Struct Struct `json:"struct,omitempty"`
}
*/
        //fmt.Printf("[template] %+v\n", t)
        //fmt.Printf("[template] name: %s\n", t.Name.Name)
        template.Name = t.Name.Name
        for _, ident := range t.Args {
          template.Args = append(template.Args, ident.Name)
        }
        if t.Type != nil {
          _type := dumpType(t.Type)
          template.Type = &_type
        } else if t.Struct != nil {
          _struct := dumpStruct(t.Struct)
          template.Struct = &_struct
        }

        output.Templates = append(output.Templates, template)
      } else {
        typedef := TypeDef{}
        typedef.Name = t.Name.Name
        typedef.Type = dumpType(t.Type)
        output.TypeDefs = append(output.TypeDefs, typedef)
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
        ret := flattenType(c.Ret, true)
        s.Ret = &ret
      }

      for _, a := range c.Args {
        arg := Arg{}
        arg.Name = a.Name.Name
        arg.Type = dumpType(a.Type)
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
