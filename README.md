A simple echo server on UDP for me to practice network programming. To run
first start the server:
```
./udp_echo
```
then connect to it with `netcat`:
```
nc -u 127.0.0.1 8888
````
If you want to stop the server send tbe `SIGINT` signal to it:
```
ctrl + c
```
