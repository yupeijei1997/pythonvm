class A(object):
    def __init__(self, v):
        self.value = v

a = A(1)

lst = []
lst.append(2)
print lst

a.foo = lst.append
a.foo(3)
print lst
