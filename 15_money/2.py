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

def process_item_3_passive(item):
    for accout in laccout:
        if item[3] == accout:
            pass


    if item[3] == '招商':
        d_zhaoshang['被动'] = item[4]
    elif item[3] == '平安':
        d_pingan = item[4]
    elif item[3] == '余额宝':
        d_yuebao = item[4]

    elif item[3] == '农商':
        d_nongshang = item[4]
    elif item[3] == '花呗':
        d_huabei = item[4]
    elif item[3] == '平安信':
        d_pinganxin = item[4]
    elif item[3] == '微粒':
        d_weili = item[4]
    elif item[3] == '白条':
        d_baitiao = item[4]


def one_item_to_one_line(item, new_day, time):
    #               0      1       2    3      4
    # new_item =  ['日常', '电池', 2.0, '平安', -2.0]

    # print(row_account)
    # ['日常', '额外', '抖音', '固定', 'income', 'vcheck', 'vlend', '招商', '平安', '余额宝', '农商', '花呗', '平安信', '微粒', '白条']
    
    # print('[func] one_item_to_one_line')

    for count in row_account:
        new_day.append(0)
    
    new_day[row_account.index(item[0])] = item[1:3]
    new_day[row_account.index(item[3])] = ['被动', item[4]]
    new_day.insert(0, time)
    return new_day

def every_record_to_one_doubule_entry_account_line(datas, fdatas):
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
                new_item.append(int(item[2]))
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

                print('==new_item = ', new_item)

                one_item_to_one_line(new_item, new_day, time)                
                print(' ===============new_day', new_day, '===end============\n')
                new_item = []

                fdatas.append(new_day)
                new_day = []
    
    #fdatas = new_datas
            
def to_df_daysum(df):
    lday = []
    datas = []
    for i in range(len(df['日期'])):
        for j in range(len(cl)):
            if j == 0:
                lday.append(df.iloc[i][j])

            elif df.iloc[i][j] != 0:
                lday.append(df.iloc[i][j][1])
            else:
                lday.append(df.iloc[i][j])
        #print('lday= ', lday)
        datas.append(lday)
        lday = []

    # 去掉lable后的 df
    df_rmlab = pd.DataFrame(data = datas, columns = cl)
    #print(df_rmlab)

    ls = list(df['日期'])
    lriqi = []
    for i in ls:
        if not i in lriqi:
            lriqi.append(i)
    print(lriqi)
    
    df_dsum = pd.DataFrame(columns = cl)
    n = 0
    for riqi in lriqi:
        print('riqi ', riqi)
        df_day = df_rmlab[ df_rmlab['日期'] == riqi ]
        #print(df_day)
        day_sum = df_day.sum()
        #day_sum = list(map(lambda x: int(x), day_sum))
        day_sum[0] = riqi       # 恢复日期
        print('day_sum = ', day_sum)
        df_dsum.loc[n] = list(day_sum)
        n += 1
    
    return df_dsum


def process_lable_balance(df_balance, cls):
    ls = []
    for label in cls:
        series = df_day[label]
        #print(series)
        for money in series:
            if ls == []:
                ls.append(money)
            else:
                ls.append(ls[len(ls) - 1] + money)
        #print(ls)
        df_balance[label] = ls
        ls = []


def to_df_balance (df_day):
    index = list(range(len(df_day)))
    
    # --------df_vbalance-----------------------------------------
    df_vbalance = pd.DataFrame(index = index)
    #df_balance['日期'] = list(df_day['日期'])
    #print(df_vbalance)

    ls = []

    #cl_tmp = cl
    #cl_tmp.pop(0)
    ls = lvirt
    #ls.remove('income')
    process_lable_balance(df_vbalance, ls)
    #print(df_vbalance)


    #current_surplus = df_vbalance.sum(axis = 1)                   
    expend = df_vbalance.drop('income', axis = 1).sum(axis = 1)    # 支出
    current_surplus = df_vbalance['income'] - expend               # 本期盈余  

    # 每天平均开销
    day_aver = []                                       # 日均支出
    days = 0
    i = 0
    for item in df_day['日期']:
        #days.append(item - (df_day[0] - 1))
        days = int(item * 100  - (df_day['日期'][0] * 100 - 1))
        #days *= 100
        
        print('expend[i]  days  ', expend[i], days)
        day_aver.append(int(expend[i] / days))
        i += 1

    print('current_surplus = ', current_surplus)
    print('expend = ', expend)
    print('day_aver = ', day_aver)

    df_vbalance['expend'] = expend
    df_vbalance['day_aver'] = day_aver
    df_vbalance['curren'] = current_surplus
    print(df_vbalance)     

  
    # --------df_balancea-----------
    df_abalance = pd.DataFrame(index = index)
    
    




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
    lvirt = ['日常', '额外', '抖音', '固定', 'vcheck', 'income']
    lvirtv = [-1,      -1,   -1,     -1,      1,       1]
    lve = ['richang', 'ewai']

    laccout = ['vlend', '招商', '平安', '余额宝', '农商', '花呗', '平安信', '微粒', '白条']
    laccoutv =[1,       1,       1,     1,        -1,      -1,     -1,     -1,      -1]

    cl = lvirt + laccout
    cl.insert(0, '日期')
    row_account = lvirt + laccout

    datas = []
    fdatas = []
    # 1. 读入数据， 拆分好日期
    print('\n  ** setp 1. to_day_datas ^^^^^')
    to_day_datas()
    for item in datas:
        print(item, '\n')

    # 2. 每条记录，变成 df的一行，用贷记记账
    print('\n\n   **setp 2. to_doubule_entry_account ^^^^ ')
    every_record_to_one_doubule_entry_account_line(datas, fdatas)
    # print("***********************************")
    #for item in fdatas:
    #    print(item, '\n')

    # 3. 组成df
    print('\n\n    **step 3. df 带label')
    pd.set_option('display.unicode.ambiguous_as_wide', True)
    pd.set_option('display.unicode.east_asian_width', True)
    pd.set_option('display.width', 300)         # 设置打印宽度(**重要**)
    # df2 = pd.DataFrame(data=data, index=index, columns = columns)
    df = pd.DataFrame(data = fdatas, columns = cl)
    print(df)

    # 4. df 相同日期合并， 祛除label
    print("\n\n 4. df_stat 不带label")
    #df = df[df['日期'] == 11.16]
    
    df_day = to_df_daysum(df)
    print(df_day)

    # 5. df_balance ;  balance 余额
    print('\n\n **step 5:  df_stat 每个账户余额 及 类型余额')
    df_balance = to_df_balance(df_day)



    #df_tmp = df_rmlab[ df_rmlab['日期'] == 11.16  ]
    #print(df_tmp) 

    #print('after\n', df)
    

    # 5. 画图