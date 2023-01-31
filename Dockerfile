# Builder
FROM gcc:12 as builder

COPY src /build
WORKDIR /build
RUN make

# Release
FROM gcr.io/distroless/cc

COPY --from=builder /build/flare .

ENTRYPOINT ["/flare"]
