#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>



namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT
signals:
    void SIG_LOGIN_RQ(QString username,QString password);
    void SIG_REGISTER_RQ(QString username,QString password);
public:
    explicit LoginDialog(QWidget *parent = 0);
    ~LoginDialog();

private slots:
    void on_login_button_clicked();
    void on_register_button_clicked();
    void slot_REGISTER_ATTENTION();
    void slot_LOGIN_ATTENTION();
private:
    Ui::LoginDialog *ui;

};

#endif // LOGINDIALOG_H
