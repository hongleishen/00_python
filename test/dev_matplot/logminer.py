def plot_box(m):
    fig = m.fig
    # -----------------box----------------------------
    ax2 = fig.add_axes([0.55, 0.55, 0.4, 0.4])
    m.ax2 = ax2
    df_fast = m.df_fast
    I_batt = df_fast['batteryCurrent'] / 1000000
    V_bus = df_fast['usbVoltage'] / 1000000
    ax2.boxplot([I_batt, V_bus], vert=True)
    plt.xticks([1, 2], ['I_batt', 'V_bus'])
    ax2.set_title('box stat for fast charge')

    std = I_batt.describe()[2]
    std = round(std, 1)
    std = str(std)
    ax2.text(1.2, 5, 'std:' + std, alpha=0.5)

    std = V_bus.describe()[2]
    std = round(std, 1)
    std = str(std)
    ax2.text(2.2, 5, 'std:' + std, alpha=0.5)


def find_taper_i(m):
    df = m.df
    #ls = [208, 209, 210, 211, 212, 213, 214, 215, 216, 217,]
    ls = df['batterychrgType'].index[df['batterychrgType'] == 'Taper']
    if len(ls) < 5:
        return -1
    for i in range(len(ls)):
        if ls[i] == ls[i + 1] - 1 == ls[i + 2] - 2 == ls[i + 3] - 3 == ls[
                i + 4] - 4:
            return ls[i]
    return -1


