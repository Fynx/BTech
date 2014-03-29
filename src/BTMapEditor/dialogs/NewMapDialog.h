#ifndef NEWMAPDIALOG_H
#define NEWMAPDIALOG_H

#include <QtWidgets>
#include "BTCommon/CommonStrings.h"
#include "BTMapEditor/Utils.h"

/**
 * \class NewMapDialog
 * NewMapDialog is a QDialog for creating new maps.
 */
class NewMapDialog : public QDialog
{
Q_OBJECT;

public:
	NewMapDialog(QWidget *parent = nullptr);
	~NewMapDialog();

	int getMapWidth() const;
	int getMapHeight() const;

private:
	QSpinBox *widthLine;
	QSpinBox *heightLine;
	QPushButton *confirmButton;

	static const int MIN_SIZE = 4;
	static const int MAX_SIZE = 100;
	static const int DEFAULT_SIZE = 10;
};

#endif // NEWMAPDIALOG_H
