FROM ubuntu:22.04 AS builder

ENV WS=/home/workspace
ENV PROJECT_NAME=mavlink_mqtt

RUN apt-get update && apt-get install -y \
    software-properties-common \
    python3 \
    python3-pip

RUN apt-get update && apt-get install -y \
    git \
    build-essential \
    cmake \
    libssl-dev

WORKDIR ${WS}
RUN git clone https://github.com/eclipse/paho.mqtt.cpp

WORKDIR ${WS}/paho.mqtt.cpp
RUN git checkout v1.6.0 && git submodule update --init
RUN cmake -Bbuild -H. -DPAHO_WITH_MQTT_C=ON -DPAHO_BUILD_EXAMPLES=ON
RUN cmake --build build/ --target install

WORKDIR ${WS}/${PROJECT_NAME}
COPY . .

RUN cmake -B build
RUN cmake --build build

FROM ubuntu:22.04 as final

COPY --from=builder /usr/local/lib /usr/local/lib
COPY --from=builder /home/workspace/mavlink_mqtt/build/mavlink_mqtt /usr/local/bin/
RUN ldconfig

ENTRYPOINT ["mavlink_mqtt"]
