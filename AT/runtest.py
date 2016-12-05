import os
import sys
import platform

from test1.test import test as test1
from test2.test import test as test2
from test3.test import test as test3
from test4.test import test as test4
from test5.test import test as test5
from test6.test import test as test6

def is_windows():
    return 'Windows' in platform.system()

def is_linux():
    return 'Linux' in platform.system()


if is_windows():
    cmd = 'clone_detect.bat'
else:
    cmd = 'clone_detect'

def run_test(target_dir, test_case):
    copy_file(cmd, target_dir+'/'+cmd)
    cwd = os.getcwd()
    os.chdir(target_dir)
    print "\t%s\t\t\t" % target_dir,
    test_case()
    os.chdir(cwd)

def copy_file(source, target):
    if os.path.isfile(source):
        if not os.path.exists(target) or \
           (os.path.exists(target) and \
            (os.path.getsize(target) != os.path.getsize(source))):  
            open(target, "wb").write(open(source, "rb").read()) 
            import stat
            os.chmod(target, stat.S_IRWXG+stat.S_IRWXU+stat.S_IRWXO)
    else:
        raise Exception("Can't find clone_detect")


if __name__ == '__main__':
    tests = [0, test1, test2, test3, test4, test5, test6]

    print 'Test:'

    if len(sys.argv) is 2:
        index = sys.argv[1]
        if index.isdigit():
            test = 'test%s' % index
            run_test(test, tests[int(index)])
        sys.exit()

    for i in range(1, len(tests)):
        test = 'test%d' % i
        if not isinstance(tests[i], int):
            run_test(test, tests[i])

