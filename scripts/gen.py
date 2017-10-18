from random import randint
import sys

total_weight = 0
for i in xrange(100000):
    value = randint(1, 100)
    weight = randint(1, 3000)
    total_weight += weight
    print '%d %d' % (value, weight)
sys.stderr.write('%d\n' % randint(10000, total_weight))