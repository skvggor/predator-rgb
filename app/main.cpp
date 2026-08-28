#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>

#include "controller.h"

int main(int argc, char* argv[]) {
    QGuiApplication application(argc, argv);
    QQuickStyle::setStyle(QStringLiteral("Material"));

    predator::Controller controller;

    QQmlApplicationEngine engine;
    engine.rootContext()->setContextProperty(QStringLiteral("controller"), &controller);

    QObject::connect(&engine, &QQmlApplicationEngine::warnings, [](const QList<QQmlError>& warnings) {
        for (const auto& warning : warnings) {
            fprintf(stderr, "QML WARNING: %s\n", warning.toString().toUtf8().constData());
        }
    });

    engine.loadFromModule("predator_rgb", "Main");

    fprintf(stderr, "rootObjects count: %d\n", static_cast<int>(engine.rootObjects().size()));
    if (engine.rootObjects().isEmpty()) {
        fprintf(stderr, "Failed to load QML module predator_rgb/Main\n");
        return -1;
    }

    return application.exec();
}
