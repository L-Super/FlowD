#include "UserAgentConfigDialog.h"
#include "ui_UserAgentConfigDialog.h"

#include <QDir>
#include <QSettings>
#include <QStandardPaths>
UserAgentConfigDialog::UserAgentConfigDialog(QWidget* parent) : QDialog(parent), ui(new Ui::UserAgentConfigDialog)
{
    ui->setupUi(this);

    QString cachePath = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
    QDir cacheDir(cachePath);
    if (!cacheDir.exists())
        cacheDir.mkpath(cachePath);
    QSettings set(cachePath + "/config.ini", QSettings::IniFormat);
    QString defaultUserAgent;

#if defined(WINDOWS_OS)
    defaultUserAgent = "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like "
                       "Gecko) Chrome/128.0.0.0 Safari/537.36 Edg/128.0.0.0";
#elif defined(MAC_OS)
    defaultUserAgent = "Mozilla/5.0 (Macintosh; Intel Mac OS X 10_15_7) AppleWebKit/537.36 (KHTML, like Gecko) "
                       "Chrome/127.0.0.0 Safari/537.36 Edg/127.0.0.0";
#elif defined(LINUX_OS)
    defaultUserAgent = "user-agent", "Mozilla/5.0 (X11; Linux x86_64; rv:125.0) Gecko/20100101 Firefox/125.0";
#endif

    ui->plainUserAgent->setPlainText(set.value("Advanced/user-agent", defaultUserAgent).toString());
    connectSlots();
}

QString UserAgentConfigDialog::getUserAgent()
{
    exec();
    if (isAccepted) {
        return ui->plainUserAgent->toPlainText();
    }
    else {
        return QString();
    }
}

void UserAgentConfigDialog::connectSlots()
{
    connect(ui->btnOk, &QPushButton::clicked, this, [&] {
        isAccepted = true;
        close();
    });
    connect(ui->btnCancel, &QPushButton::clicked, this, [&] {
        isAccepted = false;
        close();
    });
}

UserAgentConfigDialog::~UserAgentConfigDialog()
{
    delete ui;
}
