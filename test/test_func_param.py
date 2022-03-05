def foo(a, m, n = 2, *args, **kwargs):
    x = 5
    print a
    print m
    print n
    print "args:"
    for p in args:
        print p
    print "kwargs:"
    for k, v in kwargs.items():
        print k, v
    print "finished"

t = 10
foo(1, 2, 3, 4, 5, 6, b = 2, c = 3)

print 
foo(1, 2)

print
foo(1, 2, n = 3)


def foo2(a, b):
    print a
    print b

print
foo2(1, 2)

def foo3(a, *args):
    print a

print
foo3(1)

def foo4(*args):
    pass

def foo5(*args, **kwargs):
    pass

