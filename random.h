#ifndef RANDOM_H
#define RANDOM_H

#include <qmath.h>
#include <QTime>

//-------------------------------------------------------------------//
//                              ФУНКЦИИ                              //
//-------------------------------------------------------------------//

// Инициализация генератора ПСЧ
void init_rand();

// Генерация ПСЧ в диапазоне [0;1)
double rand_double();

// Генерация ПСЧ в диапазоне [min;max)
double rand_double(double min, double max);

#endif // RANDOM_H

