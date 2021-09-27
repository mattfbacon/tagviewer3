#include <QColor>
#include <QDebug>
#include <QPalette>

#include "themes.hpp"

namespace {

struct ThemeColors {
	QColor window, text, disabledText, brightText, highlight, highlightedText, base, alternateBase, shadow, button, disabledButton;
};

QPalette themeColorsToPalette(ThemeColors const& colors) {
	QPalette pal;
	pal.setColor(QPalette::Window, colors.window);
	pal.setColor(QPalette::WindowText, colors.text);
	pal.setColor(QPalette::Text, colors.text);
	pal.setColor(QPalette::ButtonText, colors.text);
	if (colors.brightText.isValid()) {
		pal.setColor(QPalette::BrightText, colors.brightText);
	}
	pal.setColor(QPalette::Disabled, QPalette::WindowText, colors.disabledText);
	pal.setColor(QPalette::Disabled, QPalette::Text, colors.disabledText);
	pal.setColor(QPalette::Disabled, QPalette::ButtonText, colors.disabledText);
	pal.setColor(QPalette::Base, colors.base);
	pal.setColor(QPalette::AlternateBase, colors.alternateBase);
	if (colors.shadow.isValid()) {
		pal.setColor(QPalette::Shadow, colors.shadow);
	}
	pal.setColor(QPalette::Button, colors.button);
	pal.setColor(QPalette::Highlight, colors.highlight);
	pal.setColor(QPalette::HighlightedText, colors.highlightedText);
	if (colors.disabledButton.isValid()) {
		pal.setColor(QPalette::Disabled, QPalette::Button, colors.disabledButton);
	}
	// Used as the shadow text color on disabled menu items
	pal.setColor(QPalette::Disabled, QPalette::Light, Qt::transparent);
	return pal;
};

}  // namespace

namespace Themes {

QPalette by_name(ThemeType theme) {
	ThemeColors colors;
	switch (theme) {
		case ThemeType::CARBON: {
			QColor const window{ 60, 61, 64 };
			QColor const button{ 74, 75, 80 };
			QColor const base{ 46, 47, 49 };
			QColor const alternateBase{ 41, 41, 43 };
			QColor const text{ 208, 209, 212 };
			QColor const highlightedText{ 0x2d'2c'27 };
			QColor disabledText{ 0xa8'89'8a'8c };
			colors.window = window;
			colors.text = text;
			colors.disabledText = disabledText;
			colors.base = base;
			colors.alternateBase = alternateBase;
			colors.shadow = base;
			colors.button = button;
			colors.disabledButton = button.darker(107);
			colors.brightText = Qt::white;
			colors.highlight = button.lighter(107);
			colors.highlightedText = text;
			break;
		}
		case ThemeType::POLAR: {
			QColor const snow{ 251, 252, 254 };
			QColor const bright{ 237, 236, 241 };
			QColor const lessBright{ 234, 234, 238 };
			// QColor const dimmer{ 221, 221, 226 };
			QColor const text{ 18, 18, 24 };
			QColor const disabledText{ 140, 140, 145 };
			colors.window = bright;
			colors.highlight = bright.darker(107);
			colors.highlightedText = text;
			colors.base = snow;
			colors.alternateBase = lessBright;
			colors.button = bright;
			colors.text = text;
			colors.disabledText = disabledText;
			break;
		}
			// case ThemeType::STEALTH: {
			// 	QColor window(30, 31, 32);
			// 	QColor button(41, 42, 44);
			// 	QColor base(23, 24, 25);
			// 	QColor alternateBase(19, 19, 22);
			// 	QColor text(212, 209, 208);
			// 	QColor highlight(211, 210, 208);
			// 	QColor highlightedText(0x2d2c27);
			// 	QColor disabledText(0x60a4a6a8);
			// 	colors.window = window;
			// 	colors.text = text;
			// 	colors.disabledText = disabledText.darker(150);
			// 	colors.base = base;
			// 	colors.alternateBase = alternateBase;
			// 	colors.shadow = base;
			// 	colors.button = button;
			// 	colors.disabledButton = button.darker(107);
			// 	colors.brightText = Qt::white;
			// 	colors.highlight = highlight;
			// 	colors.highlightedText = highlightedText;
			// 	break;
			// }
			// case ThemeType::SAKURA: {
			// 	QColor callout(156, 112, 160);
			// 	QColor bright(252, 234, 243);
			// 	QColor lessBright(242, 234, 237);
			// 	QColor dimmer(255, 219, 250);
			// 	QColor text(24, 18, 18);
			// 	QColor disabledText(145, 140, 140);
			// 	colors.window = bright;
			// 	colors.highlight = callout;
			// 	colors.highlightedText = QColor(255, 255, 255);
			// 	colors.base = QColor(255, 247, 252);
			// 	colors.alternateBase = lessBright;
			// 	colors.button = dimmer;
			// 	colors.text = text;
			// 	colors.disabledText = disabledText;
			// 	break;
			// }
	}
	return themeColorsToPalette(colors);
}

}  // namespace Themes
