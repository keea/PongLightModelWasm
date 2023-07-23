# PongLightModelWasm

## Building
### Web
#### Download and install the Emscripten 
```
$ sh .devcontainer/install_emscripten.sh
```
If you prefer to install it manually, follow the steps below:
```
# Get the emsdk repo
$ git clone https://github.com/emscripten-core/emsdk.git

# Enter that directory
$ cd emsdk

# Fetch the latest version of the emsdk (not needed the first time you clone)
$ git pull

# Download and install the 3.1.43 SDK tools.
$ ./emsdk install 3.1.43

# Make the "3.1.43" SDK "active" for the current user. (writes .emscripten file)
$ ./emsdk activate 3.1.43

# Activate PATH and other environment variables in the current terminal
$ source ./emsdk_env.sh
```
- On Windows, run `emsdk` instead of `./emsdk`, and `emsdk_env.bat` instead of source `./emsdk_env.sh`.

- On Windows, if you use the `activate` command, the step of `emsdk_env.bat` is optional. 

#### Run `cmake` a build:
```
$ sh build_web.sh

# or

$ cd [Project Path]
$ emcmake cmake -B ./build
$ emmake cmake --build ./build --config Release
```
