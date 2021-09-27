#include <QDialogButtonBox>
#include <QGridLayout>
#include <QLabel>
#include <QObject>
#include <QPushButton>

#include "AboutDialog.hpp"
#include "AspectRatioLabel.hpp"

AboutDialog::AboutDialog(QWidget* const parent)
	: QDialog{ parent } {
	QGridLayout* const layout = new QGridLayout{ this };

	AspectRatioLabel* const icon = new AspectRatioLabel{ SpacingPlacement::AROUND, SpacingPlacement::AROUND };
	icon->setMaximumSize(250, 250);
	icon->setMinimumSize(50, 50);
	icon->setScaledContents(true);
	icon->setPixmap(icon_pixmap);
	layout->addWidget(icon, 0, 0, Qt::AlignTop | Qt::AlignHCenter);

	auto* const text = new QLabel{
		tr("<b>TagViewer</b><br/><br/>TagViewer is a program that allows "
		   "viewing of media within a TagSpace, and rich filtering of that media with "
		   "tags and properties that are stored by the program. You can also sort by "
		   "values of properties or presence of tags, as well as show the media "
		   "in a slideshow.<br/><br/>See README or documentation for credits."),
	};
	text->setWordWrap(true);
	layout->addWidget(text, 0, 1, Qt::AlignLeft | Qt::AlignVCenter);

	auto* const button_box = new QDialogButtonBox{ QDialogButtonBox::Ok };
	layout->addWidget(button_box, 1, 0, 1, 2);
	QObject::connect(button_box, &QDialogButtonBox::rejected, this, &QDialog::reject);
	QObject::connect(button_box, &QDialogButtonBox::accepted, this, &QDialog::accept);

	layout->setColumnStretch(0, 1);
	layout->setColumnStretch(1, 2);
	layout->setRowStretch(0, 1);
	layout->setRowStretch(1, 0);

	setWindowTitle(tr("About TagViewer"));
	setAttribute(Qt::WA_DeleteOnClose, false);
}

AboutDialog::~AboutDialog() {
	//
}
