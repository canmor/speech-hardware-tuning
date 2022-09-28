FROM gcc:12 AS builder

RUN apt-get clean \
    && apt-get update \
    && apt-get install cmake -y \

WORKDIR /bench

COPY . .
RUN cmake . -DCMAKE_BUILD_TYPE=Release \
    && cmake --build . -j 8

CMD ["/bench/bench"]