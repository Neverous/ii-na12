/* 2012
 * Maciej Szeptuch
 * II UWr
 */
#include<cstdio>
#include<cmath>

void forward(void);
void backward(void);

int main(void)
{
    puts("        Pracownia z analizy numerycznej.\n\
            Program do zadania: P1.13.\n\
            Prowadzący: dr Paweł Woźny\n\n\
                Maciej Szeptuch\n\n\
    Obliczanie całki za pomocą wzoru rekurencyjnego.\n");

    puts("        Rekurencja wprzód(1-20):");
    forward();

    puts("        Rekurencja wstecz(20-1):");
    backward();

    return 0;
}

void forward(void)
{
    // ...F - wymusza użycie floatów, domyślnie w c++ stałe są typu double
    float integralF = logf(1.1F);
    double integralLF = log(1.1);
    printf("            I%-2d  %22.8f %23.17lf\n",
           0, integralF, integralLF);
    for(int i = 1; i < 21; ++ i)
    {
        integralF = 1.0F / i - 10.0F * integralF;
        integralLF = 1.0 / i - 10.0 * integralLF;
        printf("            I%-2d  %22.8f %23.17lf\n",
               i, integralF, integralLF);
    }

    puts("");
    return;
}

void backward(void)
{
    float integralF = 0.0F;
    double integralLF = 0.0;
    integralF = (1.0F / 21 - integralF) / 10.0F;
    integralLF = (1.0 / 21 - integralLF) / 10.0;
    for(int i = 20; i >= 0; -- i)
    {
        printf("            I%-2d  %12.11f %23.20lf\n",
               i, integralF, integralLF);
        integralF = (1.0F / i - integralF) / 10.0F;
        integralLF = (1.0 / i - integralLF) / 10.0;
    }

    puts("");
    return;
}
