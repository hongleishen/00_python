# coding=utf-8


def is_number(s):
    try:
        float(s)
        return True
    except ValueError:
        pass

    try:
        import unicodedata
        unicodedata.numeric(s)
        return True
    except (TypeError, ValueError):
        pass

    return False


def to_day_datas():
    fin = open("2.txt", encoding='utf-8')
    input_file = fin.read()
    file_lines = input_file.split("\n")

    day_flag = 0
    l_day = []

    line_source = []
    line_process = []

    for line in file_lines:
        line = line.strip()
        print("line :", line)
        if line == '' or line[0] == '#':
            print("continue #")
            continue
        

        # 处理日期
        if (is_number(line)):
            if (day_flag == 0):
                #print('day_flag = ', day_flag)
                l_day.append(float(line))
                day_flag = 1

            elif day_flag == 1:
                #print('day_flag = ', day_flag)
                datas.append(l_day)
                l_day = []

                # ---new line-----
                l_day.append(float(line))

        # 处理 数据
        else:
            line_source = line.split(' ')
            for item in line_source:
                if item != '':
                    line_process.append(item)
            print('line_process = ', line_process)
            l_day.append(line_process)
            line_process = []
        #print('=================\n')
    datas.append(l_day)


def process_doubule_entry_account(item, new_day, time):
    # ----虚拟账户--------------
    l_richang = []
    l_ewai = []
    l_douyin = []
    l_guding = []
    
    l_income = []
    l_vcheck = []

    # -----账户---------------------
    l_zhaoshang = 0
    l_pingan = 0
    l_yuebao = 0

    l_nongshang = 0
    l_huabei = 0
    l_pinganxin = 0
    l_weili = 0
    l_baitiao = 0

    # 虚拟账户 添加数据
    if item[0] == '日常':
        l_richang = item[1:3]
    elif item[0] == '额外':
        l_ewai = item[1:3]
    elif item[0] == '抖音':
        l_douyin = item[1:3]
    elif item[0] == '固定':
        l_guding = item[1:3]

    elif item[0] == 'income':
        l_income = item[1:3]
    

    # 账户添加数据
    if item[3] == '招商':
        l_zhaoshang = item[4]
    elif item[3] == '平安':
        l_pingan = item[4]
    elif item[3] == '余额宝':
        l_yuebao = item[4]

    elif item[3] == '农商':
        l_nongshang = item[4]
    elif item[3] == '花呗':
        l_huabei = item[4]
    elif item[3] == '平安信':
        l_pinganxin = item[4]
    elif item[3] == '微粒':
        l_weili = item[4]
    elif item[3] == '白条':
        l_baitiao = item[4]
    
    new_day.append(time)
    # 虚拟
    new_day.append(l_richang)
    new_day.append(l_ewai)
    new_day.append(l_douyin)
    new_day.append(l_guding)
    
    new_day.append(l_income)
    new_day.append(l_vcheck)

    # 账户
    new_day.append(l_zhaoshang)
    new_day.append(l_pingan)
    new_day.append(l_yuebao)

    new_day.append(l_nongshang)
    new_day.append(l_huabei)
    new_day.append(l_pinganxin)
    new_day.append(l_weili)
    new_day.append(l_baitiao)




