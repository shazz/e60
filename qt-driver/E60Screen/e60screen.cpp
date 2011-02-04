#include "e60screen.h"

#include <qplatformdefs.h>

#include "s3c_epd_userland.h"

E60Screen::E60Screen(int display_id) :
    QLinuxFbScreen(display_id), ctrl_data(new epd_control_data)
{
    // TODO vraiment pas propre
    QString dev = QLatin1String("/dev/fb0");
    if (access(dev.toLatin1().constData(), R_OK|W_OK) == 0)
        fd = QT_OPEN(dev.toLatin1().constData(), O_RDWR);

    ctrl_data->x=0;
    ctrl_data->y=0;
    ctrl_data->data_x=0;
    ctrl_data->data_y=0;
    ctrl_data->dfmt=2;
    ctrl_data->width=600;
    ctrl_data->height=800;
    ctrl_data->erase_flag=0;
    ctrl_data->buf_index=0;
    ctrl_data->async_mtd=0;
    ctrl_data->overlap_check=0;
    ctrl_data->update_only=0;
    ctrl_data->type=eFULL;
}

void E60Screen::setDirty(const QRect &r)
{
    ctrl_data->x = r.left();
    ctrl_data->y = r.top();
    ctrl_data->width = r.width();
    ctrl_data->height = r.height();
    // TODO send iotcl
    ioctl(fd, 0x4004405A, ctrl_data);
}

bool E60Screen::useOffscreen()
{
    return false;
}