def process_state(m):
    #global ax
    print('process_state ========================================')
    ax = m.ax
    text = ''
    df = m.df
    print('m.t_soc_100 = ', m.t_soc_100)

    if m.is_soc_100 == True:
        df = df[df['Uptime'] <= m.t_soc_100]

    ctype = df['usbRealType'].value_counts(ascending=True).index.values[-1]
    print(ctype)
    t_max = df['Uptime'].max()
    t_min = df['Uptime'].min()

    #------taper----------------------------
    i = find_taper_i(m)
    if i > 0:
        m.is_taper = True
        m.i_taper = i
        m.t_taper = df['Uptime'][i]
        m.df_fast = df[df['Uptime'] <= m.t_taper]
    else:
        m.df_fast = df

    if m.is_taper == False:
        type = 50
    elif m.is_soc_100 == False:
        type = 70
    elif m.is_soc_100:
        type = 100

    print('type = ', type)

    plot_box(m)
    #总体描述
    #1.充电类型
    ax.text(0.1,
            0.47,
            'Charger type: ' + ctype,
            horizontalalignment='left',
            verticalalignment='top',
            fontsize=15,
            color='b',
            alpha=1)

    #2. 充电 soc time过程:
    if m.is_soc_100 == False:
        soc_text = str(df['SOC'].min()) + '%' + ' ~ ' + str(
            df['SOC'].max()) + '%'
        time_text = str(df['Uptime'].min()) + 's' + ' ~ ' + str(
            df['Uptime'].max()) + 's'
    else:
        mid_text = str(df['SOC'][m.i_taper]) + '%' + '(taper) ~ '
        soc_text = str(df['SOC'].min()) + '%' + ' ~ ' + mid_text + str(
            df['SOC'].max()) + '%'

        t_fast = round((m.t_taper - df['Uptime'].min()) / 3600, 2)
        t_fast_str = ' ' + str(t_fast) + 'h'
        t_full = round((m.t_soc_100 - df['Uptime'].min()) / 3600, 2)
        t_full_str = ' (' + str(t_full) + 'h)'

        mid_text = str(m.t_taper) + 's' + '(taper' + t_fast_str + ') ~ '
        time_text = str(df['Uptime'].min()) + 's' + ' ~ ' + mid_text + str(
            df['Uptime'].max()) + 's' + t_full_str

    ax.text(0.1,
            0.4,
            'Charging SOC form: ' + soc_text,
            horizontalalignment='left',
            verticalalignment='top',
            fontsize=10,
            color='g',
            alpha=1)
    ax.text(0.1,
            0.36,
            'Charging time form: ' + time_text,
            horizontalalignment='left',
            verticalalignment='top',
            fontsize=10,
            color='g',
            alpha=1)

    #3. 充电功率
    # fast

    def calculate_power(lt, lv, li, df):
        delt = 0
        delt_w = 0
        w = 0
        for i in range(len(lt) - 1):
            delt = lt[i + 1] - lt[i]
            u = lv[i]
            i = li[i]
            delt_w = u * i * delt

            w += delt_w
            #print(u,i,delt_w, w)

        t = df['Uptime'].max() - df['Uptime'].min()
        #print(t)
        P_argv = w / t
        P_argv = round(P_argv, 2)

        w /= 3600
        w = round(w, 2)
        text = 'P_argv:' + str(P_argv) + 'w    W:' + str(w) + 'Wh'
        print(text)
        return text, w

    if type:
        #只统计fast
        df = m.df_fast

        #a. battery
        lt = df['Uptime']
        lv = df['batteryVoltage'] / 1000000
        li = df['batteryCurrent'] / 1000000
        text, w_out = calculate_power(lt, lv, li, df)
        ax.text(0.06,
                0.30,
                '              battery: ' + text,
                horizontalalignment='left',
                verticalalignment='top',
                fontsize=10,
                alpha=1)

        #b. usb
        lv = df['usbVoltage'] / 1000000
        li = df['usbCurrent'] / 1000000

        text, w_in = calculate_power(lt, lv, li, df)
        ax.text(0.06,
                0.26,
                '              usb:       ' + text,
                horizontalalignment='left',
                verticalalignment='top',
                fontsize=10,
                alpha=1)

        #c. effeciency
        effeciency = w_out / w_in * 100
        effeciency = ('%.2f' % effeciency)
        print('effeciency = ', effeciency)
        ax.text(0.41,
                0.29,
                'Effeciency: ' + effeciency + '%',
                horizontalalignment='left',
                verticalalignment='top',
                fontsize=10,
                alpha=1)

        #d. cp功率情况
        lv = df['usbVoltage'] / 1000000
        li = df['CPCurrent'] / 1000000
        text, w_cp = calculate_power(lt, lv, li, df)
        r = (w_cp / w_in) * 100
        ratio = ('%.2f' % r)

        text += '            CP energy contribution ratio:' + ratio + '%'
        ax.text(0.06,
                0.22,
                '              CP:        ' + text,
                horizontalalignment='left',
                verticalalignment='top',
                fontsize=10,
                alpha=1)

        #e. CP工作时间
        t_fast = df['Uptime'].max() - df['Uptime'].min()
        t_cp = 0

        I_cp = df['CPCurrent'] / 1000000
        lt = df['Uptime']
        for i in range(len(I_cp) - 1):
            #print(I_cp[i])
            if I_cp[i] > 0.1:
                delt = lt[i + 1] - lt[i]
                t_cp += delt
        t_cp_m = t_cp / 60

        ratio = t_cp / t_fast * 100
        ratio = ('%.2f' % ratio)

        t_cp = ('%.2f' % t_cp)
        t_cp_m = ('%.1f' % t_cp_m)

        t_fast_m = t_fast / 60
        t_fast_m = ('%.1f' % t_fast_m)

        text = 'T_cp:' + t_cp_m + '(m)    ' + 'T_fast:' + t_fast_m + '(m)    ' + 'Time ratio: ' + ratio + '%'
        #print('t_cp,t_fast =',t_cp,t_fast)
        ax.text(0.06,
                0.18,
                '              CP:        ' + text,
                horizontalalignment='left',
                verticalalignment='top',
                fontsize=10,
                alpha=1)

        ax.text(0.003,
                0.24,
                'Fast charging',
                horizontalalignment='left',
                verticalalignment='top',
                fontsize=12,
                alpha=1,
                color='b')

    # full
    if type == 100:
        df = m.df
        df = df[df['SOC'] < 100]

        #a. battery
        lt = df['Uptime']
        lv = df['batteryVoltage'] / 1000000
        li = df['batteryCurrent'] / 1000000
        text, w_out = calculate_power(lt, lv, li, df)
        ax.text(0.06,
                0.09,
                '              battery: ' + text,
                horizontalalignment='left',
                verticalalignment='top',
                fontsize=10,
                alpha=1)

        #b. usb
        lv = df['usbVoltage'] / 1000000
        li = df['usbCurrent'] / 1000000

        text, w_in = calculate_power(lt, lv, li, df)
        ax.text(0.06,
                0.05,
                '              usb:       ' + text,
                horizontalalignment='left',
                verticalalignment='top',
                fontsize=10,
                alpha=1)

        effeciency = w_out / w_in * 100
        effeciency = ('%.2f' % effeciency)
        #effeciency = round(effeciency,3)
        ax.text(0.41,
                0.075,
                'Effeciency: ' + effeciency + '%',
                horizontalalignment='left',
                verticalalignment='top',
                fontsize=10,
                alpha=1)

        ax.text(0.003,
                0.065,
                'Full charging',
                horizontalalignment='left',
                verticalalignment='top',
                fontsize=12,
                alpha=1,
                color='b')

    #5.JEITA


