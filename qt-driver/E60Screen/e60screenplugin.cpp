#include "e60screenplugin.h"

#include <QDebug>

#include "e60screen.h"

QStringList E60ScreenPlugin::keys() const
{
    return QStringList() << "e60screendriver";
}

QScreen* E60ScreenPlugin::create(const QString & key,
                                 int displayId)
{
    if (key.toLower() == "e60screendriver") {
        qDebug() << "creating E60 screen driver()";
        return new E60Screen(displayId);
    }
    return 0;
}


Q_EXPORT_PLUGIN2(E60Screen, E60ScreenPlugin)
