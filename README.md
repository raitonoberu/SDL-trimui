This is a SDL3 port of the Allwinner A133 support patch for SDL2. The original patch can be found [here](https://github.com/knulli-cfw/distribution/blob/knulli-main/board/batocera/allwinner/a133/patches/sdl2/001-add-pvr-ge8300-mali-driver.patch).

Things to consider:

- This is my first experience writing C code, so any improvements are more than welcome.
- The resolution is hardcoded to 1024x768. This may be changed in the future (see original patch).
- The driver seems to force VSync, at least I couldn't get it to output frames faster.

---

Simple DirectMedia Layer (SDL for short) is a cross-platform library
designed to make it easy to write multi-media software, such as games
and emulators.

You can find the latest release and additional information at:
https://www.libsdl.org/

Installation instructions and a quick introduction is available in
[INSTALL.md](INSTALL.md)

This library is distributed under the terms of the zlib license,
available in [LICENSE.txt](LICENSE.txt).

Enjoy!

Sam Lantinga (slouken@libsdl.org)
