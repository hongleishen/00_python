
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

def read_dir(dir):
    text = ''
    for root, dirs, files in os.walk(dir):  
        #print('the path is ...')
        #print(root)  
        # print('the current directories under current directory :')
        # print(dirs)  
        # print('the files in current directory :')
        # print(files)
        # print('')

        for file in files:
            d = root + '\\' + file
            print('reading dir', d)
            fin = open(d, encoding="utf-8")
            input_file = fin.read()
            fin.close()
            #print(input_file)
            text += input_file + '\n'

    return text


def strip_commit(str):
    i = str.find("//")
    if i != -1:
        str = str[:i].strip()

    i = str.find("/*")
    if i != -1:
        str = str[:i].strip()
    
    return str

def cal_tab(tn):
    n = 0
    if tn < 4:
        n = 4 - tn
    elif tn < 8:
        n = 8 - tn
    elif tn < 12:
        n = 12 - tn
    elif tn < 16:
        n = 16 - tn
    elif tn < 20:
        n = 20 -tn
    elif tn < 24:
        n = 24 - tn
    return n 
        
def cal_tab2(tn):
    gape = 8
    gapelen = 4

    n = 0
    for i in range(1, gapelen):
        if tn  <= gape * i :
            n = gape*i - tn
            break     
    return n 
    
def process_member(line):
    i = line.rfind(" ")
    s_type = line[0:i].strip()
    s_value = line[i+1:]

    # 控制缩进   \t 没有效果
    n = cal_tab2(len(s_type))
    if (len(s_type + s_value) >= 40):
        n = 1

    print('n = ', n)
    return '"' + s_type + '"' + ' : ' + '"' + ' '*n + s_value + '"'


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

    #(dma_channel:"*channel" , u16:"maxpacket" , void:"*buf" , u32:"length")
    if len(parameters) >= 60:
        parameters = ''
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
        # print("mulit line")
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
                # print('find second', item)
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

        #print('5.0----------item-------------', item)
        if item.startswith('class '):        # class开始
            #class_begine = 1
            #tp = item.split(' ')[1]
            str_class = find_second_not_null(item.split(' '))
            ls_class.append(str_class)

            #print('5.1 class begine, class = ', str_class)
            # continue

        elif item == '}':                    # class end
            #class_begine = 0
            #print('==end class, ls_struct2 = ', ls_struct2)
            ls_struct1.append(ls_struct2)
            ls_struct2 = []

            ls_structp.append(ls_structp2)
            ls_structp2 = []

        else:                               # class内容
            #item.strip()
            #print('else item = ', item)

            if item.find('"struct') != -1:
                #print("has find==============")
                v = find_second_not_null(item.split(' '))
                #print('v = ', v)

                if item.find('*') == -1:
                    ls_struct2.append(v[:-1])
                else:
                    ls_structp2.append(v[:-1])
            # else:
            #     print('not find==')
            #print('\n')

        
    print('ls_class = ', ls_class)
    print('ls_struct1 = ', ls_struct1)
    print('ls_structp = ', ls_structp)

    
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
                #print('struct i in class j', i, j)
                tx += ls_class[i] + ' -c-> ' + ls_class[j] + '\n'

    txp = ''
    for i in range(len(ls_class)):
        
        for j in range(len(ls_class)):
            if ls_class[i] in ls_structp[j] :    # ls_class[i] 在ls_class[j]中有
                #print('struct p i in class j', i, j)
                txp += ls_class[i] + ' -a-> ' + ls_class[j] + '\n'  
    
    return tx + txp

#def process_format(line):



