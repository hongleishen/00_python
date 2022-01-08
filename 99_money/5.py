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

#  [[11.15, ['余额宝', '对账', '4800', 'income']], [11.16, ['日常', '火锅', '138'], ['日常', '洗发水', '200']]
def to_day_datas(dir):
    fin = open(dir, encoding='utf-8')
    input_file = fin.read()
    fin.close()

    # 上期盈余
    tmp = dir.replace('.txt', '')
    cdate = dt.datetime.strptime(tmp, '%Y.%m')
    date_before = cdate - relativedelta(months=1)
    dir_before = date_before.strftime("%Y.%m") + '.current_balance.txt'
    print('dir_before = ', dir_before)

    try:
        f_before = open(dir_before, encoding='utf-8')
        before = f_before.read()
        f_before.close()
    except:
        print("上期无盈余数据")
        before = ''
    
    # 所有文本
    input_file = before + input_file
    file_lines = input_file.split("\n")

    day_flag = 0
    l_day = []

    line_source = []
    line_process = []

    t = time.ctime(os.path.getatime(dir))
    year = t.split(' ')[4]
    #print(dir, ' get year = %s\n' %year)

    for line in file_lines:
        line = line.strip()
        #print("line :", line)
        if line == '' or line[0] == '#':
            #print("continue #")
            continue

        if line[0] == '年':
            year = line[1:].strip()
            #print('year = ', year)
            continue
        
        # 处理日期
        if (is_number(line)):
            if (day_flag == 0):
                #print('day_flag = ', day_flag)
                l_day.append(year + '.' + line)
                day_flag = 1

            elif day_flag == 1:
                #print('day_flag = ', day_flag)
                datas.append(l_day)
                l_day = []

                # ---new line-----
                l_day.append(year + '.' + line)       # l_day 是 一天的所有数据

        # 处理 数据
        else:
            line_source = line.split(' ')
            for item in line_source:
                if item != '':
                    line_process.append(item)
            #print('line_process = ', line_process)
            l_day.append(line_process)
            line_process = []
        #print('=================\n')
    datas.append(l_day)


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
                # 正规  贷记记账规则：
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
                
                # print(item)
                
                new_item.append(item[0])
                new_item.append(item[1])
                new_item.append(int(item[2]))
                new_item.append(item[3])
                
                print('--new_item = ', new_item)

                # 计算 被动账户 数值
                # 账户 间  都相反
                if new_item[0] in laccout and new_item[3] in laccout:
                    # 1.        a <--> a
                    new_item.append(-new_item[2])   # 账户  to 账户
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

            elif df.iloc[i][j] != 0:            # 为 [ , ]
                lday.append(df.iloc[i][j][1])
            else:                               # 为 0
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
    #print(lriqi)
    
    df_dsum = pd.DataFrame(columns = cl)
    n = 0
    for riqi in lriqi:
        #print('riqi ', riqi)
        df_day = df_rmlab[ df_rmlab['日期'] == riqi ]
        #print(df_day)
        day_sum = df_day.sum()
        #day_sum = list(map(lambda x: int(x), day_sum))
        day_sum[0] = riqi       # 恢复日期
        #print('day_sum = ', day_sum)
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


