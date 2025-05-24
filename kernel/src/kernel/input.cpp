#include <kernel/input.hpp>
#include <kmemxx.hpp>
#include <mutex>

static std::mutex mouselock{false};
static kinput::MsData mousedata;

void kinput::writeMsData( const kinput::MsData *data )
{
    std::lock_guard lock(mouselock);
    kmemcpy(&mousedata, data, sizeof(kinput::MsData));
    // kinput::mousedata.x.store(data->x.load());
    // kinput::mousedata.y.store(data->y.load());
}

void kinput::readMsData( kinput::MsData *data )
{
    std::lock_guard lock(mouselock);
    kmemcpy(data, &mousedata, sizeof(kinput::MsData));
    // data->x.store(kinput::mousedata.x.load());
    // data->y.store(kinput::mousedata.y.load());
}


void kinput::triggerMouseEvent( uint32_t btn, uint32_t event )
{
    if (btn || event) {  }
}


