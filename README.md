# NOTICE
a simple server lib, reactor based on epoll, create a tcp server easily just implement three funcs,
examples date.cc is a simple examples
#BUILD AND RUN
* mkdir build && cd build && cmake ..
* make
* cd examples

#QPS TEST

?**accept_num: tcp connection nums**
**event_num:io event nums, is request nums**

```
[root@st01-nova-rath7-f02-sys7.st01.baidu.com examples]# ./date_server
FATAL   2017/04/24-16:57:20.456276 7c6f [date.cc:39] [print_data] [g_accept_num:4928], [g_event_num:2763556]
FATAL   2017/04/24-16:57:20.456343 7c6f [epoll_poller.cpp:75] [poll] Interrupted system call
FATAL   2017/04/24-16:57:32.023107 7c6f [date.cc:39] [print_data] [g_accept_num:4928], [g_event_num:3630272]
FATAL   2017/04/24-16:57:32.023141 7c6f [epoll_poller.cpp:75] [poll] Interrupted system call
FATAL   2017/04/24-16:57:52.263166 7c6f [date.cc:39] [print_data] [g_accept_num:4928], [g_event_num:5113002]
FATAL   2017/04/24-16:57:52.263202 7c6f [epoll_poller.cpp:75] [poll] Interrupted system call
FATAL   2017/04/24-17:01:40.837832 7c6f [date.cc:39] [print_data] [g_accept_num:4928], [g_event_num:21949160]
FATAL   2017/04/24-17:01:40.837868 7c6f [epoll_poller.cpp:75] [poll] Interrupted system call
FATAL   2017/04/24-17:01:49.828768 7c6f [date.cc:39] [print_data] [g_accept_num:4928], [g_event_num:22623990]
FATAL   2017/04/24-17:01:49.828810 7c6f [epoll_poller.cpp:75] [poll] Interrupted system call
FATAL   2017/04/24-17:04:15.501938 7c6f [date.cc:39] [print_data] [g_accept_num:4928], [g_event_num:33309137]
FATAL   2017/04/24-17:04:15.501965 7c6f [epoll_poller.cpp:75] [poll] Interrupted system call
```

#测试客户端
开了1个client，建立5000个长连接, 发送一次message，收到后继续发送，类似pingpong

**qps 83272**

**cpu内存使用率**
```

  PID USER      PR  NI  VIRT  RES  SHR S %CPU %MEM    TIME+  COMMAND
  31855 root      20   0  410m 242m 1432 S 99.9  0.3  20:37.63 date_server
  32364 zhangfan  20   0  278m 244m 1140 R 99.7  0.3  11:45.30 date_client
```