def to_df_balance(df_day):
    index_day = list(range(len(df_day)))
    
    # 1. -------df_vbalance-----------------------------------------
    df_vbalance = pd.DataFrame(index=index_day)
    #df_balance['日期'] = list(df_day['日期'])
    #print(df_vbalance)

    #cl_tmp = cl
    #cl_tmp.pop(0)
    #ls = lvirt
    #ls.remove('income')
    process_lable_balance(df_vbalance, lvirt)
    #print(df_vbalance)
    #             
    expend = df_vbalance.drop(['vcheck', 'income'], axis = 1).sum(axis = 1)    # 支出
    expend -= df_vbalance['vcheck']                                # vcheck是 资产类
    current_surplus = df_vbalance['income'] - expend               # 本期盈余  

    # 每天平均开销
    day_aver = []                                       # 日均支出
    days = 0
    i = 0
    for item in df_day['日期']:
        gap = dt.datetime.strptime(item, '%Y.%m.%d') - dt.datetime.strptime(df_day['日期'][0], '%Y.%m.%d')
        days = gap.days + 1
        #print('expend[i]  days  ', expend[i], days)
        day_aver.append(int(expend[i] / days))
        i += 1

    #print('current_surplus = ', current_surplus)
    #print('expend = ', expend)
    #print('day_aver = ', day_aver)

    df_vbalance['支出'] = expend
    df_vbalance['日均'] = day_aver
    df_vbalance['盈余'] = current_surplus
    df_vbalance['日期'] = df_day['日期']
    df_vbalance = df_vbalance.set_index('日期')
    df_vbalance['vcheck'][0] = 0
    #print(df_vbalance, '\n')     

  
    # 2. -------- df_abalance -----------
    df_abalance_cuxu = pd.DataFrame(index=index_day)

    ls = ['vlend', '招商', '平安', '余额宝']
    process_lable_balance(df_abalance_cuxu, ls)
    cash = df_abalance_cuxu.drop('vlend', axis=1).sum(axis=1)    # 现金
    df_abalance_cuxu['现金'] = cash

    ls = ['农商', '花呗', '平安信', '微粒', '白条']
    df_abalance_xinyong = pd.DataFrame(index=index_day)
    process_lable_balance(df_abalance_xinyong, ls)
    xinyong = df_abalance_xinyong.sum(axis=1)                        # 信用
    df_abalance_xinyong['信用消费'] = xinyong

    card_sum = cash - xinyong                                   # 账户 总额
    df_abalance_xinyong['账户总额'] = card_sum
    
    #df_abalance = pd.merge(df_abalance_cuxu, df_abalance_xinyong, on = index)  # df_abalance
    df_abalance = pd.concat([df_abalance_cuxu, df_abalance_xinyong], axis = 1)
    df_abalance = df_abalance.set_index(df_day['日期'])
    #print(df_abalance)
    
    return df_vbalance, df_abalance


def process_current_balance(df_abalance):
    dir_out = dir.replace('.txt', '')
    dir_out = dir_out + '.current_balance' + '.txt'
    print(dir_out)
    f_out = open(dir_out, 'w', encoding='utf-8')

    date = df_abalance.index.values[-1]
    print(date)
    f_out.write("{}\n".format(date))

    for item in laccout:
        amount = df_abalance[item][-1]
        st = '\t' + item + '\t' + '对账' + '\t' + str(amount) + '\t' + 'vcheck'
        print(st)
        f_out.write("{}\n".format(st))

    f_out.close()





def plot_vbalance(df_vbalance, dates):
    print('plot_vbalance')
    df = df_vbalance
    time = dates

    ri_chang = df['日常']
    #plt.plot(time, ri_chang, ':', color='b', linewidth=0.5, marker='o', markersize=1.5, label = '日常')
    plt.plot(time, ri_chang, linestyle=':', linewidth=0.5, c = seaborn.xkcd_rgb['bright blue'], 
                marker = '$r$', markersize=4.5, label = '日常')

    ewai = df['额外']
    # plt.plot(time, ewai, marker = 'x', label = '额外')  #有线
    # plt.plot(time, ewai, 'x', label = '额外')  # 无线
    plt.plot(time, ewai,linestyle=':', linewidth=0.45, c = seaborn.xkcd_rgb['blue blue'], 
                marker = '$e$', markersize = 4, label = '额外')  # 无线

    douyin = df['抖音']
    #plt.plot(time, douyin, ':', label = '抖音')
    plt.plot(time, douyin, linestyle=':', linewidth=0.4, c = seaborn.xkcd_rgb['purpley blue'], 
                marker = '$d$', markersize=4, label = '抖音')

    #guding = df['固定']
    #plt.plot(time, guding, ':', label = '固定')

    #vcheck = df['vcheck']
    #plt.plot(time, vcheck, label = 'vcheck')

    #income = df['income']
    #plt.plot(time, income, label = 'income')

    zhichu = df['支出']
    plt.plot(time, zhichu,    c = 'b',                        linewidth=2, 
                marker='o', markersize = 5, label = '支出')

    rijun = df['日均']
    rijun *= 10
    plt.plot(time, rijun,'--',  c=seaborn.xkcd_rgb['mid blue'],   linewidth=1.5, 
                marker='o', markersize = 5, alpha = 0.4, label = '日均')

    yingyu = df['盈余']
    plt.plot(time, yingyu, '-.', c = 'yellowgreen',
                marker='o', markersize = 5, alpha = 1, label = '盈余')  # c = seaborn.xkcd_rgb['minty green'],   cyan


