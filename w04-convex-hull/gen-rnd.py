#!/usr/bin/python3

import random

n = random.randint(10,40)

for i in range (1,n) :
    while (True) :
        x = random.random() * 9.9 - 5
        y = random.random() * 9.9 - 5
        if (x*x + y*y < 25) :
            break
    print ('%3.1f ' % (x + 5) , '%3.1f' % (y + 5))
