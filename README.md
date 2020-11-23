# bootleg

Bootleg is a hackable game console made for indie devs and homebrewers.

## How it's built and what it supports

* Designed for Xbox One controllers, however everything that exposes itself to evdev is supported
  * xpadneo could potentially give us controller batterylevels, forcefeedback etc https://github.com/atar-axis/xpadneo/
* Designed for Raspberry Pi 4b 8GB
  * Great cooling and OC/overvolt is required (>=2.0GHz)
  * M.2/Sata SSD over USB3.0 as boot device is required
  * Minimum goal is for native games to run at 1080p30 or 720p60
  * All official titles will use this as a target specification.
* Additionally supports any x86_64 system with dedicated NVidia/AMD graphics
* Built on Linux Manjaro (without a preinstalled xorg/X11 stack)
* Custom Wayland compositor/shell using the Mir Abstraction Layer: BootlegCompositor*
* Custom Dashboard application, just like on Xbox and Playstation: BootlegDashboard
* Customized KIT/Odin/WIR technology stack that powers native games as well as the dashboard and overlays:
  * Efficient and powerful Vulkan-powered rendering everywhere
  * Great input support out of the box 
 * Potentially integrated emulator support out of the box; NES, SNES, N64, PSX, PS2, GC, PSP, Switch?
   * Requires extra steps for the user to install system files.
   * This is a very low-priority feature, however it is a big want

## Wanted changes:

* Once/if mesa gets VK_KHR_display_swapchain support for their v3dv driver (they already seem to have VK_KHR_display), we can completely remove the custom Wayland compositor and present/render directly to the monitor output via DRM, which should give us a lot of performance benefits as we could completely skip the overhead of a Wayland server and clients.
* This would remove any support for external/thirdparty applications like emulators though, unless we port them?

## Where we actually are now

- [X] Get a wayland compositor up and running
- [X] Iterate on Dashboard design until its lovely
- [ ] Port the KIT/Odin/WIR stack back to Linux; Input, System, Wayland WSI
- [ ] Port the KIT/Odin/WIR stack to ARM and make sure everything works
- [ ] Implement Dashboard and get it to MVP-status
- [ ] Implement the glue between Dashboard, Compositor and native games (KIT/Odin/WIR)
- [ ] Start thinking about creating a case and physical product design

### Current design prototype:

![Splash](https://haikatekk.se/BootlegDashboardSplash.png)
![3.0](https://haikatekk.se/BootlegDashboard3.0.png)

