part = 1
end = 2

def test():
    global part
    for i in range(0, 100):
        part = part - (part*0.1)
    print part

test()
