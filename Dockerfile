FROM alpine
RUN apk update
RUN apk upgrade
RUN apk add build-base g++ gcc cmake qt5-qtbase qt5-qtbase-dev qt5-qtwebsockets qt5-qtwebsockets-dev
WORKDIR /compile/
COPY ./ ./
RUN cmake -H. -B cmake-build-debug
RUN cmake --build cmake-build-debug --target bmstu_hw_5_server
RUN mv cmake-build-debug/Server/bmstu_hw_5_server /home/bmstu_hw_5_server
WORKDIR /home/
RUN rm -rf /compile
CMD [ "./bmstu_hw_5_server" ]