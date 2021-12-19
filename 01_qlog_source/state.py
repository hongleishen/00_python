def plot_qc2_status():
    h = 15; j = 0.5
    sdll = {
        'NOT_OK_TO_HVDCP2':h,  
        'CHARGER_COOL':h+j*1,  'CHARGER_WARM':h+j*2,  'CHARGER_HOT':h+j*3,
        'LIMITED':h+j*4,  'NOT_LIMITED':h+j*5,
        'WAIT':h+j*6
    }
    
    state = re.findall('\[.*?(\d+\.\d+)\]\sHVDCP2:\sS_OPTI_(.*)\s\-\>\sS_OPTI_(.*)', text)
    t_st = []
    s = []
    for t,ps,ts in state:
        t = float(t)          
        t_st.append(t)
        s.append(ts)
    
    #print(t_st)
    #print(s)
    t_st,s = conv_square_wave(t_st,s)       #方波
    #print(sdll['CHARGER_COOL'])
    #print(sdll['CHARGER_HOT'])
    
    s_val = list( map(lambda x:sdll[x],s) )
    ax.plot(t_st, s_val, color = 'b', linewidth = 1, marker = '.', markersize = '3', alpha = 0.6)

    #----参考线-------
    t_min = t_st[0] - 50
    h_start = t_min
    len = t_st[-1] - t_st[0]
    
    #h_start = 1
    ax.hlines(h,         h_start,h_start + 250, linewidth = 1, linestyle = ':', alpha = 0.9)
    ax.text(h_start, h, 'NOT_OK_TO_HVDCP2', horizontalalignment='right', verticalalignment='center', fontsize = 7, alpha = 0.5)

    ax.hlines(h + j*1, h_start,h_start + len, linewidth = 1, linestyle = '--', color = 'lime' ,alpha = 0.8)
    ax.text(h_start, h + j*1, 'CHARGER_COOL', horizontalalignment='right', verticalalignment='center', fontsize = 7, color = 'lime',alpha = 0.9)
    
    ax.hlines(h + j*2, h_start,h_start + len, linewidth = 1, linestyle = ':',color = 'orange', alpha = 0.5)
    ax.text(h_start, h + j*2, 'CHARGER_WARM', horizontalalignment='right', verticalalignment='center', fontsize = 7, color = 'orange',alpha = 0.5)
    
    ax.hlines(h + j*3, h_start,h_start + len, linewidth = 1.5, linestyle = '--',color ='r', alpha = 0.6)
    ax.text(h_start, h + j*3, 'CHARGER_HOT', horizontalalignment='right', verticalalignment='center', fontsize = 7, alpha = 0.9, color = 'r')
    
    ax.hlines(h + j*4, h_start,h_start + len, linewidth = 1.2, linestyle = ':', color = 'brown',alpha = 0.6)
    ax.text(h_start, h + j*4, 'LIMITED', horizontalalignment='right', verticalalignment='center', fontsize = 7, alpha = 1,color = 'brown')
    
    
    ax.hlines(h + j*5, h_start,len, linewidth = 1, linestyle = ':', alpha = 0.3)
    ax.text(h_start, h + j*5, 'NOT_LIMITED', horizontalalignment='right', verticalalignment='center', fontsize = 7, alpha = 0.9)

    ax.hlines(h + j*6, h_start,len, linewidth = 1, linestyle = ':', alpha = 0.3)
    ax.text(h_start, h + j*6, 'WAIT', horizontalalignment='right', verticalalignment='center', fontsize = 7, alpha = 0.5)   

def qc2_plot():
    t_min = df['Uptime'].min()
    t_max = df['Uptime'].max()
    
    t_min_lim = t_min - (t_max - t_min)/9    
    ax.set_xlim(t_min_lim, t_max+20)

    #--a.------V_bus---adj-------------------
    res = re.findall('\[.*?(\d+\.\d+)\]\sHVDCP2:\sVoltage.*?voltage=(\d+)', text)
    lt = []
    lv = []
    for t,v in res:
        t = float(t)
        lt.append(t)
        
        v = float(v)/1000000
        lv.append(v)
    lt,lv = conv_square_wave(lt,lv)
    ax.plot(lt,lv, 'b',alpha = 0.9, linewidth = 0.3)
    min_index = int(len(lt)/2)
    #ax.text(lt[min_index],lv[min_index],'Vbus_adjust',horizontalalignment='left', verticalalignment='bottom', fontsize = 8,alpha = 1, color = 'b',rotation = 45)
    ax.text(lt[0],lv[0],'Vbus_adjust',horizontalalignment='left', verticalalignment='bottom', fontsize = 8,alpha = 1)
    #--1.----Fast---Taper--line------------------------------
    # h2 = 15
    # sd2 = {'Fast':h2 + 0.3, 'Taper':h2, 'N/A':h2-0.3 }
    
    # time = list(df['Uptime'])
    # bct = list(df['batterychrgType'])
    
    # bct = list( map(lambda x:sd2[x],bct) )
    # t,bct = conv_square_wave(time,bct)
    # plt.plot(t,bct)
  
    
    
    # res = re.findall('\[.*?(\d+\.\d+)\]\s.*taper\sentry\sscheduling\swork',text)
    # #ax.arrow(
    # i = 0
    # for t in res:
        # t = float(t)
        # print(t)
        # ax.arrow(t,h2+0.3,0,-0.3, length_includes_head = True, shape ='right',head_width=16, head_length=0.1, fc='r', ec='r', width = 0.2)
        # if i == 0:
            # ax.text(t,h2+0.3,'taper entry work',horizontalalignment='left', verticalalignment='bottom', fontsize = 8,alpha = 0.8, rotation = 45)
            # i += 1

    #--b.-4--die_health
    h4 = 10
    j = 0.4
    d4 = {
    'Cool':h4 ,'Warm':h4 + j*1,
    'Hot': h4 + j*2,   'Overheat': h4+j*3
    }
    
    res = re.findall('\[.*?(\d+\.\d+)\]\s.*?state_props\.die_health\s=\s(.*)',text)
    lt = []
    lt = []
    lh = []
    for t,h in res:
        lt.append(float(t))
        lh.append(h)
        
    lh = list( map(lambda x:d4[x], lh) )
    lt,lh = conv_square_wave(lt,lh)
    ax.plot(lt,lh)
    
    t_min = lt[0]
    t_max = lt[len(lt)-1] + 50
    t_min = t_min - (t_max - t_min)/20 
    #print('\n\n',t_min,t_max)
    ax.hlines(h4      ,t_min, t_max, linewidth = 1, linestyle = ':',color = 'g', alpha = 0.9)
    ax.text(t_min, h4, 'Die_health_Cool', horizontalalignment='right', verticalalignment='center', fontsize = 7, color = 'g',alpha = 0.5)
    
    ax.hlines(h4 + j*1, t_min,t_max, linewidth = 1, linestyle = ':',color = 'orange', alpha = 0.7)
    ax.text(t_min, h4 + j*1, 'Warm', horizontalalignment='right', verticalalignment='center', fontsize = 7, color = 'orange',alpha = 0.5) 

    ax.hlines(h4 + j*2, t_min,t_max, linewidth = 1, linestyle = ':', color = 'r',alpha = 0.7)
    ax.text(t_min, h4 + j*2, 'Hot', horizontalalignment='right', verticalalignment='center', fontsize = 7, color = 'r', alpha = 0.5) 
    
    ax.hlines(h4 + j*3, t_min,t_max, linewidth = 1, linestyle = ':',color = 'r', alpha = 1)
    ax.text(t_min, h4 + j*3, 'Overheat', horizontalalignment='right', verticalalignment='center', fontsize = 7, color = 'r',alpha = 1) 


    # c --3.-----------is_need_increment---------------------
    res = re.findall('\[.*?(\d+\.\d+)\]\s.*?is_need_increment\s\=\s(\d)',text)
    lt = []
    lv = []
    i = 0
    for t,v in res:
        if i%2 == 0:
            lt.append(float(t))
            lv.append(int(v)/2 + 12)
            
    # print(lv)
    # print(type(lv))
    lt,lv = conv_square_wave(lt,lv)
    ax.plot(lt,lv)    

    min_index = int(len(lt)/2)
    #ax.text(lt[min_index],lv[min_index],'is_need_increment',horizontalalignment='left', verticalalignment='center', fontsize = 8,alpha = 1)    
    ax.text(lt[0],lv[0],'is_need_increment',horizontalalignment='left', verticalalignment='center', fontsize = 8,alpha = 1) 
    
    # d-----2.--soc.txt--input_current_limit---------------
    time = list(df['Uptime'])
    
    il = df['inputCurrentLimited']
    #il = list( map(int, il) )
    lv =  list( map(lambda x:int(x)/2 + 12.7, il) )
    # print(lv)
    # print(type(lv))
    t = list(time)
    lt,lv = conv_square_wave(t,lv)
    ax.plot(lt,lv)    
    
    min_index = int(len(lt)/2)
    #ax.text(lt[min_index],lv[min_index],'input_current_limited',horizontalalignment='left', verticalalignment='center', fontsize = 8,alpha = 1)
    ax.text(lt[0],lv[0],'input_current_limited',horizontalalignment='left', verticalalignment='center', fontsize = 8,alpha = 1) 
    #ax.text((lt[-1]-lt[0])/5,13],'input_current_limited',horizontalalignment='left', verticalalignment='center', fontsize = 8,alpha = 1)   
    
    # e---1. --soc.txt---charge type----------------
    h2 = 13.5
    sd2 = {'Fast':h2 + 0.4, 'Taper':h2, 'N/A':h2 }
    
    
    bct = list(df['batterychrgType'])
    
    bct = list( map(lambda x:sd2[x],bct) )
    t = list(time)
    
    #print('before\n',t,bct)
    t,bct = conv_square_wave(t,bct)
    ax.plot(t,bct)
    #print('afger\n',t,bct)
    min_index = int(len(t)/2)
    #ax.text(t[min_index],bct[min_index],'is Fast',horizontalalignment='left', verticalalignment='center', fontsize = 8,alpha = 1)
    ax.text(t[0],bct[0],'is Fast',horizontalalignment='left', verticalalignment='center', fontsize = 8,alpha = 1)
    #ax.text((t[-1]-t[0])/5, 13.5,'is Fast',horizontalalignment='left', verticalalignment='bottom', fontsize = 8,alpha = 1) 
    
    # -----taper entry work-------
    res = re.findall('\[.*?(\d+\.\d+)\]\s.*taper\sentry\sscheduling\swork',text)
    i = 0
    for t in res:
        t = float(t)
        #print(t)
        ax.arrow(t,h2+0.3,0,-0.3, length_includes_head = True, shape ='right',head_width=16, head_length=0.1, fc='r', ec='r', width = 0.2)
        if i == 0:
            ax.text(t,h2+0.3,'taper entry work',horizontalalignment='left', verticalalignment='bottom', fontsize = 8,alpha = 0.8, rotation = 45)
            i += 1
            
    # f----qc2 states-----------------
    plot_qc2_status()
    
    
