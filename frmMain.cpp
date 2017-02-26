#include "frmMain.h"
#include "ui_frmMain.h"

//--------------------------------------------------------------------------------------
// Конструктор главного окна
frmMain::frmMain(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::frmMain)
{
    ui->setupUi(this);

    // Устанавливаем размер шрифта формы
    QFont font = this->font();
    font.setPointSize(11);
    this->setFont(font);

    // Начальная инициализация сетевых параметров имитаторов
    this->setDefaults();

    // Присоединение сигналов к слотам
    connect(ui->btnChangeNetProp, SIGNAL(triggered()), this, SLOT(on_btnChangeNetProp_clicked()));
    connect(ui->btnSetTargetsMatrix, SIGNAL(clicked()), this, SLOT(on_btnSetTargetsMatrix_clicked()));
    connect(ui->btnInputTargetsMatrix, SIGNAL(clicked()), this, SLOT(on_btnInputTargetsMatrix_clicked()));
    connect(ui->btnStartModeling, SIGNAL(clicked()), this, SLOT(on_btnStartModeling_clicked()));
    connect(ui->btnStopModeling, SIGNAL(clicked()), this, SLOT(on_btnStopModeling_clicked()));
    connect(udpIn, SIGNAL(readyRead()), SLOT(readMessage()));
    connect(timer, SIGNAL(timeout()), this, SLOT(on_Timer()));

    // QMessageBox::information(this, "Some title", QNetworkInterface::allInterfaces()[0].humanReadableName(), QMessageBox::Ok);
}


//--------------------------------------------------------------------------------------
// Деструктор главного окна
frmMain::~frmMain()
{
    delete ui;
}


//--------------------------------------------------------------------------------------
// Установка сетевых настроек по-умолчанию
void frmMain::setDefaults()
{
    prop = new ic_msu_net_properties();
    for (int i = 0; i < 4; ++i)
    {
        udpOut.append(new QUdpSocket(this));
    }
    udpIn = new QUdpSocket(this);

    if (!this->readSettings())  // Чтение настроек из файла
    {
        // Начальная инициализация сетевых параметров имитаторов
        prop->out_port = 3334;
        prop->in_port = 3333;
        QList<QHostAddress> IPs;
        IPs.append(QHostAddress("192.168.144.99"));
        IPs.append(QHostAddress("192.168.144.98"));
        IPs.append(QHostAddress("192.168.144.97"));
        IPs.append(QHostAddress("192.168.144.96"));
        prop->ip_list = IPs;
        prop->endianness = little_endian;
    }

    // Настройка UDP-сокетов
    for (int i = 0; i < 4; ++i)
    {
        //udpOut[i]->bind(prop->in_port, QUdpSocket::DefaultForPlatform);
        //udpOut[i]->bind(prop->ip_list[0], prop->out_port);
        //udpOut[i]->setLocalPort(prop->in_port);
    }
    udpIn->bind(prop->in_port, QUdpSocket::DefaultForPlatform);

    // Настройка таймера отправки сообщений
    timer = new QTimer(this);
    timer->setInterval(1000);
}


//--------------------------------------------------------------------------------------
// Чтение настроек по-умолчанию из файла
bool frmMain::readSettings()
{
    // Чтение файла с настройками
    QFile file(SETTINGS_FILE);
    if (!file.exists() || !file.open(QFile::ReadOnly))
    {
        return false;
    }
    QString settings = QString(file.readAll());
    file.close();

    // Инициализация настроек
    // Номер порта для отправки пакетов
    QString expression = QString("\\[out_port\\]\\s*=\\s*(\\d+)\\s*\\n");
    QRegExp reg(expression);
    if (reg.indexIn(settings, 0, QRegExp::CaretAtZero) >= 0)
    {
        prop->out_port = reg.cap(1).toInt();
        if (prop->out_port < PORT_MIN_VALUE || prop->out_port > PORT_MAX_VALUE)
        {
            return false;
        }
    }
    else
    {
        return false;
    }
    // Номер порта для получения пакетов
    expression = QString("\\[in_port\\]\\s*=\\s*(\\d+)\\s*\\n");
    reg.setPattern(expression);
    if (reg.indexIn(settings, 0, QRegExp::CaretAtZero) >= 0)
    {
        prop->in_port = reg.cap(1).toInt();
        if (prop->in_port < PORT_MIN_VALUE || prop->in_port > PORT_MAX_VALUE)
        {
            return false;
        }
    }
    else
    {
        return false;
    }
    // IP-адреса имитаторов
    for (int i = 1; i <= 4; ++i)
    {
        expression = QString("\\[imit%1_address\\]\\s*=\\s*(%2)\\s*\\n").arg(QString::number(i), IP_MASK);
        reg.setPattern(expression);
        if (reg.indexIn(settings, 0, QRegExp::CaretAtZero) >= 0)
        {
            prop->ip_list.append(QHostAddress(reg.cap(1)));
        }
        else
        {
            return false;
        }
    } 
    // Порядок следования байт
    expression = QString("\\[endianness\\]\\s*=\\s*(big_endian|little_endian)\\s*\\n");
    reg.setPattern(expression);
    if (reg.indexIn(settings, 0, QRegExp::CaretAtZero) >= 0)
    {
        QString s = reg.cap(1);
        if (s == "big_endian")
        {
            prop->endianness = big_endian;
        }
        else
        {
            prop->endianness = little_endian;
        }
    }
    else
    {
        return false;
    }

    return true;
}