#===========state end ==================================================


def time_process_dmesg(m):
    s_dir = m.dir + '\dmesg.txt'
    d_dir = m.dir + '\dmesg_py.txt'
    if m.set_min <= m.st_min and m.set_max >= m.st_max:
        shutil.copyfile(s_dir, d_dir)
        return

    f_temp = open(d_dir, 'w')
    dir = m.dir + '\dmesg.txt'
    with open(dir) as f:
        for line in f.readlines():
            res = re.findall('\[\s*?(\d*.\d*)\]', line)
            t = float(res[0])
            if t > m.set_min:
                f_temp.write(line)
                if t > m.set_max:
                    break
        f_temp.close()


# def get_soc_msg(m): #准备废弃
# print('get in soc_msg')
# datas = []
# cl =['Uptime']
# n = 0
# li = []
# t_max = 0
# #print(sys._getframe().f_code.co_name,  sys._getframe().f_lineno, m.dir)
# get_data(n,li,cl,datas,m)
# df = pd.DataFrame(data=datas,columns=cl)
# #print(m)
# index = find_i_max(df, 100)
# if index != 0 :
# m.is_soc_100 = True
# m.soc_100_index = index
# m.t_soc_100 = df['Uptime'][index]
# print('m.is_soc_100 = ',m.is_soc_100)
# m.st_min = df['Uptime'].min()
# m.st_max = df['Uptime'].max()
# m.df = df
# print('get_soc_msg honglei', m.st_min,m.st_max, m.is_soc_100,m.t_soc_100)
#print(m.df)


#a
def use_time(m, set_min, set_max):
    df = m.df
    print(sys._getframe().f_code.co_name,
          sys._getframe().f_lineno, set_min, set_max)
    print(m.st_min, m.st_max, set_min, set_max)

    if set_max <= set_min:
        print('set time max < set time min ')
        return
    if set_min < m.st_min:
        set_min = m.st_min
    if set_max > m.st_max:
        set_max = m.st_max

    m.set_min = set_min
    m.set_max = set_max
    #soc.txt 修改时间

    #1. soc图
    df = df[df['Uptime'] <= set_max]
    df = df[df['Uptime'] >= set_min]  #开始 结束时间传入

    print('after df modify\n', df)

    plot_soc(df)
    print('continue ....')

    time_process_dmesg(m)

    #2. votable图
    print('will draw votable ...')
    subprocess.Popen(['python', 'votable.py', sys.argv[1], 'pcall'])

    #3. state 图
    print('will draw state ...')
    subprocess.Popen(['python', 'state.py', sys.argv[1], 'pcall'])


