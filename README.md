# bootleg

Bootleg is a hackable game console made for indie devs and homebrewers.

## How it's built and what it supports

* Designed for Xbox One controllers, however everything that exposes itself to evdev is supported
  * xpadneo could potentially give us controller batterylevels, forcefeedback etc https://github.com/atar-axis/xpadneo/
* Designed for Raspberry Pi 4b 8GB
  * Great cooling and OC/overvolt is required (>=2.0GHz)
  * Sata SSD over USB3.0 as boot device is required
  * Minimum goal is for native games to run at 1080p30 or 720p60 (vkQuake3 testing shows we have plenty of headroom already!)
  * All official titles will use this as a target specification.
* Additionally supports any x86_64 system with dedicated NVidia/AMD graphics
* Built on a custom Linux distribution (without a preinstalled xorg/X11 stack)
* ~~Custom Wayland compositor/shell using the Mir Abstraction Layer: BootlegCompositor~~
* Renders directly to a display surface, completely bypassing any windowing system
* Custom Dashboard application, just like on Xbox and Playstation: BootlegDashboard
* Customized KIT/Odin/WIR technology stack that powers native games as well as the dashboard and overlays:
  * Efficient and powerful Vulkan-powered rendering everywhere
  * Great input support out of the box 
 * Potentially integrated emulator support out of the box; NES, SNES, N64, PSX, PS2, GC, PSP, Switch?
   * Requires extra steps for the user to install system files.
   * This is a very low-priority feature, however it is a big want
 * Integrated Steam Link, if possible. Pretty low priority, but if easy then just do it.

## Wanted changes:

* ~~Once/if mesa gets VK_KHR_display_swapchain support for their v3dv driver (they already seem to have VK_KHR_display), we can completely remove the custom Wayland compositor and present/render directly to the monitor output via DRM, which should give us a lot of performance benefits as we could completely skip the overhead of a Wayland server and clients.~~
* Apparently, we only need the `VK_KHR_display` to create a display surface using `vkCreateDisplayPlaneSurfaceKHR`. This is excellent news and the compositor can probably be thrown out the window!

## Where we actually are now

- [X] Get a wayland compositor up and running
- [X] Iterate on Dashboard design until its lovely
- [ ] Port the KIT/Odin/WIR stack back to Linux; Input, System, Wayland WSI
- [ ] Port the KIT/Odin/WIR stack to ARM and make sure everything works
- [ ] Implement Dashboard and get it to MVP-status
- [ ] Implement the glue between Dashboard, Compositor and native games (KIT/Odin/WIR)
- [X] Start thinking about creating a case and physical product design

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

vkQuake3 on X11:
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

# Getting vkQuake3 to build

```
CFLAGS="-O2 -march=armv8-a+crc+simd -mtune=cortex-a72 -DSDL_DISABLE_IMMINTRIN_H" CXXFLAGS="-O2 -march=armv8-a+crc+simd -mtune=cortex-a72 -DSDL_DISABLE_IMMINTRIN_H"  make -j4
```




