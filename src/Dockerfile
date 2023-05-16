FROM alpine
RUN apk add ; apk add gcc make valgrind bash libc-dev --no-cache check-dev
COPY . .
WORKDIR /porject
ENTRYPOINT ["/bin/bash"]
