/* 2013
 * Maciej Szeptuch
 * II UWr
 * ----------
 *  UWAGA: kompilować ze wsparciem dla C++11.
 *      make CXXFLAGS+="-std=c++11" program
 */
#include <cstdio>
#include <cfloat>
#include <algorithm>
#include <unordered_map>
#include <sys/timeb.h>

#define DELTA 0.5
#define ACCURACY 9
#define FORCE_ROMBERG 10
#define PARTS 9
#define MAX_LIMIT 30
#define _DEBUG if(false)

int countops,
    countcalls,
    returns;
double lengths[9] = {2., 1., 0.75, 0.25, 0.18, 0.12, 0.09, 0.03, 0.01};
char buffer[16];
std::unordered_map<std::string, bool> calls;

void testIt(std::function<double(double)> func, const double &start, const double &end);

double sin1x(const double &x){sprintf(buffer, "%.9lf", x); countcalls += !calls[std::string(buffer)]; calls[std::string(buffer)] = true; return sin(1. / x);}
double sin2(const double &x){sprintf(buffer, "%.9lf", x); countcalls += !calls[std::string(buffer)]; calls[std::string(buffer)] = true; return 2. / (2. + sin(10. * M_PI * x));}
double poly(const double &x){sprintf(buffer, "%.9lf", x); countcalls += !calls[std::string(buffer)]; calls[std::string(buffer)] = true; return 1. / (1. + pow(x, 4.));}
double ex(const double &x){sprintf(buffer, "%.9lf", x); countcalls += !calls[std::string(buffer)]; calls[std::string(buffer)] = true; return exp(x);}
double cheb(const double &x){sprintf(buffer, "%.9lf", x); countcalls += !calls[std::string(buffer)]; calls[std::string(buffer)] = true; return 256.*pow(x, 9.) - 576.*pow(x, 7.) + 432.*pow(x, 5.) - 120. * pow(x, 3.) + 9.*x;}

std::pair<double, int> romberg(std::function<double(double)> func, const double &a, const double &b, const int &acc, const int &limit = 16, const bool &force = false);
double variableRomberg(std::function<double(double)> func, const double &a, const double &b, const int &acc, double part);

int main(void)
{
    puts("        Pracownia z analizy numerycznej.\n\
            Program do zadania: P3.5.\n\
            Prowadzący: dr Paweł Woźny\n\n\
                Maciej Szeptuch\n\n\
    Złożona metoda Romberga.\n");

    puts("Legenda:\n\
    Romberg: zwykły Romberg\n\
    Romberg*: wynik z 10 wiersza tablicy Romberga\n\
    Złożony Romberg(x): wariant Romberga z zadania z danym podziałem początkowym.\n\
    W nawiasach liczba wykonanych operacji arytmetycznych, liczba wywołań funkcji dla różnych argumentów, liczba nawrotów, czas w sekundach.\n");

    puts("Całka 0 do 6 z e^x");
    testIt(ex, 0., 6.);

    puts("Całka 0 do 1 z 256x^9 - 576x^7 + 431x^5 - 120x^3 + 9x");
    testIt(cheb, 0., 1.);

    puts("Całka 0 do 8 z 2 / (2 + sin(pi*10*x))");
    testIt(sin2, 0., 8.);

    puts("Całka od 0.02 do 0.2 z sin(1/x)");
    testIt(sin1x, 0.02, 0.2);

    puts("Całka 0 do 7 z 1 / (1 + x^4)");
    testIt(poly, 0., 7.);
    return 0;
}

