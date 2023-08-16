FROM gcc:latest as build

RUN apt-get update && \
    apt-get install -y \
      libtbb \
      cmake

ADD CMakeLists.txt main.cpp /app/src/

WORKDIR /app/build

RUN cmake ../src && \
    cmake --build .

FROM ubuntu:latest

WORKDIR /app

COPY --from=build /app/build/freq .

ENTRYPOINT ["./freq"]
