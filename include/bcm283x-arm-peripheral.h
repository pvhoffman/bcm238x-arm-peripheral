#ifndef __BCM283X_ARM_PERIPHERAL_H__
#define __BCM283X_ARM_PERIPHERAL_H__ 1
#include <unistd.h>
#include <stdint.h>
#include <memory>

namespace bcm283x_arm_peripheral
{
    class const_defs
    {
        public:
            static constexpr uint32_t base_peripheral_physical_address = 0x20000000;
            static constexpr uint32_t base_gpio_physical_address = base_peripheral_physical_address + 0x200000;
            static constexpr const char* memory_device_path = "/dev/mem";
            static constexpr uint32_t memory_block_size = 4096;
            static constexpr uint32_t gpset0 = 7;
            static constexpr uint32_t gpclr0 = 10;
            static constexpr uint32_t gplev0 = 13;
            constexpr const_defs(){}
    };

    class gpio_t
    {
        public:
            class pin_t
            {
                private:
                    volatile uint32_t* _paddr;
                    int32_t _num;
                    pin_t() {}
                public:
                    enum class pin_direction { input, output };
                    pin_t(void* ptr, const int32_t n) : _num(n)
                    {
                        _paddr = static_cast<volatile uint32_t*>(ptr);
                    }
                    void set_direction(pin_direction dir)
                    {
                        const uint32_t mem_offset = _num / 10;
                        const uint32_t bit_offset = (_num % 10) * 3;

                        if(dir == pin_direction::input)
                        {
                            *(_paddr + mem_offset) &= ~(7 << bit_offset);
                        }
                        else if(dir == pin_direction::input)
                        {
                            *(_paddr + mem_offset) |= (1 << bit_offset);
                        }
                    }
                    void set() 
                    {
                        *(_paddr + const_defs::gpset0) = 1 << _num;
                    }
                    void clear()
                    {
                        *(_paddr + const_defs::gpclr0) = 1 << _num;
                    }
                    int32_t read()
                    {
                        return *(_paddr + const_defs::gplev0) &= (1 << _num);
                    }
            };

        private:
            int32_t     _map_fd;
            void*       _pmap;

            void _open_memory_device();
            void _map_memory_device();
        public:
            gpio_t();
            virtual ~gpio_t();

            pin_t pin(const uint32_t n);

    };
}
// end namespace bcm283x_arm_peripheral
#endif //!__BCM283X_ARM_PERIPHERAL_H__ 1
