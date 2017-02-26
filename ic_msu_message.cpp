#include "ic_msu_message.h"


// Формирование сообщения для перевода имитатора в заданный режим
msu_message form_msg_switch_mode(uint8_t imit_num, uint8_t imit_mode, uint32_t packet_number)
{
    return form_msu_message(imit_num, 0x01, imit_mode, packet_number);
}
