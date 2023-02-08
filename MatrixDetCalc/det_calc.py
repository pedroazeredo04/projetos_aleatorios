# m input = square matrix
# i input = row index
def lower_order(m, i):
    n = len(m[0])  # order of matrix
    lm = []
    for j in range(n):
        if j != i:
            lm.append(m[j][1:])
    return lm


# m input = square matrix
def recursive_det_calc(m):
    if len(m) != len(m[0]): 
        raise ValueError('Matrix must be square')
    
    n = len(m)  # order of matrix

    # base recursion case
    if n == 1: return m[0][0]

    # recursive case
    det = 0
    for i in range(n):
        det += (-1)**i * m[i][0] * recursive_det_calc(lower_order(m, i))
    
    return det


m = [[1, 0, 2],
     [0, 9, 0],
     [1, 0, 1]]

print(recursive_det_calc(m))
