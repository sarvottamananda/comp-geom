#!/usr/bin/python3

import random

print('p%d.in' % random.randint(1,3))

for i in range (0,10) :
    x = random.random() * 5 - 2
    y = random.random() * 5 - 2
    print('%5.2f' % (x), '%5.2f' % (y))


