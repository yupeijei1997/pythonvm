class A(object):
    def __init__(self, v):
        self.value = v

a = A(1)
b = A(2)

def func(self, s):
    print self.value
    print s

a.bar = func
A.bar = func

a.bar(a, "hello")
b.bar("world")