#=======================================================================
def find_t_mid(ls):
    #print(ls)
    if len(ls) <= 1 :
        return -1
    t_mid = ls[0] + (ls[-1] - ls[0])/2
    
    i = 0
    for t in ls:
        if t >= t_mid:
            return i -1
        i += 1   
####################################################
def get_pl_disable_log(dir):
    #with open('dmesg.txt') as f:
    #text
    # if len(sys.argv) == 1:
        # dir = 'dmesg.txt'
    # else:
        # dir = get_dir()
    print('====================get_pl_disable_log, dir = ',dir)
    f = open(dir)

    if 1:
        sw_on = []
        sw_off = []
        
        ls = []
        start = 0
        sw_en = 0
        end = 1
        effec = 0
        
        stepper_work = []
        for line in f.readlines():
            
            #--stepper_work---------------------
            r_stepper = re.findall('\[.*?(\d+.\d+)\]\sQCOM-BATT:\sfcc_stepper_work:\sRescheduling\sFCC_STEPPER\swork', line)
            if r_stepper != []:
                stepper_work.append( float(r_stepper[0]) )
            
            n_gcp = 0
            if end == 1:
                #start time
                str = '\[.*?(\d+.\d+)\]\sQCOM-BATT:\spl_disable_vote_callback:\sFCC Stepper'
                r_start = re.findall(str, line)
                if r_start != []:
                    start = 1
                    end = 0
                    #ls.append(float(r_start[0]))
                    t_start = float(r_start[0])
                    
                    # 截止时间250s
                    # if t_start > 250:
                        # break
                    
            if start == 1:
                ####sw on ###   chip->fcc_stepper_enable###################################################
                
                #get_fcc
                str = '\[.*?(\d+.\d+)\]\s.*get_charge_param:\sfast\scharge\scurrent\s=\s(\d+)'
                get_charge_param = re.findall(str,line)
                if get_charge_param != []:
                    n_gcp += 1
                    get_fcc = float(int(get_charge_param[0][1]))/1000000
                
                #fcc_stepper_parameters
                str = '\[.*?(\d+.\d+)\]\s.*main_step_direction:\s(\-?\d),\smain_step_count:\s(\d+),\smain_residual_fcc:\s(\d+)'
                fcc_stepper_parameters = re.findall(str,line)
                if fcc_stepper_parameters != [] :
                    sw_en = 1
                    # ls.append(int(fcc_stepper_parameters[0][1]))
                    # ls.append(int(fcc_stepper_parameters[0][2]))
                    dirc = int(fcc_stepper_parameters[0][1])
                    steps = int(fcc_stepper_parameters[0][2])
                    residual = float( int(fcc_stepper_parameters[0][3])/1000000 )
                    
                    
                #CP_ILIM vote
                
                
                ############# sw off ##### !chip->fcc_stepper_enable ###########################################
                
                #set fcc
                str = '\[.*?(\d+.\d+)\]\s.*smblib_set_charge_param:\sfast\scharge\scurrent\s=\s(\d+)'
                r_usb_icl = re.findall(str, line)
                if r_usb_icl != [] :
                    # if n_gcp != 0:
                        # del ls[1]
                    usb_icl = float( int(r_usb_icl[0][1])/1000000 )
                    #ls.append(usb_icl)
                    #ls.insert(1,usb_icl)
                    set_fcc = usb_icl
                    
                #CP_ILIM vote FCC_VOTER
                #class2  class3 可以去掉最后画图测试
                class2_ilim = re.findall('CP_ILIM:\sFCC_VOTER,0\ssame\svote\son\sof\sval=(\d+)', line)
                if class2_ilim != []:
                    #ls.append(float(int(class2_ilim[0]/1000000)))
                    #ls.insert(2,float(int(class2_ilim[0]/1000000)))
                    cp_ilim = float(int(class2_ilim[0])/1000000)
                    
                str = '\[.*?(\d+.\d+)\]\s.*sCP_ILIM:\sFCC_VOTER,0\svoting\son\sof\sval=(\d+)'
                class3_ilim = re.findall(str, line)
                if class3_ilim != []:
                    #ls.append(float(int(class3_ilim[0][1])/1000000))
                    #ls.insert(2,float(int(class3_ilim[0][1])/1000000))
                    cp_ilim = float(int(class3_ilim[0][1])/1000000)
                    
                class4_ilim = re.findall('CP_ILIM:\seffective\svote\sis\snow\s(\d+)',line)
                if class4_ilim != []:
                    effec = 1
                    # ls.append(float(int(class4_ilim[0])/1000000))
                    #ls.insert(2,float(int(class4_ilim[0])/1000000))
                    cp_ilim = float(int(class4_ilim[0])/1000000)
                    
                #end time
                str = '\[.*?(\d+.\d+)\]\sQCOM-BATT:\spl_disable_vote_callback:\sparallel\scharging'
                r_end = re.findall(str, line)
                if r_end != []:
                    t_end = float(r_end[0])
                    
                    if sw_en == 1:
                        ls = [t_start,get_fcc, dirc,steps,residual,t_end]
                        sw_on.append(ls)
                    else :
                        ls = [t_start, set_fcc, cp_ilim,t_end, effec]
                        sw_off.append(ls)
                    
                    
                    start = 0
                    end = 1
                    sw_en = 0   
                    effec = 0
                    ls = []
    f.close()
    return sw_on, sw_off, stepper_work         
    # for a in sw_on:
        # print(a)
        
    # print('*'*50)

    # for a in sw_off:
        # print(a) 

#==========================plot=sw_on==sw_off===============================
# fig = plt.figure(figsize = (14,7))
# ax = plt.subplot(111)
# cursor = Cursor(ax, useblit=True, linewidth = 0.5)

def plot_pl_disable(sw_off, sw_on, stepper_work):
    #---- sw off ---------------
    for a in sw_off:
        ax.plot(a[0], a[1], '.')
        ax.plot(a[0], a[2], '.')
        plt.vlines(a[0],a[1],a[2],linestyle=':', linewidth = 0.5, alpha= 0.4)

    #---- sw_on ----------------------    
    for a in sw_on:
        if a[2] > 0 :
            ax.plot(a[0], a[1], '2', alpha = 0.6, color = 'sienna')
            ax.vlines(a[0], a[1], a[1]+0.1*a[3] + a[4], linewidth = 0.3, color = 'goldenrod', alpha = 0.99)
        else:
            ax.plot(a[0], a[1], '1', alpha = 0.5, color = 'cyan')
            ax.vlines(a[0],a[1]- 0.1*a[3] - a[4], a[1], linewidth = 0.4, color = 'g', alpha = 0.8)

    #-----stepper_work------
    for t in stepper_work:
        ax.plot(t,4.9,'.', markersize = '3', alpha = 0.6, color = 'g')
    
def rplot_pl_disable(isplot):
    global gdir
    if isplot == 0:
        return
    sw_on,sw_off,stepper_work = get_pl_disable_log(gdir)
    plot_pl_disable(sw_off, sw_on, stepper_work)  
    
    #------pl_notify---执行时间点图-------------------
    pl_notify()      #3.pl_notify

    

