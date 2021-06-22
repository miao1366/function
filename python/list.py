import subprocess;
import os;
import sys;
import time;
import re;
import platform;

def shell(cmd):
    #print(cmd)
    pi = subprocess.Popen(cmd, shell=True, stdout=subprocess.PIPE)
    return pi.stdout.read().decode()#打印结果

def check_timesync(max_diff=500*1E6):
    cmd = 'chronyc sources -v | grep ^^'
    ntp_s = shell(cmd)
    cmd = 'chronyc sources -v | grep ^#'
    gps_s = shell(cmd)
    
    time_source = ntp_s.splitlines() + gps_s.splitlines()
    
    stamp = []
    source = []
    print(time_source)
    for tmp in time_source:
        print(tmp)
        if tmp[1] != "GPS":
            # a, b = parse_stamp(tmp)
            # stamp.append(a)
            # source.append(b)
            print("")
        else:
            if tmp[0] == '#*':
                # a, b = parse_stamp(tmp)
                # stamp.append(a)
                # source.append(b)
                print("")

    if len(stamp) > 1:
        return 0, source
        '''
        v2 = max(stamp)
        v1 = min(stamp)
        if v2 - v1 > max_diff:
            ERROR('max - min > 0.2s')
            return -1, source
        '''
    else:
        return -1, source

    return 0, source

if __name__ == '__main__':
    print(sys.path[0])
    check_timesync()
