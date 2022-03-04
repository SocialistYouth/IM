        #include "mychatdialog.h"
    #include "ui_mychatdialog.h"

    MyChatDialog::MyChatDialog(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::MyChatDialog)
    {
        ui->setupUi(this);

        setWindowTitle( "通讯软件v1.0.0");

        m_layout = new QVBoxLayout; // 垂直布局的层
        m_layout->setContentsMargins( 0 , 0, 0 ,0 ); //左下右上距离外边框多远
        m_layout->setSpacing( 3 );//每个控件彼此的距离

        ui->friend_list->setLayout( m_layout );
    }

    MyChatDialog::~MyChatDialog()
    {
        delete ui;
    }

    void MyChatDialog::slot_addFriend( userItem* item )
    {
        m_layout->addWidget( item );
    }