#---缩放--------------------
# ----------缩放--------------------------
def call_back_this(event):
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



#lime
# ----4级  effective_vote * -----------
def parse_class4_effective_vote(st,show_time,show_id,show_class4,marker,linestyle, markersize= 5, linewidth=0.5,color='lime',alpha =0.99, isplot = 1):
    if isplot == 0:
        return
        
    print('self.d4.get(',st,')')
    st1 = '\[.*?(\d*\.\d*)\]?\s' + st + ':\seffective.*?(\-?\d+)\s.*?(\d+)'
    effective_vote = re.findall(st1, text)

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
   
    if show_class4:
        #ax.plot(lt,lv, marker = '*',linestyle=':',linewidth = 0.5, color = cl)
        #画marker
        ax.plot(lt,lv,marker, markersize = markersize, color = color, alpha = alpha)
        
        lt1, lv1 = conv_square_wave(lt,lv) #转化成方波
        ax.plot(lt1,lv1,markersize = markersize,linestyle = linestyle, linewidth = linewidth, color = color, alpha = alpha)
        
        t_mid_i = find_t_mid(lt)
        print('\n',st)
        if t_mid_i >= 0 :
            ax.text(lt[t_mid_i],lv[t_mid_i], st, horizontalalignment='right', verticalalignment='center', fontsize = 7,alpha = 0.95, color = color)
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
    
    
# -----3级---打印3级 --*--和--+----voting of--
def parse_class3_voting_of(st,zero_val):
    marker_style = dict(color='cornflowerblue', linestyle=':', marker='o',
                    markersize=15, markerfacecoloralt='gray')
    st = 'FCC'
    st1 = '\[.*?(\d*\.\d*)\]?\s' + st + ':\s(\w*)_VOTER,(\d)\svoting\s(\w+)\sof\sval=(\d+)'
    res = re.findall(st1, text)
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
    for i in range(len(lval)):
        if lbool[i] == "off":
            #lval[i] = 4.7
            lval[i] = zero_val
    #print(lval)    
    # lt = lt[0:len(lt)-1]
    # lv = lv[0:len(lv)-1]
    #plt.plot(lt,lval, marker = '+',linestyle='-',linewidth = 1) 
    for i in range(len(lt)):
        if lbool[i] == 'on':
            plt.plot(lt[i],lval[i], marker = '+', markersize = 13, color = 'orange', alpha = 0.9)
            plt.text(lt[i],lval[i]+0.05, li[i], horizontalalignment='center', verticalalignment='bottom', fontsize = 7,alpha = 0.8)
        if lbool[i] == 'off':
            #plt.plot(lt[i],lval[i], fillstyle = 'none', **marker_style)
            plt.plot(lt[i],lval[i], marker = '+', markersize = 10, color = 'darkgoldenrod', alpha = 0.9)
            plt.text(lt[i],lval[i]-0.05, li[i], horizontalalignment='center', verticalalignment='top', fontsize = 7,alpha = 0.8)

# ---2级--打印2,1级voter--*-+-x->----Ignoring-----same vote------------
def parse_class2_same_but_fist_and_twiceIgnoring(st,zero_val):
    marker_style = dict(color='cornflowerblue', linestyle=':', marker='X',
                        markersize=5, markerfacecoloralt='r')
    st1 = '\[.*?(\d*\.\d*)\]?\s' + st + ':\s(\w*)_VOTER,(\d)\sIgnoring\ssimilar\svote\s(\w+)\sof\sval=(\d+)'
    res = re.findall(st1, text)
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
    
    for i in range(len(lval)):
        if lbool[i] == "off":
            #lval[i] = 4.7
            lval[i] = zero_val
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
            plt.plot(lt[i],lval[i], marker = 'x', markersize = 4, color = 'b', alpha = 0.5)
            plt.text(lt[i],y_txt, li[i], horizontalalignment='center', verticalalignment='bottom', fontsize = 6,alpha = 0.8)
        else:
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
            plt.plot(lt[i],lval[i], marker = 'x', markersize = 3.8, color = 'lightsteelblue', alpha = 0.5)
            plt.text(lt[i],y_txt, li[i], horizontalalignment='center', verticalalignment='top', fontsize = 5.5,alpha = 0.6)        


#------pl_notify----------------------
def pl_notify():
    r_pl_notify_c4 = re.findall('\[.*?(\d+.\d+)\]\sPL_ENABLE_INDIRECT:\seffective\svote\sis\snow\s1\svoted\sby\sUSBIN_I_VOTER,0', text)

    #有两三个点的干扰
    # static int usb_icl_vote_callback(struct votable *votable, void *data,
        # if (icl_ua <= 1400000)     //2. pl_siable
            # vote(chip->pl_enable_votable_indirect, USBIN_I_VOTER, false, 0);
    r_pl_notify_c2 = re.findall('\[.*?(\d+.\d+)\]\sPL_ENABLE_INDIRECT:\sUSBIN_I_VOTER,0\sIgnoring\ssimilar\svote\s', text)

    pl_c4 = list( map(float, r_pl_notify_c4) )
    pl_c3 = list( map(float, r_pl_notify_c2) )
    # print(pl_c4)
    # print('*'*50)
    # print(pl_c3)
    for t in pl_c4:
        ax.plot(t,4.7,'.', markersize = '4', alpha = 0.99, color='r')

    for t in pl_c3:
        ax.plot(t,4.8,'.', markersize = '3', alpha = 0.6, color = 'k')

# plt.show()

def get_cp_irq_data():
    global off_w_0,uv_ov_1,TSD_2,TREV_3,VPH_OV_HARD_4,VPH_OV_SOFT_5,ILIM_6,TEMP_ALARM_7
    
    off_w_0 = re.findall('\[\s*?(\d*?.\d*?)\].*?switcher-off-window\sIRQ\striggered', text)
    if off_w_0 != []:
        off_w_0 = list(map(float,off_w_0 ))


    # sw is disable
    uv_ov_1 = re.findall('\[\s*?(\d*?.\d*?)\].*?switcher-off-fault\sIRQ\striggered', text)
    if uv_ov_1 != []:
        uv_ov_1 = list(map(float,uv_ov_1 ))
        
    TSD_2 = re.findall('\[\s*?\d*?.\d*?\].*?tsd-fault\sIRQ\striggered', text)
    if TSD_2 != []:
        TSD_2 = list(map(float, TSD_2))
        
    TREV_3  = re.findall('\[\s*?(\d*?.\d*?)\].*?irev-fault\sIRQ\striggered', text)
    if TREV_3 != []:
        TREV_3 = list(map(float, TREV_3))
        
    VPH_OV_HARD_4 = re.findall('\[\s*?(\d*?.\d*?)\].*?vph-ov-hard\sIRQ\striggered', text)
    if VPH_OV_HARD_4 != []:
        VPH_OV_HARD_4 = list(map(float, VPH_OV_HARD_4))
    
    
    
    VPH_OV_SOFT_5 = re.findall('\[\s*?(\d*?.\d*?)\].*?vph-ov-soft\sIRQ\striggered', text)
    if VPH_OV_SOFT_5 != []:
        VPH_OV_SOFT_5 = list(map(float, VPH_OV_SOFT_5))
        
    ILIM_6 = re.findall('\[\s*?(\d*?.\d*?)\].*?ilim\sIRQ\striggered', text)
    if ILIM_6 != []:
        ILIM_6 = list(map(float, ILIM_6))
        
    TEMP_ALARM_7 = re.findall('\[\s*?(\d*?.\d*?)\].*?temp-alarm\sIRQ\striggered', text)
    if TEMP_ALARM_7 != []:
        TEMP_ALARM_7 = list(map(float, TEMP_ALARM_7))

