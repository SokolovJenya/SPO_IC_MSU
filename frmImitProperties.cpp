#include "frmImitProperties.h"
#include "ui_frmImitProperties.h"

//--------------------------------------------------------------------------------------
// Конструктор окна настроек имитаторов
frmImitProperties::frmImitProperties(QWidget *parent, ic_msu_net_properties *msu_prop) :
    QDialog(parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint),
    ui(new Ui::frmImitProperties)
{
    ui->setupUi(this);

    if (msu_prop == NULL)
    {
        QMessageBox::information(this,
                                 "Ошибка настройки параметров имитаторов",
                                 "Список адресов имитаторов не определён",
                                 QMessageBox::Ok);
        prop = NULL;
        this->close();
        return;
    }

    // Присваиваем первоначальные параметры имитаторов
    prop = msu_prop;

    // Устанавливаем размер шрифта формы
    QFont font = this->font();
    font.setPointSize(11);
    this->setFont(font);

    // Создаем регулярное выражение с маской IP-адреса
    QRegExp ipRegex("^" + IP_MASK + "$");
    // Создаем валидатор регулярного выражения с применением созданного регулярного выражения
    QRegExpValidator *ipValidator = new QRegExpValidator(ipRegex, this);

    // Создаём элементы формы
    QGridLayout *topLayout = new QGridLayout(this);
    int row_number = 0;

    // Создаём элементы для номеров портов
    QLabel *lblOutPort = new QLabel("Порт отправки пакетов:");
    QLabel *lblInPort = new QLabel("Порт приёма пакетов:");

    spbOutPort = new QSpinBox();
    spbOutPort->setMinimum(PORT_MIN_VALUE);
    spbOutPort->setMaximum(PORT_MAX_VALUE);
    spbOutPort->setValue(prop->out_port);

    spbInPort = new QSpinBox();
    spbInPort->setMinimum(PORT_MIN_VALUE);
    spbInPort->setMaximum(PORT_MAX_VALUE);
    spbInPort->setValue(prop->in_port);

    lblOutPort->setBuddy(spbOutPort);
    lblInPort->setBuddy(spbInPort);

    // Добавляем элементы номеров портов на Layout
    topLayout->addWidget(lblOutPort, row_number, 0, Qt::AlignLeft);
    topLayout->addWidget(spbOutPort, row_number, 1);
    row_number++;
    topLayout->addWidget(lblInPort, row_number, 0, Qt::AlignLeft);
    topLayout->addWidget(spbInPort, row_number, 1);
    row_number++;

    // Создаём таблицу элементов с IP-адресами
    for (int i = 0; i < prop->ip_list.count(); ++i)
    {
        QLabel *label = new QLabel(QString("IP-адрес имитатора №%1:").arg(i + 1), this, 0);
        ledIPs.append(new QLineEdit());
        ledIPs[i]->setText(prop->ip_list[i].toString());
        ledIPs[i]->setValidator(ipValidator);

        label->setBuddy(ledIPs[i]);

        // Добавляем элементы с IP-адресом имитатора на Layout
        topLayout->addWidget(label, row_number, 0, Qt::AlignLeft);
        topLayout->addWidget(ledIPs[i], row_number, 1);
        row_number++;
    }

    // Создаём элементы для порядка следования байт
    QLabel *lblEndianness = new QLabel("Порядок следования байт:");

    cmbEndianness = new QComboBox();
    cmbEndianness->addItem(QString("big-endian"), QVariant(big_endian));
    cmbEndianness->addItem(QString("little-endian"), QVariant(little_endian));
    cmbEndianness->setCurrentIndex(cmbEndianness->findData(QVariant(prop->endianness)));

    lblEndianness->setBuddy(cmbEndianness);

    // Добавляем элементы для порядка следования байт на Layout
    topLayout->addWidget(lblEndianness, row_number, 0, Qt::AlignLeft);
    topLayout->addWidget(cmbEndianness, row_number, 1);
    row_number++;

    // Добавляем кнопки 'Ок' и 'Отмена' на Layout
    topLayout->addWidget(ui->btnBoxResults, row_number, 0, 1, Qt::AlignRight);
    row_number++;

    // Устанавливаем Layout для текущей формы
    setLayout(topLayout);
}


//--------------------------------------------------------------------------------------
// Деструктор окна настроек имитаторов
frmImitProperties::~frmImitProperties()
{
    delete ui;
}


//--------------------------------------------------------------------------------------
// Получение сетевых настроек имитаторов
ic_msu_net_properties *frmImitProperties::get_net_properties()
{
    if (prop == NULL)
    {
        QMessageBox::information(this,
                                 "Ошибка получения параметров имитаторов",
                                 "Список адресов имитаторов не определён",
                                 QMessageBox::Ok);
        return NULL;
    }

    prop->out_port = spbOutPort->value();
    prop->in_port = spbInPort->value();
    int count = prop->ip_list.count();
    prop->ip_list.clear();
    for (int i = 0; i < count; ++i)
    {
        prop->ip_list.append(QHostAddress(ledIPs[i]->text()));
    }
    memcpy(&prop->endianness, cmbEndianness->currentData().data(), 4);

    return prop;
}
