import re
import matplotlib.pyplot as plt
#---a.------pdo请求--------------------------
def get_pdo_data(text):
    lt = []
    lv = []
    res = re.findall('\[.*?(\d+\.\d+)\].*pdo_str\s\d+\s\d+\s(\d+)', text)
    #print(res)
    for t,v in res:
        t = float(t)
        lt.append(t)
        v = float(int(v)/100000)
        lv.append(v)
        
    return lt,lv
# fig1 = plt.figure()
# ax1 = plt.subplot(111) 
#plt.figure(figsize=(10, 10))   

#fig1,ax1 = plt.subplots(figsize=(12,8))
#ax1.plot(lt,lv,  linestyle=':',linewidth = 0.1, marker='o', markersize =1.5, markerfacecolor='r')  #pdo
#cursor = Cursor(ax1, useblit=True, linewidth = 0.5)
#multi = MultiCursor(fig1.canvas,(ax1),linewidth = 0.5)


def conv_square_wave(lt,ls):
    j = 0
    t = lt[:]
    v = ls[:]
    for i in range(len(t)):
        if(i == 0):
            continue
        i += j
        if(v[i] != v[i-1]):
            t.insert(i,t[i])
            v.insert(i,v[i-1])
            j += 1
    return t,v

# def adj_v(v):
    # a_v = list(map(lambda v: v*5+30, v))
    # return a_v

def get_Properties_status2_data(text,ltt,lt_main_hot,lt_cp_hot,lt_main,lt_cp,lt_enable,lt_sw_en,lt_main_cp,lt_state):
    #rest = re.findall('\[.*?(\d+\.\d+)\]\sCP:\sProperties:\s.*?status2=(\d+).*?main_hot=(\d)\scp_hot=(\d)\smain_deciDegC=(\d+)\scp_deciDegC=(\d+)\s.*enabled=(\d)\s.*?current_state=(.*+)', text)
    
    #[  176.626058] CP: Properties: status1=6 status2=81 vin_inc=1 vin_dec=0 irq_status=1\x0a main_hot=0 cp_hot=0 main_deciDegC=420 cp_deciDegC=400 max_delta=-20 min_delta=-40\x0a inc_steps=1 dec_steps=1 type=2 enabled=0 ok_to_cp=1 current_state=S_NOT_OK_TO_CP
    rest = re.findall('\[.*?(\d+\.\d+)\]\sCP:\sProperties:\s.*?status2=(\d+).*?main_hot=(\d)\scp_hot=(\d)\smain_deciDegC=(\d+)\scp_deciDegC=(\d+)\s.*enabled=(\d)\s.*?current_state=(.*)', text)
    #print(rest)

    sw_en = lambda phex:  hex((int('0x' + str(phex), 16)) & 0x81) == '0x80'

#     时间，       ，               M温度 cp温度，sw_en
    for t,status2,main_hot,cp_hot,main,cp,enabled, state in rest:
        #在同一个模块中可以 用全局变量，但在作为模块使用时，没法导入全局变量
        #global ltt   #即使global声明也不行
    
        t = float(t)
        ltt.append(t)
        
        main_hot = (int(main_hot)*5-2)
        lt_main_hot.append(main_hot)
        
        cp_hot = (-int(cp_hot))*5-4
        lt_cp_hot.append(cp_hot)

        tmain = int(main)/10
        lt_main.append(tmain)
        tcp = int(cp)/10
        lt_cp.append(tcp)
        
        lt_enable.append(int(enabled)*4 + 20)  #bit7  Enable pin high:   1 pin脚使能
        lt_sw_en.append(sw_en(status2)*4 + 15) #bit0  Switcher_hold_off: 1 hold_off ;  bit_0 && bit_7
        
        lt_main_cp.append(tmain - tcp)

        lt_state.append(state)

# #^^^^^^^^^^get_main_fcc_config^^^^^^^^^^^^^^
# #[  183.767859] QCOM-BATT: get_main_fcc_config: Disabling FCC slewing on CP Switcher disable
# find_get_main_fcc_config = re.findall(r'\[.*?(\d+\.\d+)\].*Disabling\sFCC\sslewing\son\sCP\sSwitcher\sdisable', text)
# t_cp_disable = list(map(float,find_get_main_fcc_config))
# print('at start:',t_cp_disable)

