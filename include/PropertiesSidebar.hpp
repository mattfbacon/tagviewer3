#pragma once

#include <QFormLayout>
#include <QLineEdit>
#include <QObject>
#include <QString>

class PropertiesSidebar final : public QFormLayout {
	Q_OBJECT
public:
	explicit PropertiesSidebar();
	~PropertiesSidebar();
protected:
	QString const test_property_name;
	// make pointer when adding more
	QLineEdit test_property_widget;
};