def plot_abalance(df_abalance, dates):
    print(sys._getframe().f_code.co_name)
    df = df_abalance
    time = dates

    #vlend = df['vlend']
    #plt.plot(time, vlend)
    
    # 储蓄卡
    zhaoshang = df['招商']
    plt.plot(time, zhaoshang, linestyle=':', linewidth=0.5,  color='lime', 
                marker = '$z$', markersize=4.5, drawstyle='steps-post', label = '招商')
    pingan = df['平安']
    plt.plot(time, pingan, linestyle=':', linewidth=0.5, c = 'chartreuse',
                marker = '$p$', markersize=4.5, drawstyle='steps-post', label = '平安')

    yuebao = df['余额宝']
    plt.plot(time, yuebao, linestyle=':', linewidth=0.5, c = 'lightgreen',
                marker = '$y$', markersize=4.5, drawstyle='steps-post', label = '余额宝')

    cash = df['现金']
    plt.plot(time, cash, linestyle='-', linewidth=1, c = 'gold',
                marker = '$c$', markersize=4.5, label = '现金')   #  c = 'lawngreen',


    # 信用卡   -----------------------------
    nongshang = df['农商']
    plt.plot(time, nongshang, linestyle=':', linewidth=0.5,  color='peru', 
        marker = '$n$', markersize=4.5, drawstyle='steps-post', label = '农商')
    huabei = df['花呗']
    plt.plot(time, huabei, linestyle=':', linewidth=0.5, c = 'peru',
                marker = '$h$', markersize=4.5, drawstyle='steps-post', label = '花呗')

    # 平安信   额度太小 忽略
    # 微粒    很少用， 忽略，但文字表示

    baitiao = df['白条']
    plt.plot(time, baitiao, linestyle=':', linewidth=0.5,  color='darkkhaki', 
                marker = '$b$', markersize=4.5, drawstyle='steps-post', label = '白条')

    xinyong = df['信用消费']
    plt.plot(time, xinyong, linestyle='-', linewidth=2, c = 'royalblue',
                marker = '$x$', markersize=6, label = '信用消费')   # tan


    #-----账户总额--------------------------
    zonge = df['账户总额']
    plt.plot(time, zonge, linewidth = 2, c = 'orangered', 
                marker = '*', markersize=6, label = '账户总额')  


def plot_label(df):
    print('plot_label')

    last_day = 0
    pre_day = 0
    d = {}

    for i in range(len(df['日期'])):
        for j in range(1, len(cl)):
            if df.iloc[i][j] != 0:
                label = df.iloc[i][j][0]
                if label != '被动':



                    val = df.iloc[i][j][1]
                    val_text = df.iloc[i][j][0] + ' ' + str(val)

                    day = df['日期'][i]
                    try:
                        pre_day =  df['日期'][i + 1]
                    except:
                        pre_day = 0
                    t = dt.datetime.strptime(day, '%Y.%m.%d')

                    #print(last_day, day, pre_day)
                    # ------处理打印--------------------------------------------
                    if day != last_day and day != pre_day:      # 相邻两个日期不同, 本日期只有一行
                        #print("1. 相邻两个日期不同, 本日期只有一行")
                        # 画图
                        if j in [1, 2, 3, 4]:
                            plt.plot(t, val, marker = '$\downarrow$', color = 'dimgrey', markersize=10, alpha = 1)
                            plt.text(t, val, val_text, horizontalalignment='center', 
                                        verticalalignment='bottom', fontsize = 10, alpha = 0.8, rotation = 45)
                        elif j == 6:
                            plt.plot(t, val, marker = '$\heartsuit$', color = 'red', markersize = 10)
                            plt.text(t, val, val_text, horizontalalignment='center', 
                                        verticalalignment='bottom', fontsize = 10, alpha = 0.8, rotation = 0)                     

                        else: 
                            plt.plot(t, val, marker = '>', color = 'gray', markersize = 8)
                            plt.text(t, val, val_text, horizontalalignment='center', 
                                        verticalalignment='bottom', fontsize = 10, alpha = 0.8, rotation = 10)

                        last_day = day
                
                    elif day == pre_day:        # 不是最后一个重复日
                        #print('2 不是最后一个重复日')
                        val_text = str(j) + '_' + val_text
                        d[val_text] = val
                        last_day = day

                    elif day == last_day and day != pre_day:        # 最后一个重复日
                        #print('3. 最后一个重复日')
                        val_text = str(j) + '_' + val_text
                        d[val_text] = val
                        #print(d)

                        ls = sorted(d.items(), key = lambda kv:(kv[1], kv[0]))    # 字典排序
                        d = {}

                        ll = []     # lable
                        lv = []     # value
                        lvc = []    # value change
                        for l, v in ls:
                            ll.append(l)
                            lv.append(v)

                        #print('ll = ', ll)
                        #print('lv = ', lv )

                        step = 1000
                        k = step                         # 处理value相近的值
                        lvc.append(lv[0])
                        for i in range(1, len(lv)):
                            #print(i, lv[i] )
                            if lv[i] - lv[i-1] < 500:
                                #print(i, '< 500')
                                lvc.append(lv[i] + k)
                                k += step
                            else:
                                lvc.append(lv[i])
                                k = step
                        #print('lvc = ', lvc)
                        
                        lj = []
                        llabel = []
                        for item in ll:
                            lj.append(item.split('_')[0])
                            llabel.append(item.split('_')[1])
                        #print('lj = ', lj, '\nllabel = ', llabel)

                    

                        i = 0
                        for j in lj:
                            j = int(j)
                            if j in [1, 2, 3, 4]:
                                plt.plot(t, lv[i], marker = '$\downarrow$', color = 'dimgrey', markersize=10, alpha = 1)
                                plt.text(t, lvc[i], llabel[i], horizontalalignment='center', 
                                                verticalalignment='bottom', fontsize = 10, alpha = 0.8, rotation = 45)
                            elif j == 6:
                                plt.plot(t, lv[i], marker = '$\heartsuit$', color = 'red', markersize = 10)
                                plt.text(t, lvc[i], llabel[i], horizontalalignment='center', 
                                            verticalalignment='bottom', fontsize = 10, alpha = 0.8, rotation = 0)     
                            else: 
                                plt.plot(t, lv[i], marker = '>', color = 'gray')
                                plt.text(t, lvc[i], llabel[i], horizontalalignment='center', 
                                            verticalalignment='bottom', fontsize = 10, alpha = 0.8, rotation = 10)
                            i += 1
                        last_day = day


                              
