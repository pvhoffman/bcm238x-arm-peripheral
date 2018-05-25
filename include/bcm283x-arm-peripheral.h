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
            static constexpr uint32_t base_peripheral_physical_address = 0x3f000000;
            static constexpr uint32_t base_gpio_physical_address = base_peripheral_physical_address + 0x200000;
            static constexpr uint32_t memory_block_size = 4096;
            static constexpr uint32_t memory_page_size  = 4096;
            static constexpr uint32_t gpfsel0 = 0;
            static constexpr uint32_t gpset0 = 7;
            static constexpr uint32_t gpclr0 = 10;
            static constexpr uint32_t gplev0 = 13;
            static constexpr uint32_t gppud  = 37;
            static constexpr uint32_t gppudclk0 = 38;
            static constexpr uint32_t cycle_150 = 150;
            static constexpr const char* memory_device_path = "/dev/mem";
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

                    void _wait(const uint32_t c)
                    {
                        for(int i = 0; i < c; i++)
                        {
                            // volatile prevents optimization
                            // out of the loop.
                            asm volatile("nop");
                        }
                    }
                public:
                    enum class pin_direction { input, output };
                    enum class pin_pullstate { off = 0, pulldown, pullup };
                    enum class pin_initialstate {none, high, low };
                    pin_t(void* ptr, const int32_t n) : _num(n)
                    {
                        _paddr = static_cast<volatile uint32_t*>(ptr);
                    }
                    void setup(pin_direction direction = pin_direction::output, pin_pullstate pullstate = pin_pullstate::off, pin_initialstate initialstate = pin_initialstate::high)
                    {
                        if(direction == pin_direction::output)
                        {
                            if(pullstate != pin_pullstate::off)
                            {
                                throw std::runtime_error("Output pin cannot have pullup/pulldown");
                            }
                            // explicitly disable pullup/pulldown
                            uint32_t offset = const_defs::gppudclk0 + (_num / 32);
                            *(_paddr + const_defs::gppud) &= ~3;
                            _wait(const_defs::cycle_150);
                            *(_paddr + offset) = 1 << (_num % 32);
                            _wait(const_defs::cycle_150);
                            *(_paddr + const_defs::gppud) &= ~3;
                            *(_paddr + offset) = 0;

                            // set the inital state to high/low
                            offset = (initialstate == pin_initialstate::high) ? const_defs::gpset0 : const_defs::gpclr0;
                            offset += (_num / 32);
                            *(_paddr + offset) = 1 << (_num % 32);

                            // set pin as output
                            offset = const_defs::gpfsel0 + (_num / 10);
                            *(_paddr + offset) = (*(_paddr + offset) & ~(7 << ((_num % 10) * 3))) | (1 << ((_num % 10) * 3));
                        }
                        else
                        {
                            if(initialstate != pin_initialstate::none)
                            {
                                throw std::runtime_error("Initial high/low not valid for input pin.");
                            }
                            // set pullup/pulldown
                            uint32_t offset = const_defs::gppudclk0 + (_num / 32);
                            *(_paddr + const_defs::gppud) = (*(_paddr + const_defs::gppud) & ~3) | static_cast<uint32_t>(pullstate);
                            _wait(const_defs::cycle_150);
                            *(_paddr + offset) = 1 << (_num % 32);
                            _wait(const_defs::cycle_150);
                            *(_paddr + const_defs::gppud) &= ~3;
                            *(_paddr + offset) = 0;

                            // set pin as input
                            offset = const_defs::gpfsel0 + (_num / 10);
                            *(_paddr + offset) = (*(_paddr + offset) & ~(7 << ((_num % 10) * 3)));
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
            uint8_t*    _pmap;
            uint8_t*    _pmap_mem;
            uint32_t*   _gpio_map;

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
