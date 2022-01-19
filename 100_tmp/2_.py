
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

# def pre_process_line(line):


def strip_commit(str):
    i = str.find("//")
    if i != -1:
        str = str[:i].strip()

    i = str.find("/*")
    if i != -1:
        str = str[:i].strip()
    
    return str

    
def process_member(line):
    i = line.rfind(" ")
    s_type = line[0:i].strip()
    s_value = line[i+1:]
    return '"' + s_type + '"' + ' : ' + '"' + s_value + '"'

# "int     (*set_vbus)(struct device *dev, int is_on);"
def process_func(line):
    i = line.find(')(')
    s_f = line[0:i+1]
    s_f = '"' + s_f + '"'

    s_parameter1 = line[i+2: -2]   # (struct device *dev, int is_on);
    parameters = ''
    ls = s_parameter1.split(',')
    for item in ls:
        para = process_member(item)
        parameters += para + ' , '

    parameters = parameters[:-3]  # 去掉最后一个 ' , '
    #print(parameters)

    parameters = '(' + parameters + ')'
    #print(parameters)

    return s_f + parameters


if __name__ == '__main__':
    import numpy as np
    import sys

    fin = open("2.c", encoding="utf-8")
    input_file = fin.read()
    #print(input_file)
    in_lines = input_file.split("\n")

    # 全局变量
    text = ""
    class_t = ""
    struct_begin  = 1

    
    
    for line in in_lines:

        # 1.预处理 -----------------------------
        if line.startswith("//") or \
            line.startswith("/*") or \
            line.startswith("*")  or \
            line.startswith("*/") or \
            line.startswith("#define") or\
            line == "":

            continue

        line = line.strip()
        if line.find("//") != -1:
            line = line[0:line.find("//")]
        if line.find("/*") != -1:
            line = line[0:line.find("/*")]
            
        print("\n--0. 1.预处理 line = \n", line)

        # 2. 是 struct开始
        if line.startswith("struct"):
            struct_begin = 1
            line = line.replace("struct", "class")
            class_t = line + '\n'
            print('\n  2. 是 struct开始 class_t = \n', class_t)
            continue

        # 3. 判断 struct结束
        if line == "};":
            struct_begin = 0
            class_t += "}\n"
            text += class_t
            class_t = ''
            print('\n 3. struct结束')
            continue

        # 4. 判断是成员还是函数
        if struct_begin:
            print('\n--4. struct_begin')
            line = strip_commit(line)

            i = line.find(";")
            if i == -1:
                print("not a c statement")
                continue
            else:
                line = line[:i]

            i = line.find(r")(")
            print(i)
            if i == -1:
                # 4.1 处理line成员
                member = process_member(line)
                print('member = ', member)
                class_t += member + '\n'

            else:
                # 4.2 处理line方法
                func = process_func(line)
                print('func = ', func)
                class_t += func + '\n'

    print(text)