def plot_datas(df_vbalance, df_abalance):
    print('plot_datas')
    
    plt.rcParams['font.sans-serif'] = ['SimHei'] # 显示中文   步骤一（替换sans-serif字体）
    plt.rcParams['axes.unicode_minus'] = False   # 步骤二（解决坐标轴负数的负号显示问题）
    
    fig = plt.figure(figsize=(16, 8))
    #plt.figure(dpi=50)  
    ax = plt.subplot(111)
    ax.spines['top'].set_visible(False)         # 取消 上 右边框
    ax.spines['right'].set_visible(False)
    # ax_soc.set_title('SOC.txt map')

    plt.subplots_adjust(top=0.99, bottom=0, left=0.05, right=0.95)  # 调整边距
    #plt.subplots_adjust(right=0.8)
    
    ''' 处理日期 '''
    riqi = df_vbalance.index.values
    dates = [dt.datetime.strptime(c, '%Y.%m.%d') for c in riqi]
    #print(dates)


    """设置坐标轴的格式"""
    # 设置主刻度, 每6个月一个刻度
    daygap = mdates.DayLocator(interval=7)
    ax.xaxis.set_major_locator(daygap)

    # 设置次刻度，每个月一个刻度
    daygap = mdates.DayLocator(interval=1)
    ax.xaxis.set_minor_locator(daygap)

    xfmt = matplotlib.dates.DateFormatter('%Y %m.%d')   # 设置 x 坐标轴的刻度格式
    ax.xaxis.set_major_formatter(xfmt)


    # ^^^^^^^^^^^^^^^^^^^ 画图 ^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
    # 1. 虚拟账户
    plot_vbalance(df_vbalance, dates)

    # 2. 账户
    plot_abalance(df_abalance, dates)

    # 3. label
    plot_label(df)
    


    # =============================================================================

    #plt.xlabel('riqi')
    #plt.ylabel('jine')
    #plt.title('Charging chart')
    #plt.legend(loc='best')  loc='upper center'
    plt.legend(loc='upper right', bbox_to_anchor=(1.05, 1))
    #ax.legend(loc='upper center', ncol=1, bbox_to_anchor=(1.15,1))

    # ---------坐标精度---------------------
    #ymajorLocator = MultipleLocator(500)  # set Y 坐标刻度 精度为1
    #ax_soc.yaxis.set_major_locator(ymajorLocator)

    #xmajorLocator = MultipleLocator(0.01)  # set x 坐标刻度 精度为1
    #ax.xaxis.set_major_locator(xmajorLocator)
    #plt.xticks(rotation = 45)
    
    fig.autofmt_xdate() 
    plt.show()






