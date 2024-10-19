FROM ubuntu:latest

WORKDIR /app

COPY hello.cpp /app

RUN apt-get update && \
    apt-get install g++ -y && \
    g++ hello.cpp -o hello

CMD ["./hello"]