#-------------class Button Process-----------------------
class ButtonProcess(object):
    def __init__(self):
        self.button = 1
        
bu = ButtonProcess()
#==============span selector================================
# ax3 = plt.subplot(313)

# fig_3 = plt.figure()
# ax3 = plt.subplot(111)

def findmin_index(a,ls):
    min = 0
    if a <= ls[0]:
        min = 0
        return min
    for val in ls:
        if val > a:
            min = ls.index(val)
            break
    return min
    
def findmax_index(b,ls):
    max = 0
    l = len(ls)
    if b > ls[l-1] :
        max = l
        return max
    for val in ls:
        if val > b:
            max = ls.index(val)
            break
    return max



def onselect(xmin, xmax):
    if xmax - xmin < 0.3 :
        return
    fig3 = plt.figure() #每次选择范围，新建一个图，并把key_span重新的fig3重新设置，这样update为新fig图
    ax3 = plt.subplot(111)
    print(xmin, xmax)
    print('bu.button=',bu.button)
    
    if bu.button & 1 == 1:
        print('bu.button == 1')
        
        t_min_index = findmin_index(xmin, lt)
        t_max_index = findmax_index(xmax, lt)
        lt3 = lt[t_min_index : t_max_index]
        lv3 = lv[t_min_index : t_max_index]
        
        #ax3.plot(lt3,lv3,linestyle=':',linewidth = 0.5, marker='*', markersize =4, markerfacecolor='g')
        ax3.plot(lt3,lv3,linestyle=':',linewidth = 0.5, marker='*', color = 'lime')
        for t,v in zip(lt3,lv3):
            ax3.text(t,v,t, horizontalalignment='center', verticalalignment='bottom', fontsize = 6,alpha = 0.8, rotation = 65)
            ax3.text(t,v,v, horizontalalignment='center', verticalalignment='top', fontsize = 7,alpha = 0.95,rotation = 40)
    
    if bu.button & 2 == 2:  
        print('bu.button == 2')
        #t_switcher,v_switcher =  conv_square_wave(ltt,lt_sw_en)
        t_min_index = findmin_index(xmin, t_switcher)
        t_max_index = findmax_index(xmax, t_switcher)
        ts = t_switcher[t_min_index : t_max_index]
        vs = v_switcher[t_min_index : t_max_index]
        
        t_min_index = findmin_index(xmin, t_cp_disable)
        t_max_index = findmax_index(xmax, t_cp_disable)       
        t_cp = t_cp_disable[t_min_index : t_max_index]
        print(t_cp)
        
        if bu.button != 3:
            vs = list(map(lambda x:(x-15)/4,vs))
        ax3.spines['top'].set_visible(False)
        ax3.spines['right'].set_visible(False)
        
        ax3.plot(ts, vs, linewidth = 0.5, marker='.',markersize='1')                  #bit7 & bit0
        for i in range(len(ts)):
            if i == 0:
                continue
            if vs[i] > vs[i-1]:
                ax3.text(ts[i],vs[i],ts[i], horizontalalignment='left', verticalalignment='bottom', fontsize = 6,alpha = 0.8, rotation = 70)
            if vs[i] < vs[i-1]:
                ax3.text(ts[i-1],vs[i-1],ts[i-1], horizontalalignment='left', verticalalignment='bottom', fontsize = 5.8,alpha = 0.7, rotation = 70)
                
        #---cp_disable--from pl_disable-> get_main_fcc_config---   
        print(t_cp)
        for t in t_cp:
            ax3.vlines(t, 0.5,1,color = 'g',linestyle = 'dashed', linewidth = 0.5)
            ax3.text(t,0.5,t,horizontalalignment='left', verticalalignment='top', fontsize = 6,alpha = 0.8,rotation = -60)
            
    fig3.canvas.draw()
    fig3.show()
    


#^^^^^^radio button^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^










#####################################################################
#####################################################################
#####################################################################

