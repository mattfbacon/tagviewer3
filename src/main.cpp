#include <QApplication>
#include <QCoreApplication>
#include <QLocale>
#include <QSettings>
#include <QStyleFactory>
#include <QTranslator>

#include "phantomstyle.h"
#include "qfonticon.h"

#include "MainWindow.hpp"
#include "Settings.hpp"

#include "definitions.hpp"
#include "icons.hpp"
#include "themes.hpp"

QStyle* original_style = nullptr;
std::unique_ptr<Settings> g_settings{ nullptr };

static QString const APP_NAME = QStringLiteral("TagViewer");

int main(int argc, char* argv[]) {
	using QCore = QCoreApplication;
	using QApp = QApplication;
	QCore::setAttribute(Qt::AA_EnableHighDpiScaling);
	QCore::setAttribute(Qt::AA_UseHighDpiPixmaps);

	QCore::setOrganizationName(APP_NAME);
	QCore::setOrganizationDomain(QStringLiteral("tagviewer.github.io"));
	QCore::setApplicationName(APP_NAME);

	g_settings = std::make_unique<Settings>();

	original_style = QStyleFactory::create(QStringLiteral("fusion"));

	QApp::setStyle(new PhantomStyle());
	QApp::setPalette(Themes::light);

	auto const application = std::make_unique<QApp>(argc, argv);
	application->setWindowIcon(QIcon{ QStringLiteral(":/icon.svg") });

	QFontIcon::addFont(QStringLiteral(":/MaterialIcons-Regular.ttf"));

	{
		auto const locale = std::make_unique<QLocale>();
		auto const translator = std::make_unique<QTranslator>();
		// QApplication::installTranslator, why take by pointer if you just make a copy?
		// Inquiring minds want to know. No mention of taking ownership or not in Qt docs,
		// and inspecting the source code was inconclusive...
		if (translator->load(*locale, EMPTY_STRING, EMPTY_STRING, QStringLiteral(":/i18n"))) {
			QApp::installTranslator(translator.get());
		}
	}

	Icons::load();

#define CUSTOM_TYPE(TYPE_NAME) \
	qRegisterMetaType<TYPE_NAME>(); \
	qRegisterMetaTypeStreamOperators<TYPE_NAME>(#TYPE_NAME);
#include "custom_types.hpp"

	auto const window = std::make_unique<MainWindow>();

	return QApp::exec();
}
