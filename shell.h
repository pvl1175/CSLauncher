#ifndef SHELL_H
#define SHELL_H

#include <QHBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QProcess>
#include <QPushButton>
#include <QTextEdit>
#include <QWidget>
#include <QTabWidget>


class Shell : public QWidget {
Q_OBJECT
private:
    QProcess*  m_process1;
    QProcess*  m_process2;
    QProcess*  m_process3;
    QLineEdit* m_ptxtCommand;
    QLineEdit* m_pCSFolder;
    QTextEdit* m_ptxtDisplay1;
    QTextEdit* m_ptxtDisplay2;
    QTextEdit* m_ptxtDisplay3;
    QTabWidget* m_pTab;

public:
    Shell(QWidget* pwgt = 0) : QWidget(pwgt)
    {
        m_process1     = new QProcess(this);
        m_process2     = new QProcess(this);
        m_process3     = new QProcess(this);

#ifdef Q_OS_LINUX
        m_pCSFolder = new QLineEdit("/home/vita/testnet");
#endif

#ifdef Q_OS_WIN
        m_pCSFolder = new QLineEdit("d:\\CREDITS_TEST");
#endif

        m_pTab        = new QTabWidget;
        m_ptxtDisplay1 = new QTextEdit();
        m_ptxtDisplay2 = new QTextEdit();
        m_ptxtDisplay3 = new QTextEdit();

        setProps(m_ptxtDisplay1);
        setProps(m_ptxtDisplay2);
        setProps(m_ptxtDisplay3);

        QPushButton* pcmd1 = new QPushButton("Run ALL");
        QPushButton* pcmd2 = new QPushButton("Kill ALL");

        connect(m_process1,
                SIGNAL(readyReadStandardOutput()),
                SLOT(slotDataOnStdout1())
               );
        connect(m_process1,
                SIGNAL(readyReadStandardError()),
                SLOT(slotDataOnStderror1())
               );

        connect(m_process2,
                SIGNAL(readyReadStandardOutput()),
                SLOT(slotDataOnStdout2())
               );
        connect(m_process2,
                SIGNAL(readyReadStandardError()),
                SLOT(slotDataOnStderror2())
               );

        connect(m_process3,
                SIGNAL(readyReadStandardOutput()),
                SLOT(slotDataOnStdout3())
               );
        connect(m_process3,
                SIGNAL(readyReadStandardError()),
                SLOT(slotDataOnStderror3())
               );

        connect(pcmd1, SIGNAL(clicked()), SLOT(slotRunPressed()));
        connect(pcmd2, SIGNAL(clicked()), SLOT(slotKillPressed()));

        this->setWindowTitle("Simple Launcher");
        this->window()->resize(1200, 600);

        QVBoxLayout* pvbxLayout = new QVBoxLayout;
        QHBoxLayout* phbxLayout = new QHBoxLayout;
        phbxLayout->addWidget(pcmd1);
        phbxLayout->addWidget(pcmd2);
        pvbxLayout->addWidget(m_pCSFolder);
        pvbxLayout->addLayout(phbxLayout);

        QVBoxLayout* pvInfoLayout = new QVBoxLayout;
        m_pTab->addTab(m_ptxtDisplay1, "Node");
        m_pTab->addTab(m_ptxtDisplay2, "Executor");
        m_pTab->addTab(m_ptxtDisplay3, "Wallet");
        pvInfoLayout->addWidget(m_pTab);

        pvInfoLayout->addLayout(pvbxLayout);
        setLayout(pvInfoLayout);
    }

    void setProps(QTextEdit* pte)
    {
        QPalette p = pte->palette();
        p.setColor(QPalette::Base, Qt::black);
        pte->setPalette(p);

        pte->setTextColor(QColor(0, 255, 0));
        pte->setFontFamily("Lucida Console");
        pte->setFontPointSize(qreal(14));
    }

public slots:
    void slotDataOnStdout1()
    {
        m_ptxtDisplay1->append(m_process1->readAllStandardOutput());
    }
    void slotDataOnStderror1()
    {
        m_ptxtDisplay1->append(m_process1->readAllStandardError());
    }

    void slotDataOnStdout2()
    {
        m_ptxtDisplay2->append(m_process2->readAllStandardOutput());
    }
    void slotDataOnStderror2()
    {
        m_ptxtDisplay2->append(m_process2->readAllStandardError());
    }

    void slotDataOnStdout3()
    {
        m_ptxtDisplay3->append(m_process3->readAllStandardOutput());
    }
    void slotDataOnStderror3()
    {
        m_ptxtDisplay3->append(m_process3->readAllStandardError());
    }

    void slotRunPressed()
    {
        QString env{this->m_pCSFolder->text()};

        m_process1->setWorkingDirectory(env);
        m_process2->setWorkingDirectory(env);
        m_process3->setWorkingDirectory(env);

#ifdef Q_OS_LINUX
        m_process1->start("./client");
        m_process2->start("java -jar contract-executor.jar");
        m_process3->start("java -jar wallet-desktop.jar");
#endif

#ifdef Q_OS_WIN
        m_process1->start(env + "\\client.exe");
        m_process2->start("java -jar " + env + "\\contract-executor\\target\\contract-executor.jar");
        m_process3->start("java -jar " + env + "\\wallet-desktop\\target\\wallet-desktop.jar");
#endif

#ifdef Q_OS_LINUX
        m_process1->start("./client");
#endif

    }

    void slotKillPressed()
    {
        m_process1->kill();
        m_ptxtDisplay1->append("Node was killed");
        m_process2->kill();
        m_ptxtDisplay2->append("Executor was killed");
        m_process3->kill();
        m_ptxtDisplay3->append("Desktop Wallet was killed");
    }
};

#endif // SHELL_H
