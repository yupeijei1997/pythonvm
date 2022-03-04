# build dict
d = {1: "hello", "world": 2}

# dict print
print d

# dict subscr
print d[1]
print d["world"]

# dict store subscr
d[2] = "hello world"
print d

# dict setdefault
d.setdefault(1, 2)
d.setdefault(5, "hello")
print d

# dict remove del
d.remove(1)
del d[5]
print d