#b
def use_soc(m, set_soc_min, set_soc_max):
    print(sys._getframe().f_code.co_name,
          sys._getframe().f_lineno, set_soc_min, set_soc_max)
    df = m.df

    if set_soc_min >= set_soc_max or set_soc_min > df['SOC'].max(
    ) or set_soc_max < df['SOC'].min():
        print('soc type Error!')
        return

    if set_soc_max > df['SOC'].max():
        set_soc_max = df['SOC'].max()
    if set_soc_min < df['SOC'].min():
        set_soc_min = df['SOC'].min()

    #1. soc图
    df = df[df['SOC'] <= set_soc_max]
    df = df[df['SOC'] >= set_soc_min]  #开始 结束时间传入
    print('after df modify\n', df)
    plot_soc(df)
    #df = df.reset_index(drop = True)

    #2. votable图
    df = m.df
    i_max = find_i_max(df, set_soc_max)
    if i_max != 0:
        set_soc_t_max = df['Uptime'][i_max]
        m.set_max = set_soc_t_max
        print('i_max,set_soc_t_max = ', i_max, set_soc_t_max)

    i_min = find_i_max(df, set_soc_min)
    if i_min != 0:
        set_soc_t_min = df['Uptime'][i_min]
        m.set_min = set_soc_t_min
        print(i_min, set_soc_t_min)
    if m.set_min == 0 and m.set_max == 0:
        print('soc type Error!')
        return

    time_process_dmesg(m)

    #2. votable图
    subprocess.Popen(['python', 'votable.py', sys.argv[1], 'pcall'])

    #3. state 图
    subprocess.Popen(['python', 'state.py', sys.argv[1], 'pcall'])


#c
def use_str(m, str):
    print(sys._getframe().f_code.co_name, sys._getframe().f_lineno, str)
    s_dir = m.dir + '\dmesg.txt'
    d_dir = m.dir + '\dmesg_py.txt'

    df = m.df
    if str == 'default':
        if m.is_soc_100 == False:
            shutil.copyfile(s_dir, d_dir)
        else:
            df = df[df['SOC'] < 100]
            m.set_max = df['Uptime'].max()
            m.set_min = 0
            time_process_dmesg(m)

    elif str == 'full':
        shutil.copyfile(s_dir, d_dir)

    elif str == 'fast':
        i_taper = 0
        t_taper = 0
        try:
            i_taper = df['batterychrgType'].index[df['batterychrgType'] ==
                                                  'Taper'][0]
            t_taper = df['Uptime'][i_taper]
            df = df[df['Uptime'] < t_taper]
        except:
            t_taper = df['Uptime'].max()
        m.set_max = t_taper
        m.set_min = 0

        print('\n i_tapper,t_tapper', i_taper, t_taper)
        print(m.set_max)
        time_process_dmesg(m)

    print('end\n m.set_max', m.set_max)
    #plot_soc(df)

    #2. votable图
    print('\nwill draw votable ...')
    subprocess.Popen(['python', 'votable.py', sys.argv[1], 'pcall'])

    #3. state 图
    print('\nwill draw state ...')
    subprocess.Popen(['python', 'state.py', sys.argv[1], 'pcall'])

    plot_soc(df)


def pre_process_file(m, type, v1=None, v2=None):
    print(sys._getframe().f_code.co_name,
          sys._getframe().f_lineno, type, v1, v2)

    if type == 'use_time':
        use_time(m, v1, v2)
    if type == 'use_soc':
        use_soc(m, v1, v2)
    if type == 'use_str':
        use_str(m, v1)


# def function():
# print(sys._getframe().f_code.co_name,  sys._getframe().f_lineno)

# class Msg(object):
# def __init__(self):
# self.st_min = 0
# self.st_max = 0

# self.is_soc_100 = False
# self.t_soc_100 = 0
# self.soc_100_index = 0

# self.is_taper = False
# self.t_taper = 0
# self.i_taper = 0

# self.df = None
# self.dir = '.'

# self.set_min = 0
# self.set_max = 0

# #self.text_i = 0

# self.use_time = ''
# self.use_soc = ''
# self.use_str = ''

# self.fig = None
# self.ax2 = None
# self.df_fast = None
# self.ax = None

