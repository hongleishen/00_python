def get_pl_disable_log(dir):
    #with open('dmesg.txt') as f:
    #text
    # if len(sys.argv) == 1:
    # dir = 'dmesg.txt'
    # else:
    # dir = get_dir()
    print('====================get_pl_disable_log, dir = ', dir)
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

        # 以行尾单位处理
        for line in f.readlines():

            #--stepper_work---------------------
            r_stepper = re.findall(
                '\[.*?(\d+.\d+)\]\sQCOM-BATT:\sfcc_stepper_work:\sRescheduling\sFCC_STEPPER\swork',
                line)
            if r_stepper != []:
                stepper_work.append(float(r_stepper[0]))

            n_gcp = 0
            if end == 1:
                #start time
                str = '\[.*?(\d+.\d+)\]\sQCOM-BATT:\spl_disable_vote_callback:\sFCC Stepper'
                r_start = re.findall(str, line)
                #print("r_start = ", r_start, "\n")

                if r_start != []:
                    start = 1
                    end = 0
                    #ls.append(float(r_start[0]))
                    t_start = float(r_start[0])
                    print("t_start = ", t_start, "\n\n")

            if start == 1:
                ####sw on ###   chip->fcc_stepper_enable###################################################

                #get_fcc
                str = '\[.*?(\d+.\d+)\]\s.*get_charge_param:\sfast\scharge\scurrent\s=\s(\d+)'
                get_charge_param = re.findall(str, line)                                # is a list, [('87.567667', '3000000')]
                if get_charge_param != []:
                    print("get_charge_param = ", get_charge_param)
                    n_gcp += 1
                    get_fcc = float(int(get_charge_param[0][1])) / 1000000
                    #print(get_charge_param[0])
                    #print(get_charge_param[0][0])
                    #print(get_charge_param[0][1])

                #fcc_stepper_parameters
                str = '\[.*?(\d+.\d+)\]\s.*main_step_direction:\s(\-?\d),\smain_step_count:\s(\d+),\smain_residual_fcc:\s(\d+)'
                fcc_stepper_parameters = re.findall(str, line)
                if fcc_stepper_parameters != []:
                    print("fcc_stepper_parameters = ", fcc_stepper_parameters)
                    sw_en = 1
                    dirc = int(fcc_stepper_parameters[0][1])
                    steps = int(fcc_stepper_parameters[0][2])
                    residual = float(
                        int(fcc_stepper_parameters[0][3]) / 1000000)

                #CP_ILIM vote

                ############# sw off ##### !chip->fcc_stepper_enable ###########################################

                #set fcc
                str = '\[.*?(\d+.\d+)\]\s.*smblib_set_charge_param:\sfast\scharge\scurrent\s=\s(\d+)'
                r_usb_icl = re.findall(str, line)
                if r_usb_icl != []:
                    # if n_gcp != 0:
                    # del ls[1]
                    usb_icl = float(int(r_usb_icl[0][1]) / 1000000)
                    #ls.append(usb_icl)
                    #ls.insert(1,usb_icl)
                    set_fcc = usb_icl

                #CP_ILIM vote FCC_VOTER
                #class2  class3 可以去掉最后画图测试
                class2_ilim = re.findall(
                    'CP_ILIM:\sFCC_VOTER,0\ssame\svote\son\sof\sval=(\d+)',
                    line)
                if class2_ilim != []:
                    #ls.append(float(int(class2_ilim[0]/1000000)))
                    #ls.insert(2,float(int(class2_ilim[0]/1000000)))
                    cp_ilim = float(int(class2_ilim[0]) / 1000000)

                str = '\[.*?(\d+.\d+)\]\s.*sCP_ILIM:\sFCC_VOTER,0\svoting\son\sof\sval=(\d+)'
                class3_ilim = re.findall(str, line)
                if class3_ilim != []:
                    #ls.append(float(int(class3_ilim[0][1])/1000000))
                    #ls.insert(2,float(int(class3_ilim[0][1])/1000000))
                    cp_ilim = float(int(class3_ilim[0][1]) / 1000000)

                class4_ilim = re.findall(
                    'CP_ILIM:\seffective\svote\sis\snow\s(\d+)', line)
                if class4_ilim != []:
                    effec = 1
                    # ls.append(float(int(class4_ilim[0])/1000000))
                    #ls.insert(2,float(int(class4_ilim[0])/1000000))
                    cp_ilim = float(int(class4_ilim[0]) / 1000000)

                #end time
                str = '\[.*?(\d+.\d+)\]\sQCOM-BATT:\spl_disable_vote_callback:\sparallel\scharging'
                r_end = re.findall(str, line)
                if r_end != []:
                    t_end = float(r_end[0])

                    if sw_en == 1:
                        ls = [t_start, get_fcc, dirc, steps, residual, t_end]
                        sw_on.append(ls)
                    else:
                        ls = [t_start, set_fcc, cp_ilim, t_end, effec]
                        sw_off.append(ls)

                    start = 0
                    end = 1
                    sw_en = 0
                    effec = 0
                    ls = []
        
            
    f.close()
    return sw_on, sw_off, stepper_work


