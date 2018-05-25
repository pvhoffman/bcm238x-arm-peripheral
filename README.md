# bcm283x_arm_peripheral

This is a C++ library with which to interface the BCM283X arm GPIO peripheral, e.g. Raspberry Pi, on Linux.

Example program to blink an LED, linking to this library:
```
#include <iostream>
#include <stdexcept>

#include <bcm283x-arm-peripheral.h>

int main()
{
    using namespace bcm283x_arm_peripheral;

    try
    {
        gpio_t gpio;

        std::cout << "Setting pin 7 direction to default output." << std::endl;
        gpio.pin(4).setup();

        while(true)
        {
           // set pin 7 low
            std::cout << "Setting pin 7 low." << std::endl;
            gpio.pin(4).clear();
            sleep(3);
            // set pin 7 high
            std::cout << "Setting pin 7 high." << std::endl;
            gpio.pin(4).set();
            sleep(3);
        }
    }
    catch(std::runtime_error& rex)
    {
        std::cerr << "Runtime error: [" << rex.what() << "]" << std::endl;
    }
    return 0;
}
```

## Getting Started

This library can be compiled directly on a Raspberry Pi or cross compiled using the toolchain provided by Raspberry Pi.  The cross compile method is preferable to compiling directly on the Raspberry Pi because the C++ compiler may not be available on the device or you may wish to keep your device clean to limit the surface for attacks by malicious users.

### Prerequisites

CMake >= Version 3.1
```
pvhoffman@host:~$ sudo apt-get install cmake
```

Raspberry Pi cross compiler toolchain
```
pvhoffman@host:~$ git clone https://github.com/raspberrypi/tools ~/tools
```

### Building

Add the cross compiler tool chain to the PATH of the host build machine. 
```
pvhoffman@host:~$ PATH=~/tools/arm-bcm2708/gcc-linaro-arm-linux-gnueabihf-raspbian-x64/bin:$PATH
```

Edit the CMakeLists.txt changing the CMAKE_FIND_ROOT_PATH to the correct of the libc for the toolchain

Execute cmake within the top level directory to generate the build system
```
pvhoffman@host:~$ cmake .
```

Execute make to build the library
```
pvhoffman@host:~$ make
```
## Authors

* **Paul Hoffman** - *Initial work* - [pvhoffman](https://github.com/pvhoffman)

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details


## Referenced Material

[BCM2835 ARM Peripherals manual](BCM2835-ARM-Peripherals.pdf)