//--------------------------------------------------------------------------------------
// Запись настроек по-умолчанию
void frmMain::writeSettings()
{
    QFile file(SETTINGS_FILE);

    if (prop == NULL || !file.open(QFile::WriteOnly))
    {
        QMessageBox::information(this,
                                 "Ошибка записи параметров имитаторов",
                                 "Параметры имитаторов не могут быть сохранены",
                                 QMessageBox::Ok);
        return;
    }

    // Запись номеров портов
    file.write(QString("[out_port] = %1\r\n").arg(prop->out_port).toUtf8());
    file.write(QString("[in_port] = %1\r\n").arg(prop->in_port).toUtf8());
    // Запись IP-адресов имитаторов
    for (int i = 0; i < 4; ++i)
    {
        file.write(QString("[imit%1_address] = %2\r\n").arg(QString::number(i + 1), prop->ip_list[i].toString()).toUtf8());
    }
    file.write(QString("[endianness] = %1\r\n").arg(prop->endianness == big_endian ? "big_endian" : "little_endian").toUtf8());

    file.close();
}


//--------------------------------------------------------------------------------------
// Слот нажатия кнопки настройки сетевых параметров имитаторов
void frmMain::on_btnChangeNetProp_clicked()
{
    frmImitProperties *frm = new frmImitProperties(this, prop);
    if (frm->exec() == QDialog::Accepted)
    {
        prop = frm->get_net_properties();
        this->writeSettings();
    }
    delete frm;
}


//--------------------------------------------------------------------------------------
// Слот нажатия кнопки рассчёта матрицы целей
void frmMain::on_btnSetTargetsMatrix_clicked()
{
    // Создание пустой матрицы
    int targets_count = (int)ui->dsbTargetsCount->value();
    ui->tblTargetsMatrix->clear();
    ui->tblTargetsMatrix->setRowCount(4);
    ui->tblTargetsMatrix->setColumnCount(targets_count);
    // Вставка заголовков
    for (int i = 0; i < targets_count; ++i)
    {
        QTableWidgetItem *item = new QTableWidgetItem();
        item->setText(QString("Цель №%1").arg(i + 1));
        ui->tblTargetsMatrix->setHorizontalHeaderItem(i,item);
    }
    ui->tblTargetsMatrix->setVerticalHeaderItem(0, new QTableWidgetItem("X:"));
    ui->tblTargetsMatrix->setVerticalHeaderItem(1, new QTableWidgetItem("Y:"));
    ui->tblTargetsMatrix->setVerticalHeaderItem(2, new QTableWidgetItem("Z:"));
    ui->tblTargetsMatrix->setVerticalHeaderItem(3, new QTableWidgetItem("Ампл.:"));

    // Рассчёт матрицы целей
    double X = ui->dsbXmin->value();
    double dX = 0;
    double max_ampl = ui->dsbAmplitude->value() / 100.0;
    double min_ampl = (100.0 - ui->dsbDeltaAmplitude->value()) / 100.0 * max_ampl;
    init_rand();
    if (targets_count > 1)
    {
        dX = (ui->dsbXmax->value() - X) / (targets_count - 1);
    }
    for (int i = 0; i < targets_count; ++i)
    {
        ui->tblTargetsMatrix->setItem(0, i, new QTableWidgetItem(QString::number(X, 'f', 2)));
        X += dX;
        ui->tblTargetsMatrix->setItem(1, i, new QTableWidgetItem(QString::number(
            rand_double(-ui->dsbDeltaY->value(), ui->dsbDeltaY->value()), 'f', 2)));
        ui->tblTargetsMatrix->setItem(2, i, new QTableWidgetItem(QString::number(
            rand_double(-ui->dsbDeltaZ->value(), ui->dsbDeltaZ->value()), 'f', 2)));
        ui->tblTargetsMatrix->setItem(3, i, new QTableWidgetItem(QString::number(
            rand_double(min_ampl, max_ampl), 'f', 4)));
    }
}


//--------------------------------------------------------------------------------------
// Слот нажатия кнопки ввода матрицы целей
void frmMain::on_btnInputTargetsMatrix_clicked()
{
    ;
}


//--------------------------------------------------------------------------------------
// Слот срабатывания таймера, отправка сообщений
void frmMain::on_Timer()
{
    msu_message msg = form_msg_switch_mode(imit_1, mode_set_params, ++lastSendedMessageNum);
    sendMessage(imit_1, &msg);
}


