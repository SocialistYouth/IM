#ifndef USERITEM_H
#define USERITEM_H

#include <QWidget>

namespace Ui {
class userItem;
}

class userItem : public QWidget
{
    Q_OBJECT
signals:
    void SIG_userItemClicked( int id );
public:
    explicit userItem(QWidget *parent = 0);
    ~userItem();
public slots:
    void slot_setInfo( int id , QString name , int state = 1
                       ,int iconid = 1 , QString feeling ="比较懒,啥也没写");

private:
    Ui::userItem *ui;
public:
    int m_id;
    QString m_name;
    int m_state;
    int m_iconid;
    QString m_feeling;
private slots:
    void on_pb_icon_clicked();
};

#endif // USERITEM_H
