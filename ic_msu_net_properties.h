#ifndef IC_MSU_NET_PROPERTIES_H
#define IC_MSU_NET_PROPERTIES_H

#include <stdint.h>
#include <QList>
#include <QNetworkInterface>
#include "ic_msu_message.h"

//-------------------------------------------------------------------//
//                             ОПРЕДЕЛЕНИЯ                           //
//-------------------------------------------------------------------//

#define IP_NUMBER_MASK QString("(?:[0-1]?[0-9]?[0-9]|2[0-4][0-9]|25[0-5])") // Маска для одного числа (байта) в IP-адресе
#define IP_MASK QString("%1\\.%1\\.%1\\.%1").arg(IP_NUMBER_MASK)            // Маска для IP-адреса
#define PORT_MIN_VALUE 1024     // Минимальное значение номера порта
#define PORT_MAX_VALUE 49151    // Максимальное значение номера порта

//-------------------------------------------------------------------//
//                             СТРУКТУРЫ                             //
//-------------------------------------------------------------------//

// Сетевые параметры имитаторов
struct ic_msu_net_properties
{
    uint32_t in_port;               // Порт приёма пакетов от имитаторов
    uint32_t out_port;              // Порт отправки пакетов в имитаторы
    QList<QHostAddress> ip_list;    // IP-адреса имитаторов с соответствующими номерами
    msu_endianness endianness;      // Порядок следования байт в пределах каждого слова (4 байта)
};

#endif // IC_MSU_NET_PROPERTIES_H

