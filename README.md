# bootleg

Bootleg is a hackable game console made for indie devs and homebrewers.

## How it's built and what it supports

* Designed for Xbox One controllers, however everything that exposes itself to evdev is supported
  * xpadneo could potentially give us controller batterylevels, forcefeedback etc https://github.com/atar-axis/xpadneo/
* Designed for Raspberry Pi 4b 8GB
  * Overclocked to 2.1GHz CPU and 900MHz GPU
  * Cooled by tower heatsink/fan cooler
  * 240GB SATA SSD
  * Minimum goal is for native games to run at 1080p30 or 720p60 (vkQuake3 testing shows we have plenty of headroom already!)
* Additionally supports any x86_64 system with dedicated NVidia/AMD graphics
* Built on a custom Linux distribution (without a preinstalled xorg/X11 stack)
* Renders directly to a display surface, completely bypassing any windowing system
* Custom Dashboard application, just like on Xbox and Playstation: BootlegDashboard
* Customized KIT/Odin/WIR technology stack that powers native games as well as the dashboard and overlays:
  * Efficient and powerful Vulkan-powered rendering everywhere
  * Great input support out of the box 
 * Integrated Steam Link, if possible. Pretty low priority, but if easy then just do it.
  * Potentially integrated emulator support out of the box; NES, SNES, N64, PSX, PS2, GC, PSP, Switch?
   * Requires extra steps for the user to install system files.
   * Requires lot of work
   * Is a big want

## Where we actually are now

- [X] Preliminary case design
- [X] Testfitting and full internal system build
- [X] Port WIR to Linux and ARM64
- [X] Port Odin to Linux and ARM64
- [X] Port KIT to Linux and ARM64
- [X] Get Dashboard running on X11 for development purposes
- [X] Temporarily workaround lack of `shaderSampledImageArrayDynamicIndexing` support (mesa issue #3943)
- [ ] Implement input in Linux using evdev (/dev/input)
- [ ] Optimize performance to get at least 60 FPS @ 1080p in dashboard 
- [ ] Get Dashboard to MVP status
- [ ] Implement direct rendering using VK_KHR_display


### End goal
![Design](https://haikatekk.se/combine3.0.png)
![Internals](https://haikatekk.se/combine3.0rear.png)

# Development notes

## Stable OC

```
2.1 GHz arm_freq
900 MHz v3d_freq
6 overvolt
28c idle (and running OpenArena)
36c heavy CPU load

vkQuake3 (High settings) on X11:
800x480 250fps+
1280x720 120fps+
1920x1080 55fps

```

## Mesa build cheatsheet

Clone repo:

```
git clone https://gitlab.freedesktop.org/mesa/mesa
```

Install buildtools:
```
sudo apt install meson flex bison
sudo -H pip install Mako
```

Install dependencies:
```
sudo apt build-dep mesa
sudo apt install libxcb-shm0-dev
```

X11 devel:
```
CFLAGS="-mcpu=cortex-a72" CXXFLAGS="-mcpu=cortex-a72" meson --prefix /usr -Dplatforms=x11 -Dvulkan-drivers=broadcom -Ddri-drivers= -Dgallium-drivers=kmsro,v3d,vc4 -Dbuildtype=release build-system/
```

Optimized headless:
```
CFLAGS="-O2 -march=armv8-a+crc+simd -mtune=cortex-a72" CXXFLAGS="-O2 -march=armv8-a+crc+simd -mtune=cortex-a72" meson --prefix=/usr -Dglx=disabled -Dplatforms= -Dllvm=disabled -Dvulkan-drivers=broadcom -Ddri-drivers='' -Dgallium-drivers=v3d,vc4,kmsro -Dbuildtype=release build-system/
```

Testing it:
```
sudo apt install vulkan-utils
vkcube
vulkaninfo
```

## Getting vkQuake3 to build

```
CFLAGS="-O2 -march=armv8-a+crc+simd -mtune=cortex-a72 -DSDL_DISABLE_IMMINTRIN_H" CXXFLAGS="-O2 -march=armv8-a+crc+simd -mtune=cortex-a72 -DSDL_DISABLE_IMMINTRIN_H"  make -j4
```




