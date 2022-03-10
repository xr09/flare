## Flare

Little tool to test how signals are (or are not) reaching containers.


### Why use Flare

The idea is to be able to determine if SIGTERM signals are reaching the processes inside containers for them to be able to [terminate gracefully](https://cloud.google.com/blog/products/containers-kubernetes/kubernetes-best-practices-terminating-with-grace).

After running this image the flare process will print its PID and wait for signals.

Signals captured:

- SIGUSR1: will print to stdout
- SIGTERM: will print to stdout and exit the program

### Process output

```shell
$ docker run --rm -ti --name flare xr09/flare
Flare running with PID 1

received SIGUSR1
received SIGUSR1
received SIGTERM
SUCCESS: program exits correctly
```

Send USR1 with `docker kill -s USR1 flare`

Send TERM with `docker stop flare` or `docker kill -s TERM flare`

After receiving a SIGTERM it will print it then wait 15 seconds and shutdown.

This tools is convenient for debugging cases where `terminationGracePeriodSeconds` is needed on Kubernetes.

```yaml
apiVersion: apps/v1
kind: Deployment
metadata:
  name: flare
spec:
  replicas: 1
  template:
    spec:
      containers:
      - name: flare
        image: xr09/flare
    terminationGracePeriodSeconds: 60
```

### ROADMAP:

- Pass `sleep time` as a parameter, it's hard coded to 15s right now (this sleep emulates the terminating work done by the program when receiving a SIGTERM)
