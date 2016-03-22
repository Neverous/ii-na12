from math import log

result = 14.02585092994046

def simpson(func, a, b, n):
    h = 1.0 * (b - a) / n
    return sum([(func(a + k*h) + 4.0*func(a + (k + 1) * h) + func(a + (k + 2) * h)) for k in xrange(0, n, 2)]) * h / 3.0

start = 1
end = 10
eps = 0.0001
for n in xrange(2, 1000, 2):
    h = 1.0 * (end - start) / n
    S = simpson(log, start, end, n)
    E = abs(S - result)
    print "h = %.6f, S%d = %.6f, |S%d - I| = %.6f" % (h, n, S, n, E)
    if E <= eps:
        break
