def func(x = 5, y = 10):
    def say():
        print x
        print y

    x = 3
    print x
    return say

f = func()
f()