if __name__ == '__main__':
    import numpy as np
    import pandas as pd
    import seaborn

    import sys
    import os
    # %matplotlib inline
    import matplotlib
    import matplotlib.pyplot as plt
    from matplotlib.ticker import MultipleLocator, FormatStrFormatter
    import matplotlib.dates as mdates

    import datetime as dt
    from dateutil.relativedelta import relativedelta
    import time

    #ditem = {'extera': 10, 'fixed': 11, 'douyin': 12}
    #print(ditem)
    #time = []
    #datas = []
    #get_data(time, datas)
    #df = pd.DataFrame(data=datas, columns=time)

    # 1 资产类；  -1 负债类
    # dvirtual = [{'日常'：1}, {'额外'}}]
    lvirt = ['日常', '额外', '抖音', '固定', 'vcheck', 'income']
    lvirtv = [-1,      -1,   -1,     -1,      1,       1]

    laccout = ['vlend', '招商', '平安', '余额宝', '农商', '花呗', '平安信', '微粒', '白条']
    laccoutv =[1,       1,       1,     1,        -1,      -1,     -1,     -1,      -1]

    cl = lvirt + laccout
    cl.insert(0, '日期')
    row_account = lvirt + laccout





    # 1. file 读入数据， 拆分好日期
    print('\n  ** setp 1. to_day_datas ^^^^^')
    # datas内容：  日期 及 此日期 记录的list 组成的list 
    # [ [11.15, ['余额宝', '对账', '4800', 'income']], 
    #   [11.16, ['日常', '火锅', '138'], ['日常', '洗发水', '200'],
    #   ...
    #  ]
    datas = []
    try:
        dir = sys.argv[1]
    except:
        dir = '4.txt'

    to_day_datas(dir)

    # sys.exit(0)
    print('datas = \n', datas)

    

    # 2. 每条记录，变成 df的一行，用贷记记账
    # fdatas 贷记记账法转化过后的数据
    #[  ['2021.11.15', 0, 0, 0, 0, 0, ['被动', 4800], 0, 0, 0, ['对账', 4800], 0, 0, 0, 0, 0], 
    #   ['2021.11.16', ['火锅', 138], 0, 0, 0, 0, 0, 0, 0, 0, 0, ['被动', 138], 0, 0, 0, 0], 
    #   ... 
    # ]
    fdatas = []
    print('\n\n   **setp 2. to_doubule_entry_account ^^^^ ')
    every_record_to_one_doubule_entry_account_line(datas, fdatas)
    # print("***********************************")
    # for item in fdatas:
    #    print(item, '\n')
 


    # 3. 贷记记账文本   组成df
    #              日期           日常         额外 抖音 固定         vcheck        income  vlend 招商 平安        余额宝         农商 花呗       平安信  微粒          白条
    # 0   2021.11.15              0            0    0    0              0  [被动, 4800]      0    0    0  [对账, 4800]            0    0            0     0             0
    # 1   2021.11.16    [火锅, 138]            0    0    0              0             0      0    0    0             0  [被动, 138]    0            0     0             0

    print('\n\n    **step 3. df 带label')
    pd.set_option('display.unicode.ambiguous_as_wide', True)    # 支持中文
    pd.set_option('display.unicode.east_asian_width', True)
    pd.set_option('display.width', 300)         # 设置打印宽度(**重要**)    打印对齐
    # df2 = pd.DataFrame(data=data, index=index, columns = columns)
    df = pd.DataFrame(data = fdatas, columns = cl)
    print('df = \n', df)



    # 4. df 相同日合并， 祛除label
    print("\n\n 4. df 相同日合并， 祛除label")
    df_day = to_df_daysum(df)
    print('df_day = \n', df_day)



    # 5. df_balance ;  balance 余额
    print('\n\n **step 5:  df_stat 每个账户余额 及 类型余额')
    df_vbalance, df_abalance = to_df_balance(df_day)

    print()
    print('df_vbalance = \n', df_vbalance)
    print()
    print('df_abalance = \n', df_abalance)

    # 6. 本期结余处理, 写入到dir_out.txt文件中
    print('\n\n# 6. 本期结余处理, 写入到dir_out.txt文件中')
    process_current_balance(df_abalance)


    sys.exit(0)
    # 7. 画图
    plot_datas(df_vbalance, df_abalance)