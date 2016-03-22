from pylab import *
from matplotlib.ticker import MultipleLocator

forw = (
    (0, 7, 16),
    (1, 6, 15),
    (2, 5, 14),
    (3, 5, 13),
    (4, 4, 12),
    (5, 3, 11),
    (6, 2, 10),
    (7, 0, 10),
    (8, 0, 9),
    (9, 0, 7),
    (10, 0, 6),
    (11, 0, 6),
    (12, 0, 4),
    (13, 0, 3),
    (14, 0, 2),
    (15, 0, 0),
    (16, 0, 1),
    (17, 0, 0),
    (18, 0, 0),
    (19, 0, 0),
    (20, 0, 0),
)

rev = (
    (20, 4, 4),
    (19, 5, 5),
    (18, 5, 5),
    (17, 7, 7),
    (16, 7, 7),
    (15, 8, 8),
    (14, 10, 10),
    (13, 10, 10),
    (12, 10, 12),
    (11, 9, 11),
    (10, 10, 13),
    (9, 10, 14),
    (8, 9, 13),
    (7, 10, 14),
    (6, 9, 18),
    (5, 9, 18),
    (4, 8, 18),
    (3, 9, 17),
    (2, 9, 18),
    (1, 10, 16),
    (0, 8, 17),
)

if __name__ == '__main__':
    pl = subplot(1, 1, 1)
    pl.xaxis.set_major_locator(MultipleLocator(1))
    pl.yaxis.set_major_locator(MultipleLocator(1))
    pl.xaxis.grid(True, 'major')
    pl.yaxis.grid(True, 'major')
    if True:
    #if False:
        xlim(0, 20)
        ylim(-1, 17)
        pl.plot(range(0, 21), [s for _, s, _ in forw], '-o', label = 'float')
        pl.plot(range(0, 21), [d for _, _, d in forw], '-o', label = 'double')
        pl.legend(loc='upper center')
        savefig('../doc/wykresbledudoprzodu.png', bbox_inches=0)
        show()

    else:
        xlim(20, 0)
        ylim(-1, 19)
        pl.plot(range(20, -1, -1), [s for _, s, _ in rev], '-o', label = 'float')
        pl.plot(range(20, -1, -1), [d for _, _, d in rev], '-o', label = 'double')
        pl.legend(loc='upper center')
        savefig('../doc/wykresbleduwstecz.png', bbox_inches=0)
        show()
