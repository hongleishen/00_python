if __name__ == '__main__':
    import numpy as np
    import sys
    import os

    #fin = open("2.c", encoding="utf-8")
    #input_file = fin.read()
    #fin.close()

    if len(sys.argv) >= 2:
        dir = sys.argv[1]
    else:
        dir = '.\\'


    print(len(sys.argv))
    print(sys.argv)
    str = sys.argv[1] + '.dotuml'
    str = str.replace('./', '')
    str = str.replace('/', '_')

    str = str.replace('.\\', '')
    str = str.replace('\\', '_')

    print(str)