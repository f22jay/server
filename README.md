# NOTICE
a simple server lib, reactor based on epoll, create a tcp server easily just implement three funcs,
examples date.cc is a simple examples
#BUILD AND RUN
* mkdir build && cd build && cmake ..
* make
* cd examples

#QPS TEST

**accept_num: tcp connection nums**
**event_num:io event nums, is request nums**

```
[root@st01-nova-rath7-f02-sys7.st01.baidu.com examples]# ./date_server
FATAL   2017/04/25-10:10:08.137673 16d1 [date.cc:40] [print_data] [g_accept_num:9814], [g_event_num:808871]
FATAL   2017/04/25-10:10:08.138641 16d1 [epoll_poller.cpp:75] [poll] Interrupted system call
FATAL   2017/04/25-10:10:26.921950 16d1 [date.cc:40] [print_data] [g_accept_num:19806], [g_event_num:2645239]
FATAL   2017/04/25-10:10:26.921990 16d1 [epoll_poller.cpp:75] [poll] Interrupted system call
FATAL   2017/04/25-10:10:41.847822 16d1 [date.cc:40] [print_data] [g_accept_num:29514], [g_event_num:4964250]
FATAL   2017/04/25-10:10:41.847866 16d1 [epoll_poller.cpp:75] [poll] Interrupted system call
FATAL   2017/04/25-10:11:00.584699 16d1 [date.cc:40] [print_data] [g_accept_num:37575], [g_event_num:8935445]
FATAL   2017/04/25-10:11:00.584735 16d1 [epoll_poller.cpp:75] [poll] Interrupted system call
FATAL   2017/04/25-10:11:24.641740 16d1 [date.cc:40] [print_data] [g_accept_num:46290], [g_event_num:15043000]
FATAL   2017/04/25-10:11:24.641776 16d1 [epoll_poller.cpp:75] [poll] Interrupted system call
FATAL   2017/04/25-10:11:46.904354 16d1 [date.cc:40] [print_data] [g_accept_num:48950], [g_event_num:20611772]
FATAL   2017/04/25-10:11:46.904390 16d1 [epoll_poller.cpp:75] [poll] Interrupted system call
FATAL   2017/04/25-10:11:56.847259 16d1 [date.cc:40] [print_data] [g_accept_num:48950], [g_event_num:23087247]
FATAL   2017/04/25-10:11:56.847295 16d1 [epoll_poller.cpp:75] [poll] Interrupted system call
FATAL   2017/04/25-10:12:35.985343 16d1 [date.cc:40] [print_data] [g_accept_num:49562], [g_event_num:33079294]
FATAL   2017/04/25-10:12:35.985384 16d1 [epoll_poller.cpp:75] [poll] Interrupted system call
FATAL   2017/04/25-10:13:58.176232 16d1 [date.cc:40] [print_data] [g_accept_num:49562], [g_event_num:53741159]
FATAL   2017/04/25-10:13:58.176277 16d1 [epoll_poller.cpp:75] [poll] Interrupted system call
FATAL   2017/04/25-10:17:42.105243 16d1 [date.cc:40] [print_data] [g_accept_num:49562], [g_event_num:109778608]
FATAL   2017/04/25-10:17:42.105286 16d1 [epoll_poller.cpp:75] [poll] Interrupted system call
```

#测试客户端
*开了5个client，每个客户端10000个长连接, 发送"hello"，收到"world"后继续发送，类似pingpong
*server端，1个event_loop线程专门处理io连接，8个event_loop线程处理io请求

**qps 254715**

**cpu内存使用率**
```
Tasks: 570 total,   8 running, 557 sleeping,   4 stopped,   1 zombie
Cpu(s): 37.3% us, 35.6% sy,  0.2% ni, 10.7% id,  0.0% wa,  0.0% hi, 16.2% si
Mem:  99045300k total, 92683548k used,  6361752k free,  4331400k buffers
Swap:        0k total,        0k used,        0k free, 45445868k cached

PID USER      PR  NI  VIRT  RES  SHR S %CPU %MEM    TIME+  COMMAND
5841 root      20   0 1614m 1.3g 1436 S 99.9  1.4  36:17.89 date_server
32152 workflow  20   0  776m 248m 5108 S 99.3  0.3 354458:27 mysqld
6009 root      20   0  415m 373m 1168 R 84.8  0.4   8:17.41 date_client
11377 root      20   0  408m 363m 1172 R 84.1  0.4   7:00.46 date_client
8471 root      20   0  415m 372m 1168 R 83.1  0.4   7:57.32 date_client
8788 root      20   0  415m 370m 1168 R 81.1  0.4   7:33.24 date_client
13214 root      20   0  390m 347m 1172 R 73.8  0.4   6:00.07 date_client
18891 zhangfan  20   0 1676m 1.6g 5356 R 65.9  1.7  27159:40 emacs


```
