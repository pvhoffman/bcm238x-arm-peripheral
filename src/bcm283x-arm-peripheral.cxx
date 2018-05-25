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
gpio_t::gpio_t() : _map_fd(0), _pmap(nullptr), _pmap_mem(nullptr), _gpio_map(nullptr)
{
    _open_memory_device();
    _map_memory_device();
}
//---------------------------------------------------------
gpio_t::~gpio_t()
{
    if(_pmap != nullptr)
    {
        munmap(_gpio_map, const_defs::memory_block_size);
        _pmap = nullptr;
    }
    if(_map_fd > 0)
    {
        close(_map_fd);
        _map_fd = 0;
    }
    if(_pmap_mem)
    {
        delete [] _pmap_mem;
        _pmap_mem = nullptr;

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

    // allocate and align on page boundry
    _pmap_mem = new uint8_t[const_defs::memory_block_size + (const_defs::memory_page_size -1)];
    if(nullptr == _pmap_mem)
    {
        throw std::runtime_error("Cannot allocate memory to map virtual address space.");
    }
    _pmap = _pmap_mem;
    uint32_t boundry = reinterpret_cast<uint32_t>(_pmap_mem);
    if(boundry % const_defs::memory_page_size)
    {
        _pmap += const_defs::memory_page_size - (boundry % const_defs::memory_page_size);
    }
    // map to the gpio location
    void* memmap = mmap(static_cast<void*>(_pmap)
            , const_defs::memory_block_size
            , PROT_READ | PROT_WRITE
            , MAP_SHARED | MAP_FIXED
            , _map_fd
            , const_defs::base_gpio_physical_address);
    if(MAP_FAILED == memmap)
    {
        throw std::runtime_error("Cannot map memory location.");
    }
    _gpio_map = static_cast<uint32_t*>(memmap);
}
//---------------------------------------------------------
gpio_t::pin_t gpio_t::pin(const uint32_t n)
{
    gpio_t::pin_t res(_gpio_map, n);
    return res;
}
//---------------------------------------------------------
} // namespace bcm283x_arm_peripheral
//---------------------------------------------------------

