
"""
struct s1 {
    int a;
    int b;
    //(*func)(int a, int b);
    //int (*func2)(int m);
    //void (*writel)(void __iomem *addr, unsigned offset, u32 data);
    //void	(*pre_root_reset_end)(struct musb *musb);
};
"""


"""
ClassDiagram {

    class s1 {
        int : a;
        int : b;
        func(int:d, int:e);
        "int (*func2)"(int: m, int:b);
        
    }

}
"""

if __name__ == '__main__':
    import numpy as np
    import sys

    fin = open("1_source.c", encoding="utf-8")
    input_file = fin.read()
    #print(input_file)
    in_lines = input_file.split("\n")


    text = "ClassDiagram {\n\n"

    struct_begin  = 1
    for line in in_lines:
        line = line.strip()
        print('line = ', line)

        if line.startswith("//") or line == "":
            continue

        if line == "}":
            text += "}\n"
            break

        if struct_begin:
            st = line.split(" ")
            print(st)
            text += "class " + st[1] + " {" + "\n"
            struct_begin = 0

        else:
            line = line.replace(';', '')
            ls = line.split(" ")
            print('ls = ', ls)
            temp = "\t" + "\"" + ls[0] + "\"" + ":" + "\"" + ls[1] + "\""
            text = text + temp + "\n"


    text += "\n}\n"
    print('text = \n', text)

    dir_out = "1_out.dotuml"
    f_out = open(dir_out, 'w', encoding='utf-8')
    f_out.write(text)
    f_out.close()
