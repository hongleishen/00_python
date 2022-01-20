
def read_dir(dir):
    #print(os.sep)  # 显示当前平台下文件路径分隔符
    # fileDir = "E:" + os.sep + "test"
    #fileDir = os.sep.join(["E:","test"])  # 以分隔符连接路径名

    text = ''
    for root, dirs, files in os.walk(dir):  
        # print('the path is ...')
        print(root)  
        # print('the current directories under current directory :')
        # print(dirs)  
        # print('the files in current directory :')
        # print(files)
        # print('')

        for file in files:
            d = root + '/' + file
            fin = open(d, encoding="utf-8")
            input_file = fin.read()
            fin.close()
            print(input_file)
            text += input_file + '\n'

    return text




if __name__ == '__main__':
    import sys
    import os


    if len(sys.argv) >= 2:
        dir = sys.argv[1]
    else:
        dir = '.\\'

    read_lines = read_dir(dir)
    print('read_lines = ')
    print(read_lines)