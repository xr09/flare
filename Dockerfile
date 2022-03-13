# Builder
FROM debian:11 as builder

RUN apt-get update && \
    apt-get install -y --no-install-recommends build-essential

COPY src /build
WORKDIR /build
RUN make

# Release
FROM gcr.io/distroless/cc

COPY --from=builder /build/flare .

ENTRYPOINT ["/flare"]