def to_doubule_entry_account(datas, fdatas):
    new_item = []
    new_day = []
    #new_datas = []
    time = 0

    for day in datas:      #day 整个一天的记录   有日期 和很多item
        data_flag = 1      # 新的一天开始 

        #  [11.17, ['计数器', '20', '花呗'], ['抖音', '150'], ['约会', '130']]
        for item in day:  # item 每个日常记录
            print('item =', item)

            if data_flag:   # 第一个 item 是日期
                data_flag = 0
                #new_day.append(item)
                time = item
                continue

            else:
                #正规  贷记记账规则：
                #  0        1       2      3       4
                #  账户     条目    金额    账户    金额（程序计算）
                
                # 单独处理 抖音 没有条目
                # 0-抖音   1-缺    2-金额
                # 抖音             500 
                if item[0] == '抖音':
                    item.insert(1, '充值')
                    if len(item) == 3:
                        item.append('农商')

                # 2行只有 虚拟账户 日常， 账户 农商 ， 这一种情况
                # 0-缺   1-条目   2-金额   3-缺
                #        吃饭     20
                elif len(item) == 2:
                    item.insert(0, '日常')
                    item.append('农商')
                
                elif len(item) == 3:
                    # a -> v
                    # 3.  0-账户    1-条目  2-金额  3-缺
                    #     平安      对账    500
                    if item[1] == '对账':
                        item.append('vcheck')

                    # 1.  0-缺  1-条目  2-金额  3-账户
                    #           吃饭    90      花呗
                    elif not (item[0] in lvirt):
                        item.insert(0, '日常')

                    # 2.  0-账户    1-条目  2-金额  3-缺
                    #     额外      红包    66
                    else:
                        item.append('农商')

                elif len(item) > 4:
                    print("Error input, should be: 账户 条目 金额 账户")
                
                #print(item)
                
                new_item.append(item[0])
                new_item.append(item[1])
                new_item.append(float(item[2]))
                new_item.append(item[3])
                
                print('--new_item = ', new_item)

                # 账户 间  都相反
                if new_item[0] in laccout and new_item[3] in laccout:
                    # 1.        a <--> a
                    new_item.append(-new_item[2])   # 账户  to 账户
                    #process_doubule_entry_account(new_item, new_day, time)
                
                else:
                    # v a之间 同 相同； 反 相反
                    # 2.        v <--> a
                    
                    #       v -> a
                    if new_item[0] in lvirt  and new_item[3] in laccout:
                        if lvirtv[lvirt.index(new_item[0])] == laccoutv[laccout.index(new_item[3])]:
                            new_item.append(new_item[2])
                        else:
                            new_item.append(-new_item[2])
                    else:
                        #   a -> v
                        if laccoutv[laccout.index(new_item[0])] == lvirtv[lvirt.index(new_item[3])]:
                            new_item.append(new_item[2])
                        else:
                            new_item.append(-new_item[2])

                    # new_day.append(lrichang)
                process_doubule_entry_account(new_item, new_day, time)

                print('==new_item = ', new_item)
                print(' ===============new_day', new_day, '===end============\n')
                new_item = []

                fdatas.append(new_day)
                new_day = []
    
    #fdatas = new_datas
            



if __name__ == '__main__':
    import numpy as np
    import pandas as pd
    import matplotlib.pyplot as plt
    import sys
    # %matplotlib inline
    from matplotlib.ticker import MultipleLocator, FormatStrFormatter

    ditem = {'extera': 10, 'fixed': 11, 'douyin': 12}
    #print(ditem)
    #time = []
    #datas = []
    #get_data(time, datas)
    #df = pd.DataFrame(data=datas, columns=time)

    # 1 资产类；  -1 负债类
    # dvirtual = [{'日常'：1}, {'额外'}}]

    lvirt = ['日常', '额外', '抖音', '固定',  'vlend', 'income', 'vcheck']
    lvirtv = [-1,      -1,   -1,     -1,     -1,        1,      1]

    laccout = ['招商', '平安', '余额宝', '农商', '花呗', '平安信', '微粒', '白条']
    laccoutv =[1,       1,     1,        -1,      -1,     -1,     -1,      -1]

    cl = lvirt + laccout
    cl.insert(0, '日期')

    datas = []
    fdatas = []
    # 1. 读入数据， 拆分好日期
    print('\nto_day_datas ^^^^^')
    to_day_datas()
    for item in datas:
        print(item, '\n')

    # 2. 每个日期 格式化 为 贷记记账
    print('\n\n to_doubule_entry_account ^^^^ ')
    to_doubule_entry_account(datas, fdatas)
    #for item in fdatas:
    #    print(item, '\n')

    # 3. 组成df
    print('\n\n df')
    pd.set_option('display.unicode.ambiguous_as_wide', True)
    pd.set_option('display.unicode.east_asian_width', True)
    pd.set_option('display.width', 300)         # 设置打印宽度(**重要**)
    #df = pd.DataFrame(data = fdatas, columns = cl)
    #print(df)


    # 4. df添加统计  #归一化

    # 5. 画图