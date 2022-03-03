#include "logindialog.h"
#include "ui_logindialog.h"

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    setWindowTitle("QQ");
    ui->login_error->setVisible(false);
    ui->login_attention->setVisible(false);
    ui->register_error->setVisible(false);
    ui->register_attention->setVisible(false);

    //m_kernel = new CKernel;

}

LoginDialog::~LoginDialog()
{
    delete ui;
}

//登录
void LoginDialog::on_login_button_clicked()
{
    ui->login_error->setVisible(false);
    ui->login_attention->setVisible(false);
    //1.先判断输入是否合法
    QString username = ui->login_username->text();
    QString password = ui->login_password->text();
    if(username.isEmpty() || password.isEmpty()) {
        ui->login_attention->setText("用户名或密码错误");
        ui->login_error->setVisible(true);
        ui->login_attention->setVisible(true);
        ui->login_username->setText("");
        ui->login_password->setText("");
        return;
    }
    //2.提交数据-->发送信号(在Server判断用户是否存在)
    Q_EMIT SIG_LOGIN_RQ(username,password);
}

//注册
void LoginDialog::on_register_button_clicked()
{
    ui->register_error->setVisible(false);
    ui->register_attention->setVisible(false);
    //1.先判断输入是否合法
    QString username = ui->register_username->text();
    QString password = ui->register_password->text();
    QString password_again = ui->register_password_again->text();
    if(username.isEmpty() || password.isEmpty()) {
        ui->register_attention->setText("用户名或密码不能为空");
        ui->register_error->setVisible(true);
        ui->register_attention->setVisible(true);
        ui->register_username->setText("");
        ui->register_password->setText("");
        ui->register_password_again->setText("");
        return;
    }
    if(password != password_again){
        ui->register_attention->setText("两次密码不一致");
        ui->register_error->setVisible(true);
        ui->register_attention->setVisible(true);
        ui->register_password->setText("");
        ui->register_password_again->setText("");

        return;
    }

    //2.提交数据-->发送信号(在Server判断用户是否存在)
    Q_EMIT SIG_REGISTER_RQ(username,password);
}

void LoginDialog::slot_REGISTER_ATTENTION()
{
    ui->register_attention->setText("该用户名已存在");
    ui->register_error->setVisible(true);
    ui->register_attention->setVisible(true);
}
void LoginDialog::slot_LOGIN_ATTENTION()
{
    ui->login_attention->setText("该用户名不存在");
    ui->login_error->setVisible(true);
    ui->login_attention->setVisible(true);
}


