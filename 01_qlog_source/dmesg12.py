#画一个图然后plt.show，接着读剩余的数据
#由于执行plt.show后，后面的代码不再继续执行，  数据预先处理需要在画图之前处理，而不能再画图后继续处理
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
    

def parse_str_id(self,st,x,y,xmin=0,xmax=0):   
    # -------解析---client_str-----client_id----打印一个vote的所有str-id--
    if self.str_id.get(st) == None:
        print('self.str_id.get(',st,')','None')
        client_str_id = {}
        #'\[.*?(\d*\.\d*)\]?\s' + st + ':\s(\w+)_VOTER,(\d+)\s\w'
        st2 = '\[.*?(\d*\.\d*)\]?\s' + st + ':\s(\w+)_VOTER,(\d+)'
        res = re.findall(st2, text)
        for t,s,id in res:
            client_str_id[s] = id
            
        print(client_str_id)
        client_str_id = sorted(client_str_id.items(),key = lambda x:x[1],reverse = False)
        self.str_id[st] = client_str_id
        
        print(self.str_id)
        if self.firstrun:
            print('init except USB_ICL, return will not draw picture')
            return
    else:
        print('self.str_id.get(', st, ')', 'true')
        client_str_id = self.str_id.get(st)
    
    
    if xmin != 0 and xmax != 0:
        self.ax3.text(x-0.007,y,st,horizontalalignment='left', verticalalignment='bottom', fontsize = 9, transform=ax.transAxes, color ='g')
        for item in client_str_id:
            y -= 0.05
            res = item[1] +' ' + item[0]
            #plt.text(x,y,res, horizontalalignment='left', verticalalignment='bottom', fontsize = 6,alpha = 0.8) 
            self.ax3.text(x-0.007,y,res,horizontalalignment='left', verticalalignment='bottom', fontsize = 5,alpha = 0.9, transform=ax.transAxes, color = 'g')
        return
    
    
    ax2.text(x,y,st,horizontalalignment='left', verticalalignment='bottom', fontsize = 9, transform=ax.transAxes)
    #print(client_str_id)
    for item in client_str_id:
        y -= 0.05
        res = item[1] +' ' + item[0]
        #plt.text(x,y,res, horizontalalignment='left', verticalalignment='bottom', fontsize = 6,alpha = 0.8) 
        ax2.text(x,y,res,horizontalalignment='left', verticalalignment='bottom', fontsize = 7,alpha = 0.8, transform=ax.transAxes)
    # for st,id in client_str_id.items():
        # y -= 0.5
        # res = id +' ' + st
        # plt.text(x,y,res, horizontalalignment='left', verticalalignment='bottom', fontsize = 6,alpha = 0.8)

