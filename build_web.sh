SAMPLE_DIRECTORY="$( cd "$( dirname "$0" )" && pwd -P )"
echo ${SAMPLE_DIRECTORY}

EMSDK=${SAMPLE_DIRECTORY}/.devcontainer/emsdk
echo ${EMSDK}

cmake -S ${SAMPLE_DIRECTORY} -B ${SAMPLE_DIRECTORY}/.web-build -DCMAKE_TOOLCHAIN_FILE="${EMSDK}/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake"
cmake --build ${SAMPLE_DIRECTORY}/.web-build --config Release