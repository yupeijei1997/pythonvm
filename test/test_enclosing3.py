def func(m = 1, x = 5, y = 10):
    print x
    z = 20
    def say():
        print x
        print y
        print z

    x = 3
    print x
    return say

f = func()
f()
