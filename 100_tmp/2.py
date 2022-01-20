
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

# "int     (*set_vbus)(struct device *dev, int is_on)"
def process_func(line):
    i = line.find(')(')
    s_f = line[0:i+1]
    s_f = '"' + s_f + '"'

    s_parameter1 = line[i+2: -1]   # (struct device *dev, int is_on)
    parameters = ''
    ls = s_parameter1.split(',')
    for item in ls:
        #para = process_member(item)
        #i = line.rfind(" ")

        # type 中不能有  “ ” ， 空格隔开的多个字符串 因此只取最后一个 
        i = item.rfind(" ")
        s_type = item[:i].strip()
        ls_type = s_type.split(' ')
        s_type = ls_type[-1]

        s_value = item[i+1:]
        para = s_type + ':' + '"' + s_value + '"'

        parameters += para + ' , '

    parameters = parameters[:-3]  # 去掉最后一个 ' , '
    #print(parameters)

    parameters = '(' + parameters + ')'
    #print(parameters)

    return s_f + parameters



def process_line(line, m_flag, m_lines):
    i = line.find(";")
    if i != -1:
        if m_flag != 0:
            m_flag = 0
            line = m_lines + line
            i = line.find(";")

        line = line[:i]
        return line, m_flag, m_lines
    else:
        print("mulit line")
        m_flag = 1
        m_lines += line
        return -1, m_flag, m_lines

def find_second_not_null(ls):
    first = 1
    for item in ls:
        if first:
            first = 0
            continue
        else:
            if item != '':
                print('find second', item)
                return item


def process_relation(text):
    class_begine = 0

    ls_class = []
    ls_struct1 = []
    ls_struct2 = []

    ls_structp = []
    ls_structp2 = []

    ls_text = text.split('\n')

    # 创建ls
    for item in ls_text:
        item.strip()
        if item == '':                      # 空行跳过
            continue

        print('5.0----------item-------------', item)
        if item.startswith('class '):        # class开始
            #class_begine = 1
            #tp = item.split(' ')[1]
            str_class = find_second_not_null(item.split(' '))
            ls_class.append(str_class)

            print('5.1 class begine, class = ', str_class)
            # continue

        elif item == '}':                    # class end
            #class_begine = 0
            print('==end class, ls_struct2 = ', ls_struct2)
            ls_struct1.append(ls_struct2)
            ls_struct2 = []

            ls_structp.append(ls_structp2)
            ls_structp2 = []

        else:                               # class内容
            #item.strip()
            print('else item = ', item)

            if item.find('"struct') != -1:
                print("has find==============")
                v = find_second_not_null(item.split(' '))
                print('v = ', v)

                if item.find('*') == -1:
                    ls_struct2.append(v[:-1])
                else:
                    ls_structp2.append(v[:-1])
            else:
                print('not find==')
            print('\n')

        
    print('ls_class = ', ls_class)
    print('ls_struct1 = ', ls_struct1)
    
    #  分析 关系
    #ls_class =  ['test_class', 'musb_hw_ep', 'musb_ep', 'dma_channel']
    #ls_struct1 =  [[], 
    #           ['musb', 'dma_channel', 'dma_channel', 'musb_ep', 'musb_ep'], ['usb_ep', 'musb_hw_ep', 'musb', 'usb_endpoint_descriptor', 'dma_channel'], 
    # []]
    ls_relation = []
    tx = ''
    for i in range(len(ls_class)):
        
        for j in range(len(ls_class)):
            if ls_class[i] in ls_struct1[j] :    # ls_class[i] 在ls_class[j]中有
                print('struct i in class j', i, j)
                tx += ls_class[i] + ' -c-> ' + ls_class[j] + '\n'

    txp = ''
    for i in range(len(ls_class)):
        
        for j in range(len(ls_class)):
            if ls_class[i] in ls_structp[j] :    # ls_class[i] 在ls_class[j]中有
                print('struct p i in class j', i, j)
                txp += ls_class[i] + ' -a-> ' + ls_class[j] + '\n'  
    
    return tx + txp


if __name__ == '__main__':
    import numpy as np
    import sys

    fin = open("2.c", encoding="utf-8")
    input_file = fin.read()
    fin.close()
    #print(input_file)
    in_lines = input_file.split("\n")

    # 全局变量
    text = ""
    class_t = ""
    struct_begin  = 0
    m_flag = 0
    m_lines = ''

    lcommit_start = 0

    
    
    for line in in_lines:
        #print('source line = \n', line)

        # 1.预处理 -----------------------------
        if line.startswith("//") or line == "":
            #line.startswith("/*") or \
            #line.startswith("*")  or \
            #line.startswith("*/") or \
            #line.startswith("#define") or\
            
            continue

        line = line.strip()
        if line.find("//") != -1:
            line = line[0:line.find("//")]
        if line.find("/*") != -1 and line.find("*/") != -1:    #  在同一行
            line = line[0:line.find("/*")]

        
        # 跨行注释
        if line.find("/*") != -1:
            lcommit_start = 1
            continue

        if line.find("*/") != -1:
            lcommit_start = 0
            continue

        if lcommit_start == 1:
            continue
            
        print("\n\n----1.通过预处理的 line = \n", line)
        #sys.exit(0)
        # 2. 是 struct开始
        if line.startswith("struct") and struct_begin == 0:
            struct_begin = 1
            line = line.replace("struct", "class")
            class_t = line + '\n'
            print('\n  2. 是 struct开始 class_t = \n', class_t)
            continue

        # 3. 判断 struct结束
        if line == "};":
            struct_begin = 0
            class_t += "}\n"
            text += '\n' + class_t
            class_t = ''
            print('\n 3. struct结束')
            continue

        # 4. 判断是成员还是函数
        if struct_begin:
            print('\n--4. in struct, line = ', line)
            line = strip_commit(line)

            line, m_flag, m_lines = process_line(line, m_flag, m_lines)
            if line == -1:
                continue
            
            m_lines = ''
            #print('----line = ----------------', line)
            i = line.find(r")(")
            if i == -1:
                # 4.1 处理line成员
                member = process_member(line)
                print('member = ', member)
                class_t += '\t' + member + '\n'

            else:
                # 4.2 处理line方法
                func = process_func(line)
                print('func = ', func)
                class_t += '\t' + func + '\n'

    print('\ntext =\n', text)

    # 5. 祛除多余部分，比如  const __attribute__
    #text.replace('__iomem', '')
    text = text.replace('const ', '')


    # 6. 处理 关系
    print('-----6. 处理 关系')
    text_relation = process_relation(text)
    print('text_relation = \n', text_relation)


    # 7. 处理 ClassDiagram
    print('\n --------7. 处理 ClassDiagram')
    text += '\n\n' + text_relation
    text = "ClassDiagram {\n\n" + text + '\n\n}'
    dir_out = "2_out.dotuml"
    f_out = open(dir_out, 'w', encoding='utf-8')
    f_out.write(text)
    f_out.close()