import os

files = ['clone_detect', 'clone_detect.dat', 'clone_code.json', 'clone_code.result', '*.pyc']
paths = ['test%s' % i for i in range(1, 7)]
for path in paths:
    for file in files:
        fullpath = path+'/'+file
        if os.path.exists(fullpath) or file.startswith('*'):
            os.system("rm %s" % fullpath)
