#include "FileInformationWidget.h"
#include "ui_FileInformationWidget.h"

#include <QClipboard>
#include <QDesktopServices>
#include <QDir>
#include <QMessageBox>
#include <QUrl>

#include "Logger.hpp"

FileInformationWidget::FileInformationWidget(QString FileName, QString SavedFilePath, QString URL, qint64 FileSize,
                                             QString Status, QWidget* parent)
    : QWidget(parent), ui(new Ui::FileInformationWidget), fileName(FileName), savedFilePath(SavedFilePath), url(URL),
      fileSize(FileSize), status(Status)
{
    ui->setupUi(this);
}

void FileInformationWidget::iniUi()
{
    //remove frame
    this->setWindowFlags(Qt::Popup);

    //make the text of label selectable
    ui->labSavePath->setTextInteractionFlags(Qt::TextSelectableByMouse);
    ui->labTaskName->setTextInteractionFlags(Qt::TextSelectableByMouse);
    ui->labUrl->setTextInteractionFlags(Qt::TextSelectableByMouse);
    ui->labFileSize->setTextInteractionFlags(Qt::TextSelectableByMouse);

    //set details of the target file
    ui->labSavePath->setText(tr("Save to: %1").arg(savedFilePath));
    ui->labTaskName->setText(tr("Task Name: %1").arg(fileName));
    ui->labUrl->setText(tr("URL: %1").arg(url));
    ui->labFileSize->setText(tr("File Size: %1").arg(fileSize));
}

void FileInformationWidget::connectSlots()
{
    connect(ui->btnOpenFolder, &QPushButton::clicked, this, [&] {
        if (!QDesktopServices::openUrl(QUrl(savedFilePath))) {
            QMessageBox::critical(this, tr("Error"),
                                  tr("Failed in opening %1 !\nPlease check whether the folder exists and the "
                                     "application can access the folder.")
                                          .arg(QDir::toNativeSeparators(savedFilePath)));
            spdlog::error("Failed in opening {} !", QDir::toNativeSeparators(savedFilePath));
        }
    });
    connect(ui->btnOpen, &QPushButton::clicked, this, [&] {
        QFileInfo fileInfo(savedFilePath, fileName);
        QString savedFilePathName = fileInfo.absoluteFilePath();
        QUrl fileUrl = QUrl::fromLocalFile(savedFilePathName);
        if (!QDesktopServices::openUrl(fileUrl)) {
            QMessageBox::critical(this, tr("Error"),
                                  tr("Failed in opening %1 !\nPlease check whether the file exists and the "
                                     "application can access the file.")
                                          .arg(QDir::toNativeSeparators(savedFilePathName)));
            spdlog::error("Failed in opening {} !", QDir::toNativeSeparators(savedFilePathName));
        }
    });
    connect(ui->btnDelete, &QPushButton::clicked, this, [&] {
        //TODO:Add delete logic
    });
    connect(ui->btnCopyLink, &QPushButton::clicked, this, [&] {
        QClipboard* systemClipboard = QApplication::clipboard();
        systemClipboard->setText(url);
        QMessageBox::information(this, tr("Information"), tr("Url %1 copied!").arg(url));
        spdlog::info("Url {} copied!", url);
    });
    connect(ui->btnRestart, &QPushButton::clicked, this, [&] {
        //TODO:Add the logic of restart the task
    });
    connect(ui->btnOpen, &QPushButton::clicked, this, [&] {});
}

FileInformationWidget::~FileInformationWidget()
{
    delete ui;
}
