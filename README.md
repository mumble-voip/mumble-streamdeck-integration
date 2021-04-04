# mumble-streamdeck-integration

A plugin for integrating the Mumble client with Elgato's Stream Deck

## Requirements

In order for the plugin to work, you have to have the [JSON Bridge for Mumble](https://github.com/mumble-voip/mumble-json-bridge) set up on your
machine. This means that the respective Mumble plugin needs to be installed (and enabled) inside Mumble and the corresponding CLI needs to be in your
PATH.

## Building

### Dependencies

In order to build the Stream Deck plugin, you require the libraries that are specified in the `vcpkg.json` file. You can either install these manually
or use [vcpkg](https://docs.microsoft.com/en-us/cpp/build/vcpkg?view=msvc-160) to obtain them. To do so, simply run
```bash
vcpkg install
```
from the repository's root.

If you installed the dependencies via vcpkg, you'll have to specify the respective toolchain file when invoking cmake as written below.

If you want the build to produce a ready-to-distribute packaged version of the plugin, you'll also have to install Elgato's distribution tool. You can
download it [for Windows](https://developer.elgato.com/documentation/stream-deck/distributiontool/DistributionToolWindows.zip) or
[for macOS](https://developer.elgato.com/documentation/stream-deck/distributiontool/DistributionToolMac.zip).


### Build

1. Create a build directory: `mkdir build`
2. Change into that directory: `cd build`
3. Invoke cmake with the desired options (see below): `cmake ..`
4. Build the code: `cmake --build .`

Options can be passed in the format `-D<option>=<value>`. Available options are
- `static`: Causes static versions of the Boost libraries to be used (Try this if Boost isn't found but you have it installed). Example: `-Dstatic=ON`
- `enable-packaging`: Enable packaging support. Use this if you want to package the plugin. Example: `-Denable-packaging=ON`
- `STREAMDECK_DISTRIBUTION_TOOL`: The path to Elgato's dsitribution tool. Setting this explicitly is not required, if the tool is in PATH. Example:
  `-DSTREAMDECK_DISTRIBUTION_TOOL=C:\Users\bla\Downloads\DistributionTool.exe`

### Packaging

If you enabled packaging when running cmake, you can run
```bash
cmake --build . --target package
```
from your build directory in order to package the plugin.

The packaged version of the plugin will reside in the directory `bundled` under the repository's root after a successful run of this command.