//--------------------------------------------------------------------------------------
// Слот нажатия кнопки начала моделирования
void frmMain::on_btnStartModeling_clicked()
{
    lastSendedMessageNum = 0;
    lastRecievedMessageNum = 0;
    lostMessagesNum = 0;
    ui->ledLostMessagesCount->setText("0");
    QString fileName = "D:/Temp/"
            + (new QDate())->currentDate().toString("yyyy.MM.dd") + "_"
            + (new QTime())->currentTime().toString("hh:mm:ss") + ".txt";
    tlmFile = new QFile(fileName);
    tlmFile->open(QFile::WriteOnly);
    udpIn->flush();
    timer->start();
}


//--------------------------------------------------------------------------------------
// Слот нажатия кнопки остановки моделирования
void frmMain::on_btnStopModeling_clicked()
{
    udpIn->flush();
    timer->stop();
    if (tlmFile != NULL && tlmFile->isOpen())
    {
        tlmFile->close();
    }
}


//--------------------------------------------------------------------------------------
// Слот нажатия кнопки отправки сообщения
void frmMain::on_btnSendMessage_clicked()
{
    //sendMessage();
}


//--------------------------------------------------------------------------------------
// Слот чтения сообщения UDP
void frmMain::readMessage()
{
    while (udpIn->hasPendingDatagrams())
    {
        std::vector<uint8_t> buff(udpIn->pendingDatagramSize());
        QHostAddress sender;
        udpIn->readDatagram((char*)&buff.front(), udpIn->pendingDatagramSize(), &sender);

        msu_message msg = to_msu_message(buff, prop->endianness);

        if (msg.packet_number - lastRecievedMessageNum > 1)
        {
            lostMessagesNum++;
            ui->ledLostMessagesCount->setText(QString::number(lostMessagesNum));
        }
        if (msg.packet_number > lastRecievedMessageNum)
        {
            lastRecievedMessageNum = msg.packet_number;
        }

        writeMessageToFile(&msg);

        //message.data.push_back('\0'); // TEST Временно

        /*if (!QNetworkInterface::allAddresses().contains(sender))
        {
            ui->pteDialogWindow->appendPlainText(tr("Received datagram: \"%1\"").arg((char*)&message.data.front()));
        }*/
    }
}


//--------------------------------------------------------------------------------------
// Отправка сообщения по UDP
void frmMain::sendMessage(uint32_t imit_num, msu_message *msg)
{
    //std::vector<uint8_t> data;
    //data.push_back(56);
    //data.push_back(57);
    //msu_message message = form_msg_switch_mode(imit_1, mode_set_params, 1);

    std::vector<uint8_t> buff = to_byte_array(msg, prop->endianness);

    int index = -1;
    switch (imit_num)
    {
    case imit_1:
        index = 0;
        break;
    case imit_2:
        index = 1;
        break;
    case imit_3:
        index = 2;
        break;
    case imit_4:
        index = 3;
        break;
    }

    if (index < 0)
        return;

    udpIn->writeDatagram((char*)&buff.front(), buff.size(), prop->ip_list[index], prop->out_port);
    writeMessageToFile(msg);
    //udpOut[index]->writeDatagram((char*)&buff.front(), buff.size(), prop->ip_list[index], prop->out_port);
    //udp->writeDatagram((char*)&buff.front(), buff.size(), MSU_BROADCAST_ADDR, MSU_BROADCAST_PORT2);

    /*
    msu_message m2 = to_msu_message(buf);

    QByteArray datagramm = ui->ledMessage->text().toUtf8();
    udp->writeDatagram(datagramm.data(), datagramm.size(), MSU_BROADCAST_ADDR, MSU_BROADCAST_PORT);
    ui->pteDialogWindow->appendPlainText(tr("Sent datagram: \"%1\"").arg(ui->ledMessage->text()));
    ui->ledMessage->clear();
    */
}


//--------------------------------------------------------------------------------------
// Запись сообщения из обмена в файл
void frmMain::writeMessageToFile(msu_message *msg)
{
    tlmFile->write(QString("Message #%1:\r\n").arg(lastSendedMessageNum
                                                   + lastRecievedMessageNum).toUtf8());
    tlmFile->write(QString("Header: %1\r\n").arg(msg->header_int).toUtf8());
    tlmFile->write(QString("Data: ").toUtf8());
    for (int i = 0; i < msg->data.size(); ++i)
    {
        tlmFile->write(QString("%1").arg(msg->data[i]).toUtf8());
    }
    tlmFile->write(QString("\r\nPacket number: %1\r\n").arg(msg->packet_number).toUtf8());
}


//--------------------------------------------------------------------------------------
// Отладочный слот
void frmMain::checkFunction()
{
    QMessageBox::information(this, "Checking", "OK", QMessageBox::Ok);
}
