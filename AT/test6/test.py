import os
import os.path
import time
import json


reference_file = "./expect.json"
output_file = "./clone_code.json"
extra_info = ""


def test():
    cmd = "./clone_detect src/ -n 10 -bcj clone_code.json"
    os.system(cmd)

    output = read_json_file(output_file)
    ref = read_json_file(reference_file)

    if cmp_json(output, ref):
        print "PASS" + extra_info
    else:
        print "FAILED"


def read_json_file(json_file):
    with open(json_file, 'r') as f:
        result = json.load(f)
    return result


def cmp_json(output, ref):
    if len(output) != len(ref):
        return False

    if type(output) != type(ref):
        return False

    if isinstance(output, list):
        return cmp_list(output, ref)
    else:
        return cmp_dict(output, ref)


def cmp_list(output, ref):
    if len(output) != len(ref):
        return False

    output.sort()
    ref.sort()

    for i, data in enumerate(ref):
        if isinstance(data, dict):
            if not cmp_dict(data, output[i]):
                return False
        else:
            if data != output[i]:
                print '"', output[i], '" is not expected', '"', data, '"'
                return False

    return True


def cmp_dict(output, ref):
    global extra_info

    if len(output) != len(ref):
        return False

    for key, value in ref.items():
        if isinstance(value, list):
            if not cmp_list(value, output[key]):
                return False
        else:
            if value != output[key]:
                print 'key "%s" "'%key, output[key], '" is not matched to expected', '"', value, '"'
                if key == 'totalDuplicate' or key == 'duplicate':
                    return False
                else:
                    extra_info = ", but the results aren't entirely as expected!"
    return True


if __name__ == '__main__':
    test()
