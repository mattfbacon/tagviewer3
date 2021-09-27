#pragma once

#include <QLabel>
#include <QObject>

class CenterPanel final : public QLabel {
	Q_OBJECT
public:
	explicit CenterPanel();
	~CenterPanel();
};
