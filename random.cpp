#include "random.h"

//--------------------------------------------------------------------------------------
// Инициализация генератора ПСЧ
void init_rand()
{
    srand((new QTime())->currentTime().msecsSinceStartOfDay());
}


//--------------------------------------------------------------------------------------
// Генерация ПСЧ в диапазоне [0;1)
double rand_double()
{
    return (double) rand() / RAND_MAX;
}


//--------------------------------------------------------------------------------------
// Генерация ПСЧ в диапазоне [min;max)
double rand_double(double min, double max)
{
    return (max - min) * rand_double() + min;
}