# ----4级  effective_vote * -----------
def parse_class4_effective_vote(self, st,ax3,show_time,show_id,show_class4, xmin=0,xmax=0):
    if self.d4.get(st) == None:
        print('self.d4.get(',st,')','None')
        st1 = '\[.*?(\d*\.\d*)\]?\s' + st + ':\seffective.*?(\-?\d+)\s.*?(\d+)'
        effective_vote = re.findall(st1, text)
        self.d4[st] = effective_vote
        return
    else:
        print('self.d4.get(', st, ')', 'true')
        effective_vote = self.d4.get(st)
        
    # st1 = '\[.*?(\d*\.\d*)\]?\s' + st + ':\seffective.*?(\d+)\s.*?(\d)'
    # effective_vote = re.findall(st1, text)
    #print(effective_vote)
    lt = []
    lv = []
    li = []
    f = lambda t: float(t)
    i = lambda v: int(v)/1000000
    for t,v,id in effective_vote:
        temp = t
        temp = f(temp)
        lt.append(temp)
        temp = v
        temp = i(temp)
        lv.append(temp)
        
        temp = id
        temp = int(temp)
        li.append(temp)
    # lt = lt[0:len(lt)-1]
    # lv = lv[0:len(lv)-1]
    #print('\nlt before process', lt)
    if xmin != 0 and xmax != 0 and lt != []:
        print('\n^^^^^^^^^^^^^class4 span start^^^^^^^^^^^^^^^^^^')
        #print('class4 print xmin,xmax: ', xmin,xmax)
        t_min_index = findmin_index(xmin, lt)
        t_max_index = findmax_index(xmax, lt)
        #print('min_index,max_index: ',t_min_index,t_max_index)
        lt = lt[t_min_index : t_max_index]
        lv = lv[t_min_index : t_max_index]
        li = li[t_min_index : t_max_index]
        #print("lt after process: ",lt)
        #print("\nlv after process: ",lv)
        
        #ax3.plot(lt,lv, marker = '*',linestyle=':',linewidth = 0.5, color = 'lime')
        if show_class4:
            ax3.plot(lt,lv, marker = '*',linestyle=':',linewidth = 0.5, color = 'lime')
        else:
            ax3.plot(lt,lv, marker = '*',linestyle=':',linewidth = 0.5, color = 'lime', alpha = 0.1)
            return
        for t,v,i in zip(lt,lv,li):
            #plt.plot(t,v, marker = '*',linestyle=':',linewidth = 0.5, color = 'g',markersize = 7)  
            if show_time:
                ax3.text(t,v,t, horizontalalignment='center', verticalalignment='bottom', fontsize = 6,alpha = 0.8, rotation = 45)
            if show_id:
                ax3.text(t,v,i, horizontalalignment='center', verticalalignment='bottom', fontsize = 7,alpha = 0.95)        
        print('==========class4 span end===========================\n')
        return
    #---------------------------------------------
    # print(lt)
    # print(lv)
    if show_class4:
        ax2.plot(lt,lv, marker = '*',linestyle=':',linewidth = 0.5, color = 'lime')
    else:
        ax2.plot(lt,lv, marker = '*',linestyle=':',linewidth = 0.5, color = 'lime', alpha = 0.1)
        return
    for t,v,i in zip(lt,lv,li):
        #plt.plot(t,v, marker = '*',linestyle=':',linewidth = 0.5, color = 'g',markersize = 7)  
        if show_time:
            ax2.text(t,v,t, horizontalalignment='center', verticalalignment='bottom', fontsize = 6,alpha = 0.8, rotation = 45)
        if show_id:
            ax2.text(t,v,i, horizontalalignment='center', verticalalignment='bottom', fontsize = 7,alpha = 0.95)
    print('class4 run full end ======')        
            
