/* -*- C++ -*- */
// Copyright maverick Inc. All Rights Reserved.
// Author : zhangfangjie (f22jay@163.com)
// Date 2016/03/08 17:41:38
// Breif :

#ifndef NET_FUNCTION_H
#define NET_FUNCTION_H
#include <functional>
#include <memory>

namespace net {
class Buffer;
class TcpConnection;
class IpAddress;
typedef std::shared_ptr<TcpConnection> TcpConnectionPtr;
// typedef std::function<void()> TimerCallback;
typedef std::function<void (int, IpAddress &)>NewConncetionCallack;
typedef std::function<void (const TcpConnectionPtr&)> ConnectionCallBack;
typedef std::function<void (const TcpConnectionPtr&)> CloseCallBack;
typedef std::function<void (const TcpConnectionPtr&)> WriteCompleteCallBack;
typedef std::function<void (const TcpConnectionPtr&, Buffer*)> MessageCallBack;
typedef std::function<void ()>  EventCallBack;

//todo
typedef std::function<void (const TcpConnectionPtr&)> WriteCallBack;


}//namespace net
#endif
