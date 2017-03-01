#ifndef FRMMAIN_H
#define FRMMAIN_H

#include <QMainWindow>
#include <QDialog>
#include <QNetworkInterface>
#include <QMessageBox>
#include <QUdpSocket>
#include <QtEndian>
#include <QFile>
#include <QTimer>
#include <vector>
#include "msu_message.h"
#include "frmImitProperties.h"
#include "random.h"

//-------------------------------------------------------------------//
//                             ОПРЕДЕЛЕНИЯ                           //
//-------------------------------------------------------------------//

#define SETTINGS_FILE QString("settings.ini")   // Имя файла настроек
#define TIMER_INTERVAL 10                       // Интервал таймера отправки пакетов
#define MAX_LOST_MESSAGES_IN_ROW 10             // Максимальное количество потерянных сообщений подряд

//-------------------------------------------------------------------//
//                               КЛАССЫ                              //
//-------------------------------------------------------------------//

namespace Ui {
    class frmMain;
}


//--------------------------------------------------------------------------------------
// Главное окно
class frmMain : public QMainWindow
{
    Q_OBJECT

public:
    explicit frmMain(QWidget *parent = 0);
    ~frmMain();

private slots:
    void on_btnChangeNetProp_clicked();     // Слот нажатия кнопки настройки сетевых параметров имитаторов
    void on_btnSendMessage_clicked();       // Слот нажатия кнопки отправки сообщения
    void on_btnSetTargetsMatrix_clicked();  // Слот нажатия кнопки рассчёта матрицы целей
    void on_btnInputTargetsMatrix_clicked();// Слот нажатия кнопки ввода матрицы целей
    void on_btnStartModeling_clicked();     // Слот нажатия кнопки начала моделирования
    void on_btnStopModeling_clicked();      // Слот нажатия кнопки остановки моделирования
    void on_btnAboutProgram_clicked();      // Слот нажатия кнопки "О программе"
    void checkFunction();                   // Отладочный слот
    void readMessage();                     // Слот чтения сообщения UDP
    void on_Timer();                        // Слот срабатывания таймера, отправка сообщений

private:
    Ui::frmMain *ui;                // Интерфейс пользователя
    QList<QUdpSocket*> udpOut;      // UDP-клиенты отправки пакетов
    QUdpSocket *udpIn;              // UDP-клиент приёма пакетов
    QTimer *timer;                  // Таймер отправки пакетов
    QFile *tlmFile;                 // Файл с данными обмена
    ic_msu_net_properties *prop;    // Сетевые параметры имитаторов
    uint32_t lastSendedMessageNum;  // Номер последнего отправленного сообщения
    uint32_t lastRecievedMessageNum;// Номер последнего полученного сообщения
    uint32_t receivedMessagesCount; // Количество потерянных сообщений


    void sendMessage(uint32_t imit_num, msu_message *msg);  // Отправка сообщения по UDP
    void setDefaults();                                     // Установка сетевых настроек по-умолчанию
    bool readSettings();                                    // Чтение настроек по-умолчанию из файла
    void writeSettings();                                   // Запись настроек по-умолчанию
    void writeMessageToFile(msu_message *msg);              // Запись сообщения из обмена в файл
};

#endif // FRMMAIN_H
