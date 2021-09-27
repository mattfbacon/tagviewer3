#include <QGridLayout>
#include <QPushButton>

#include "DebugPanel.hpp"

#include "pragmas.hpp"

DebugPanel::DebugPanel(QWidget* const parent)
	: QDialog{
		parent,
	} {
	auto* const layout = new QGridLayout();
	auto* const button_reset_style = new QPushButton(tr("Reset style"));
	QObject::connect(button_reset_style, &QPushButton::clicked, this, &DebugPanel::reset_style);
	layout->addWidget(button_reset_style);
	{
		int const count_ = layout->count();
		Q_ASSERT(count_ > 0);
		size_t const count = (size_t)count_;
		COMPILER_PRAGMA(poison count_);
		for (size_t i = 0; i < count; ++i) {
			layout->itemAt(i)->widget()->setSizePolicy(QSizePolicy{
				QSizePolicy::Expanding,
				QSizePolicy::Expanding,
				QSizePolicy::Frame,
			});
		}
	}
	setLayout(layout);
	setAttribute(Qt::WA_DeleteOnClose, false);
}

DebugPanel::~DebugPanel() {
	//
}
