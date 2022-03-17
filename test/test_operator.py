class A(object):
    def __init__(self, v):
        self.value = v

    def __add__(self, a):
        print "executing operator +"
        return A(self.value + a.value)

    def __sub__(self, a):
        print "executing operator -"
        return A(self.value - a.value)

    def __mul__(self, a):
        print "executing operator *"
        return A(self.value * a.value)

    def __div__(self, a):
        print "executing operator /"
        return A(self.value / a.value)

    def __mod__(self, a):
        print "executing operator %"
        return A(self.value % a.value)

    def __gt__(self, a):
        print "executing operator >"
        return self.value > a.value

    def __lt__(self, a):
        print "executing operator <"
        return self.value < a.value

    def __eq__(self, a):
        print "executing operator =="
        return self.value == a.value

    def __ne__(self, a):
        print "executing operator !="
        return self.value != a.value

    def __ge__(self, a):
        print "executing operator >="
        return self.value >= a.value

    def __le__(self, a):
        print "executing operator <="
        return self.value <= a.value

a = A(1)
b = A(2)
c = a + b
d = a - b
e = a * b
f = a / b
g = a % b

h = a > b
i = a < b
j = a == b
k = a != b
l = a >= b
m = a <= b

print a.value
print b.value
print c.value
print d.value
print e.value
print f.value
print g.value

print h
print i
print j
print k
print l
print m