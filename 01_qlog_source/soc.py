#!C:\ProgramData\Anaconda3\python.exe
# 7th 更多参数，删除输入时间，线条颜色，透明度调整


def dmseg_temp_process(dir,set_tmin,set_tmax):
    import re
    f_temp = open('temp.txt','w')
    with open(dir) as f:
        for line in f.readlines():
            res = re.findall('\[\s*?(\d*.\d*)\]', line)
            t = float(res[0])
            if t > set_tmin:
                f_temp.write(line)
                if t > set_tmax:
                    break
    f_temp.close()
    f_temp = open('temp.txt')
    text = f_temp.read()
    #print(text)
    f_temp.close()
    return text       #局部变量函数反回会消失


class Msg_soc(object):
    def __init__(self):
        self.st_min = 0
        self.st_max = 0
        
        self.is_soc_100 = False
        self.t_soc_100 = 0
        self.soc_100_index = 0
        
        self.is_taper = False
        self.t_taper = 0
        self.i_taper = 0

        
        self.df = None
        self.dir = '.'
        
        self.set_min = 0
        self.set_max = 0
        
        #self.text_i = 0
        
        self.use_time = ''
        self.use_soc = ''
        self.use_str = ''
        
        self.fig = None
        self.ax2 = None
        self.df_fast = None
        self.ax = None
        

def get_soc_message(m):
    import pandas as pd
    datas = []
    cl =['Uptime']
    n = 0
    li = []
    t_max = 0 
    get_data(n,li,cl,datas,m)
    df = pd.DataFrame(data=datas,columns=cl)
    
    index = find_i_max(df, 100)
    if index != 0 :
        m.is_soc_100 = True
        m.soc_100_index = index
        m.t_soc_100 = df['Uptime'][index]

    m.st_min = df['Uptime'].min()
    m.st_max = df['Uptime'].max()
    m.df = df
    print('get_soc_msg', m.st_min,m.st_max, m.is_soc_100,m.t_soc_100)
    #print(m.df)
#=======================================

def find_i_max(df, v_soc):
    i = 0
    for s in df['SOC']:
        if s >= v_soc:
            return i
        i += 1
    return 0
    
def find_t_max(df, v_soc = 100):
    i = find_i_max(df, v_soc)
    if i != 0 : 
        t_max = df['Uptime'][i]
    else :
        t_max = 0
    return t_max

def find_t_mid_index(df, v_soc = 100):
    #print(df)
    i_max = find_i_max(df, v_soc)
    if i_max == 0 :
        return int( len(df['SOC'])/2 )
    else :
        return int(i_max/2)
    
    #
    # t_max = df['Uptime'].max()
    # t_min = df['Uptime'].min()
    # t_mid = t_min + (t_max - t_min)/2
    # return t_mid
    
def parse(info,str,n,cl,li):
    #global li
    if info.startswith(str):
        temp = info.split(':')
        if n == 2:              #cl是行名，只解一行即可，其余相同
            cl.append(temp[0])
        if temp[1] == '':
            temp[1] = '0'
        try:   
            temp = int(temp[1])
        except:
            temp = temp[1]
        li.append(temp)        #li 是line 一行的数据,, 一行数据，添加一行的指定元素值

def get_data(n,li,cl,datas,m = None):
    #global li
    #global n
    #print(m.dir)
    if m!= None and m.dir != None:
        soc_dir = m.dir + '\SOC.txt'
    else:
        soc_dir = 'SOC.txt'
    print('soc_dir =', soc_dir)
    with open(soc_dir) as f:
        for line in f.readlines(): 
            #global n
            n = n+1
            if(line.startswith('device')):
                #print("device here")
                device = line.split('=')
                temp = device[1].split(' ')
                temp = float(temp[0])
                li.append(temp)
                #cl.append("Uptime")
                #print(li)
            else :
                #print(line)
                l = line.split(':: ')
                for index,info in enumerate(l):
                    if index == len(l)-1:
                        datas.append(li)            #datas是整体数据, 添加一整行数据
                        #print('empty li[]')
                        li = []
                        break
                    
                    parse(info,'usbVoltage',n,cl,li)
                    parse(info,'usbCurrent',n,cl,li)
                    parse(info,'inputCurrentSettled',n,cl,li)
                    parse(info,'inputCurrentLimited',n,cl,li)
                    
                    parse(info,'batteryTemp',n,cl,li)
                    parse(info,'batteryCurrent',n,cl,li)
                    parse(info,'batteryVoltage',n,cl,li)
                    
                    parse(info,'SOC',n,cl,li)
                    parse(info,'chargerTemp',n,cl,li)
                    
                    parse(info,'CP_temp',n,cl,li)
                    parse(info,'CPEnable',n,cl,li)
                    parse(info,'CPCurrent',n,cl,li)
                    
                    parse(info,'batterychrgType',n,cl,li)
                    parse(info,'usbRealType',n,cl,li)
    f.close()



#fig.canvas.mpl_connect('button_press_event', call_back)
# -----------------------------