def plot_pl_disable(sw_off, sw_on, stepper_work):
    #---- sw off ---------------
    for a in sw_off:
        ax.plot(a[0], a[1], '.')
        ax.plot(a[0], a[2], '.')
        #plt.vlines(a[0], a[1], a[2], linestyle=':', linewidth=0.5, alpha=0.4)

    
    #---- sw_on ----------------------
    #   0          1     2      3       4       5
    # [t_start, get_fcc, dirc, steps, residual, t_end]
    for a in sw_on:
        if a[2] > 0:
            ax.plot(a[0], a[1], '2', alpha=0.6, color='sienna')
            ax.vlines(a[0],
                      a[1],
                      a[1] + 0.1 * a[3] + a[4],
                      linewidth=0.3,
                      color='goldenrod',
                      alpha=0.99)
        
        else:
            ax.plot(a[0], a[1], '1', alpha=0.5, color='cyan')
            ax.vlines(a[0],
                      a[1] - 0.1 * a[3] - a[4],
                      a[1],
                      linewidth=0.4,
                      color='g',
                      alpha=0.8)
        

    #-----stepper_work------
    for t in stepper_work:
        ax.plot(t, 4.9, '.', markersize='3', alpha=0.6, color='g')


#------pl_notify----------------------
def pl_notify():
    r_pl_notify_c4 = re.findall(
        '\[.*?(\d+.\d+)\]\sPL_ENABLE_INDIRECT:\seffective\svote\sis\snow\s1\svoted\sby\sUSBIN_I_VOTER,0',
        text)

    #有两三个点的干扰
    # static int usb_icl_vote_callback(struct votable *votable, void *data,
    # if (icl_ua <= 1400000)     //2. pl_siable
    # vote(chip->pl_enable_votable_indirect, USBIN_I_VOTER, false, 0);
    r_pl_notify_c2 = re.findall(
        '\[.*?(\d+.\d+)\]\sPL_ENABLE_INDIRECT:\sUSBIN_I_VOTER,0\sIgnoring\ssimilar\svote\s',
        text)

    pl_c4 = list(map(float, r_pl_notify_c4))
    pl_c3 = list(map(float, r_pl_notify_c2))
    # print(pl_c4)
    # print('*'*50)
    # print(pl_c3)
    for t in pl_c4:
        ax.plot(t, 4.7, '.', markersize='4', alpha=0.99, color='r')

    for t in pl_c3:
        ax.plot(t, 4.8, '.', markersize='3', alpha=0.6, color='k')


def rplot_pl_disable(isplot):
    global gdir
    gdir = 'learn_dmesg.txt'
    if isplot == 0:
        return
    sw_on, sw_off, stepper_work = get_pl_disable_log(gdir)
    print("sw_on = ", sw_on, "\n\n")
    print("sw_off = ", sw_off, "\n\n")
    print("stepper_work = ", stepper_work, "\n\n")

    plot_pl_disable(sw_off, sw_on, stepper_work)

    #------pl_notify---执行时间点图-------------------
    pl_notify()      #3.pl_notify


# 入口参数:
# 1.名字
# 2.地址 可选；  如果不输入， 默认为本文件
if __name__ == '__main__':
    import numpy as np
    import pandas as pd
    import matplotlib.pyplot as plt
    import sys
    # %matplotlib inline
    from matplotlib.ticker import MultipleLocator, FormatStrFormatter
    import re

    f = open('./learn_dmesg.txt')  #soc没有超过100
    text = f.read()
    f.close()

    fig = plt.figure(figsize=(14, 7))
    ax = plt.subplot(111)

    rplot_pl_disable(1)
    plt.show()
