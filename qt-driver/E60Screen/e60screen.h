#ifndef E60SCREEN_H
#define E60SCREEN_H

#include <QLinuxFbScreen>

struct epd_control_data;

class E60Screen : public QLinuxFbScreen
{
public:
    explicit E60Screen(int display_id);

    virtual void setDirty(const QRect&);
    virtual bool useOffscreen();

private:
    epd_control_data *ctrl_data;
    int fd;
};

#endif // E60SCREEN_H
