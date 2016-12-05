import os
import sys
import json
import time

def test():
    cmd = "./clone_detect src/ -c > clone_code.result"
#    print cmd
    os.system(cmd)
    test = get_duplicate('clone_code.result')
    expect = get_duplicate('expect.txt')

    if not test.has_key('totalDuplicate'):
        print "can't find totalDuplicate in cmd output"
        sys.exit()
    print 'PASS' if  test['totalDuplicate'] == expect['totalDuplicate'] else 'FAILED'

def get_duplicate(filename):
    with open(filename, 'r') as result:
        file_op = {}
        dup = result.read().strip().split(':')
        if len(dup) is not 2:
            print "cmd output format error!"
            sys.exit()
        file_op[dup[0]] = int(dup[1])
    return file_op

def cmp_json(expect, test):
    if len(expect) != len(test):
        print 'json file format not expected'
        return False
    for key, value in expect.items():
        if isinstance(value, list):
            if not cmp_list(value, test[key]):
                return False
        else:
            if value != test[key]:
                print '%s:%s diffe with expected %s' %(key, test[key], value)
                return False
    return True

def cmp_list(expect, test):
    expect.sort()
    test.sort()

    if len(expect) != len(test):
        print 'Found: %s, but diff with expected %s' %(len(expect), len(test))
        return False

    for i, data in enumerate(expect):
        if not cmp_slide(data, test[i]):
            return False
    return True

def cmp_slide(expect, test):
    if len(expect) != len(test):
        return False

    for key, value in expect.items():
        if value != test[key]:
            print '%s:%s diff with expected %s' %(key, test[key], value)
            return False
    return True

if __name__ == '__main__':
    test()
