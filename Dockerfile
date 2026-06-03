FROM debian:bookworm-slim

RUN apt-get update \
    && apt-get install -y --no-install-recommends build-essential make git ca-certificates \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /pipex

COPY . .

RUN make -C libft && make

CMD ["/bin/bash"]
