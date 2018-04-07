#include "clickable_label.h"


void ClickableLabel::mousePressEvent(QMouseEvent* event) {
	QLabel::mousePressEvent(event);
	emit clicked();
}
