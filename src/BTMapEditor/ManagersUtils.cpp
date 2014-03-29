#include "BTMapEditor/ManagersUtils.h"

/**
 * \class ClickableLabel
 */

ClickableLabel::ClickableLabel(QString text)
{
	setText(text);
}

void ClickableLabel::mousePressEvent(QMouseEvent *event)
{
	QLabel::mousePressEvent(event);
	emit clicked();
}