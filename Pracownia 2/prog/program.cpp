/* 2012
 * Maciej Szeptuch
 * II UWr
 * ----------
 *  UWAGA: kompilować ze wsparciem dla C++11.
 *      make CXXFLAGS+="-std=c++11" program
 */
#include <cassert>
#include <cstdio>
#include <vector>
#include <algorithm>

#define $(x) ((x) % 3)

#define N_START 1
#define N_STOP 12
#define N_STEP 3
#define POINTS 1024
#define PRINT_POINTS false // dla wykres.py

std::vector<double> points,
                    chebyshevZeros[N_STOP + 2],
                    chebyshevExtremes[N_STOP + 2];

double poly(const double &x){ return pow(x, 10.) * 0.125 + pow(x, 9.) * 0.5 + pow(x, 8.) * 0.167 + pow(x, 5.) * 0.66 + pow(x, 3.) * 2. + pow(x, 2.) * 18. + x;}

double countError(std::function<double(double)> func, const int &n, const int &div, const std::vector<double> &c);
std::vector<double> countZeros(const int &n);
std::vector<double> countInPoints(std::function<double(double)> func, const std::vector<double> &pts);
std::vector<double> countExtremes(const int &n);
std::vector<double> bruteChebyshevFactors(const int &n, const std::vector<double> &c, const std::vector<double> &x);
std::vector<double> clenshawChebyshevFactors(const int &n, const std::vector<double> &c, const std::vector<double> &x);
double clenshawCount(const int &n, const std::vector<double> &c, const double &x);
void testIt(std::function<double(double)> func);
void preprocessing(void);

int main(void)
{
    if(PRINT_POINTS)
        fprintf(stderr, "from pylab import *\n");

    preprocessing();
    puts("        Pracownia z analizy numerycznej.\n\
            Program do zadania: P2.17.\n\
            Prowadzący: dr Paweł Woźny\n\n\
                Maciej Szeptuch\n\n\
    Interpolacja wielomianowa za pomocą wielomianów Czebyszewa.\n");

    puts("Funkcja wykładnicza e^x.");
    testIt(exp);

    puts("Funkcja trygonometryczna sin x.");
    testIt(sin);

    puts("Wielomian stopnia 10.");
    testIt(poly);
    return 0;
}

void testIt(std::function<double(double)> func)
{
    static int _id = 0;
    std::vector<double> c; // współczynniki
    for(unsigned int n = N_START; n < N_STOP; n += N_STEP)
    {
        if(PRINT_POINTS)
            fprintf(stderr, "pl = subplot(1, 1, 1)\nerr = []\nxlabel('x')\nylabel('blad')\n");
        printf("    n = %d\n", n);
        // In
        c = countInPoints(func, chebyshevZeros[n + 1]);
        assert(c.size() == n + 1);
        c = bruteChebyshevFactors(n, c, chebyshevZeros[n + 1]);
        assert(c.size() == n + 1);
        printf("        Błąd In = %.18lf\n", countError(func, n, n + 1, c));
        if(PRINT_POINTS)
        {
            fprintf(stderr, "pl.plot([x for x, _ in err], [y for _, y in err], '-', label = 'I')\n\
err = []\n");
        }

        // Jn
        c = countInPoints(func, chebyshevExtremes[n - 1]);
        assert(c.size() == n + 1);
        c.back() /= 2;
        c = clenshawChebyshevFactors(n, c, chebyshevExtremes[n - 1]);
        assert(c.size() == n + 1);
        c.back() /= 2;
        printf("        Błąd Jn = %.18lf\n", countError(func, n, n, c));
        if(PRINT_POINTS)
        {
            fprintf(stderr, "pl.plot([x for x, _ in err], [y for _, y in err], '-', label = 'J')\n\
err = []\n");
        }

        // Kn
        c = countInPoints(func, chebyshevExtremes[n]);
        assert(c.size() == n + 2);
        c.back() /= 2;
        c = clenshawChebyshevFactors(n + 1, c, chebyshevExtremes[n]);
        assert(c.size() == n + 2);
        printf("        Błąd Kn = %.18lf\n", countError(func, n, n + 1, c));
        if(PRINT_POINTS)
        {
            fprintf(stderr, "pl.plot([x for x, _ in err], [y for _, y in err], '-', label = 'K')\n\
err = []\n\
pl.legend(loc='upper center')\n\
savefig('../doc/graph%d_%d.png', bbox_inches=0)\n\
clf()\n", _id, n);
        }
    }

    ++ _id;
}

