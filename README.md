# Wi-Fi network synchronization
An algorith made to emulate the operation of a shopping's Wi-Fi network where only 1 client can use the internet per time and the rest of it must stay in a queue of a given size (that size is defined by parameter in the execution of the code). 
The synchronization is made using **semaphores** and **threads** (that are the clients) from the `pthread.h` library.

# How to use
1. Compile `web-sync.c` using:
```
gcc -pthread web-sync.c -o web-sync
```
2. Now, execute the `web-sync` file. You'll need to pass the queue size. Ex.:
```
./web-sync 5 
```
`5` being the queue's size.

3. The output for the above command would be like:
```
-------------------------------------------------------

            Shopping XYZ's Wi-fi network            

- Queue size: 5

-------------------------------------------------------

-------------------------------------------------------
[Client 1] is trying to connect!

Clients waiting:
---------------------
Slot [0] - Client 1
Slot [1] - 
Slot [2] - 
Slot [3] - 
Slot [4] - 
-------------------------------------------------------

-------------------------------------------------------
[Client 1] estabilished connection with the network!

Clients waiting:
---------------------
Slot [0] - 
Slot [1] - 
Slot [2] - 
Slot [3] - 
Slot [4] - 
-------------------------------------------------------

-------------------------------------------------------
[Client 2] is trying to connect!

Clients waiting:
---------------------
Slot [0] - Client 2
Slot [1] - 
Slot [2] - 
Slot [3] - 
Slot [4] - 
-------------------------------------------------------

-------------------------------------------------------
[Client 2] estabilished connection with the network!

Clients waiting:
---------------------
Slot [0] - 
Slot [1] - 
Slot [2] - 
Slot [3] - 
Slot [4] - 
-------------------------------------------------------

-------------------------------------------------------
[Client 3] is trying to connect!

Clients waiting:
---------------------
Slot [0] - Client 3
Slot [1] - 
Slot [2] - 
Slot [3] - 
Slot [4] - 
-------------------------------------------------------

                          ...

```

*Obs.: the execution only stops when you interrupt it (using `ctrl+c` for example).
