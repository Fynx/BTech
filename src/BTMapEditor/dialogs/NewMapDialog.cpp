#include "BTMapEditor/dialogs/NewMapDialog.h"

/**
 * \class NewMapDialog
 */

NewMapDialog::NewMapDialog(QWidget *parent)
	: QDialog(parent)
{
	setFixedSize(200, 200);

QHBoxLayout *widthLayout = new QHBoxLayout;
	QLabel *widthLabel = new QLabel(BTech::Strings::LabelWidth + ": ");
	widthLine = new QSpinBox;
	widthLabel->setBuddy(widthLine);
	widthLine->setRange(MIN_SIZE, MAX_SIZE);
	widthLine->setValue(DEFAULT_SIZE);
	widthLayout->addWidget(widthLabel);
	widthLayout->addWidget(widthLine);

	QHBoxLayout *heightLayout = new QHBoxLayout;
	QLabel *heightLabel = new QLabel(BTech::Strings::LabelHeight + ": ");
	heightLine = new QSpinBox;
	heightLabel->setBuddy(heightLine);
	heightLine->setRange(MIN_SIZE, MAX_SIZE);
	heightLine->setValue(DEFAULT_SIZE);
	heightLayout->addWidget(heightLabel);
	heightLayout->addWidget(heightLine);

	confirmButton = new QPushButton(BTech::Strings::ButtonCreateMap);
	connect(confirmButton, &QPushButton::pressed, this, &NewMapDialog::accept);

	QVBoxLayout *layout = new QVBoxLayout;
	layout->addItem(widthLayout);
	layout->addItem(heightLayout);
	layout->addWidget(confirmButton);

	setLayout(layout);
}

NewMapDialog::~NewMapDialog()
{}

int NewMapDialog::getMapWidth() const
{
	return widthLine->value();
}

int NewMapDialog::getMapHeight() const
{
	return heightLine->value();
}