def plot_cp_irq(isplot = 1):
    if isplot == 0:
        return
    
    for t in off_w_0:                                               #电压     vin/VPH_PWR
        ax.plot(t,7.75, color = 'blue', marker = '.', markersize = '3')
        ax.text(t,7.75,0,horizontalalignment='center', verticalalignment='bottom', fontsize = 7, alpha = 0.6)
    
    
    #bit1, bit2, bit3, bit4会 sw disable 
    # 电压 .      电流x     温度2
    for t in uv_ov_1:                                               #电压     uv/ov
        ax.plot(t,7.5, color = 'blue', marker = '.', markersize = '5')
        ax.text(t,7.5, 1, horizontalalignment='center', verticalalignment='bottom', fontsize = 7, alpha = 0.5)
    
    for t in TSD_2:                                                 #温度     大于 140℃
        ax.plot(t,7.5, color = 'red', marker = '2', markersize = '5')
        ax.text(t,7.5, 2, horizontalalignment='center', verticalalignment='bottom', fontsize = 7, alpha = 0.5)

    for t in TREV_3:                                                #电流     反转 200~300ma
        ax.plot(t,7.5, color = 'green', marker = 'x', markersize = '5')
        ax.text(t,7.5, 3, horizontalalignment='center', verticalalignment='bottom', fontsize = 7, alpha = 0.5)
 
    for t in VPH_OV_HARD_4:                                         #电压     V_batt大于5v（5.2v）
        ax.plot(t,7.5, color = 'blue', marker = '.', markersize = '5')
        ax.text(t,7.5, 4, horizontalalignment='center', verticalalignment='bottom', fontsize = 7, alpha = 0.5) 
    #------不会关闭 sw -----------------
    
    for t in VPH_OV_SOFT_5:                                         #电压     V_batt大于4.8v（5v）
        ax.plot(t,7.75, color = 'blue', marker = '.', markersize = '3')
        ax.text(t,7.75, 5,horizontalalignment='center', verticalalignment='bottom', fontsize = 7, alpha = 0.6)
    
    for t in ILIM_6:                                                #电流     电流大于ILIM
        ax.plot(t,7.75, color = 'green', marker = 'x', markersize = '3')
        ax.text(t,7.75, 6, horizontalalignment='center', verticalalignment='bottom', fontsize = 7, alpha = 0.6)

    for t in TEMP_ALARM_7:                                          #温度     大于 80 90 115(可设定)
        ax.plot(t,7.75, color = 'red', marker = '2', markersize = '3')
        ax.text(t,7.75, 7, horizontalalignment='center', verticalalignment='bottom', fontsize = 7, alpha = 0.5)    
    
    ax.text(ltt[0], 7.5, 'cp_irq: 0:WIN/VPH\n1:UN/OV  3:IREV  6:ILIM', horizontalalignment='right', verticalalignment='center', fontsize = 7, alpha = 0.99, color = 'orange')
    

def plot_cp_status(is_plot = 1):
    if is_plot == 0:
        return
    #^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
    #    cp status
    #^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
    #sd = { 'S_DISABLED':[8,'disable'], 'S_VIN_MIN':[8,'VIN_MIN'],'S_NOT_OK_TO_CP':[8.5,'NOT_OK'], 'S_BALANCED':[8.5, 'balanced'], 'S_VIN_MAX':[9,'VIN_MAX'], 'S_MAIN_COOLING':[9,'MAIN_cooling'], 'S_CP_COOLING':[9,'CP_cooling'], 'S_MAIN_HOT':[9.5,'main_hot'], 'S_CP_HOT':[9.5, 'cp_hot'], 'S_CP_IRQ':[10,'CP_IRQ'], 'S_VIN_REQ':[10,'VIN_REQ']}
    
    h = 8; j = 0.3
    sd11 = {'S_NOT_OK_TO_CP': h,   'S_DISABLED': h + j*1,  
            
            'S_VIN_MIN': h+j*2,    'S_VIN_MAX': h + j*3,
         
            'S_VIN_REQ': h + j*4,   
         
             'S_CP_HOT': h + j*5,          'S_CP_COOLING': h + 0.3*6,
             'S_BALANCED': h + j*7, 
             'S_MAIN_COOLING' : h + j*8,  'S_MAIN_HOT': h + j*9,
             
             'S_CP_IRQ':  h + j*10,
             'S_THERMAL': h + j*11
             }
    
    #lt_state
   
    #print('\n',lt_state)
    c_state = re.findall('\[.*?(\d+\.\d+)\]\sCP:\s(.*)\s->\s(.*)', text)
    
    # ('176.627771', 'S_NOT_OK_TO_CP', 'S_DISABLED'), 
    # ('180.713886', 'S_DISABLED',     'S_VIN_REQ')
    a = ( str(ltt[0]),lt_state[0],lt_state[0] )
    c_state.insert(0,a)
    #print('\n', c_state)
    
    t_st = []
    s = []
    #tc -> to state  ;   c_state -> change state
    for t,ps,ts in c_state:
        t = float(t)
        # sval = sd[ts][0]
        # sstr = sd[ts][1]
        #ax.plot(t,sval, marker = '*')
          
        t_st.append(t)
        s.append(ts)
        #print('befort conv t_st:\n',t_st)
        #print('before conv s:\n', s)
            
    t_st,s = conv_square_wave(t_st,s)       #方波
    #print('t_st\n', t_st)
    #print('s\n', s)
    
    s_val = list(map(lambda x:sd11[x],s)) #转化成值
    ax.plot(t_st,s_val,color = 'b', linewidth = 1 , marker ='.', markersize = '3', alpha = 0.6)  #状态曲线
    
    h_start = ltt[1] - 0.5
    ax.hlines(h,         h_start,h_start + 10, linewidth = 1, linestyle = ':', alpha = 0.9)
    ax.text(h_start, h, 'S_NOT_OK', horizontalalignment='right', verticalalignment='center', fontsize = 7, alpha = 0.5)
    
    ax.hlines(h + j*1, h_start,h_start + 10, linewidth = 1, linestyle = ':', alpha = 0.7)
    ax.text(h_start, h + j*1, 'S_DISABLED', horizontalalignment='right', verticalalignment='center', fontsize = 7, alpha = 0.5)
    
    ax.hlines(h + j*2, h_start,h_start + 10, linewidth = 1, linestyle = ':', alpha = 0.5)
    ax.text(h_start, h + j*2, 'S_VIN_MIN', horizontalalignment='right', verticalalignment='center', fontsize = 7, alpha = 0.5)
    
    ax.hlines(h + j*3, h_start,h_start + 10, linewidth = 1, linestyle = ':', alpha = 0.3)
    ax.text(h_start, h + j*3, 'S_VIN_MAX', horizontalalignment='right', verticalalignment='center', fontsize = 7, alpha = 0.5)
    
    ax.hlines(h + j*4, h_start,h_start + 10, linewidth = 1, linestyle = ':', alpha = 0.3)
    ax.text(h_start, h + j*4, 'S_VIN_REQ', horizontalalignment='right', verticalalignment='center', fontsize = 7, alpha = 0.5)
    
    
    ax.hlines(h + j*5, h_start,ltt[-1], linewidth = 1, linestyle = ':', alpha = 0.3)
    ax.text(h_start, h + j*5, 'S_CP_HOT', horizontalalignment='right', verticalalignment='center', fontsize = 7, alpha = 0.9,color = 'r')

    ax.hlines(h + j*6, h_start,ltt[-1], linewidth = 1, linestyle = ':', alpha = 0.3)
    ax.text(h_start, h + j*6, 'S_CP_COOLING', horizontalalignment='right', verticalalignment='center', fontsize = 7, alpha = 0.5)

    ax.hlines(h + j*7, h_start,ltt[-1], linewidth = 2, linestyle = '--', color = 'lime' ,alpha = 0.8)
    ax.text(h_start, h + j*7, 'S_BALANCED', horizontalalignment='right', verticalalignment='center', fontsize = 7, alpha = 0.7)

    ax.hlines(h + j*8, h_start,ltt[-1], linewidth = 1, linestyle = ':', color = 'peru', alpha = 0.7)
    ax.text(h_start, h + j*8, 'S_MAIN_COOLING', horizontalalignment='right', verticalalignment='center', fontsize = 7, alpha = 0.7)    
    ax.hlines(h + j*9, h_start,ltt[-1], linewidth = 1.5, linestyle = '--',color ='r', alpha = 0.6)
    ax.text(h_start, h + j*9, 'S_MAIN_HOT', horizontalalignment='right', verticalalignment='center', fontsize = 7, alpha = 0.9, color = 'r')
    
    
    ax.hlines(h + j*10, h_start,ltt[-1], linewidth = 1, linestyle = ':', alpha = 0.3)
    ax.text(h_start, h + j*10, 'S_CP_IRQ', horizontalalignment='right', verticalalignment='center', fontsize = 7, alpha = 1)
    #=======================
    # cp state end
    #======================