if __name__ == '__main__':
    import numpy as np
    import pandas as pd
    #%matplotlib inline
    import re
    from matplotlib.widgets import SpanSelector
    from matplotlib.widgets import Cursor
    from matplotlib.widgets import MultiCursor
    from matplotlib.widgets import RadioButtons
    
    fig1,ax1 = plt.subplots(figsize=(12,8))
    cursor = Cursor(ax1, useblit=True, linewidth = 0.5)
    
    f = open('dmesg.txt')
    text = f.read()
    f.close()   
    
    span = SpanSelector(ax1, onselect, 'horizontal', useblit=True,
                rectprops=dict(alpha=0.5, facecolor='red')) #SpanSelector给 onselect返回 xmin，xmax两个参数
    
    #---缩放--------------------
    # ----------缩放--------------------------
    def call_back(event):
        axtemp=event.inaxes
        x_min, x_max = axtemp.get_xlim()
        fanwei = (x_max - x_min) / 10
        if event.button == 'up':
            axtemp.set(xlim=(x_min + fanwei, x_max - fanwei))
            #print('up')
        elif event.button == 'down':
            axtemp.set(xlim=(x_min - fanwei, x_max + fanwei))
            #print('down')
        fig1.canvas.draw_idle()  # 绘图动作实时反映在图像上
    fig1.canvas.mpl_connect('scroll_event', call_back)
    
    #^^^^^^radio button    
    axcolor = 'lightgoldenrodyellow'
    rax = plt.axes([0.001, 0.61, 0.05, 0.25], facecolor=axcolor) #位置
    radio = RadioButtons(rax, ('rdo','sw_en','all'))       #按钮

    def select_votable_func(label):
        print('radio select label: ', label)
        if label == 'rdo':
            bu.button = 1
        elif label == 'sw_en':
            bu.button = 2
        elif label == 'all':
            bu.button = 3
    radio.on_clicked(select_votable_func)
    
    #^^^^^^^^^^^^^^^^^pdo^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
    lt,lv = get_pdo_data(text)
    ax1.plot(lt,lv,  linestyle=':',linewidth = 0.1, marker='o', markersize =1.5, markerfacecolor='r')  #pdo
    
    #^^^^^^^^^^^^^^^^get_Properties_status2_data^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
    ltt = []
    lt_main_hot = []
    lt_cp_hot = []
    lt_main = []
    lt_cp = []

    lt_enable = []
    lt_sw_en = []

    lt_main_cp = []
    
    lt_state = []
    get_Properties_status2_data(text,ltt,lt_main_hot,lt_cp_hot,lt_main,lt_cp,lt_enable,lt_sw_en,lt_main_cp,lt_state)
    ax1.plot(ltt,lt_main)                                                     #main温度
    ax1.plot(ltt,lt_cp)                                                       #cp温度

    t,v = conv_square_wave(ltt,lt_main_hot)
    ax1.plot(t,v,linewidth = 0.4,linestyle=':')                               #main_hot 布尔值

    t,v = conv_square_wave(ltt,lt_cp_hot)
    ax1.plot(t,v,linewidth = 0.4,linestyle=':')                               #cp_hot布尔值

    plt.hlines(-2, ltt[0],ltt[-1] , color = 'b',linestyle=':', linewidth = 0.5)     #main_hot参考线
    plt.hlines(-4, ltt[0],ltt[-1] ,linestyle=':', linewidth = 0.5, alpha = 0.6)     #cp_hot  参考线
    ax1.plot(ltt, lt_main_cp,linewidth=0.5)                                         #Tmain - Tcp

    t,v = conv_square_wave(ltt,lt_enable)
    ax1.plot(t, v, linewidth = 0.5, marker ='.',markersize='1' )               #bit7  

    #ax1.plot(ltt,  lt_sw_en, linewidth = 0.5, marker ='.',markersize='1', alpha = 0.9,color='y')
    t_switcher,v_switcher =  conv_square_wave(ltt,lt_sw_en)
    ax1.plot(t_switcher,v_switcher,linewidth = 0.5, marker='.',markersize='1')                  #bit7 & bit0
    for i in range(len(t_switcher)):
        if i == 0:
            continue
        if v_switcher[i] > v_switcher[i-1]:
            ax1.text(t_switcher[i],v_switcher[i],t_switcher[i], horizontalalignment='left', verticalalignment='bottom', fontsize = 6,alpha = 0.8, rotation = 70)
        if v_switcher[i] < v_switcher[i-1]:
            ax1.text(t_switcher[i-1],v_switcher[i-1],t_switcher[i-1], horizontalalignment='left', verticalalignment='bottom', fontsize = 5.8,alpha = 0.7, rotation = 70)

    ax1.set_xlim(170,480)
    plt.show()
