class A(object):
    def __init__(self, v):
        self.value = v

    def __len__(self):
        print "len() called"
        return len(self.value)

a = A("hello")
print len(a)
