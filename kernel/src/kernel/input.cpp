#include <kernel/input.hpp>
#include <mutex>

std::mutex mouselock{false};
kinput::MsData kinput::mousedata = {0, 0, 0, 0, 0};

void kinput::writeMsData( const kinput::MsData *data )
{
    mouselock.lock();
    kinput::mousedata = *data;
    mouselock.unlock();
}

void kinput::readMsData( kinput::MsData *data )
{
    mouselock.lock();
    *data = kinput::mousedata;
    mouselock.unlock();
}


void kinput::triggerMouseEvent( uint32_t btn, uint32_t event )
{
    if (btn || event) {  }
}


