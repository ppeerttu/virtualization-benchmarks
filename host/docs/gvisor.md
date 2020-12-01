# gVisor

1. Install Docker according to [official instructions](https://docs.docker.com/engine/install/ubuntu/)
2. [Install gVisor](https://gvisor.dev/docs/user_guide/install/)
3. [Configure Docker](https://gvisor.dev/docs/user_guide/quick_start/docker/)

Verify it works:

```
docker run --runtime=runsc -it ubuntu dmesg
```
