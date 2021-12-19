
def findmin_index(a,ls):
    min = 0
    # print('findmin_index of ls\n',ls)
    # print('ls[0]',ls[0])
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

# ----4级  effective_vote * -----------
def parse_class4_effective_vote(self,st,show_time,show_id,show_class4,marker,linestyle, markersize= 5, linewidth=0.5,color='lime',alpha =0.99, isplot = 1, isbool = 0, base = 5, xmin=0,xmax =0, ax3 = None, fig_3 = None):
    client_str_id = {}
    client_t_v_i = {}
    if isplot == 0:
        return
    
    if self.d4.get(st) == None:
        print('self.d4.get(',st,')')
        #st1 = '\[.*?(\d*\.\d*)\]?\s' + st + ':\seffective.*?(\-?\d+)\s.*?(\d+)'
        st2 = '\[.*?(\d*\.\d*)]?\s' + st + ':\seffective.*?(\d+)\svoted\sby\s(.*?)\_VOTER\,(\d)'
        effective_vote = re.findall(st2, text)
        #print(effective_vote)
        self.d4[st] = effective_vote
        
    else:
        print('span get vote in parse_4')
        effective_vote = self.d4.get(st)
        
    lt = []
    lv = []
    li = []
    f = lambda t: float(t)
    i = lambda v: int(v)/1000000
    for t,v,voter,id in effective_vote:
        temp = t
        temp = f(temp)
        lt.append(temp)
        temp = v
        temp = i(temp)
        lv.append(temp)
        
        temp = id
        temp = int(temp)
        li.append(temp)
    
        client_str_id[voter] = id
    if isbool:
        lv = list( map(lambda x: x*100000*8 + base, lv) ) 
        #print('isbool',lv)
    
    client_str_id = sorted(client_str_id.items(), key = lambda x:x[1], reverse = True)
    
    if self.str_id.get(st)== None:
        self.str_id[st] = client_str_id   
    
    
    #process span
    if xmin != 0 and xmax != 0 and lt != []:
        print('\n^^^^^^^^^^^^^class4 span start^^^^^^^^^^^^^^^^^^')
        print('class4 print xmin,xmax: ', xmin,xmax)
        #print(lt)
        t_min_index = findmin_index(xmin, lt)
        t_max_index = findmax_index(xmax, lt)
        print('min_index,max_index: ',t_min_index,t_max_index)
        lt = lt[t_min_index : t_max_index]
        lv = lv[t_min_index : t_max_index]
        li = li[t_min_index : t_max_index]
        #print("lt after process: ",lt)
        #print("\nlv after process: ",lv)
  
        #ax3.plot(lt,lv, marker = '*',linestyle=':',linewidth = 0.5, color = 'lime')
        if show_class4:
            ax3.plot(lt,lv, marker = '*',linestyle=':',linewidth = 0.5, color = color)
        else:
            ax3.plot(lt,lv, marker = '*',linestyle=':',linewidth = 0.5, color = color, alpha = 0.1)
            return
        for t,v,i in zip(lt,lv,li):
            #plt.plot(t,v, marker = '*',linestyle=':',linewidth = 0.5, color = 'g',markersize = 7)  
            if show_time:
                ax3.text(t,v,t, horizontalalignment='center', verticalalignment='bottom', fontsize = 6,alpha = 0.8, rotation = 45)
            if show_id:
                ax3.text(t,v,i, horizontalalignment='center', verticalalignment='bottom', fontsize = 7,alpha = 0.95)        
                
        fig_3.show()
        print('==========class4 span end===========================\n')
        return
    #---------------------------------------------    
    
    
    if show_class4:
        #ax.plot(lt,lv, marker = '*',linestyle=':',linewidth = 0.5, color = cl)
        ax.plot(lt,lv,marker, markersize = markersize, color = color, alpha = alpha)
        
        lt1, lv1 = conv_square_wave(lt,lv) #转化成方波
        ax.plot(lt1,lv1,markersize = markersize,linestyle = linestyle, linewidth = linewidth, color = color, alpha = alpha)
        
        if lt != [] :  
            ax.text(lt[0],lv[0], st, horizontalalignment='right', verticalalignment='bottom', fontsize = 7,alpha = 0.95, color = color)
        
    else:
        lt2, lv2 = conv_square_wave(lt,lv)
        ax.plot(lt2,lv2, line_mark,linewidth = linewidth, color = color, alpha = alpha)
        return
    for t,v,i in zip(lt,lv,li):
        #plt.plot(t,v, marker = '*',linestyle=':',linewidth = 0.5, color = 'g',markersize = 7)  
        if show_time:
            ax.text(t,v,t, horizontalalignment='center', verticalalignment='bottom', fontsize = 6,alpha = 0.8, rotation = 45)
        if show_id:
            ax.text(t,v,i, horizontalalignment='center', verticalalignment='bottom', fontsize = 7,alpha = 0.95)
    print('class4 run full end ======')    
    
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
    fig.canvas.draw_idle()  # 绘图动作实时反映在图像上


