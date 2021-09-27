#include "PropertiesSidebar.hpp"

PropertiesSidebar::PropertiesSidebar()
	: QFormLayout{}
	, test_property_name(QStringLiteral("Prop 1"))
	, test_property_widget{
		QStringLiteral("Value 1"),
	} {
	addRow(test_property_name, &test_property_widget);
}

PropertiesSidebar::~PropertiesSidebar() {
	//
}
