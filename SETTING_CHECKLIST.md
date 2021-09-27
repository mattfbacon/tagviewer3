- declare setting name and default in Settings.hpp (possibly also Settings.cpp)

- declare slot and input in SettingsDialog.hpp
- init and add input in SettingsDialog.cpp
- add signal impl in SettingsDialog::SettingsDialog
- add assertion in SettingsDialog::show
- add slot impl in SettingsDialog.cpp
- use the impl in SettingsDialog::update_all_settings