class Message(object):
    def __init__(self):
        self.str_id = {}
        self.d4 = {}
        
        self.xmin = 0
        self.xmax = 0
        self.ax3 = None
        self.fig_3 = None
    
    def draw_ax(self):
        parse_class4_effective_vote(self,'USB_ICL',0,1,1,marker = '+',linestyle = ':' ,markersize = 10,color = 'deeppink', alpha = 0.7)
    
        parse_class4_effective_vote(self,'CP_ILIM',0,0,1,marker = '.',linestyle = '--' ,markersize = 2, alpha = 0.6)
        
        parse_class4_effective_vote(self,'FCC',0,1,1,marker='*',    linestyle = '-.', markersize = 7,color = 'blue', alpha = 0.5)
        
        parse_class4_effective_vote(self,'FV',0,1,1,marker='*',    linestyle = '-.', markersize = 7,color = 'g', alpha = 0.5)
        
            
        parse_class4_effective_vote(self,'SMB_EN_OVERRIDE',0,1,1,marker='.',    linestyle = '-.', markersize = 3,color = 'r', alpha = 0.5, isbool = 1, base = 5)
        
        parse_class4_effective_vote(self,'CP_DISABLE',0,1,1,marker='.',    linestyle = '-.', markersize = 3,color = 'g', alpha = 0.5, isbool = 1, base = 6)
        
        parse_class4_effective_vote(self,'PL_DISABLE',0,1,1,marker='.',    linestyle = '-.', markersize = 3,color = 'r', alpha = 0.5, isbool = 1, base = 7)
        
        parse_class4_effective_vote(self,'PL_ENABLE_INDIRECT',0,1,1,marker='.',    linestyle = '-.', markersize = 3,color = 'y', alpha = 0.5, isbool = 1, base = 8)
        
        parse_class4_effective_vote(self,'CHG_DISABLE',0,1,1,marker='.',    linestyle = '-.', markersize = 3,color = 'g', alpha = 0.5, isbool = 1, base = 9)
        
    def draw_span(self,xmin,xmax,fig_3,ax3):
            parse_class4_effective_vote(self,'USB_ICL',0,1,1,marker = '+',linestyle = ':' ,markersize = 10,color = 'deeppink', alpha = 0.7, xmin=xmin, xmax=xmax,ax3 = ax3,fig_3 = fig_3)
    
            parse_class4_effective_vote(self,'CP_ILIM',0,0,1,marker = '.',linestyle = '--' ,markersize = 2, alpha = 0.6, xmin=xmin, xmax=xmax,ax3 = ax3,fig_3 = fig_3)
            
            parse_class4_effective_vote(self,'FCC',0,1,1,marker='*',    linestyle = '-.', markersize = 7,color = 'blue', alpha = 0.5, xmin=xmin, xmax=xmax,ax3 = ax3,fig_3 = fig_3)
            
            parse_class4_effective_vote(self,'FV',0,1,1,marker='*',    linestyle = '-.', markersize = 7,color = 'g', alpha = 0.5, xmin=xmin, xmax=xmax,ax3 = ax3,fig_3 = fig_3)
            
                
            parse_class4_effective_vote(self,'SMB_EN_OVERRIDE',0,1,1,marker='.',    linestyle = '-.', markersize = 3,color = 'r', alpha = 0.5, isbool = 1, base = 5, xmin=xmin, xmax=xmax,ax3 = ax3,fig_3 = fig_3)
            
            parse_class4_effective_vote(self,'CP_DISABLE',0,1,1,marker='.',    linestyle = '-.', markersize = 3,color = 'g', alpha = 0.5, isbool = 1, base = 6, xmin=xmin, xmax=xmax,ax3 = ax3,fig_3 = fig_3)
            
            parse_class4_effective_vote(self,'PL_DISABLE',0,1,1,marker='.',    linestyle = '-.', markersize = 3,color = 'r', alpha = 0.5, isbool = 1, base = 7, xmin=xmin, xmax=xmax,ax3 = ax3,fig_3 = fig_3)
            
            parse_class4_effective_vote(self,'PL_ENABLE_INDIRECT',0,1,1,marker='.',    linestyle = '-.', markersize = 3,color = 'y', alpha = 0.5, isbool = 1, base = 8, xmin=xmin, xmax=xmax,ax3 = ax3,fig_3 = fig_3)
            
            parse_class4_effective_vote(self,'CHG_DISABLE',0,1,1,marker='.',    linestyle = '-.', markersize = 3,color = 'g', alpha = 0.5, isbool = 1, base = 9, xmin=xmin, xmax=xmax,ax3 = ax3,fig_3 = fig_3)
            
