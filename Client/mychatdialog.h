#ifndef MYCHATDIALOG_H
#define MYCHATDIALOG_H

#include <QDialog>
#include<QVBoxLayout>
#include"useritem.h"
namespace Ui {
class MyChatDialog;
}

class MyChatDialog : public QDialog
{
    Q_OBJECT

public:
    explicit MyChatDialog(QWidget *parent = 0);
    ~MyChatDialog();
public slots:
    void slot_addFriend( userItem* item );

private:
    Ui::MyChatDialog *ui;

    QVBoxLayout * m_layout;
};

#endif // MYCHATDIALOG_H