def draw_ticks(df,sw,plt):
    #global has_ticks
    time = df['Uptime']
    # print(time)
    # print('max - min =',df['Uptime'].max() - df['Uptime'].min())
    if df['Uptime'].max() - df['Uptime'].min() < 500 or sw:
        #has_ticks = 1
        for t in time:
            plt.plot(t,10, marker = 'v', alpha = 0.4)
            plt.text(t,10-0.2,t, rotation = 35, alpha = 0.7, fontsize = 6, horizontalalignment='center')
            plt.vlines(t,0,10, linewidth = 0.5, linestyle = 'dashed', alpha = 0.1)    
        # --------------------------------------   


def plot_soc(df):
    import matplotlib.pyplot as plt
    from matplotlib.ticker import MultipleLocator, FormatStrFormatter
    print('plot_soc')
    
    # ----------缩放--------------------------
    def call_back(event):
        global has_ticks
        axtemp=event.inaxes
        x_min, x_max = axtemp.get_xlim()
        fanwei = (x_max - x_min) / 10
        if event.button == 'up':
            axtemp.set(xlim=(x_min + fanwei, x_max - fanwei))
            #print('up')
        elif event.button == 'down':
            axtemp.set(xlim=(x_min - fanwei, x_max + fanwei))
            #print('down')
        fig_soc.canvas.draw_idle()  # 绘图动作实时反映在图像上
    
        # if x_max - x_min < 500 and has_ticks == 0 :
            # has_ticks = 1
            # draw_ticks(df,1)
   
    fig_soc = plt.figure(figsize=(16,8))
    fig_soc.canvas.mpl_connect('scroll_event', call_back)
    ax_soc = plt.subplot(111)
    ax_soc.spines['top'].set_visible(False)
    ax_soc.spines['right'].set_visible(False)
    ax_soc.set_title('SOC.txt map')
    
    ######################画图###################################
    time = df['Uptime']
    try:
        V_bus = df['usbVoltage']
        plt.plot(time,V_bus/1000000,'b',linewidth = 3.0)
    except:
        print('missing Uptime')
        
    try:
        I_bus = df['usbCurrent']
        plt.plot(time,I_bus/1000000,'--', color='olive', linewidth = 0.7, alpha = 0.8)
    except:
        print('Missing usbCurrent')
    #plt.plot(time,I_bus_set)
        
    try:
        T_batt  = df['batteryTemp']
        plt.plot(time,T_batt/100,'-.',color='r',alpha = 0.5)
    except:
        print('Missing batteryTemp')
        
    try:
        I_batt  = df['batteryCurrent']
        plt.plot(time,I_batt/1000000,'g',linewidth = 2.0)
    except:
        print('Missing batteryCurrent')

    #SOC

    try:
        T_main = df['chargerTemp']
        plt.plot(time,T_main/100,':', color = 'hotpink')
    except:
        print('Missing chargerTemp')

    # ----------------------
    try:
        T_cp = df['CP_temp']
        plt.plot(time,T_cp/100, '-.',color = 'orange', linewidth = 0.5, marker='.')
    except:
        print('Missing CP_temp')

    try:
        I_cp = df['CPCurrent']
        plt.plot(time, I_cp/1000000,color = '#ADFF2F',linewidth= 0.6)
    except:
        print('Missing CPCurrent')

    try:
        En_cp = df['CPEnable']
        plt.plot(time,En_cp, color = 'b', linewidth =0.3, alpha = 0.7)
    except:
        print('Missing CPEnable')
    ############################################################
    
    
    #plt.grid(True)
    plt.xlabel('time(s)')
    plt.ylabel('normalization')
    #plt.title('Charging chart')
    plt.legend(loc='best')
    #ax.legend(loc='upper center', ncol=1, bbox_to_anchor=(1.15,1))

    # ----------tick---------------------
    ymajorLocator = MultipleLocator(1)
    ax_soc.yaxis.set_major_locator(ymajorLocator)

    # str_time = list(map(str, time))
    # plt.xticks(time, str_time, rotation=35)
    # plt.grid(linestyle = '--', axis = 'x', linewidth =0.2)
    pltt = plt
    draw_ticks(df, 0,pltt)
    
    plt.show()

class Msoc(object):
    def __init__(self):
        self.dir = None
    

# 入口参数:
# 1.名字
# 2.地址 可选；  如果不输入， 默认为本文件
if __name__ == '__main__':
    import numpy as np
    import pandas as pd
    import matplotlib.pyplot as plt
    import sys
    #%matplotlib inline
    from matplotlib.ticker import MultipleLocator, FormatStrFormatter
    
    datas = []
    cl =['Uptime']
    n = 0
    li = []
    t_max = 0
    
    m_soc = Msoc()
    if len(sys.argv) >= 2:
        m_soc.dir = sys.argv[1]
    get_data(n,li,cl,datas, m_soc)
    # np_datas = np.array(datas)
    # index = np_datas[:,0]
    
    # print(cl)
    # print(datas)
    df = pd.DataFrame(data=datas,columns=cl)

    #四个参数 截取时间
    #两个参数 默认soc_100
    #不会被程序调用 只用plt_soc函数df都是已经被截取好的
    if len(sys.argv) == 4:
        if float(sys.argv[3]) <= float(sys.argv[2]):
            print('t_max < t_min')
            sys.exit(1)
        else :
            df = df[df['Uptime'] <= float(sys.argv[3]) ]
            df = df[df['Uptime'] >= float(sys.argv[2]) ]   #开始 结束时间传入 
    elif len(sys.argv) <= 2:
        df = df[df['SOC'] < 100]
    plot_soc(df)
