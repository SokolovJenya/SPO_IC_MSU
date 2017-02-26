#ifndef IC_MSU_MESSAGE_H
#define IC_MSU_MESSAGE_H

#include "msu_message.h"

//-------------------------------------------------------------------//
//                       ТИПЫ ДАННЫХ, СТРУКТУРЫ                      //
//-------------------------------------------------------------------//

// Номера имитаторов
enum ic_msu_numbers
{
    imit_1 = 0b0001,    // 1-й имитатор
    imit_2 = 0b0010,    // 2-й имитатор
    imit_3 = 0b0100,    // 3-й имитатор
    imit_4 = 0b1000     // 4-й имитатор
};

// Режимы работы имитатора
enum ic_msu_modes
{
    mode_imit_scene = 0x01,         // Режим имитации сцены
    mode_calibration = 0x02,        // Режим калибровки
    mode_set_params = 0x03,         // Режим установки параметров
    mode_imit_arm_gsn_work = 0x04,  // Режим имитации работы АРМ-ГСН
    mode_self_control = 0x05,       // Режим самоконтроля
    mode_start = 0x11,              // Запуск выбранного режима работы
    mode_stop = 0x12                // Останов выбранного режима работы
};

// Параметры имитируемой сцены
struct ic_msu_scene_params
{
    float T;        // Текущее время [с]
    float R;        // Расстояние до точки привязки сцены (ТПС) [м]
    float V;        // Радиальная скорость сближения с ТПС [м/с]
    float a;        // Радиальное ускорение сближения с ТПС [м/с^2]
    float sigma;    // Суммарная ЭПО всех целей, входящих в имитируемую сцену [м^2]
};

// Параметры имитируемой цели (последовательность полей определена протоколом)
struct ic_msu_target_params
{
    uint16_t range;
    uint16_t number;
    uint16_t amplitude_is;
    uint16_t amplitude_ic;
    uint16_t acceleration;
    uint16_t velocity;
    uint16_t reserved_2;
    uint16_t reserved_1;
};

//-------------------------------------------------------------------//
//                              ФУНКЦИИ                              //
//-------------------------------------------------------------------//

// Формирование сообщения для перевода имитатора в заданный режим
msu_message form_msg_switch_mode(uint8_t imit_num, uint8_t imit_mode, uint32_t packet_number);

// Формирование сообщения для загрузки параметров имитируемой сцены
msu_message form_msg_load_scene_param(uint8_t imit_num, ic_msu_scene_params params, uint32_t packet_number);

// Формирование сообщения для загрузки параметров одной имитируемой цели
msu_message form_msg_load_target_params(uint8_t imit_num, ic_msu_target_params params, uint32_t packet_number);

// Формирование сообщения для загрузки параметров нескольких имитируемых целей
msu_message form_msg_load_target_params(uint8_t imit_num, std::vector<ic_msu_target_params> params, uint32_t packet_number);

#endif // IC_MSU_MESSAGE_H
