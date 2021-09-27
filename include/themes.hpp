#pragma once

#include <QPalette>

namespace Themes {

enum class ThemeType : unsigned int {
	CARBON = 0,
	POLAR,
	// STEALTH,
	// SAKURA,
};

QPalette by_name(ThemeType const theme);
extern QPalette const light;
extern QPalette const dark;

}  // namespace Themes
