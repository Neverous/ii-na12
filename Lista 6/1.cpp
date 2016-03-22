/* 2012
 * Maciej Szeptuch
 * II UWr
 */
#include <cstdio>
#include <cmath>

double factor[8][8];
double node[8] = {1., 2., 4., 7., 8., 10.};

inline double mul(double x, int n)
{
    double res = 1.;
    for(int q = 0; q < n; ++ q)
        res *= (x - node[q]);

    return res;
}

inline double f(double x){return log(x) - 2*(x-1)/x;}
inline double L(double x)
{
    double res = factor[5][5];
    for(int n = 4; n >= 0; -- n)
        res = res * (x - node[n]) + factor[n][n];

    return res;
}

int main(void)
{
    for(int h = 0; h < 6; ++ h)
    {
        factor[h][0] = f(node[h]);
        for(int w = 1; w <= h; ++ w)
            factor[h][w] = (factor[h][w-1] - factor[h-1][w-1]) / (node[h] - node[h-w]);
    }

    
    for(int h = 0; h < 6; ++ h)
    {
        for(int w = 0; w < 6; ++ w)
            printf("%18.16lf ", factor[h][w]);

        puts("");
    }

    printf("%18.16lf\n", L(2.9) - f(2.9));
    printf("%18.16lf\n", L(5.25) - f(5.25));

    return 0;
}
