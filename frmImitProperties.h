#ifndef FRMIMITPROPERTIES_H
#define FRMIMITPROPERTIES_H

#include <QDialog>
#include <QMessageBox>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QComboBox>
#include <QGridLayout>
#include <QRegExp>
#include <QRegExpValidator>
#include "ic_msu_net_properties.h"

//-------------------------------------------------------------------//
//                               КЛАССЫ                              //
//-------------------------------------------------------------------//

namespace Ui {
class frmImitProperties;
}


//--------------------------------------------------------------------------------------
// Окно настроек имитаторов
class frmImitProperties : public QDialog
{
    Q_OBJECT

public:
    explicit frmImitProperties(QWidget *parent = 0, ic_msu_net_properties *msu_prop = NULL);
    ~frmImitProperties();
    ic_msu_net_properties *get_net_properties();

private:
    Ui::frmImitProperties *ui;
    ic_msu_net_properties *prop;
    QSpinBox *spbInPort;
    QSpinBox *spbOutPort;
    QList<QLineEdit*> ledIPs;
    QComboBox *cmbEndianness;
};

#endif // FRMIMITPROPERTIES_H