# -----3级---打印3级 --*--和--+----voting of--
def parse_class3_voting_of(self, st,ax3,show_id,show_class3, xmin=0, xmax=0):
    if show_class3 == False:
        return
        
    marker_style = dict(color='cornflowerblue', linestyle=':', marker='o',
                    markersize=15, markerfacecoloralt='gray')
                       
    if self.d3.get(st) == None:
        print('self.d3.get(',st,')','None')
        st1 = '\[.*?(\d*\.\d*)\]?\s' + st + ':\s(\w*)_VOTER,(\d+)\svoting\s(\w+)\sof\sval=(\-?\d+)'
        res = re.findall(st1, text)
        self.d3[st] = res
        return
    else:
        print('self.d3.get(', st, ')', 'true')
        res = self.d3.get(st)

        
        
    # st = st
    # st1 = '\[.*?(\d*\.\d*)\]?\s' + st + ':\s(\w*)_VOTER,(\d)\svoting\s(\w+)\sof\sval=(\d+)'
    # res = re.findall(st1, text)
    #print(res)
    lt = []
    lvoter = []
    li = []
    lbool = []
    lval = []
    f = lambda t: float(t)
    i = lambda v: int(v)/1000000
    for t,voter,id,bool,val in res:
        lt.append(f(t))
        lvoter.append(voter)
        li.append(int(id))
        lbool.append(bool)
        lval.append(i(val))
    # for i in range(len(lval)):
        # if lbool[i] == "off":
            # #lval[i] = 4.7
            # lval[i] = zero_val
    #print(lval)    
    # lt = lt[0:len(lt)-1]
    # lv = lv[0:len(lv)-1]
    #plt.plot(lt,lval, marker = '+',linestyle='-',linewidth = 1) 
    # print(lt)
    # print(lval)
    
    #print('\nclass3 before modefie lt',lt)
    if xmin != 0 and xmax != 0 and lt != []:
        t_min_index = findmin_index(xmin, lt)
        t_max_index = findmax_index(xmax, lt)
        lt = lt[t_min_index : t_max_index]
        li = li[t_min_index : t_max_index]
        lbool = lbool[t_min_index : t_max_index]
        lval = lval[t_min_index : t_max_index]
        #print('\nclass3 after modefiy lt: ', lt)
        #ax3.plot(lt,lval, marker = '+', markersize = 13, color = 'orange', alpha = 0.99)
        
        for i in range(len(lt)):
            if lbool[i] == 'on':
                ax3.plot(lt[i],lval[i], marker = '+', markersize = 13, color = 'orange', alpha = 0.99)
                if show_id:
                    ax3.text(lt[i],lval[i], li[i], horizontalalignment='right', verticalalignment='bottom', fontsize = 7,alpha = 0.9, rotation = 45)
            if lbool[i] == 'off':
                #plt.plot(lt[i],lval[i], fillstyle = 'none', **marker_style)
                ax3.plot(lt[i],lval[i], marker = '+', markersize = 10, color = 'darkgoldenrod', alpha = 0.95)
                if show_id:
                    ax3.text(lt[i],lval[i], li[i], horizontalalignment='right', verticalalignment='top', fontsize = 7,alpha = 0.9, rotation = 45)
        print('=====class3 end=====')
        return
    #----------------------------------------
    
    for i in range(len(lt)):
        if lbool[i] == 'on':
            ax2.plot(lt[i],lval[i], marker = '+', markersize = 13, color = 'orange', alpha = 0.99)
            if show_id:
                ax2.text(lt[i],lval[i], li[i], horizontalalignment='right', verticalalignment='bottom', fontsize = 7,alpha = 0.9, rotation = 45)
        if lbool[i] == 'off':
            #plt.plot(lt[i],lval[i], fillstyle = 'none', **marker_style)
            ax2.plot(lt[i],lval[i], marker = '+', markersize = 10, color = 'darkgoldenrod', alpha = 0.95)
            if show_id:
                ax2.text(lt[i],lval[i], li[i], horizontalalignment='right', verticalalignment='top', fontsize = 7,alpha = 0.9, rotation = 45)
    