def plot_main_irq(is_plot = 1):
    if is_plot == 0:
        return 
    #^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
    #   main irq 
    #^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
    #res = re.findall('\[\s*(\d+?.\d+?)\].*(?<!wdog_snarl_irq_handler:\s)IRQ:\s(.*)', text)
    
    d_irq = {
        'default':0.5, 'irq':1
    }
    d_irqn = {}
    #res = re.findall('\[\s*(\d+?.\d+?)\]\spm6150_charger:\s(.*?):\sIRQ:\s(.*)', text)
    res = re.findall('\[\s*(\d+?.\d+?)\]\s.*?:\s(.*?):\sIRQ:\s(.*)', text)
    #print(res)
    # lt_irq =  []
    # lh = []
    # ln = []
    t_on = []
    t_off = []
    i = 0
    #   handler,name
    for t,h,n in res:
        if h != 'wdog_snarl_irq_handler' :
            # lt_irq.append( float(t)  )
            # lh.append(h)
            # ln.append(n)
            
            res = re.findall('cc-state-change;\sType-C\s(.*)\sdetected',n)
            if res != []:
                n = res[0]
            if d_irqn.get(n) == None:
                i = i + 1
                d_irqn[n] = i
            
            if h == 'default_irq_handler':
                t_off.append(float(t))
                h = 0
                
                if len(t_off) > 1 :
                    for j in range(len(t_off)):
                        #print('in for  j=%d' % j)
                        if j == len(t_off)-1 or j >= 9:
                            break
                        elif t_off[-j-1] - t_off[-j-2] < 1 :
                            h += 0.15
                        else:
                            break
                ax.plot(float(t), h,marker='.',markersize = 4,color = 'b',alpha = 0.6 )
                #ax.text(float(t), 0, n, horizontalalignment='center', verticalalignment='bottom', fontsize = 6,alpha = 0.5, rotation = 70)
                ax.text(float(t),h-0.01, d_irqn[n], horizontalalignment='center', verticalalignment='top', fontsize = 6,alpha = 0.8)
                
            else :
                t_on.append(float(t))
                h = 0.5
                
                if len(t_on) > 1 :
                    for j in range(len(t_on)):
                        #print('in for  j=%d' % j)
                        if j == len(t_on)-1 or j >= 9:
                            break
                        elif t_on[-j-1] - t_on[-j-2] < 1 :
                            h += 0.175
                        else:
                            break
                
                ax.plot(float(t), h, marker='.',markersize = 5,alpha = 0.7)
                #ax.text(float(t), 0.5,n, horizontalalignment='center', verticalalignment='bottom',  fontsize = 6,alpha = 0.7, rotation = 65)
                ax.text(float(t), h,d_irqn[n], horizontalalignment='center', verticalalignment='bottom',  fontsize = 6,alpha = 0.99)
    # print(lt_irq)
    # print(lh)
    # print(ln)
    # print(d_irqn)
    
    #左侧的irq名字序号
    x = 0.01; y = 0.74
    ax.text(x,y,'Main_irq',horizontalalignment='left', verticalalignment='bottom', fontsize = 8,alpha = 1, transform=ax.transAxes)
    for item in d_irqn:
        y -= 0.02
        res = str(d_irqn[item]) + ' ' + item
        ax.text(x,y,res,horizontalalignment='left', verticalalignment='bottom', fontsize = 7,alpha = 0.6, transform=ax.transAxes)
          
    #=======================
    # main irq end
    #======================



    
class Buttonprocess(object):
    def __init__(self):
        self.switch = {'pl_disable':1, 'FCC':1, 'USB_ICL':1, 'CP_ILIM':1, 'CP_IRQ':1, 'CP_status':1, 'main_irq':1}
        self.check = None
        self.visable = []
        self.first_press_button = True
        
    def press_pl_disable(self,event):
        self.switch['pl_disable'] = not self.switch['pl_disable']
        if self.switch['pl_disable']:
            bpl_disable.color = 'palegreen'
        else :
            bpl_disable.color = 'lightgrey'
        print('press pl_disable, switch =', self.switch)
        self.update()
        
    def press_FCC(self,event):
        self.switch['FCC'] = not self.switch['FCC']
        if self.switch['FCC']:
            bFCC.color = 'palegreen'
        else :
            bFCC.color = 'lightgrey'
        print('press FCC, switch =', self.switch)
        self.update()        
        
    def press_USB_ICL(self,event):
        self.switch['USB_ICL'] = not self.switch['USB_ICL']
        if self.switch['USB_ICL']:
            bUSB_ICL.color = 'palegreen'
        else :
            bUSB_ICL.color = 'lightgrey'
        print('press USB_ICL, switch =', self.switch)
        self.update()  

    def press_CP_ILIM(self,event):
        self.switch['CP_ILIM'] = not self.switch['CP_ILIM']
        if self.switch['CP_ILIM']:
            bCP_ILIM.color = 'palegreen'
            #print('bCP_ILIM.color = palegreen')
        else :
            bCP_ILIM.color = 'lightgrey'
            #print('bCP_ILIM.color = lightgrey')
        print('press CP_ILIM, switch =', self.switch)
        self.update() 
        #return self.check

    def press_CP_IRQ(self,event):
        self.switch['CP_IRQ'] = not self.switch['CP_IRQ']
        if self.switch['CP_IRQ']:
            bCP_IRQ.color = 'palegreen'
            print('bCP_IRQ.color = palegreen')
        else :
            bCP_IRQ.color = 'lightgrey'
            print('bCP_IRQ.color = lightgrey')
        print('press CP_IRQ, switch =', self.switch)
        self.update()     
    
    def press_cp_status(self, event):
        self.switch['CP_status'] = not self.switch['CP_status']
        if self.switch['CP_status']:
            bCP_status.color = 'palegreen'
            print('bCP_status.color = palegreen')
        else :
            bCP_status.color = 'lightgrey'
            print('bCP_status.color = lightgrey')
        print('press CP_status, switch =', self.switch)
        self.update()                 

    def press_main_irq(self, event):
        self.switch['main_irq'] = not self.switch['main_irq']
        if self.switch['main_irq']:
            bmain_irq.color = 'palegreen'
            print('bmain_irq.color = palegreen')
        else :
            bmain_irq.color = 'lightgrey'
            print('bmain_irq.color = lightgrey')
        print('press main_irq, switch =', self.switch)
        self.update()    
        
    def update(self):
        print('update start ....')
        
        global glines
        global lines
        if self.first_press_button == True:
            self.visable = [ line.get_visible()  for line in glines]
        else :
            self.visable= [ line.get_visible()  for line in lines]
        print(self.visable)
        
        self.first_press_button = False
        
        ax.cla()        #线条图会清除，但是buttom不会清除。之间创建button继续使用
        #fig.cla()
        plt.cla()       
        #check button  rax = plt.axes([0.02,...
        #所以跟plt有关，需要清除plt，再重新画
        #如果不清楚,check button新旧重叠，x号干扰
        
        # l8, = ax.plot([1,2,3],[9,9,9], label = 'l8')
        # l9, = ax.plot([1,2,3],[5,6,7], label = 'l9')
        
        if pro_use == True:
            rplot_pl_disable(self.switch['pl_disable'])
            
        parse_class4_effective_vote('FCC',1,1,1, marker='*', linestyle = '-.', markersize = 7,color = 'blue', alpha = 0.5, isplot =self.switch['FCC'])
        
        parse_class4_effective_vote('USB_ICL',1,1,1,marker = '+', linestyle = ':', markersize = 10,color = 'deeppink', alpha = 0.7, isplot =self.switch['USB_ICL'])
        
        parse_class4_effective_vote('CP_ILIM',0,0,1, marker = '.', linestyle = '--', markersize = 2, alpha = 0.6, isplot = self.switch['CP_ILIM'])
        
        #^^^^^CP_IRQ^^^^^^^^^^^^^^^^^^^^
        plot_cp_irq(self.switch['CP_IRQ'])
        
        #^^^^^CP_status^^^^^^^^^^^^^^^^^^^^^^^^^^^^
        plot_cp_status(self.switch['CP_status'])
        
        #^^^^^^ main_irq ^^^^^^^^^^^^^^^^^^^^
        plot_main_irq(self.switch['main_irq'])
        
        
        
        #^^^^^^添加 cp图^^^cp.c log^^^^^^^^^^^^^^^^^^^^^^^
        l0, =  ax.plot(lt_pdo,lv_pdo,  linestyle=':',linewidth = 0.1, marker='o', markersize =1.5, markerfacecolor='r', label = 'rpdo')
        

        if ctype != 'USB_HVDCP':
            l1, = ax.plot(ltt,lt_main,color = 'r', linewidth = 0.4, label = 'T_main')                                                #main温度
            l2, = ax.plot(ltt,lt_cp,color ='orange', linewidth = 0.3,   label = 'T_cp')                                                  #cp温度    
        
        l3, = ax.plot(t_main_hot,v_main_hot,linewidth = 0.4,linestyle=':', label='M_hot') #main_hot布尔值 
        l4, = ax.plot(t_cp_hot,v_cp_hot,linewidth = 0.4,linestyle=':', label = 'cp_hot')    #cp_hot布尔值
        l5, = ax.plot(ltt, lt_main_cp, linewidth = 0.5,color = 'gray',linestyle = '-.', label = 'Tm-cp')      #T_main - T_cp
        if pro_use == False:
            l5.set_visible(0)
        
        l6, = ax.plot(t_smb_en, v_smb_en, linewidth = 0.5, marker ='.',markersize='1', label = 'smb_en' )     #bit7 = 1, smb_en 高电平
        l7, = ax.plot(t_switcher,v_switcher,color = 'c',linewidth = 0.5, marker='.',markersize='1', label = 'sw_en')    #bit7 & bit0
        
        # print('soc time in Buttonprocess')
        # print(time)
        
        try:
            T_batt  = df['batteryTemp']
            la, = ax.plot(time,T_batt/100,'-.',color='r',alpha = 0.5, label = 'T_batt')
            #ax.text(df['Uptime'][mid_index], df['batteryTemp'][mid_index]/100, 'T_batt', horizontalalignment='center', verticalalignment='center', fontsize = 9, alpha = 0.99, color = 'r')
        except:
            print('Missing batteryTemp')
            
        try:
            I_batt  = df['batteryCurrent']
            lb, = ax.plot(time,I_batt/1000000,'g',linewidth = 1.0, label = 'I_batt', alpha = 0.6)
        except:
            print('Missing batteryCurrent')
            
        try:
            I_bus = df['usbCurrent']
            lc, = ax.plot(time,I_bus/1000000,'--', color='b', linewidth = 0.6, alpha = 0.8, label = 'I_bus')
        except:
            print('Missing usbCurrent')
            
        try:
            I_cp = df['CPCurrent']
            ld, = ax.plot(time, I_cp/1000000,color = '#ADFF2F',linewidth= 0.6, label = 'I_cp')
        except:
            print('Missing CPCurrent')        
        #==========================================================
      
        #^^^^^^check button^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
        lines = [l0, l1, l2, l3, l4, l5, l6, l7,la,lb,lc,ld]
        
        for l,v in zip(lines, self.visable):
            l.set_visible(v)
        
        #lines = [l8,l9]
        rax = plt.axes([0.02,0.9-0.03*16-0.03*len(lines),0.07,0.03*len(lines)])
        labels =    [str(line.get_label()) for line in lines] #labels列表
        visibility = [ line.get_visible()  for line in lines] #visibility列表
        print('new lines visibility',visibility)
        check = CheckButtons(rax, labels, visibility)         #lables显示在按钮中
        

        
        
        def func(label):                                      #按中某个按钮会返回它
            index = labels.index(label)                       #通过label找到line
            lines[index].set_visible(not lines[index].get_visible())
            #ax.set_ylim(0,10)
            plt.draw()
        check.on_clicked(func)
        
        self.check = check
               
        print('update end ===================')
        fig.canvas.draw()
        
    
