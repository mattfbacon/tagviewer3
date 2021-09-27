#pragma once

#include <QFormLayout>
#include <QObject>
#include <QToolButton>

class FiltersSidebar final : public QFormLayout {
	Q_OBJECT
public:
	explicit FiltersSidebar();
	~FiltersSidebar();
protected:
	// make pointer when adding more
	QToolButton test_button;
};