# ---2级--打印2,1级voter--*-+-x->----Ignoring-----same vote------------
def parse_class2_same_but_fist_and_twiceIgnoring(self, st,ax3, show_id, show_class2,xmin=0,xmax=0):
    if show_class2 == False:
        return
        
    marker_style = dict(color='cornflowerblue', linestyle=':', marker='X',
                        markersize=5, markerfacecoloralt='r')
    
    if self.d2.get(st) == None:
        #print('self.d2.get(',st,')','None')
        st1 = '\[.*?(\d*\.\d*)\]?\s' + st + ':\s(\w*)_VOTER,(\d+)\sIgnoring\ssimilar\svote\s(\w+)\sof\sval=(\-?\d+)'
        res = re.findall(st1, text)
        # for st in res:
            # print(st)
        self.d2[st] = res
        return
    else:
        print('self.d2.get(', st, ')', 'true')
        res = self.d2.get(st)
        
    # print('\n\n\n')
    # print('self.d2---->\n',self.d2)        
    # st1 = '\[.*?(\d*\.\d*)\]?\s' + st + ':\s(\w*)_VOTER,(\d)\sIgnoring\ssimilar\svote\s(\w+)\sof\sval=(\d+)'
    # res = re.findall(st1, text)
    #print(res)
    lt = []
    lvoter = []
    li = []
    lbool = []
    lval = []
    f = lambda t: float(t)
    i = lambda v: int(v)/1000000
    for t,voter,id,bool,val in res:
        lt.append(f(t))
        lvoter.append(voter)
        li.append(int(id))
        lbool.append(bool)
        lval.append(i(val))
    
    
    if xmin != 0 and xmax != 0 and lt != []:
        print('\n^^^^^^^class2 span start ^^^^')
        
        t_min_index = findmin_index(xmin, lt)
        t_max_index = findmax_index(xmax, lt)
        lt = lt[t_min_index : t_max_index]
        li = li[t_min_index : t_max_index]
        lbool = lbool[t_min_index : t_max_index]
        lval = lval[t_min_index : t_max_index]
        
        t_on = []
        t_off = []
        a = 0
        for i in range(len(lt)):
            if lbool[i] == 'on':
                t_on.append(lt[i])
                y_txt = lval[i]
                if len(t_on) > 1 :
                    for j in range(len(t_on)):
                        if j == len(t_on)-1 or j >= 9:
                            break
                        elif t_on[-j-1] - t_on[-j-2] < 2 :
                            y_txt += 0.1
                        else:
                            break
                ax3.plot(lt[i],lval[i], marker = 'x', markersize = 4, color = 'b', alpha = 0.5)
                if show_id:
                    ax3.text(lt[i],y_txt, li[i], horizontalalignment='center', verticalalignment='bottom', fontsize = 6.5,alpha = 0.85)
            else:
                t_off.append(lt[i])
                y_txt = lval[i]
                if len(t_off) > 1 :
                    for j in range(len(t_off)):
                        if j == len(t_off)-1:
                            break
                        elif t_off[-j-1] - t_off[-j-2] < 2 :
                            y_txt -= 0.1
                        else:
                            break
                ax3.plot(lt[i],lval[i], marker = 'x', markersize = 3.9, color = 'lightsteelblue', alpha = 0.8)
                if show_id:
                    ax3.text(lt[i],y_txt, li[i], horizontalalignment='center', verticalalignment='top', fontsize = 6,alpha = 0.8)        
        print('===class2 span end====')
        return
    #--------------------------------------------------
    # for i in range(len(lval)):
        # if lbool[i] == "off":
            # #lval[i] = 4.7
            # lval[i] = zero_val
    #print(lval)    
    # lt = lt[0:len(lt)-1]
    # lv = lv[0:len(lv)-1]
    #plt.plot(lt,lval, marker = '+',linestyle='-',linewidth = 1) 
    t_on = []
    t_off = []
    a = 0
    for i in range(len(lt)):
        #plt.plot(lt[i],lval[i], marker = 'x', markersize = 15, color = 'b', alpha = 0.5)
        if lbool[i] == 'on':
            # y_txt = lval[i]
            # if i >0 and lt[i] - t_on < 1 :
                # y_txt += 0.15
            # plt.plot(lt[i],lval[i], marker = 'x', markersize = 4, color = 'r', alpha = 0.5)
            # plt.text(lt[i],y_txt, li[i], horizontalalignment='center', verticalalignment='bottom', fontsize = 6,alpha = 0.8)
            # t_on = lt[i]
            #plt.plot(lt[i],lval[i], fillstyle = 'none', **marker_style)
            
            t_on.append(lt[i])
            # print(lt[i])
            # print(t_off)
            y_txt = lval[i]
            #print(y_txt)
            if len(t_on) > 1 :
                for j in range(len(t_on)):
                    #print('in for  j=%d' % j)
                    if j == len(t_on)-1 or j >= 9:
                        break
                    elif t_on[-j-1] - t_on[-j-2] < 2 :
                        y_txt += 0.1
                        #print(y_txt)
                    else:
                        break
            #plt.plot(lt[i],lval[i], fillstyle = 'top', **marker_style)
            ax2.plot(lt[i],lval[i], marker = 'x', markersize = 4, color = 'b', alpha = 0.7)
            if show_id:
                ax2.text(lt[i],y_txt, li[i], horizontalalignment='center', verticalalignment='bottom', fontsize = 6.5,alpha = 0.85)
        else:
            #print('\n  off of val: ',lt[i],lvoter[i],li[i],lval[i],'*'*20)
            t_off.append(lt[i])
            # print(lt[i])
            # print(t_off)
            y_txt = lval[i]
            #print(y_txt)
            if len(t_off) > 1 :
                for j in range(len(t_off)):
                    #print('in for  j=%d' % j)
                    if j == len(t_off)-1:
                        break
                    elif t_off[-j-1] - t_off[-j-2] < 2 :
                        y_txt -= 0.1
                        #print(y_txt)
                    else:
                        break
            #plt.plot(lt[i],lval[i], fillstyle = 'top', **marker_style)
            ax2.plot(lt[i],lval[i], marker = 'x', markersize = 3.8, color = 'lightsteelblue', alpha = 0.7)
            if show_id:
                ax2.text(lt[i],y_txt, li[i], horizontalalignment='center', verticalalignment='top', fontsize = 6,alpha = 0.8)        