def func_button(bt):
    p3 = plt.axes([0.02,h_buttom-0.03/6-0.03*2,0.05,0.03])
    bUSB_ICL = Button(p3, 'USB_ICL', color = 'palegreen')
    bUSB_ICL.on_clicked(bt.press_USB_ICL)
    return bUSB_ICL
    # p2 = plt.axes([0.01,0.9-0.03/10-0.03*1,0.05,0.03])
    # bFCC = Button(p2, 'FCC')
    # bFCC.on_clicked(bt.press_FCC)

def get_dir():
    #不清楚是启动程序调用 还是 手动启动本文件， 
    #如果是启动程序调用一定有dmesg_py文件
    #手动启动则没有dmesg_py文件
    #if sys.argv[1].endswith('')
    dir = sys.argv[1] + '\dmesg_py.txt'  
    if os.access(dir, os.F_OK) == False:  
        dir = sys.argv[1] + '\dmesg.txt'    
    return dir

class Soc_d(object):
    def __init__(self):
        self.dir = '.'
        
def binding_soc_df_with_dmesg(dir,df):
    with open(dir) as f:
        lines = f.readlines()
        l_min = lines[0]
        l_max = lines[-1]
        st = '\[.*?(\d*\.\d*)\].*'
        
        res = re.findall(st,l_min)
        t_min_dmesg = float(res[0]) 
        
        res = re.findall(st,l_max)
        t_max_dmesg = float(res[0]) #如果dmesg时间被切割，soc.txt对不上，故用dmesg时间对齐

        if t_max_dmesg > 0 :
            df = df[df['Uptime'] < t_max_dmesg]
            df = df[df['Uptime'] > t_min_dmesg]        
        return df
