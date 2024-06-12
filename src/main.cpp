#include "mainwindow.h"
#include "singleinstance.h"
#include <QApplication>
#include <QFontDatabase>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    // Set application information
    app.setApplicationName("Notes");
    app.setApplicationVersion(APP_VERSION);

#if defined(Q_OS_UNIX) && !defined(Q_OS_MACOS)
    app.setDesktopFileName(APP_ID);
#endif

#if QT_VERSION >= QT_VERSION_CHECK(5, 10, 0) && QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    app.setAttribute(Qt::AA_DisableWindowContextHelpButton);
#endif

    if (QFontDatabase::addApplicationFont(":/fonts/fontawesome/fa-solid-900.ttf") < 0)
        qWarning() << "FontAwesome Solid cannot be loaded !";

    if (QFontDatabase::addApplicationFont(":/fonts/fontawesome/fa-regular-400.ttf") < 0)
        qWarning() << "FontAwesome Regular cannot be loaded !";

    if (QFontDatabase::addApplicationFont(":/fonts/fontawesome/fa-brands-400.ttf") < 0)
        qWarning() << "FontAwesome Brands cannot be loaded !";

    if (QFontDatabase::addApplicationFont(":/fonts/material/material-symbols-outlined.ttf") < 0)
        qWarning() << "Material Symbols cannot be loaded !";

    // Load fonts from resources
    // Roboto
    QFontDatabase::addApplicationFont(":/fonts/roboto-hinted/Roboto-Bold.ttf");
    QFontDatabase::addApplicationFont(":/fonts/roboto-hinted/Roboto-Medium.ttf");
    QFontDatabase::addApplicationFont(":/fonts/roboto-hinted/Roboto-Regular.ttf");

    // Source Sans Pro
    QFontDatabase::addApplicationFont(":/fonts/sourcesanspro/SourceSansPro-Black.ttf");
    QFontDatabase::addApplicationFont(":/fonts/sourcesanspro/SourceSansPro-BlackItalic.ttf");
    QFontDatabase::addApplicationFont(":/fonts/sourcesanspro/SourceSansPro-Bold.ttf");
    QFontDatabase::addApplicationFont(":/fonts/sourcesanspro/SourceSansPro-BoldItalic.ttf");
    QFontDatabase::addApplicationFont(":/fonts/sourcesanspro/SourceSansPro-ExtraLight.ttf");
    QFontDatabase::addApplicationFont(":/fonts/sourcesanspro/SourceSansPro-ExtraLightItalic.ttf");
    QFontDatabase::addApplicationFont(":/fonts/sourcesanspro/SourceSansPro-Italic.ttf");
    QFontDatabase::addApplicationFont(":/fonts/sourcesanspro/SourceSansPro-Light.ttf");
    QFontDatabase::addApplicationFont(":/fonts/sourcesanspro/SourceSansPro-LightItalic.ttf");
    QFontDatabase::addApplicationFont(":/fonts/sourcesanspro/SourceSansPro-Regular.ttf");
    QFontDatabase::addApplicationFont(":/fonts/sourcesanspro/SourceSansPro-SemiBold.ttf");
    QFontDatabase::addApplicationFont(":/fonts/sourcesanspro/SourceSansPro-SemiBoldItalic.ttf");

    // Prevent many instances of the app to be launched
    QString name = "com.awsomeness.notes";
    SingleInstance instance;
    if (instance.hasPrevious(name)) {
        return EXIT_SUCCESS;
    }

    instance.listen(name);

    // Create and Show the app
    MainWindow w;
    w.show();

    // Bring the Notes window to the front
    QObject::connect(&instance, &SingleInstance::newInstance, &w,
                     [&]() { (&w)->setMainWindowVisibility(true); });

    return app.exec();
}
