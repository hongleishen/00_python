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


def get_data(t, datas):
    data_dir = 'tmp.txt'
    richang = []

    with open(data_dir, encoding='utf-8') as f:

        # 处理行
        for line in f.readlines():
            print("source line" + line)
            line_source = []
            line_msg = []
            ldata = []

            if (is_number(line)):
                # print("is number" + line)
                t.append(float(line))
                ldata.append(float(line))
            else:
                line = line.strip()
                line_source = line.split(' ')
                for item in line_source:
                    if item != '':
                        line_msg.append(item)
                print(line_msg)

                # 处理 line_msg
                #if ditem.get(line_msg[0] == None):


def get_data2():
    ldata = []
    #datas = []

    d_consume = {}
    d_extera = {}
    douyin = 0

    # lines = []
    fin = open("tmp.txt", encoding='utf-8')
    input_file = fin.read()
    #print(input_file)
    in_lines = input_file.split("\n")
    print(in_lines)

    line_flag = 0
    i = 0
    next = 0
    for str in in_lines:
        str.strip()
        line_source = []
        line_msg = []

        # print(str)

        if (is_number(str)):
            if (line_flag == 0):
                print('line_flag = ', line_flag)
                ldata.append(float(str))
                line_flag = 1

            elif (line_flag == 1):
                ldata.append(d_consume)
                d_consume = {}

                ldata.append(d_extera)
                d_extera = {}

                ldata.append(douyin)
                douyin = 0

                datas.append(ldata)
                ldata = []
                # ----new line--------------------------
                ldata.append(float(str))

        else:
            line_source = str.split(' ')
            for item in line_source:
                if item != '':
                    line_msg.append(item)
            print(line_msg)

            if ditem.get(line_msg[0]) == None:
                d_consume[line_msg[0]] = line_msg[1:]
                print("d_consume = ", d_consume)
            elif line_msg[0] == 'extera':
                d_extera[line_msg[1]] = line_msg[2:]
                print("d_extera = ", d_extera)
            elif line_msg[0] == 'douyin':
                douyin = line_msg[1]


        print('=========\n\n')

    # 添加最后一行
    ldata.append(d_consume)
    d_consume = {}

    ldata.append(d_extera)
    d_extera = {}

    ldata.append(douyin)
    douyin = 0

    # 最后一行数据 添加到 datas
    datas.append(ldata)
    ldata = []

    for item in datas:
        print(item)


if __name__ == '__main__':
    import numpy as np
    import pandas as pd
    import matplotlib.pyplot as plt
    import sys
    # %matplotlib inline
    from matplotlib.ticker import MultipleLocator, FormatStrFormatter

    ditem = {'extera': 10, 'fixed': 11, 'douyin': 12}
    #print(ditem)
    time = []
    datas = []
    #get_data(time, datas)
    #df = pd.DataFrame(data=datas, columns=time)
    #print(time)
    get_data2()
    #print(datas)