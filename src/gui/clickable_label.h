#pragma once

#include <QLabel>
#include <QWidget>
#include <Qt>

/**
 * @brief Label that emits clicked signal same as QPushButton does
 */
class ClickableLabel : public QLabel {
    Q_OBJECT

public:
    using QLabel::QLabel;

signals:
    void clicked();

protected:
    void mousePressEvent(QMouseEvent* event) override;

};