"""
musb_hw_ep -c-> musb
musb_csr_regs -c-> musb_context_registers
musb_context_registers -c-> musb
musb_platform_ops -a-> musb
musb_hw_ep -a-> musb
musb -a-> musb_hw_ep
"""
def process_relation_num(relation_text, text):
    # key class name, value class cotion struct's list
    dclass = {}
    ls = []
    relation_lines = relation_text.split('\n')
    for item in relation_lines:                   # 初始化key
        item = item.strip()
        if item != '':
            #print('relation item =', item)
            class_ = item.split(' ')[2]
            dclass[class_] = []
    #print(dclass)

    for item in relation_lines:                  # 初始化 value
        item = item.strip()
        if item != '':
            #print('2. item = ', item)
            struct_ = item.split(' ')[0]
            class_ = item.split(' ')[2]

            ls = dclass[class_]
            ls.append(struct_)
            dclass[class_] = ls
        

    print('dclass =\n', dclass)
    # ----text-----需要重写--------------
    ls_class = list(dclass.keys())          # 所有 class
    class_begine = 0
    n = 0
    new_t = ''
    text_lines = text.split('\n')
    text_line_number = ""                    # 处理连线num          
    #relation_number = ''
    # dict of struct relation number
    d_srn = {}          # 记录 key: struct name, value : "class : number"
                        # number 是本 class用过的 struct的编号  
    rn = []
    for line in text_lines:
        #line = process_format(line)
        if line.startswith('class '):        # class开始
            class__ = line.split(' ')[1]     #  class__, 现在遍历到的  class
            if class__ in ls_class:          # 如果此 class被其它使用
                class_begine = 1
            new_t += line + '\n'

        elif line.startswith('}'):
            class_begine = 0
            n = 0
            new_t += line + '\n'
        
        elif class_begine:
            # 每个 class 用到的 struct已经在 dclass[class__]中记录
            # 找出每个class并编号，覆盖写入文本             
            if line.find('struct') != -1:
                struc = line.split(' ')[1][:-1]     # "struct musb_platform_ops" : "*ops
                if struc in dclass[class__] :
                    line = line[:-1]  + '  /*' + str(n) + '*/"'
                    text_line_number += struc + ' "' + str(n) + '"' + '-- ' + class__ + '\n'
                    #relation_number += struc + ':' + str(n)
                    
                    #tmp = class__ + ' : ' +  struc + ' : ' + str(n)
                    tmp = class__ + ' : ' + str(n)

                    # print('tmp = \n', tmp) 
                    # -------------d_srn{} 处理-----------------------
                    try:
                        rn = d_srn[struc]
                        rn.append(tmp)
                        d_srn[struc] = rn
                    except:
                        rn = []
                        rn.append(tmp)
                        d_srn[struc] = rn

                    # =========================================

                    n += 1
            new_t += line + '\n'

        else:
            new_t += line + '\n'

    return new_t, text_line_number, d_srn

# print('8. 在本class中添加 被 其它class包含的地方 的 number信息')
# 需要 添加到本class行后面
#  "// 3 "(musb)
def add_relation_num(text, d_srn):
    text_lines = text.split('\n')
    ls_class = list(d_srn.keys())
    new_t = ''
    class_begine = 0
    class__ = ''

    s_be_p = set(ls_be_part) - set(ls_class)
    l_be_p = list(s_be_p)

    for line in text_lines:

        if line.startswith('class '):        # class开始
            class__ = line.split(' ')[1]     #  class__, 现在遍历到的  class
            if class__ in ls_class:         # 如果此 class被其它使用
                class_begine = 1             # class_begine 是非空 class的开始
                new_t += line + '\n'

            elif class__ in l_be_p:         # 被包含的也要写入
                new_t += line + '\n'

            elif simple == 0:               # 如果
                new_t += line + '\n'
                
        elif line.startswith('}') and class_begine == 1:   # class 结束
            class_begine = 0

            # -------class的最后添加信息- "// 3 "(musb) --------------------
            # print('\nclass__ = ', class__)
            for item in d_srn[class__]:
                num = item.split(':')[1]
                func = item.split(':')[0].strip()
                add_mesg = '"// ' + num + ' "' + '(' + func + ')'
                # print(add_mesg)

                new_t += '    ' + add_mesg + '\n'
            # ===========================
            
            new_t += line + '\n\n'
        
        elif class_begine:
            new_t += line + '\n'

        elif class__ in l_be_p:         # 被包含的也要写入
            new_t += line + '\n'

        elif simple == 0:
            new_t += line + '\n'

    return new_t


def process_be_point(text_relation):
    ls = []
    for item in text_relation.split('\n'):
        if item.strip() == '':
            continue
        temp = item.split(' ')[2]
        ls.append(temp)
    return ls



