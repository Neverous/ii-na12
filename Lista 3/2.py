from math import exp

def bisect(func, a, b, zero, steps):
    s = 0
    while s < steps:
        s += 1
        m = (a + b) / 2
        err = abs(zero - m)
        est = 1.0 * (b - a) / 2
        print('{0:3}: real_error={1:.10f} estimated_error={2:.10f}'.format(s, err, est))
        if func(m)*func(a) > 0:
            a = m

        else:
            b = m

    return a

if __name__ == '__main__':
    bisect(lambda x: x*exp(-x) - 0.06064, 0, 1, 0.0646926359947960, 15)
