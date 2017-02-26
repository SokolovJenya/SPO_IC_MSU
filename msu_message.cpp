#include "msu_message.h"

//--------------------------------------------------------------------------------------
// Формирование информационного пакета
msu_message form_msu_message(uint8_t imit_num, uint8_t packet_type, uint8_t data_type, std::vector<uint8_t> data, uint32_t packet_number)
{
    msu_message message;

    // Наполнение идентификатора пакета
    message.header.message_id.imit_num = imit_num;
    message.header.message_id.packet_type = packet_type;
    message.header.message_id.data_type = data_type;

    // Наполнение массива данных и его размера
    if (data.size() > 0)
    {
        int d_len = data.size() % 4;
        if (d_len > 0)
        {
            for (int i = 0; i < (4 - d_len); ++i)
            {
                data.push_back(0);
            }
        }
        message.data = std::vector<uint32_t>(data.data(), data.data() + data.size());
        message.header.data_length = data.size();
    }
    else
    {
        message.header.data_length = 0;
    }

    // Наполнение контрольного слова
    message.packet_number = packet_number;

    return message;
}


//--------------------------------------------------------------------------------------
// Формирование информационного пакета
msu_message form_msu_message(uint8_t imit_num, uint8_t packet_type, uint8_t data_type, std::vector<uint16_t> data, uint32_t packet_number)
{
    return form_msu_message(imit_num, packet_type, data_type, std::vector<uint32_t>(data.data(), data.data() + data.size() * sizeof(uint16_t)), packet_number);
}


//--------------------------------------------------------------------------------------
// Формирование информационного пакета
msu_message form_msu_message(uint8_t imit_num, uint8_t packet_type, uint8_t data_type, std::vector<uint32_t> data, uint32_t packet_number)
{
    return form_msu_message(imit_num, packet_type, data_type, std::vector<uint32_t>(data.data(), data.data() + data.size() * sizeof(uint32_t)), packet_number);
}


//--------------------------------------------------------------------------------------
// Формирование информационного пакета
msu_message form_msu_message(uint8_t imit_num, uint8_t packet_type, uint8_t data_type, std::vector<float> data, uint32_t packet_number)
{
    return form_msu_message(imit_num, packet_type, data_type, std::vector<uint32_t>(data.data(), data.data() + data.size() * sizeof(float)), packet_number);
}


//--------------------------------------------------------------------------------------
// Формирование информационного пакета без данных
msu_message form_msu_message(uint8_t imit_num, uint8_t packet_type, uint8_t data_type, uint32_t packet_number)
{
    std::vector<uint8_t> data;

    return form_msu_message(imit_num, packet_type, data_type, data, packet_number);
}


//--------------------------------------------------------------------------------------
// Преобразование информационного пакета в массив байт
std::vector<uint8_t> to_byte_array(msu_message *message, msu_endianness endianess)
{
    std::vector<uint8_t> buff;

    // Добавление заголовка пакета
    uint8_t *array = (uint8_t*)message;
    for (int i = 0; i < 4; ++i)
    {
        buff.push_back(array[i]);
    }

    // Добавление данных
    if (message->data.size() > 0)
    {
        buff.insert(buff.end(), message->data.begin(), message->data.end());
    }

    // Добавление номера пакета
    array = (uint8_t*)&message->packet_number;
    for (int i = 3; i >= 0; --i)
    {
        buff.push_back(array[i]);
    }

    // Преобразование порядка байт
    if (endianess == little_endian)
    {
        change_byte_order(buff);
    }

    return buff;
}


//--------------------------------------------------------------------------------------
// Преобразование массива байт в информационный пакет
msu_message to_msu_message(std::vector<uint8_t> array, msu_endianness endianess)
{
    // Преобразование порядка байт
    std::vector<uint8_t> buff = array;
    if (endianess == little_endian)
    {
        change_byte_order(buff);
    }

    msu_message message;

    // Преобразование заголовка пакета
    memcpy(&message, &buff.front(), 4);

    // Преобразование данных
    if (message.header.data_length > 0)
    {
        std::vector<uint8_t>::iterator it = buff.begin() + 4;
        message.data.insert(message.data.begin(), it, it + message.header.data_length);
    }

    // Преобразование номера пакета
    memcpy(&message.packet_number, &(buff[4 + message.header.data_length]), 4);

    return message;
}


//--------------------------------------------------------------------------------------
// Изменение порядка байт в массиве байт в пределах каждого слова (4 байта)
void change_byte_order(std::vector<uint8_t> &data)
{
    int words_count = data.size() / 4;
    for (int i = 0; i < words_count; ++i)
    {
        swap(data[4 * i], data[4 * i + 3]);
        swap(data[4 * i + 1], data[4 * i + 2]);
    }
}


//--------------------------------------------------------------------------------------
// Взаимозамена двух байт
void swap(uint8_t &a, uint8_t &b)
{
    uint8_t u = a;
    a = b;
    b = u;
}
