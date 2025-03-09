#pragma once

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
    class HelpWidget;
}
QT_END_NAMESPACE

class HelpWidget : public QWidget {
    Q_OBJECT

public:
    explicit HelpWidget(QWidget* parent = nullptr);
    ~HelpWidget() override;

private:
    Ui::HelpWidget* ui;
};
