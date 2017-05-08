# NOTICE
a simple server lib, reactor based on epoll, create a tcp server easily just implement three funcs,
examples date.cc & date_client.cc are simple examples
# BUILD AND RUN
* mkdir build && cd build && cmake ..
* make
* cd examples

# QPS TEST

* accept_num: tcp connection nums
* event_num: io event nums, is request nums

```
[root@st01-nova-rath7-f02-sys7.st01.baidu.com examples]# ./date_server
FATAL   2017/05/07-16:42:25.623068 133b [date.cc:43] [print_data] [current_connection_nums:992], [sum_connection_nums:992], [sum_event_nums:4558062]
FATAL   2017/05/07-16:42:27.050049 133b [date.cc:43] [print_data] [current_connection_nums:992], [sum_connection_nums:992], [sum_event_nums:5025495]
FATAL   2017/05/07-16:42:27.514021 133b [date.cc:43] [print_data] [current_connection_nums:992], [sum_connection_nums:992], [sum_event_nums:5182507]
FATAL   2017/05/08-10:19:04.954794 133b [date.cc:43] [print_data] [current_connection_nums:992], [sum_connection_nums:992], [sum_event_nums:20970161951]
FATAL   2017/05/08-10:19:05.466801 133b [date.cc:43] [print_data] [current_connection_nums:992], [sum_connection_nums:992], [sum_event_nums:20970338723]
FATAL   2017/05/08-10:19:10.474771 133b [date.cc:43] [print_data] [current_connection_nums:992], [sum_connection_nums:992], [sum_event_nums:20972062250]
FATAL   2017/05/08-10:19:10.947728 133b [date.cc:43] [print_data] [current_connection_nums:992], [sum_connection_nums:992], [sum_event_nums:20972226181]

```

# QPS (pingpong测试)
* client端，4个event_loop线程，1000个连接，发送'h',收到server端的'w'后再发送'h'
* server端，1个event_loop线程专门处理io连接，4个event_loop线程处理io请求

**qps 346608**

**cpu内存使用率**
```
top - 11:31:30 up 177 days, 16:50,  6 users,  load average: 8.53, 7.86, 7.64
Tasks: 497 total,   1 running, 490 sleeping,   5 stopped,   1 zombie
Cpu(s): 20.1% us,  2.5% sy,  0.2% ni, 76.7% id,  0.0% wa,  0.0% hi,  0.4% si
Mem:  99045300k total, 98477608k used,   567692k free,  5911604k buffers
Swap:        0k total,        0k used,        0k free, 30011316k cached

  PID USER      PR  NI  VIRT  RES  SHR S %CPU %MEM    TIME+  COMMAND
   4923 root      20   0 39.2g  20g 1480 S 99.9 22.1   5301:45 date_server
    5243 zhangfan  20   0 39.1g  20g 1244 S 99.9 22.1   4377:17 date_client

```
**cpu sys系统态占用很高，用strace查看event_loop线程，一直在read，write，是系统调用太频繁导致**