if __name__ == '__main__':
    import os
    import shutil
    import sys
    import matplotlib.pyplot as plt
    import subprocess
    import numpy as np
    import pandas as pd
    import re

    from matplotlib.widgets import RadioButtons
    from matplotlib.widgets import CheckButtons
    from matplotlib.widgets import Button
    from matplotlib.widgets import TextBox
    from soc import *

    print(sys.argv)

    if len(sys.argv) == 3:
        if sys.argv[2] != 'option':
            sys.exit(
                "the 2th parameter should be 'option', if you want use option function!"
            )
    if len(sys.argv) >= 2:
        st = sys.argv[1]
        if st != '.' and st.find('\\') == -1 and st.find('/') == -1:
            sys.exit('the 1th parameter should directory!')
    if len(sys.argv) >= 1:
        st = sys.argv[0]
        if st.find('py') == -1:
            sys.exit('the 0th parameter should input python script name!')
    if len(sys.argv) == 1:
        sys.exit('plese input directory after xxx.py!')

    m = Msg_soc()
    m.dir = sys.argv[1]

    print('before get_soc_message =========================')
    get_soc_message(m)  #从soc.py调用
    print('end get_soc_message=== \n\n')

    if len(sys.argv) == 2:
        print('use default')
        pre_process_file(m, 'use_str', 'default')

    else:
        print('draw picture too choise')

        fig = plt.figure(figsize=(12, 7))
        ax = plt.subplot(111)
        #print('ax type =',type(ax))
        m.fig = fig
        m.ax = ax
        ax.text(0.25, 0.92, 'option', fontsize=13, color='b')
        ax.text(0.4,
                0.65,
                'use: default (soc to 100)\nfull (all log)\n or fast',
                horizontalalignment='left',
                verticalalignment='top',
                fontsize=7,
                alpha=0.5)

        plt.subplots_adjust(left=0.05, right=0.95, top=0.95, bottom=0.05)
        ax.spines['top'].set_visible(False)
        ax.spines['right'].set_visible(False)
        plt.title('LogMiner')

        #pylab.setp(histx.get_xticklabels(), visible=False)	# no X label
        #plt.subplots_adjust(uper=0.2)
        #plt.subplots_adjust(left=0.05)
        #plt.subplots_adjust(right=0.2)

        #处理state--------------------------------
        process_state(m)

        #state ===============================

        # -----------------box----------------------------
        # ax2 = fig.add_axes([0.55,0.55,0.4,0.4])
        # m.ax2 = ax2
        # df_fast = m.df_fast
        # I_batt = df_fast['batteryCurrent']/1000000
        # V_bus  = df_fast['usbVoltage']/1000000
        # ax2.boxplot([I_batt,V_bus],vert = True)
        # plt.xticks([1,2],['I_batt','V_bus'])
        # ax2.set_title('box stat for fast stage')

        # std = I_batt.describe()[2]
        # std = round(std,1)
        # std = str(std)
        # ax2.text(1.2, 0,'std:'+std )

        # std = V_bus.describe()[2]
        # std = round(std,1)
        # std = str(std)
        # ax2.text(2.2,0,'std:'+std )

        # --------------- use_time ---------------------------------------
        def use_time_submit(text):
            #m.text_i += 1
            if m.use_time != text:
                m.use_time = text
                m.use_soc = ''
                m.use_str = ''
                print('use_time_submit', text)
                ls = text.split(' ')
                ls = list(map(float, ls))
                pre_process_file(m, 'use_time', ls[0], ls[1])

        pxbox = plt.axes([0.2, 0.8, 0.2, 0.04])
        t_box = TextBox(pxbox, 'use_time')
        t_box.on_submit(use_time_submit)

        # --------------- use_soc -------------------------------------
        def use_soc_submit(text):
            #m.text_i += 1
            if m.use_soc != text:
                m.use_time = ''
                m.use_soc = text
                m.use_str = ''
                print('use_soc_submit', text)
                ls = text.split(' ')
                ls = list(map(float, ls))
                pre_process_file(m, 'use_soc', ls[0], ls[1])

        pxbox = plt.axes([0.2, 0.7, 0.2, 0.04])
        soc_box = TextBox(pxbox, 'use_soc')
        soc_box.on_submit(use_soc_submit)

        # ---------------- use_str -------------------------------------------
        def use_str_submit(text):
            #m.text_i += 1
            if m.use_str != text:
                m.use_time = ''
                m.use_soc = ''
                m.use_str = text
                print('use_str_submit', text)
                pre_process_file(m, 'use_str', text)

        pxbox = plt.axes([0.2, 0.6, 0.2, 0.04])
        str_box = TextBox(pxbox, 'use_str')
        str_box.on_submit(use_str_submit)

        plt.show()

    try:
        os.remove(m.dir + '\dmesg_py.txt')
    except:
        print('dmesg_py has remove')

    print('all over!')
