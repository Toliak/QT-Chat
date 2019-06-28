FROM alpine:3.9

# Create non-root user
RUN addgroup -S qt_chat && \
        adduser -S qt_chat -G qt_chat

# Install neccesary for start packages
RUN apk update && \
    apk add --no-cache qt5-qtbase \
                       qt5-qtwebsockets

# Deploy
WORKDIR /tmp/compile
COPY . .
RUN apk add --no-cache build-base \
                       g++ \
                       gcc \
                       cmake \
                       qt5-qtbase-dev \
                       qt5-qtwebsockets-dev && \
    cmake -H. -B build && \
    cmake --build build --target qt_chat_server && \
    mv build/Server/qt_chat_server /home/qt_chat/qt_chat_server && \
    apk del --no-cache build-base \
                       g++ \
                       gcc \
                       cmake \
                       qt5-qtbase-dev \
                       qt5-qtwebsockets-dev && \
    rm -rf /tmp/compile

WORKDIR /home/qt_chat
USER qt_chat
VOLUME /home/qt_chat/config
ENTRYPOINT ./qt_chat_server