#=====func===end===================================   


#==========key健设置class显示属性==============================
class Keyprocess(object):
    def __init__(self):
        self.isspan = 0
        self.str = 'CP_ILIM'
        self.switch = {'4':1, '4_i':1, '4_t':1, '3':1, '3_i':1,   '2':1, '2_i':1, 'firstrun':1}
        self.fig3 = None
        self.ax3    = None
        
        self.firstrun = 1
        self.str_id = {}
        self.d4 = {}
        self.d3 = {}
        self.d2 = {}
    def init_new_str(self,str):
        #self.str = str
        #每个votable都有一个switch，切换votable，switch也得更新 
        self.switch = {'4':1, '4_i':1, '4_t':1, '3':1, '3_i':1,   '2':1, '2_i':1, 'firstrun':1}
        
    def onpress(self, event):
        if event.key == 'Z':
            self.switch['4'] = not self.switch['4']
        elif event.key == 'a':
            self.switch['4_t'] = not self.switch['4_t']
        elif event.key == 'z':
            self.switch['4_i'] = not self.switch['4_i']
            
        elif event.key == 'X':
            self.switch['3'] = not self.switch['3']
        elif event.key == 'x':
            self.switch['3_i'] = not self.switch['3_i']
        
        elif event.key == 'C':
            self.switch['2'] = not self.switch['2']
        elif event.key == 'c':
            self.switch['2_i'] = not self.switch['2_i']
        
        elif event.key == 'n':
            self.switch = {'4':1, '4_i':1, '4_t':1, '3':1, '3_i':1,   '2':1, '2_i':1}
            self.str = 'USB_ICL'
        elif event.key == 'p':
            self.str = 'CP_ILIM'
         
        else:

            return
            
        self.update()
 


 
    def update(self, xmin=0, xmax=0): #self必须加
        print('in update xmin = ', xmin, 'xmax=',xmax)
        if self.isspan == 0:
            print('^^^begin ax2^update^^^^^')
            #if not self.firstrun:
            ax2.cla()
            #self.votable(self)    
            self.votable() #self不必加  self.的调用函数，不需要加self参数，默认添加
            fig.canvas.draw()
            print('=====end ax2 update ======\n')
            
        else:
            print('^^^begin ax3^update^^^^^')
            self.ax3.cla()
            self.votable(xmin,xmax)     #self不能加
            print('in update else xmin = ', xmin, 'xmax=',xmax)
            self.fig3.canvas.draw()
            self.fig3.show()
            print('=====end ax3 update ======\n')

    #def votable(str,ax3,xmin=0,xmax=0):
    #self必须加
    def votable(self,xmin=0,xmax=0):
        print('self.switch[firstrun] = ',self.switch['firstrun'])
        #if self.isspan == 0 and self.switch['firstrun'] == 1:
        #if self.isspan == 0 :
        if 1 :
            #parse_str_id(self.str, -0.11,-0.5)
            parse_str_id(self,self.str, -0.11,-0.5,xmin,xmax)
            self.switch['firstrun'] = 0
            #self.firstrun = 0
            
        parse_class4_effective_vote(self, self.str, self.ax3,self.switch['4_t'], self.switch['4_i'], self.switch['4'],xmin,xmax) #t,i,整体开关
        parse_class3_voting_of(self, self.str,self.ax3,self.switch['3_i'], self.switch['3'],xmin,xmax)
        parse_class2_same_but_fist_and_twiceIgnoring(self, self.str,self.ax3,self.switch['2_i'], self.switch['2'],xmin,xmax)       
       
        print('\nkeyprocess self.str = ',self.str)
        print('keyprocess self.switch: ',self.switch)
    

#^^^^^^^^^end^^^^^^^^^^^^


