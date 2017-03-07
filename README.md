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
[root@st01-nova-rath7-f02-sys7.st01.xxxxxx examples]# ./date_server
FATAL   2017/03/07-16:42:37.855866 48aa [date.cc:39] [print_data] [g_accept_num:14768], [g_event_num:639512]
FATAL   2017/03/07-16:42:37.855937 48aa [epoll_poller.cpp:75] [poll] Interrupted system call
FATAL   2017/03/07-16:46:08.615061 48aa [date.cc:39] [print_data] [g_accept_num:64460], [g_event_num:3484281]
FATAL   2017/03/07-16:46:08.615089 48aa [epoll_poller.cpp:75] [poll] Interrupted system call
3484281639512FATAL   2017/03/07-16:51:28.131216 48aa [date.cc:39] [print_data] [g_accept_num:114415], [g_event_num:6479734]
FATAL   2017/03/07-16:51:28.131239 48aa [epoll_poller.cpp:75] [poll] Interrupted system call
FATAL   2017/03/07-16:55:15.735742 48aa [date.cc:39] [print_data] [g_accept_num:164026], [g_event_num:8715881]
FATAL   2017/03/07-16:55:15.735764 48aa [epoll_poller.cpp:75] [poll] Interrupted system call
```

**established tcp connections, max 7847**,尽管最大打开文件数目开了2w，由于处理新连接是一个线程，上不去。
```
[root@st01-nova-rath7-f02-sys7.st01.xx.com test]# lsof -p 18602 | grep ESTAB | wc -l
7031
[root@st01-nova-rath7-f02-sys7.st01.xx.com test]# lsof -p 18602 | grep ESTAB | wc -l
7847
[root@st01-nova-rath7-f02-sys7.st01.xx.com test]# lsof -p 18602 | grep ESTAB | wc -l
4349
[root@st01-nova-rath7-f02-sys7.st01.xx.com test]# lsof -p 18602 | grep ESTAB | wc -l
4357
[root@st01-nova-rath7-f02-sys7.st01.xx.com test]# lsof -p 18602 | grep ESTAB | wc -l
4355
[root@st01-nova-rath7-f02-sys7.st01.xx.com test]# kill -SIGUSR1 18602
[root@st01-nova-rath7-f02-sys7.st01.xx.com test]# kill -SIGUSR1 18602
[root@st01-nova-rath7-f02-sys7.st01.xx.com test]# lsof -p 18602 | grep ESTAB | wc -l
3497
[root@st01-nova-rath7-f02-sys7.st01.xx.com test]# lsof -p 18602 | grep ESTAB | wc -l
385
[root@st01-nova-rath7-f02-sys7.st01.xx.com test]# lsof -p 18602 | grep ESTAB | wc -l
385
[root@st01-nova-rath7-f02-sys7.st01.xx.com test]# kill -SIGUSR1 18602
[root@st01-nova-rath7-f02-sys7.st01.xx.com test]# lsof -p 18602 | grep ESTAB | wc -l
```

#测试客户端
开了3个client，每个client建立5000个长连接，循环100次，每个连接send一个消息 ,发完后，client sleep 10s,退出程序。由脚本每分钟启动一个这样的client，有3个脚本程序分别周期性启动client

**qps 最大13546，最小7056**
