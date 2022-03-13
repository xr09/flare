## Flare

Little tool to test how signals are (or are not) reaching containers.


### Why use Flare

The idea is to be able to determine if SIGTERM signals are reaching the processes inside containers for them to be able to [terminate gracefully](https://cloud.google.com/blog/products/containers-kubernetes/kubernetes-best-practices-terminating-with-grace).

After running this image the flare process will print its PID and wait for signals.

Signals captured:

- SIGUSR1: will print to stdout
- SIGTERM: will print to stdout and exit the program


### Usage

Flare takes 1 positional parameter called sleep_time, this will be the amount of time the program will take to close its unfinished business when receiving a TERM signal, emulating this way a real world workload.

```
❯ ./flare 5
Flare running with PID 91082

received SIGTERM
the program will take 5s to finish correctly
...
SUCCESS: the program had enough time to finish
```

### Flare on Docker

Pass the desired sleep time as the CMD.

```shell
$ docker run --rm -ti --name flare xr09/flare 5
Flare running with PID 1

received SIGUSR1
received SIGUSR1
received SIGUSR1
received SIGTERM
the program will take 5s to finish correctly
...
SUCCESS: the program had enough time to finish

```

Send USR1 with `docker kill -s USR1 flare`

Send TERM with `docker stop flare` or `docker kill -s TERM flare`

After receiving a SIGTERM it will print it then wait 5 seconds and shutdown.


### Flare on Kubernetes

This tools is convenient for debugging cases where `terminationGracePeriodSeconds` is needed on Kubernetes.

```yaml
apiVersion: apps/v1
kind: Deployment
metadata:
  name: flare
  labels:
    app: flare
spec:
  replicas: 3
  selector:
    matchLabels:
      app: flare
  template:
    metadata:
      labels:
        app: flare
    spec:
      terminationGracePeriodSeconds: 60
      containers:
      - name: flare
        image: xr09/flare
        args: ["45"]
```


Proceed to delete one of the pods:

```
❯ kubectl get po
NAME                     READY   STATUS    RESTARTS   AGE
flare-777c45d94c-75lmx   1/1     Running   0          11s
flare-777c45d94c-w6kxm   1/1     Running   0          11s
flare-777c45d94c-xxsdn   1/1     Running   0          11s

❯ kubectl delete pod flare-777c45d94c-xxsdn
pod "flare-777c45d94c-xxsdn" deleted

```

It should have enough time to close by itself:

```
❯ kubectl logs -f flare-777c45d94c-xxsdn
Flare running with PID 1

received SIGTERM
the program will take 45s to finish correctly
...
SUCCESS: the program had enough time to finish
```
