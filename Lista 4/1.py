import math
from pylab import *

err = 10**-7
alfa = 0.5149332647

func = lambda x: math.cos(2 * x)**2 - x**2
deri = lambda x: -4 * math.cos(2 * x) * math.sin(2 * x) - 2 * x

def newton(f, fd, start):
    n = 0
    x = start
    xs = []
    ys = []
    print('a) Metoda Newtona. z x0 = %.2f' % x)
    print(' n |      xn      |   |xn - a|   |')
    print('-'*33 + '|')
    fixup = 1048576
    while abs(fixup) >= err:
        fixup = f(x) / fd(x)
        x = x - fixup
        n += 1
        print('%2d | %11.10f | %11.10f |' % (n, x, abs(x - alfa)))
        xs += (n,)
        ys += (abs(x - alfa),)

    plot(xs, ys, '-', label = 'newton')
    print('')
    return x

def simplerNewton(f, fd, start):
    n = 0
    x = start
    xs = []
    ys = []
    print('b) Uproszczona metoda Newtona. z x0 = %.2f' % x)
    print(' n |      xn      |   |xn - a|   |')
    print('-'*33 + '|')
    fixup = 1048576
    while abs(fixup) >= err:
        fixup = f(x) / fd(start)
        x = x - fixup
        n += 1
        print('%2d | %11.10f | %11.10f |' % (n, x, abs(x - alfa)))
        xs += (n,)
        ys += (abs(x - alfa),)

    plot(xs, ys, '-', label = 'uproszczony newton')
    print('')
    return x

def regulaFalsi(f, start, end):
    n = 0
    x = (start * f(end) - end * f(start)) / (f(end) - f(start))
    xs = []
    ys = []
    print('d) Metoda regula falsi. z a = %.2f b = %.2f' % (start, end))
    print(' n |      xn      |   |xn - a|   |')
    print('-'*33 + '|')
    fixup = 1048576
    while abs(fixup) >= err:
        if f(start) * f(x) <= 0:
            fixup = f(x) * (start - x) / (f(start) - f(x))

        else:
            fixup = f(x) * (end - x) / (f(end) - f(x))

        n += 1
        x -= fixup
        print('%2d | %11.10f | %11.10f |' % (n, x, abs(x - alfa)))
        xs += (n,)
        ys += (abs(x - alfa),)

    plot(xs, ys, '-', label = 'regula falsi')
    print('')
    return x

def slashing(f, start, end):
    n = 0
    x = start
    x2 = end
    xs = []
    ys = []
    print('c) Metoda siecznych. z a = %.2f b = %.2f' % (start, end))
    print(' n |      xn      |   |xn - a|   |')
    print('-'*33 + '|')
    fixup = 1048576
    while abs(fixup) >= err:
        fixup = f(x2) * (x2 - x) / (f(x2) - f(x))
        x, x2 = x2, x2 - fixup
        n += 1
        print('%2d | %11.10f | %11.10f |' % (n, x2, abs(x2 - alfa)))
        xs += (n,)
        ys += (abs(x - alfa),)

    plot(xs, ys, '-', label = 'sieczne')
    print('')
    return x

if __name__ == '__main__':
    ylim(-0.01, 0.1)
    newton(func, deri, 0.75)
    simplerNewton(func, deri, 0.75)
    slashing(func, 0., 0.75)
    regulaFalsi(func, 0., 0.75)
    legend()
    show()