double countError(std::function<double(double)> func, const int &n, const int &div, const std::vector<double> &c)
{
    double err = 0;
    for(double &p: points)
    {
        if(PRINT_POINTS)
            fprintf(stderr, "err.append((%.18lf, abs(%.18lf - %.18lf)))\n", p, func(p), 2.*clenshawCount(n, c, p) / div);

        err = std::max(err, std::abs(func(p) - 2. * clenshawCount(n, c, p) / div));
    }

    return err;
}

/* Wyliczanie zer wielomianu Czebyszewa. t_{n,k} */
std::vector<double> countZeros(const int &n)
{
    std::vector<double> res;
    for(int k = 0; k < n; ++ k)
        res.push_back(cos(M_PI * (2. * k + 1.) / (2. * n)));

    return res;
}

/* Wyliczanie ekstremów wielomianu Czebyszewa. u_{n,k} */
std::vector<double> countExtremes(const int &n)
{
    std::vector<double> res;
    for(int k = 0; k <= n + 1; ++ k)
        res.push_back(cos(M_PI * k / (n + 1.)));

    return res;
}

/* Wyliczanie współczynników dla I_n. c_k = f(x_k), x_k = t_{n+1, k} */
std::vector<double> bruteChebyshevFactors(const int &n, const std::vector<double> &c, const std::vector<double> &x)
{
    std::vector<double> res(n + 1, 0.);
    double T[3] = {};
    for(int j = 0; j <= n; ++ j)
    {
        T[0] = c[j];
        T[1] = c[j] * x[j];
        res[0] += T[0];
        res[1] += T[1];
        for(int i = 2; i <= n; ++ i)
            res[i] += T[$(i)] = 2 * x[j] * T[$(i-1)] - T[$(i-2)];
    }

    return res;
}

/* Wyliczanie współczynników dla J_n i K_n. */
std::vector<double> clenshawChebyshevFactors(const int &n, const std::vector<double> &c, const std::vector<double> &x)
{
    std::vector<double> res;
    for(const double &q: x)
        res.push_back(clenshawCount(n, c, q));

    return res;
}

/* Wyliczanie wartości kombinacji liniowej wielomianów Czebyszewa w x Clenshawem. (z pierwszym współczynnikiem dzielonym przez 2) */
double clenshawCount(const int &n, const std::vector<double> &c, const double &x)
{
    double b[3] = {};
    for(int k = n; k >= 0; -- k)
        b[$(k)] = 2. * x * b[$(k + 1)] - b[$(k + 2)] + c[k];

    return (b[0] - b[2]) / 2.;
}

/* Wyliczanie wartości funkcji "func" w pts. */
std::vector<double> countInPoints(std::function<double(double)> func, const std::vector<double> &pts)
{
    std::vector<double> res;
    for(const double &p: pts)
        res.push_back(func(p));

    return res;
}

void preprocessing(void)
{
    for(int p = 0; p < POINTS; ++ p)
        points.push_back(-1. + 2. * p / POINTS);

    // Zera wielomianów Czebyszewa
    for(int n = N_START; n < N_STOP; n += N_STEP)
        chebyshevZeros[n + 1] = countZeros(n + 1);

    // Ekstrema wielomianów Czebyszewa
    for(int n = N_START; n < N_STOP; n += N_STEP)
    {
        chebyshevExtremes[n - 1] = countExtremes(n - 1);
        chebyshevExtremes[n] = countExtremes(n);
    }
}