################################################
###################################################    
if __name__ == '__main__':
    import numpy as np
    import pandas as pd
    import matplotlib.pyplot as plt
    #%matplotlib inline
    import re
    from matplotlib.widgets import SpanSelector
    from matplotlib.widgets import Cursor
    from matplotlib.widgets import MultiCursor
    from matplotlib.widgets import RadioButtons
    from matplotlib.widgets import CheckButtons
    from matplotlib.widgets import Button
    from matplotlib.widgets import TextBox
    from matplotlib.ticker import MultipleLocator, FormatStrFormatter
    import os 
    import subprocess
    import sys
    
    from cp_func import *
    from soc import *
    #from access_4 import *
    
   
    d = Soc_d()
    print('\n\nsys.argv',sys.argv)
    if len(sys.argv) >= 2:
        d.dir = sys.argv[1]
        # dir = get_dir()          #pcall限制df
    
    #----soc.txt-------
    datas = []
    cl =['Uptime']
    n = 0
    li = []
    t_max = 0 
    get_data(n,li,cl,datas,d)
    df = pd.DataFrame(data=datas,columns=cl)
    # if t_max_dmesg > 0 :
        # df = df[df['Uptime'] < t_max_dmesg]
        # df = df[df['Uptime'] > t_min_dmesg]
    # if len(sys.argv) >= 4 and float(sys.argv[2]) != 0 and float(sys.argv[3]) != 0  : 
        # t_start = float(sys.argv[2])
        # t_end = float(sys.argv[3])
        
        # df = df[df['Uptime'] < t_end]
        # df = df[df['Uptime'] > t_start]   #开始 结束时间传入 
    # df = df.reset_index(drop = True)
    # ===========================
    

    #==time=span=text=====dmest.txt process=============================
    # f = open('dmesg.txt')
    # text = f.read()
    # f.close()
    
    
    # USB_PD, 
    # USB_HVDCP_3, 
    # USB_HVDCP
    # USB_CDP, 
    # USB_DCP, 
    # USB ( USB_SDP: usb3.0, usb2.0) 
    # Unknown
    ctype = df['usbRealType'].value_counts(ascending = True).index.values[-1]
    print('charger type is ', ctype)
    #ctype = 'QC3'
    pro_use = False
    
    
    
    m = Msg_soc()
    time_span = 0
    pcall = 0
    gdir = ''
    if len(sys.argv) == 5:
        if sys.argv[4] == 'pro' :
            pro_use = True
        else : 
            pro_use = False
        
        # if float(sys.argv[2]) == 0 and float(sys.argv[3]) == 0 :
            # s_dir = get_dir()
            # f = open(s_dir)
            # text = f.read()
            # f.close()
    
    if len(sys.argv) >= 4  and sys.argv[3] != '0': #手起，t截时间
        set_tmin = float(sys.argv[2])
        set_tmax = float(sys.argv[3])
        print(set_tmin,set_tmax)
        dir = sys.argv[1] + '\dmesg.txt'
        text = dmseg_temp_process(dir,set_tmin,set_tmax) #其实不用这个,后面会覆盖
        gdir = 'temp.txt'
        df = df[df['Uptime'] > set_tmin]
        df = df[df['Uptime'] < set_tmax]
        time_span = 1
        
    if len(sys.argv) >= 3:
        if sys.argv[2] == 'pcall':
            print('access pcall')
            dir = get_dir()
            f = open(dir)
            text = f.read()
            f.close()
            #text = dmseg_temp_process(dir,set_tmin,set_tmax)
            df = binding_soc_df_with_dmesg(dir,df) #不需要，soc在access中画出
            pcall = 1
            
    if time_span == 0 and pcall == 0:    
        #找soc100时间
        if len(sys.argv) >= 2 and sys.argv[1] != 0:            #2个参数
            m.dir = sys.argv[1]
            #if time_span == 0:
            dir = m.dir + '\dmesg.txt'
            print('2 argv, dir = ', dir)
        else:                             #1个参数
            m.dir = '.'
            dir = 'dmesg.txt'
            print('1 arg, dir = ', dir)
        
        get_soc_message(m)
    
        if m.is_soc_100:
            set_max = m.df['Uptime'][m.soc_100_index]
            text = dmseg_temp_process(dir,0,set_max) #soc超过100
            gdir = 'temp.txt'
            #print(text)
            df = binding_soc_df_with_dmesg('temp.txt',df)
            print('soc >100, set_t = ',set_max,dir)            
        else:
            f = open(dir)    #soc没有超过100
            text = f.read()
            f.close()
            print('soc < 100')         
    if gdir == '':
        gdir = dir
    print('gdir = ', dir)
    print('=====================pro_use ,use dir =  ', pro_use,dir)
    
    #sw_on,sw_off,stepper_work = get_pl_disable_log()
    #==========================plot准备================================
    fig = plt.figure(figsize = (15,9))
    ax = plt.subplot(111)
    ax.spines['top'].set_visible(False)
    ax.spines['right'].set_visible(False)
    ax.set_title('State machine and overall view')
    
    plt.subplots_adjust(top = 0.99, bottom = 0.05, left = 0.02, right = 0.99)  #left = 0.02
    
    ymajorLocator = MultipleLocator(1)
    ax.yaxis.set_major_locator(ymajorLocator)
    
    cursor = Cursor(ax, useblit=True, linewidth = 1)
    fig.canvas.mpl_connect('scroll_event', call_back_this) #缩放
    
    
    if ctype != 'USB_HVDCP':    
        #^^^^^^^^Button^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
        bt = Buttonprocess()
        #func_button(bt) #局部变量会不起效，要在主函数中运行
        
        h_buttom = 0.95
        
        if pro_use == True:
            pl = plt.axes([0.02,h_buttom,0.05,0.03])
            bpl_disable = Button(pl, 'pl_disable',color = 'palegreen')
            bpl_disable.on_clicked(bt.press_pl_disable)
           
        p2 = plt.axes([0.02,h_buttom-0.03/6-0.03*1,0.05,0.03])
        bFCC = Button(p2, 'FCC',color = 'palegreen')
        bFCC.on_clicked(bt.press_FCC)
        
        bUSB_ICL = func_button(bt) #使用函数，需要把创建的button实例返回
        
        p4 = plt.axes([0.02,h_buttom-0.03/6-0.03*3,0.05,0.03])
        bCP_ILIM= Button(p4, 'CP_ILIM',color = 'palegreen')
        bCP_ILIM.on_clicked(bt.press_CP_ILIM)
        
        p5 = plt.axes([0.02,h_buttom-0.03/6-0.03*4,0.05,0.03])
        bCP_IRQ= Button(p5, 'CP_IRQ',color = 'palegreen')
        bCP_IRQ.on_clicked(bt.press_CP_IRQ)
        
        p6 = plt.axes([0.02,h_buttom-0.03/6-0.03*5,0.05,0.03])
        bCP_status= Button(p6, 'CP_status',color = 'palegreen')
        bCP_status.on_clicked(bt.press_cp_status)
        
        p7 = plt.axes([0.02,h_buttom-0.03/6-0.03*6,0.05,0.03])
        bmain_irq = Button(p7, 'main_irq',color = 'palegreen')
        bmain_irq.on_clicked(bt.press_main_irq)
        

        check = bt.check    #check = CheckButtons(rax, labels, visibility)         #lables显示在按钮中
                            #self.check = check
                        

    
    
    
    #^^^^^^^^^^^^^^^^^^^pl_disable^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
    #                   pl_disable 使用Button
    #…………………………………………pl_disable and stepper_work执行时间点图…………………………………………………………………………………………………………
    if pro_use == True:
        rplot_pl_disable(1) #1.pl_disable
    
    #^^^^^^^^^^^^^^^^^^^^^2.voter图=====不可以check button==============================  
    parse_class4_effective_vote('FCC',0,1,1,marker='*',    linestyle = '-.', markersize = 7,color = 'blue', alpha = 0.5)
    
    #parse_class3_voting_of('FCC',1)
    
    parse_class4_effective_vote('USB_ICL',0,1,1,marker = '+',linestyle = ':' ,markersize = 10,color = 'deeppink', alpha = 0.7)
    
    if ctype != 'USB_HVDCP':
        parse_class4_effective_vote('CP_ILIM',0,0,1,marker = '.',linestyle = '--' ,markersize = 2, alpha = 0.6)
    
    #------pl_notify---执行时间点图-------------------
    #pl_notify()      #3.pl_notify

    
    
    #^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
    #  import cp  使用checkbox
    #^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
    # def find_mid_i(ls):
        # t_mid = ls[0] + (ls[-1]-ls[0])/2
        
        # i = 0
        # for t in ls:
            # if t_mid <= t:
                # return i
            # i += 1
    
    if ctype == 'USB_PD':
        try:
            lt_pdo,lv_pdo = get_pdo_data(text)          #pdo-----------------
            mid = find_t_mid(lt_pdo)
            
            # print('\n\n lt_pdo = ', lt_pdo)
            # print(mid)
            print('pdo_mid = ', mid, lt_pdo[mid], lv_pdo[mid]/10)
            
            lv_pdo = list(map(lambda x:x/10,lv_pdo))
            l0, =  ax.plot(lt_pdo,lv_pdo,  linestyle=':',linewidth = 0.1, marker='o', markersize =1.5, markerfacecolor='r', label = 'rdo', color = 'g')
            ax.text(lt_pdo[mid], lv_pdo[mid]-0.1, 'rdo', horizontalalignment='center', verticalalignment='top', fontsize = 9, alpha = 1, color = 'g')
        except:
            print('pdo str missing ... ')
    
    elif ctype == 'USB_HVDCP_3':
        print('will process qc3 v_bus and voltage adjust message ')
        #画SOC.TXT电压 条件未成熟，在后面画
        
        #画 cp.c调电压
        st = '\[.*?(\d*\.\d*)\].*?HVDCP3:\s(..)_pulse\svbefore_mv=(\d*),\svafter_mv=(\d*)'
        res = re.findall(st,text)
        #print(res)
        for t,dp_dm,b,a in res:
            t = float(t)
            b = float(b)/1000
            a = float(a)/1000
            # ax.plot(t,b,'')
            #print(t,b,a,dp_dm)
            if dp_dm == 'dp':
                #画上升的箭头
                #ax.plot(t,b,marker = '^', alpha = 0.01)
                ax.arrow(t,b,0,a-b, length_includes_head = True, shape ='right',head_width=4.5, head_length=0.1, fc='r', ec='r', width = 0.1)
            else :
                #ax.plot(t,b, marker = 'v', alpha = 0.01)
                ax.arrow(t,b,0,a-b, length_includes_head = True, shape ='right',head_width=4.5, head_length=0.1, fc='b', ec='b', width = 0.05)
                #ax.arrow(t,b,0,a-b, length_includes_head = True, shape ='right',head_width=6, head_length=0.1, fc='orange', ec='b', width = 0.1)

        
    if ctype != 'USB_HVDCP':        
        #^^^^^^^^^^^^^^^^get_Properties_status2_data^^^^^大量数据反回，不适宜用函数反回^^^^^^^^^^^^^^^^^^^^
        ltt = []
        lt_main_hot = []
        lt_cp_hot = []
        
        lt_main = []
        lt_cp = []
        
        lt_enable = []
        lt_sw_en = []

        lt_main_cp = []
        
        lt_state = []
        get_Properties_status2_data(text,ltt,lt_main_hot,lt_cp_hot,lt_main,lt_cp,lt_enable,lt_sw_en,lt_main_cp, lt_state)
        
       
        t_mid_i = find_t_mid(ltt)
        print('\n t_mid_i = ', t_mid_i)
        #print(ltt)
        
        #main温度
        lt_main = list(map(lambda x:x/10,lt_main))
        l1, = ax.plot(ltt,lt_main,color = 'r', linewidth = 0.4, label = 'T_main')                                                
        ax.text(ltt[t_mid_i], lt_main[t_mid_i], 'T_Main', horizontalalignment='center', verticalalignment='center', fontsize = 7, alpha = 1, color = 'r')
        
        #cp温度 
        lt_cp = list(map(lambda x:x/10,lt_cp))
        l2, = ax.plot(ltt,lt_cp,color ='orange', linewidth = 0.3,   label = 'T_cp')                                                    
        ax.text(ltt[t_mid_i], lt_cp[t_mid_i], 'T_cp', horizontalalignment='center', verticalalignment='center', fontsize = 7, alpha = 1, color = 'orange')

        
        #main_hot布尔值 
        lt_main_hot = list(map(lambda x:((x+2)/5/2 + 5.8), lt_main_hot))
        t_main_hot,v_main_hot = conv_square_wave(ltt,lt_main_hot)
        l3, = ax.plot(t_main_hot,v_main_hot,linewidth = 0.4,linestyle=':', label='M_hot', color = 'r')   
        ax.text(ltt[0], 5.85, 'Main_HOT_bool', horizontalalignment='right', verticalalignment='center', fontsize = 7, alpha = 0.99,color = 'r')

        #cp_hot布尔值
        lt_cp_hot = list(map(lambda x:((x+4)/5/2 + 5.6), lt_cp_hot))
        t_cp_hot,v_cp_hot = conv_square_wave(ltt,lt_cp_hot)
        l4, = ax.plot(t_cp_hot,v_cp_hot,linewidth = 0.4,linestyle=':', label = 'cp_hot', color = 'orange')    
        ax.text(ltt[0], 5.4, 'CP_HOT_bool', horizontalalignment='right', verticalalignment='center', fontsize = 7, alpha = 0.99, color = 'orange')
     
     
        #t_main - t_cp
        lt_main_cp = list(map(lambda x:x/10 + 6, lt_main_cp))
        l5, = ax.plot(ltt, lt_main_cp, linewidth = 0.5,color = 'gray',linestyle = '-.', label = 'Tm-cp')                #T_main - T_cp
        if pro_use == False:
            l5.set_visible(0)
        
        # smb_en
        lt_enable = list(map(lambda x:(x-20)/4/3  ,lt_enable))
        t_smb_en,v_smb_en = conv_square_wave(ltt,lt_enable)
        l6, = ax.plot(t_smb_en, v_smb_en, linewidth = 0.5, marker ='.',markersize='1', label = 'smb_en', color = 'orange' )  #bit7 = 1, smb_en 高电平
        ax.text(ltt[t_mid_i], lt_enable[t_mid_i], 'SMB_EN', horizontalalignment='center', verticalalignment='bottom', fontsize = 10, alpha = 0.99, color = 'orange')
        
        #sw_en
        lt_sw_en = list( map(lambda x:(x-15)/4/2 + 7, lt_sw_en) )             #bit0  Switcher_hold_off: 1 hold_off  bit7=1,bit0=0
        t_switcher,v_switcher =  conv_square_wave(ltt,lt_sw_en)
        l7, = ax.plot(t_switcher,v_switcher,color = 'c',linewidth = 0.5, marker='.',markersize='1', label = 'sw_en')    #bit7 & bit0
        ax.text(ltt[0], 7, 'sw_en', horizontalalignment='right', verticalalignment='bottom', fontsize = 10, alpha = 0.99,color = 'c')
        
        # v_switcher  = map(lambda x:x+1,v_switcher)
        # l8, = ax.plot([200,300],[9,9.5],color = 'r',linewidth = 0.5, marker='.',markersize='1', label = 'sw_en2') 
        # l8.set_visible(0)
    
    # ^^^soc.txt^^^^^^^^^^^^^^^^^^^

    
    # datas = []
    # cl =['Uptime']
    # n = 0
    # li = []
    # get_data(n,li,cl,datas)
    
    #df = pd.DataFrame(data=datas,columns=cl)
    
    mid_index = find_t_mid_index(df)
    print('soc_mid_index = ', mid_index)
    

    
    #V_bus = df['usbVoltage']
    
    time = df['Uptime']
    #l8, = ax.plot(time,V_bus/1000000,'b',linewidth = 3.0, label = 'V_bus')
    
    
    # V_bus = df['usbVoltage']
    # ax.plot(time,V_bus/100,'-.',color='yellowgreen',linewidth = '2',  alpha = 1, label = 'V_bus')
    
    if ctype == 'USB_HVDCP_3' or ctype == 'USB_HVDCP':
        try:
            print('QC3 draw vbus !!!!!!!!!!!!!!!')
            V_bus = df['usbVoltage']
            
            ax.plot(time,V_bus/1000000,'-.',color='yellowgreen',linewidth = '2',  alpha = 1, label = 'V_bus')
            ax.text(df['Uptime'][mid_index], df['usbVoltage'][mid_index]/1000000, 'V_bus', horizontalalignment='center', verticalalignment='bottom', fontsize = 9, alpha = 0.99, color = 'yellowgreen')
        except:
            print('Missing usbVoltage')

    try:
        T_batt  = df['batteryTemp']
        la, = ax.plot(time,T_batt/100,'-.',color='r',linewidth = '0.5',  alpha = 0.5, label = 'T_batt')
        #print(df['Uptime'][mid_index], df['batteryTemp'][mid_index]/100)
        ax.text(df['Uptime'][mid_index], df['batteryTemp'][mid_index]/100, 'T_batt', horizontalalignment='center', verticalalignment='center', fontsize = 9, alpha = 0.99, color = 'r')
    except:
        print('Missing batteryTemp')
        
    try:
        I_batt  = df['batteryCurrent']
        if df['batteryCurrent'].mean() < 0:
            I_batt = -I_batt   
        lb, = ax.plot(time,I_batt/1000000,'g',linewidth = 1.0, label = 'I_batt', alpha = 0.6)
        ax.text(df['Uptime'][mid_index], df['batteryCurrent'][mid_index]/1000000, 'I_batt', horizontalalignment='center', verticalalignment='center', fontsize = 9, alpha = 0.99, color = 'g')
    except:
        print('Missing batteryCurrent')
        
    try:
        I_bus = df['usbCurrent']
        #lc, = ax.plot(time,I_bus/1000000,'--', color='aquamarine', linewidth = 0.6, alpha = 0.8, label = 'I_bus')
        lc, = ax.plot(time,I_bus/1000000,'--', color='b', linewidth = 0.6, alpha = 0.8, label = 'I_bus')
        ax.text(df['Uptime'][mid_index], df['usbCurrent'][mid_index]/1000000, 'I_bus', horizontalalignment='center', verticalalignment='bottom', fontsize = 9, alpha = 0.99, color = 'b')
    except:
        print('Missing usbCurrent')
    
    if ctype != 'USB_HVDCP':
        try:
            I_cp = df['CPCurrent']
            ld, = ax.plot(time, I_cp/1000000,color = '#ADFF2F',linewidth= 0.6, label = 'I_cp')
            ax.text(df['Uptime'][mid_index], df['CPCurrent'][mid_index]/1000000, 'I_cp', horizontalalignment='center', verticalalignment='bottom', fontsize = 9, alpha = 0.99, color = '#ADFF2F')
        except:
            print('Missing CPCurrent')  
    
    if ctype == 'USB_HVDCP':
        try:
            T_main = df['chargerTemp']/100
            le, = ax.plot(time, T_main,color = 'r',linewidth= 0.6, label = 'T_main')
            
            ax.text(df['Uptime'][mid_index], df['chargerTemp'][mid_index]/100, 'T_main', horizontalalignment='center', verticalalignment='bottom', fontsize = 9, alpha = 0.99, color = 'r')
            print(df['Uptime'][mid_index], df['chargerTemp'][mid_index],'=================')
        except:
            print('Missing CPCurrent')  
    
    # print('soc time in main')
    # print(time)
    #==========================================================
    
    #^^^^^^check button^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
    if ctype == 'USB_PD':
        try:
            glines = [l0, l1, l2, l3, l4, l5, l6, l7,la,lb,lc,ld]
            rax = plt.axes([0.02,0.9-0.03*16-0.03*len(glines),0.07,0.03*len(glines)])
        except:
            glines = [l0, l1, l2, l3, l4, l5, l6, l7,la,lb,lc]
            rax = plt.axes([0.02,0.9-0.03*16-0.03*len(glines),0.07,0.03*len(glines)])
            
    elif ctype == 'USB_HVDCP_3':
        glines = [l1, l2, l3, l4, l5, l6, l7,la,lb,lc,ld]
        rax = plt.axes([0.02,0.9-0.03*16-0.03*len(glines)-0.1,0.07,0.03*len(glines)])
    
    elif ctype == 'USB_HVDCP':
        try:
            glines = [la,lb,lc,le]
            rax = plt.axes([0.02,0.9-0.03*16-0.03*len(glines)-0.1,0.07,0.03*len(glines)])
        except:
            glines = [la,lb,lc]
            rax = plt.axes([0.02,0.9-0.03*16-0.03*len(glines)-0.1,0.07,0.03*len(glines)])
        
    labels =    [str(line.get_label()) for line in glines] #labels列表
    visibility = [ line.get_visible()  for line in glines] #visibility列表
    check = CheckButtons(rax, labels, visibility)         #lables显示在按钮中
    
    def func(label):                                      #按中某个按钮会返回它
        index = labels.index(label)                       #通过label找到line
        glines[index].set_visible(not glines[index].get_visible())
        #ax.set_ylim(0,10)
        plt.draw()
    check.on_clicked(func)
    
        
    #^^^^^^textbox^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^
    def x_submit(text):
        ls = text.split(' ')
        ls = list(map(float, ls))
        xmin = ls[0]
        xmax = ls[1]
        ax.set_xlim(xmin, xmax)
        plt.draw()
        
    pxbox = plt.axes([0.095,0.06,0.07,0.02])
    x_box = TextBox(pxbox,'x')
    x_box.on_submit(x_submit)
    
    # y ----------------------------
    def y_submit(text):
        ls = text.split(' ')
        ls = list(map(float, ls))
        ymin = ls[0]
        ymax = ls[1]
        ax.set_ylim(ymin, ymax)
        plt.draw()
    
    pybox = plt.axes([0.095,0.96,0.07,0.02])
    y_box = TextBox(pybox,'y')
    y_box.on_submit(y_submit)
    
    #^^^^^^cp irq^^^^^irq^^^^^^^^^^^^^^^^^^^^^^^^^^^^   
    # 电压 蓝 .
    # 电流 绿 x
    # 温度 红 ‘2’ 上三叉
    
    #sw disable 低0.5 
    
    off_w_0 = []; uv_ov_1 = []; TSD_2 = []; TREV_3 = []
    VPH_OV_HARD_4 = []; VPH_OV_SOFT_5 = []; ILIM_6 = []; TEMP_ALARM_7 = []
    if ctype != 'USB_HVDCP':
        get_cp_irq_data()
        plot_cp_irq()  

        plot_cp_status()
        plot_main_irq()
    else :
        qc2_plot()
    
    
    ############## span ##########################
    def onselect(xmin, xmax):
        if xmax - xmin < 0.3 :
            return
            
        # fig3 = plt.figure() #每次选择范围，新建一个图，并把key_span重新的fig3重新设置，这样update为新fig图
        # ax3 = plt.subplot(111)
        print('onselect  : ',xmin, xmax)
        
        
        subprocess.Popen(['python',sys.argv[0], sys.argv[1], str(xmin), str(xmax)])
            

    span = SpanSelector(ax, onselect, 'horizontal', useblit=True,
                    rectprops=dict(alpha=0.5, facecolor='red')) #SpanSelector从 ax 给 onselect返回 xmin，xmax两个参数   
    
    if len(sys.argv) >=4  and float(sys.argv[2]) != 0 and float(sys.argv[3]) != 0 :
        l = float(sys.argv[3]) - float(sys.argv[2])
        min_x = float(sys.argv[2]) - 0.1/(1-0.1)*l
        print('min_x = ',min_x)
        ax.set_xlim(min_x, float(sys.argv[3]))  #数据都已经按时间截取，纯粹起始时间设置显示
        plt.draw()
        
    #^^^soc.txt^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^    
    
    
    plt.show()
    

