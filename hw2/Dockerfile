FROM gcc as builder

COPY . /src
WORKDIR /src
RUN make && mkdir /app && cp ./myshell /app/

FROM ubuntu:20.04
COPY --from=builder /app /app
WORKDIR /app

RUN apt-get update && \
    apt-get install -y valgrind

