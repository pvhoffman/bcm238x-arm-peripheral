#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <stdexcept>

#include <bcm283x-arm-peripheral.h>
//---------------------------------------------------------
namespace bcm283x_arm_peripheral
{
//---------------------------------------------------------
gpio_t::gpio_t() : _map_fd(0), _pmap(nullptr)
{
    _open_memory_device();
    _map_memory_device();
}
//---------------------------------------------------------
gpio_t::pin_t gpio_t::pin(const uint32_t n)
{
    gpio_t::pin_t res(_pmap, n);
    return res;
}
gpio_t::~gpio_t()
{
    if(_pmap != nullptr)
    {
        munmap(_pmap, const_defs::memory_block_size);
        _pmap = nullptr;
    }
    if(_map_fd > 0)
    {
        close(_map_fd);
    }
}
//---------------------------------------------------------
void gpio_t::_open_memory_device()
{
    _map_fd = open(const_defs::memory_device_path, O_RDWR | O_SYNC);
    if(_map_fd < 0)
    {
        throw std::runtime_error("Cannot open memory device.");
    }
}
//---------------------------------------------------------
void gpio_t::_map_memory_device()
{
    if(_map_fd <= 0)
    {
        throw std::runtime_error("Memory device not open.");
    }
    _pmap = mmap(nullptr
            , const_defs::memory_block_size
            , PROT_READ | PROT_WRITE
            , MAP_SHARED
            , _map_fd
            , const_defs::base_gpio_physical_address);
    if(_pmap == nullptr)
    {
        throw std::runtime_error("Cannot map memory location.");
    }
}
//---------------------------------------------------------
} // namespace bcm283x_arm_peripheral
//---------------------------------------------------------
