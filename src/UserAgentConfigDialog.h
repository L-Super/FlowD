#pragma once

#include <QDialog>

namespace Ui {
    class UserAgentConfigDialog;
}

class UserAgentConfigDialog : public QDialog {
    Q_OBJECT

public:
    explicit UserAgentConfigDialog(QWidget* parent = nullptr);
    ~UserAgentConfigDialog();

    QString getUserAgent();

private:
    Ui::UserAgentConfigDialog* ui;
    bool isAccepted = false;

    void connectSlots();
};
