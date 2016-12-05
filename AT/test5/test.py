import os
import json
import time

def test():
    cmd = "./clone_detect src/ -bcj clone_code.json"
#    print cmd
    os.system(cmd)

    test = open('clone_code.json')
    expect = open('expect.json')

    test_op = json.load(test)
    expect_op = json.load(expect)

    print 'PASS' if cmp_json(expect_op, test_op) else 'FAILED'


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