def get_dir():
    #不清楚是启动程序调用 还是 手动启动本文件， 
    #如果是启动程序调用一定有 dmesg_py 文件
    #手动启动则没有dmesg_py文件
    #if sys.argv[1].endswith('')
    dir = sys.argv[1] + '\dmesg_py.txt'  
    if os.access(dir, os.F_OK) == False:  
        dir = sys.argv[1] + '\dmesg.txt'    
    return dir

# def dmseg_process_temp(dir,set_tmin,set_tmax):
    # f_temp = open('temp.txt','w')
    # with open(dir) as f:
        # for line in f.readlines():
            # res = re.findall('\[\s*?(\d*.\d*)\]', line)
            # t = float(res[0])
            # if t > set_tmin:
                # f_temp.write(line)
                # if t > set_tmax:
                    # break
    # f_temp.close()
    # f_temp = open('temp.txt')
    # text = f_temp.read()
    # #print(text)
    # f_temp.close()
    # return text       #局部变量函数反回会消失

#入口参数：
# 1. 文件名
# 2. 地址（可选）； 如缺省，使用当前地址
# 3. t_start   单独使用此文件的情况下，使用时间参数
# 4. t_end   
if __name__ == '__main__':
    import numpy as np
    import pandas as pd
    import matplotlib.pyplot as plt
    #%matplotlib inline
    import re
    from matplotlib.ticker import MultipleLocator, FormatStrFormatter
    #import allvotable_1
    from matplotlib.widgets import RadioButtons
    from matplotlib.widgets import SpanSelector
    import sys
    import os
    from soc import *
    #from access_5 import *
    
    mes = Message()
    m = Msg_soc()
    print('sys.argv',sys.argv)
    if len(sys.argv) == 4: #单独使用此文件的情况下，使用时间参数
        set_tmin = float(sys.argv[2])
        set_tmax = float(sys.argv[3])
        print(set_tmin,set_tmax)
        dir = sys.argv[1] + '\dmesg.txt'
        text = dmseg_process_temp(dir,set_tmin,set_tmax)
        # f_temp = open('temp.txt','w')
        # dir = get_dir() 
        # with open(dir) as f:
            # for line in f.readlines():
                # res = re.findall('\[\s*?(\d*.\d*)\]', line)
                # t = float(res[0])
                # if t > t_start:
                    # f_temp.write(line)
                    # if t > t_end:
                        # break
        # f_temp.close()
        # f_temp = open('temp.txt')
        # text = f_temp.read()
        # f_temp.close()
    
    
    
    # elif len(sys.argv) == 2:
        # dir = get_dir()
        # f = open(dir)
        # text = f.read()
        # f.close()
    # elif len(sys.argv) == 1:
        # f = open('dmesg.txt')
        # text = f.read()
        # f.close()
    
    elif len(sys.argv) == 3:  #调用，不用处理时间
        print('access call')
        if sys.argv[2] == 'pcall':
            dir = get_dir()
            f = open(dir)
            text = f.read()
            f.close()    
    
    elif len(sys.argv) <= 2:
        #找soc100时间
        if len(sys.argv) == 2:            #2个参数
            m.dir = sys.argv[1]
            dir = m.dir + '\dmesg.txt'
            print('2 argv, dir = ', dir)
        else:                             #1个参数
            m.dir = '.'
            dir = 'dmesg.txt'
            print('1 arg, dir = ', dir)
        get_soc_message(m)
    
        if m.is_soc_100:
            set_max = m.df['Uptime'][m.soc_100_index]
            print('soc >100, set_t = ',set_max,dir)
            text = dmseg_temp_process(dir,0,set_max) #soc超过100
            #print(text)

        else:
            f = open(dir)    #soc没有超过100
            text = f.read()
            f.close()
            print('soc < 100')
    
    
    
    fig = plt.figure(figsize=(16,8))
    fig.canvas.mpl_connect('scroll_event', call_back)
    
    ax  = plt.subplot(111)
    ax.spines['top'].set_visible(False)
    ax.spines['right'].set_visible(False)
    ax.set_title('Votable view')
    
    
    mes.draw_ax()
    
    
    


    ymajorLocator = MultipleLocator(1)
    ax.yaxis.set_major_locator(ymajorLocator)
    
    
    #画str_id
    x = -0.15
    y = 0.05
    
    x2 = 1
    y2 = 0.3
    
    i = 0
    for k,v in mes.str_id.items():
        i += 1      
        if i == 5:
            x = x2
            y = y2

        #parse_str_id(self,self.str, -0.11,-0.5,xmin,xmax)
        
        #print(k,v)
        for item in v:
            i_s =  item[1] + ' ' + item[0]
            ax.text(x,y,i_s,horizontalalignment='left', verticalalignment='bottom', fontsize = 7, alpha = 0.8,color = 'g',transform=ax.transAxes)
            y += 0.03
              
        ax.text(x,y,k,horizontalalignment='left', verticalalignment='bottom', fontsize = 9, transform=ax.transAxes)
        y += 0.07
    
    
    
     #==============span selector================================
    def onselect(xmin, xmax):
        if xmax - xmin > 0.1 :
            fig_3 = plt.figure() #每次选择范围，新建一个图，并把key_span重新的fig3重新设置，这样update为新fig图
            ax3 = plt.subplot(111)
            #fig_3,ax3 = plt.subplots()
            # xmin = xmin
            # xmax = xmax
            print('span onselect',xmin, xmax)
            
            mes.draw_span(xmin,xmax,fig_3,ax3) #span 直接用parse_class4_effective_vote 函数，不用再 处理文件
            #parse_class4_effective_vote('FCC',0,1,1,marker = '+',linestyle = ':' ,markersize = 10,color = 'deeppink', alpha = 0.7, xmin=xmin, xmax=xmax,ax3 = ax3,fig_3 = fig_3)
            
            
            #----parse start----

                
    span = SpanSelector(ax, onselect, 'horizontal', useblit=True,
                                rectprops=dict(alpha=0.5, facecolor='red')) #SpanSelector给 onselect返回 xmin，xmax两个参数   
            
    plt.show()
