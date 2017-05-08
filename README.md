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

# QPS (本机pingpong测试)
* client端，1000个连接，发送'h',收到server端的'w'后再发送'h'
* server端，收到'h', 发送'w'

|client io thread nums|server io thread nums|qps|
|----|----|----|
|4|4|346608|
|6|6|422946|
|8|8|425742|

**cpu内存使用率**
```
top - 11:45:11 up 177 days, 17:04,  6 users,  load average: 7.09, 6.96, 7.20
Tasks: 507 total,   3 running, 496 sleeping,   5 stopped,   3 zombie
Cpu(s): 41.7% us, 36.7% sy,  0.2% ni,  3.9% id,  0.0% wa,  0.0% hi, 17.5% si
Mem:  99045300k total, 68927364k used, 30117936k free,  5930388k buffers
Swap:        0k total,        0k used,        0k free, 44113880k cached

  PID USER      PR  NI  VIRT  RES  SHR S %CPU %MEM    TIME+  COMMAND
   6081 root      20   0  458m  51m 1384 S 99.9  0.1   3:12.98 date_server
   11055 zhangfan  20   0  331m  56m 1236 S 99.9  0.1   2:47.69 date_client

```
**cpu sys系统态占用很高，用strace查看event_loop线程，一直在read，write，是系统调用太频繁导致**
```
[root@st01-nova-rath7-f02-sys7.st01.baidu.com ~]# strace -T -r -p 6082
Process 6082 attached
     0.000000 write(807, "w", 1)        = 1 <0.000015>
          0.000052 readv(75, [{"h\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"..., 10240}, {"\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"..., 65536}], 2) =
          1 <0.000010>
               0.000040 write(75, "w", 1)         = 1 <0.000017>
                    0.000036 epoll_wait(3, {{EPOLLIN, {u32=321, u64=321}}, {EPOLLIN, {u32=867, u64=867}}, {EPOLLIN, {u32=57, u64=57}}, {EPOLLIN, {u32=63, u64=63}}, {EPOLLIN, {u32=303, u64=303}}, {EPOLLIN,
                    {u32=201, u64=201}}, {EPOLLIN, {u32=891, u64=891}}, {EPOLLIN, {u32=105, u64=105}}, {EPOLLIN, {u32=45, u64=45}}, {EPOLLIN, {u32=159, u64=159}}}, 256, 1) = 10 <0.000009>
                         0.000052 readv(321, [{"h\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"..., 10240}, {"\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"..., 65536}], 2) =
                          1 <0.000007>
                               0.000027 write(321, "w", 1)        = 1 <0.000016>
                                    0.000031 readv(867, [{"h\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"..., 10240}, {"\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0"..., 65536}], 2) =
                                     1 <0.000007>

```