if __name__ == '__main__':
    import numpy as np
    import pandas as pd
    import matplotlib.pyplot as plt
    #%matplotlib inline
    import re
    from matplotlib.ticker import MultipleLocator, FormatStrFormatter
    import allvotable_1
    from matplotlib.widgets import RadioButtons
    from matplotlib.widgets import SpanSelector
    import sys
    
    df = allvotable_1.parse_votes()
    fig,ax = allvotable_1.plot()    #ax是第一个子图
    allvotable_1.annotation(ax,18)
    allvotable_1.parse_effect_voters()
    
    #^^^^^^^^^^^^^^^^^^ax2^^^^^^^sub dmesg的图^^^^^^^^^^^^^^^^^^^^^
    print('\n\n')
    print('*'*50)
    
    print('sys.argv',sys.argv)
    if len(sys.argv) == 3: 
        t_start = float(sys.argv[1])
        t_end = float(sys.argv[2])
        print(t_start,t_end)
        f_temp = open('temp.txt','w')
        with open('dmesg.txt') as f:
            for line in f.readlines():
                res = re.findall('\[\s*?(\d*.\d*)\]', line)
                t = float(res[0])
                if t > t_start:
                    f_temp.write(line)
                    if t > t_end:
                        break
        f_temp.close()
        f_temp = open('temp.txt')
        text = f_temp.read()
        f_temp.close()
    
    
    
    else:
        f = open('dmesg.txt')
        text = f.read()
        f.close()
    ax2 = plt.subplot(212)                        #ax2是第二个子图

    #^^^^^^^^^^^^^^^^^^key^^^^^^^^^^^^^^^^^^^^^^^^^^
    key = Keyprocess()
    fig.canvas.mpl_connect('key_press_event', key.onpress)  #ax1,ax2共用fig
    
    key_span = Keyprocess()  #key_span 指的是新图
    
    
    #--------------RadioButton-------调用Key process----------------
    axcolor = 'lightgoldenrodyellow'
    rax = plt.axes([0.001, 0.61, 0.05, 0.25], facecolor=axcolor) #位置
    radio2 = RadioButtons(rax, ('USB_ICL', 'CP_ILIM', 'FCC', 'FV', 
        'SMB_EN_OVERRIDE','CP_DISABLE', 'PL_DISABLE', 'PL_ENABLE_INDIRECT', 'CHG_DISABLE'))       #按钮

    def select_votable_func(label):           #回调函数, 给收音机按钮使用
        print('select label', label)
        key.str = label
        key_span.str = label   
        key.init_new_str(label)
        key.update()
        # print('key.str',key.str)
        
        
    radio2.on_clicked(select_votable_func)    #绑定收音机 回调函数
    
    #^^^^^^^^^^^^^初始化数据^^^^^^^^^^^^^^^^^^^^^^
    select_votable_func('USB_ICL') 
    select_votable_func('CP_ILIM')
    select_votable_func('FCC')
    select_votable_func('FV')
    
    select_votable_func('SMB_EN_OVERRIDE')
    select_votable_func('CP_DISABLE')
    select_votable_func('PL_DISABLE')
    select_votable_func('PL_ENABLE_INDIRECT')
    select_votable_func('CHG_DISABLE')
    key.firstrun = 0
    key_span.firstrun = 0
    key_span.str_id = key.str_id
    key_span.d4 = key.d4
    key_span.d3 = key.d3
    key_span.d2 = key.d2
    
    select_votable_func('USB_ICL')   #第一次画ax2-----------------------
    #==============span selector================================
    def onselect(xmin, xmax):
        if xmax - xmin > 0.1 :
            fig_3 = plt.figure() #每次选择范围，新建一个图，并把key_span重新的fig3重新设置，这样update为新fig图
            ax3 = plt.subplot(111)
            #fig_3,ax3 = plt.subplots()
            # xmin = xmin
            # xmax = xmax
            print(xmin, xmax)
            key_span.switch = key.switch
            key_span.isspan = 1
            
            #print('fig_3=',fig_3, type(fig_3),'\n  ax3=',ax3, type(ax3))
            
            key_span.fig3 = fig_3
            key_span.ax3 = ax3
            key_span.update(xmin,xmax)
        
        
    span = SpanSelector(ax2, onselect, 'horizontal', useblit=True,
                    rectprops=dict(alpha=0.5, facecolor='red')) #SpanSelector给 onselect返回 xmin，xmax两个参数
    plt.show()
