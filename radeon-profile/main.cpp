#include "radeon_profile.h"
#include <QApplication>
#include <QTranslator>


int main(int argc, char *argv[])
{
    qDebug() << "Creating application object";

    QApplication app(argc, argv);
    QSettings *settings = getUserSettings();

    QTranslator translator;
    QLocale locale;

    QStringList uiLanguages = QLocale::system().uiLanguages();
    QString overrideLanguage = settings->value(QLatin1String("language")).toString();

    if (!overrideLanguage.isEmpty()) {
        uiLanguages.prepend(overrideLanguage);
    }

    for (QString language : qAsConst(uiLanguages)) {
        locale = QLocale(language);

        if (translator.load(locale, "strings", ".")
            || translator.load(locale, "strings", ".", QApplication::applicationDirPath())
            || translator.load(locale, "strings", ".", translationPath)) {
            app.installTranslator(&translator);
            break;
        }
        else if (language == QLatin1String("C") /* language == "English" */) {
            // use built-in
            break;
        }
        else if (language.startsWith(QLatin1String("en")) /* "English" is built-in */) {
            // use built-in
            break;
        }
        else {
            qWarning() << "Translation not found.";
        }
    }

    qDebug() << "Creating radeon_profile";
    radeon_profile w;

    return app.exec();
}