def remove_c_a_realtion(text_relation):
    text = ''
    ls_create_a = []
    for item in text_relation.split('\n'):
        if item.strip() == '':
            continue   
        ls = item.split(' ')
        creat_a = ls[2] + ' -a-> ' + ls[0]
        ls_create_a.append(creat_a)
    
    print('\nlc_create_a = ', ls_create_a)
    for item in text_relation.split('\n'):
        if item.strip() == '':
            continue   
        
        if item in ls_create_a:
            item = '//' + item
            text += item + '\n'
            print("commit double relation :", item)
        else:
            text += item + '\n'
    return text


if __name__ == '__main__':
    import numpy as np
    import sys
    import os

    #fin = open("2.c", encoding="utf-8")
    #input_file = fin.read()
    #fin.close()

    if len(sys.argv) >= 2:
        dir = sys.argv[1]
    else:
        dir = '.\\'
    
    input_files = read_dir(dir)

    #print(input_files)
    # sys.exit(0)
    in_lines = input_files.split("\n")

    # 全局变量
    text = ""
    class_t = ""
    struct_begin  = 0
    m_flag = 0
    m_lines = ''

    lcommit_start = 0
    next_line_skip = 0
    simple = 0
    
    
    for line in in_lines:
        #print('source line = \n', line)

        # 1.预处理 -----------------------------
        if line.startswith("//") or line == "":
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
    
        if line.find("#define") != -1:
            if line.find('\\') != -1:
                next_line_skip = 1
            continue

        if next_line_skip:
            next_line_skip = 0
            continue


        print("\n\n--1.通过预处理的 line = \n", line)

        #sys.exit(0)
        # 2. 是 struct开始
        if line.startswith("struct") \
            and line.find("=") == -1 \
            and line.find('{') != -1 \
            and struct_begin == 0:

            struct_begin = 1
            line = line.replace("struct", "class")
            class_t = line + '\n'
            print('\n  --2. 是 struct开始 class_t = \n', class_t)
            continue

        # 3. 判断 struct结束
        if line.find("}") != -1 and line.find(";") != -1 and struct_begin:
            struct_begin = 0
            class_t += "}\n"
            text += '\n' + class_t
            class_t = ''                    # class_t 是每个 struct内容
            print('\n --3. struct结束')
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
                #print('member = ', member)
                class_t += '\t' + member + '\n'

            else:
                # 4.2 处理line方法
                func = process_func(line)
                #print('func = ', func)
                class_t += '\t' + func + '\n'

    # print('\ntext =\n', text)
    # 遍历完成=================================================

    # 5. 祛除多余部分，比如  const __attribute__
    #text.replace('__iomem', '')
    text = text.replace('const ', '')


    # 6. 处理 关系
    print('\n---------6. 处理 关系')
    text_relation = process_relation(text)
    print('text_relation = \n', text_relation)


    # 7. 聚合and组合地方加   /*number*/
    print('-------# 7. 聚合and组合地方加   /*number*/')
    text, struct_n, d_srn = process_relation_num(text_relation, text)
    #print('\n\n\n struct_n = ', struct_n)
    print('\n d_srn = ', d_srn)


    # 8. 是否去掉没有连线的 class
    if len(sys.argv) >= 3:
        if sys.argv[2] == 'simple':
            simple = 1
    ls_be_part = process_be_point(text_relation)
    print('\n-----8. ls_be_point = ', ls_be_part)

    
    # 9. 在本class中添加 被 其它class包含的地方 的 number信息
    #  "// 3 "(musb)
    print('\n------------9. 在本class中添加 被 其它class包含的地方 的 number信息')
    text = add_relation_num(text, d_srn)

    # 10.  s -c-> class    class -a-> s, 祛除 -a-> 关系
    text_relation = remove_c_a_realtion(text_relation)

    # 10. 处理 ClassDiagram
    print('\n --------10. 处理 ClassDiagram')
    text += '\n\n' + text_relation + '\n'
    text = "ClassDiagram {\n\n" + text + '\n\n}'
    dir_out = "4_out.dotuml"
    f_out = open(dir_out, 'w', encoding='utf-8')
    f_out.write(text)
    f_out.close()

    print("====end====")
