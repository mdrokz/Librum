#include <qdiriterator.h>
#include <qfontdatabase.h>
#include <qqml.h>
#include <QApplication>
#include <QDateTime>
#include <QGuiApplication>
#include <QIcon>
#include <QLocale>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>
#include <QStandardPaths>
#include <QString>
#include <QTranslator>
#include <memory>
#include "book_dto.hpp"
#include "book_operation_status.hpp"
#include "dependency_injection.hpp"
#include "document_item.hpp"
#include "i_book_service.hpp"
#include "i_user_service.hpp"
#include "key_sequence_recorder.hpp"
#include "library_proxy_model.hpp"
#include "message_handler.hpp"
#include "app_info_controller.hpp"
#include "page_item.hpp"
#include "setting_groups.hpp"
#include "setting_keys.hpp"
#include "shortcuts_proxy_model.hpp"
#include "sidebar_state.hpp"
#include "tag_dto.hpp"
#include "user_controller.hpp"


using namespace adapters::controllers;
using namespace application::services;


void registerTypes();
void setupFonts();
void addTranslations();

int main(int argc, char* argv[])
{
    // clang-format off
    // App
    QApplication app(argc, argv);
    QGuiApplication::setOrganizationName("Librum-Reader");
    QGuiApplication::setOrganizationDomain("librumreader.com");
    QGuiApplication::setApplicationName("Librum");
    QQuickStyle::setStyle(QStringLiteral("Default"));

    QIcon icon(":/src/logo.ico");
    app.setWindowIcon(icon);

    qInstallMessageHandler(logging::messageHandler);

    addTranslations();
    setupFonts();

    // Register types
    qmlRegisterSingletonType(QUrl("qrc:/StyleSheet.qml"), "Librum.style", 1, 0, "Style");
    qmlRegisterSingletonType(QUrl("qrc:/IconSheet.qml"), "Librum.icons", 1, 0, "Icons");
    qmlRegisterSingletonType(QUrl("qrc:/Globals.qml"), "Librum.globals", 1, 0, "Globals");
    qmlRegisterType<adapters::data_models::LibraryProxyModel>("Librum.models", 1, 0, "LibraryProxyModel");
    qmlRegisterType<adapters::data_models::ShortcutsProxyModel>("Librum.models", 1, 0, "ShortcutsProxyModel");
    qmlRegisterType<cpp_elements::KeySequenceRecorder>("Librum.elements", 1, 0, "KeySequenceRecorder");
    qmlRegisterType<cpp_elements::PageItem>("Librum.elements", 1, 0, "PageItem");
    qmlRegisterType<cpp_elements::DocumentItem>("Librum.elements", 1, 0, "DocumentItem");
    qRegisterMetaType<adapters::dtos::BookDto>();
    qRegisterMetaType<adapters::dtos::TagDto>();


    // Authentication Stack
    auto* authenticationService =
        config::diConfig().create<application::IAuthenticationService*>();
    auto authenticationController =
        std::make_unique<AuthenticationController>(authenticationService);
    qmlRegisterSingletonInstance("Librum.controllers", 1, 0, "AuthController",
                                 authenticationController.get());

    // App Info Stack
    auto* appInfoService =
        config::diConfig().create<application::IAppInfoService*>();
    auto appInfoController =
        std::make_unique<AppInfoController>(appInfoService);
    qmlRegisterSingletonInstance("Librum.controllers", 1, 0, "AppInfoController",
                                 appInfoController.get());

    // User Stack
    auto* userService = config::diConfig().create<application::IUserService*>();
    auto userController = std::make_unique<UserController>(userService);
    qmlRegisterSingletonInstance("Librum.controllers", 1, 0, "UserController",
                                 userController.get());

    // Book Stack
    auto* bookService = config::diConfig().create<application::IBookService*>();
    auto bookController = std::make_unique<BookController>(bookService);
    qmlRegisterSingletonInstance("Librum.controllers", 1, 0, "BookController",
                                 bookController.get());

    // Settings Stack
    auto* settingsService = config::diConfig().create<application::ISettingsService*>();
    auto settingsController = std::make_unique<SettingsController>(settingsService);
    qmlRegisterSingletonInstance("Librum.controllers", 1, 0, "SettingsController",
                                 settingsController.get());

    // Sidebar
    auto sidebarState = std::make_unique<cpp_elements::SidebarState>();
    qmlRegisterSingletonInstance("Librum.elements", 1, 0, "SidebarState",
                                 sidebarState.get());

    // Enums
    qmlRegisterUncreatableMetaObject(application::book_operation_status::staticMetaObject, "Librum.controllers",
                                     1, 0, "BookOperationStatus",
                                     "This is an uncreatable enum!");
    
    qmlRegisterUncreatableMetaObject(application::setting_keys::staticMetaObject, "Librum.controllers",
                                     1, 0, "SettingKeys",
                                     "This is an uncreatable enum!");

    qmlRegisterUncreatableMetaObject(application::setting_groups::staticMetaObject, "Librum.controllers",
                                     1, 0, "SettingGroups",
                                     "This is an uncreatable enum!");

    qmlRegisterUncreatableMetaObject(application::error_codes::staticMetaObject, "Librum.controllers",
                                     1, 0, "ErrorCode",
                                     "This is an uncreatable enum!");


    // Setup login connections
    QObject::connect(
        authenticationService,
        &application::IAuthenticationService::loggedIn,
        bookService, &application::IBookService::setupUserData);

    QObject::connect(authenticationService, &application::IAuthenticationService::loggedOut,
                     bookService, &application::IBookService::clearUserData);


    QObject::connect(authenticationService, &application::IAuthenticationService::loggedIn,
                     userService, &application::IUserService::setupUserData);

    QObject::connect(authenticationService, &application::IAuthenticationService::loggedOut,
                     userService, &application::IUserService::clearUserData);


    QObject::connect(authenticationService, &application::IAuthenticationService::loggedIn,
                     settingsService, &application::ISettingsService::loadUserSettings);

    QObject::connect(authenticationService, &application::IAuthenticationService::loggedOut,
                     settingsService, &application::ISettingsService::clearUserData);


    // Setup other connections
    QObject::connect(userService, &application::IUserService::bookStorageDataUpdated,
                     bookService, &application::IBookService::updateUsedBookStorage);



    // Startup
    QQmlApplicationEngine engine;
    engine.addImportPath("qrc:/modules");
    engine.addImportPath(QCoreApplication::applicationDirPath() + "/src/presentation/qt_tree_view/qml/");

    QQuickStyle::setStyle("Basic");

    const QUrl url("qrc:/main.qml");
    QObject::connect(
        &engine, &QQmlApplicationEngine::objectCreated, &app,
        [url](QObject* obj, const QUrl& objUrl)
        {
            if(obj == nullptr && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
    engine.load(url);

    return QGuiApplication::exec();
    // clang-format on
}

void addTranslations()
{
    QTranslator translator;
    const QStringList uiLanguages = QLocale::system().uiLanguages();
    for(const QString& locale : uiLanguages)
    {
        const QString baseName = "Librum_" + QLocale(locale).name();
        if(translator.load(":/i18n/" + baseName))
        {
            QGuiApplication::installTranslator(&translator);
            break;
        }
    }
}

void loadFont(const QString& path);

void setupFonts()
{
    loadFont(":/resources/fonts/SF-Pro-Display-Bold.otf");
    loadFont(":/resources/fonts/SF-Pro-Display-Medium.otf");
    loadFont(":/resources/fonts/SF-Pro-Display-Regular.otf");
    loadFont(":/resources/fonts/SF-Pro-Display-Semibold.otf");

    QFont defaultFont("SF Pro Display");
    defaultFont.setLetterSpacing(QFont::AbsoluteSpacing, 0.1);
    QGuiApplication::setFont(defaultFont);
}

void loadFont(const QString& path)
{
    int result = QFontDatabase::addApplicationFont(path);
    if(result == -1)
        qWarning() << QString("Loading font file: %1 failed.").arg(path);
}
