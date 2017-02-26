#ifndef MSU_MESSAGE_H
#define MSU_MESSAGE_H

#include <string.h>
#include <stdint.h>
#include <vector>

//-------------------------------------------------------------------//
//                       ТИПЫ ДАННЫХ, СТРУКТУРЫ                      //
//-------------------------------------------------------------------//

// Информационный пакет между ИЦ-МСУ и ВЧ-МСУ
struct msu_message
{
    union   // Заголовок пакета
    {
        struct  // Структура заголовка пакета
        {
            union   // Идентификатор пакета
            {
                struct  // Структура идентификатора пакета
                {
                    uint8_t imit_num : 4;       // Номер имитатора
                    uint8_t packet_type : 4;    // Тип пакета
                    uint8_t data_type;          // Тип данных
                } message_id;               // Идентификатор пакета (структура)
                uint16_t message_id_int;    // Идентификатор пакета (целое число)
            };
            uint16_t data_length;   // Длина поля данных в байтах
        } header;               // Заголовок пакета (структура)
        uint32_t header_int;    // Заголовок пакета (целое число)
    };

    std::vector<uint32_t> data; // Поле данных пакета
    uint32_t packet_number;     // Номер пакета
};

// Подярок байт в информационном пакете в пределах каждого слова (4 байта)
enum msu_endianness
{
    big_endian,     // Прямой порядок байт
    little_endian   // Обратный порядок байт
};

//-------------------------------------------------------------------//
//                              ФУНКЦИИ                              //
//-------------------------------------------------------------------//

// Формирование информационного пакета
msu_message form_msu_message(uint8_t imit_num, uint8_t packet_type, uint8_t data_type, std::vector<uint8_t> data, uint32_t packet_number);

// Формирование информационного пакета
msu_message form_msu_message(uint8_t imit_num, uint8_t packet_type, uint8_t data_type, std::vector<uint16_t> data, uint32_t packet_number);

// Формирование информационного пакета
msu_message form_msu_message(uint8_t imit_num, uint8_t packet_type, uint8_t data_type, std::vector<uint32_t> data, uint32_t packet_number);

// Формирование информационного пакета
msu_message form_msu_message(uint8_t imit_num, uint8_t packet_type, uint8_t data_type, std::vector<float> data, uint32_t packet_number);

// Формирование информационного пакета без данных
msu_message form_msu_message(uint8_t imit_num, uint8_t packet_type, uint8_t data_type, uint32_t packet_number);

// Преобразование информационного пакета в массив байт
std::vector<uint8_t> to_byte_array(msu_message *message, msu_endianness endianess = big_endian);

// Преобразование массива байт в информационный пакет
msu_message to_msu_message(std::vector<uint8_t> array, msu_endianness endianess = big_endian);

// Изменение порядка байт в массиве байт в пределах каждого слова (4 байта)
void change_byte_order(std::vector<uint8_t> &data);

// Взаимозамена двух байт
void swap(uint8_t &a, uint8_t &b);

#endif // MSU_MESSAGE_H

