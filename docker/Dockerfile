FROM ubuntu:25.04

RUN apt update && apt install -y cmake clang libgl-dev libglx-dev libboost-all-dev libyaml-cpp-dev libmsgsl-dev libvulkan-dev vulkan-utility-libraries-dev libxcb1-dev libxcb-icccm4-dev libwayland-dev pkg-config wayland-protocols
RUN mkdir -p /build/cmake
WORKDIR /build
COPY . .
RUN cmake -B /build/cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_C_COMPILER=/usr/bin/clang -DCMAKE_CXX_COMPILER=/usr/bin/clang++ && \
    cmake --build /build/cmake --config Debug --target adelie_engine