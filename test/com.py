#! /usr/bin/env python
#coding=utf-8
# 串口接收程序（模拟短信报警模块的接收端）

import pty
import os
import select

def mkpty():
    #
    master1, slave = pty.openpty()
    slaveName1 = os.ttyname(slave)
    master2, slave = pty.openpty()
    slaveName2 = os.ttyname(slave)
    print '/nslave device names: ', slaveName1, slaveName2
    return master1, master2

if __name__ == "__main__":

    master1, master2 = mkpty()
    while True:        
        rl, wl, el = select.select([master1,master2], [], [], 1)
        for master in rl:
            data = os.read(master, 128)
            if master==master1:
                print "read %d data:" % len(data)
                print data
                #os.write(master2, data)
            else:
                print "to write %d data:" % len(data)
                print data.strip()
                os.write(master1, data.strip())
