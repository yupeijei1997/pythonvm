def func1(j = 12, k = 13):
    print j
    def func2(m = 1, x = 5, y = 10):
        print j
        print k
        print x
        z = 20
        def say():
            print j
            print x
            print y
            print z

        x = 3
        print x
        return say

    return func2

f1 = func1()
f2 = f1()
f2()
