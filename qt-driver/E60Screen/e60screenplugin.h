#ifndef E60SCREENPLUGIN_H
#define E60SCREENPLUGIN_H

#include <QScreenDriverPlugin>

class E60ScreenPlugin : public QScreenDriverPlugin {
    Q_OBJECT
public:
    QScreen* create(const QString & key, int displayId);
    QStringList keys() const;
};

#endif // E60SCREENPLUGIN_H
