#include <QApplication>
#include <QDir>
#include <QSettings>
#include <QStandardPaths>
#include "autorun.h"

// Paths
#ifdef Q_OS_LINUX
static QDir path(QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/.config/autostart");
static const QString file = "ckb.desktop";
static const QString internalFile(":/txt/ckb.desktop");
#elif defined(Q_OS_MACX)
static QDir path(QStandardPaths::writableLocation(QStandardPaths::HomeLocation) + "/Library/LaunchAgents");
static const QString file = "com.ckb.ckb.plist";
static const QString internalFile(":/txt/com.ckb.ckb.plist");
#endif
static const QString settingPath = "Program/DidLoginItem";

bool AutoRun::available(){
    // Allow autostart if the program is located in a system path
#ifdef Q_OS_LINUX
    return QDir::root().absoluteFilePath(QStandardPaths::findExecutable("ckb")) == qApp->applicationFilePath();
#elif defined(Q_OS_MACX)
    return qApp->applicationFilePath().startsWith("/Applications/ckb.app", Qt::CaseInsensitive);
#endif
}

bool AutoRun::once(){
    QSettings settings;
    return settings.value(settingPath).toBool();
}

bool AutoRun::isEnabled(){
    // Check if the file exists. If not, autostart is disabled.
    if(!path.exists() || !path.exists(file))
        return false;
    // If autostart is enabled, set the flag from once() (in case it hasn't been done yet)
    QSettings settings;
    settings.setValue(settingPath, true);
    return true;
}

void AutoRun::enable(){
    if(!available())
        return;
    // Copy file into place
    if(!path.exists())
        QDir::home().mkpath(path.absolutePath());
    QFile::copy(internalFile, path.absoluteFilePath(file));
    // Mark once() as done
    QSettings settings;
    settings.setValue(settingPath, true);
}

void AutoRun::disable(){
    if(!available())
        return;
    // Remove file
    QFile::remove(path.absoluteFilePath(file));
}
