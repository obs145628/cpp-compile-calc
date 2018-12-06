import os
import subprocess

SRC_FILE = "../src/input.txt"

TESTS = [
    '12',
    '4+6',
    '12-3',
    '4*5',
    '12/5',
    '19+6-3',
    '2*(3+4-((12)))',
    '12+3*8+15/3-3-4*5*2+7+8+12-5-6+120/2/5',
    '1' + ('+1') * 100
]



def run_calc_compile(s):
    s = ("\"{}\"\n").format(s)
    with open(SRC_FILE, 'w') as f:
        f.write(s)
    os.system("make > /dev/null")
    res = subprocess.run(["./calc"], stdout=subprocess.PIPE)
    return int(res.stdout.decode('ascii'))

def run_calc_python(s):
    res = eval(s)
    return int(res)


ntests = 0
nsuccs = 0

def run_test(calc):
    global ntests
    global nsuccs
    
    out = run_calc_compile(calc)
    ref = run_calc_python(calc)

    valid = out == ref
    ntests += 1
    if valid: nsuccs += 1
    
    print('[{}]: {}'.format(calc, 'OK' if valid else 'KO'))
    
for t in TESTS:
    run_test(t)

print('Total: {} / {} ({}%)'.format(ntests, nsuccs, nsuccs / ntests * 100))
