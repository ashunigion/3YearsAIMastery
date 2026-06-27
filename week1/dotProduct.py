def dotProduct(a, b):
    sum = 0
    if len(a) != len(b):
        return "The size of both arrays not compatible for dot product"
    elif len(a) == 1 :
        for i in b:
            sum += (i*a)
        return sum
    elif len(b) == 1:
        for j in a:
            sum += (j*b)
        return sum
    else:
        for i, j in zip(a,b):
            sum+=(i * j) 
        return sum

a = [5, -3, 2, -1]
b = [2, 4, -3, 6]
print(dotProduct(a,b))