void testIt(std::function<double(double)> func, const double &start, const double &end)
{
    struct timeb sysTime;
    ftime(&sysTime);
    long long unsigned int startTime = (long long unsigned int)sysTime.time * 1000 + sysTime.millitm;
    calls.clear(); countops = countcalls = returns = 0;
    printf("Romberg:\t\t\t%11.10lf ", romberg(func, start, end, ACCURACY).first);
    ftime(&sysTime);
    printf("(%d, %d, %d, %.4lf)\n", countops, countcalls, returns, (sysTime.time * 1000LLU + sysTime.millitm - startTime) / 1000.); // od start do end z "dokładnością" ACCURACY cyfr

    for(int i = 1; i <= FORCE_ROMBERG; ++ i)
    {
        ftime(&sysTime);
        startTime = (long long unsigned int)sysTime.time * 1000 + sysTime.millitm;
        calls.clear(); countops = countcalls = returns = 0;
        printf("Romberg*(%d):\t\t\t%11.10lf ", i, romberg(func, start, end, ACCURACY, i, true).first);
        ftime(&sysTime);
        printf("(%d, %d, %d, %.4lf)\n", countops, countcalls, returns, (sysTime.time * 1000LLU + sysTime.millitm - startTime) / 1000.); // od start do end z "dokładnością" ACCURACY cyfr
    }

    for(int p = 0; p < PARTS; ++ p)
    {
        ftime(&sysTime);
        startTime = (long long unsigned int)sysTime.time * 1000 + sysTime.millitm;
        calls.clear(); countops = countcalls = returns = 0;
        printf("Złożony Romberg(%3.2lf):\t\t%11.10lf ", lengths[p], variableRomberg(func, start, end, ACCURACY, lengths[p]));
        ftime(&sysTime);
        printf("(%d, %d, %d, %.4lf)\n", countops, countcalls, returns, (sysTime.time * 1000LLU + sysTime.millitm - startTime) / 1000.); // od start do end z "dokładnością" ACCURACY cyfr
    }

    puts("");
}

std::pair<double, int> romberg(std::function<double(double)> func, const double &a, const double &b, const int &acc, const int &limit, const bool &force)
{
    /*
     * X_{0,0}
     * X_{1,0} X_{1,1}
     * X_{2,0} X_{2,1} X_{2,2}
     * X_{3.0} X_{3.1} X_{3,2} X_{3,3}
     * ...
     *
     * X_{r,0} = T_{0,r}
     * X_{r,c} = T_{c,r-c} = (4^c*X_{r,c-1} - X_{r-1,c-1})/(4^c-1)
     */

_DEBUG printf("Tablica romberga na przedziale [%3.2lf, %3.2lf].\n", a, b);

    double err = 1.,
           X[MAX_LIMIT + 1][MAX_LIMIT + 1] = {};

    for(int a = 0; a < acc; ++ a)
        err /= 10.;

    for(int r = 0; r <= limit; ++ r)
    {
        double h = (b - a) / (1LL << r);
        countops += 1;
        for(long long int k = 0; k < (1LL << r); ++ k)
        {
            countops += 7;
            X[r][0] += (func(a + h * k) + func(a + h * (k + 1))) / 2.; // pierwsza komórka wiersza
        }

        X[r][0] *= h; // cd.

        for(int c = 1; c <= r; countops += c + 4, ++ c)
            X[r][c] = (pow(4., c) * X[r][c - 1] - X[r - 1][c - 1]) / (pow(4., c) - 1.); // następne komórki

_DEBUG  for(int c = 0; c <= r; ++ c)
            printf("%10.9lf ", X[r][c]);

_DEBUG  puts("");
        if(!force && r && std::abs(X[r - 1][r - 1] - X[r][r]) <= err)
            return std::make_pair(X[r][r], r);
    }

    if(force)
        return std::make_pair(X[limit][limit], limit);

    return std::make_pair(DBL_MAX, 0);
}

double variableRomberg(std::function<double(double)> func, const double &a, const double &b, const int &acc, double part)
{
    double res = 0;
    std::pair<double, int> tmp = std::make_pair(DBL_MAX, 4);

    for(double act = a; act < b; act += part)
    {
        if(tmp.second <= 1)
            part *= (1. + DELTA);

        tmp = std::make_pair(DBL_MAX, 0);
        while(tmp.first == DBL_MAX)
        {
            part = std::min(part, b - act);
            tmp = romberg(func, act, act + part, acc, 4);
_DEBUG      puts("--");
            if(tmp.first == DBL_MAX)
            {
                part *= (1. - DELTA);
                ++ returns;
            }
        }

        res += tmp.first;
    }

    return res;
}
