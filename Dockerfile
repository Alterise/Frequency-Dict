FROM gcc:latest as build

RUN apt-get update && \
    apt-get install -y \
      libtbb-dev \
      cmake

ADD CMakeLists.txt main.cpp /app/src/

WORKDIR /app/build

RUN cmake ../src && \
    cmake --build . && \
    mv ./freq ..

workdir /app

ENTRYPOINT ["./freq"]
