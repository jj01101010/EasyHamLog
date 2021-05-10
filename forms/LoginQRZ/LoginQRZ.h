#ifndef LOGINQRZ_H
#define LOGINQRZ_H

#include <QDialog>
#include <string>

namespace Ui {
class LoginQRZ;
}

class LoginQRZ : public QDialog
{
    Q_OBJECT

public:
    explicit LoginQRZ(std::string callsign, bool firstTry, QWidget *parent = nullptr);
    ~LoginQRZ();

    void getLoginCredentials(std::string* username, std::string* password) const;

private:
    Ui::LoginQRZ *ui;
};

#endif // LOGINQRZ_H
