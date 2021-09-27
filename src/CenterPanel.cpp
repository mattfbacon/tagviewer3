#include "CenterPanel.hpp"

CenterPanel::CenterPanel() : QLabel{ QStringLiteral("Center panel") } {
	setAlignment(Qt::AlignCenter);
}

CenterPanel::~CenterPanel() {
	//
}
