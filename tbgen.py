def montgomery_reduce(a, b):
    print('a=',a,'b=',b,'result=',(a*b)%3329)


montgomery_reduce(1234, 1236)
montgomery_reduce(989, 678)
