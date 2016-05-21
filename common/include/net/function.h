
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>

namespace net {
class Buffer;
class TcpConnection;
typedef boost::shared_ptr<TcpConnection> TcpConnectionPtr;
// typedef boost::function<void()> TimerCallback;
typedef boost::function<void (const TcpConnectionPtr&)> ConnectionCallback;
typedef boost::function<void (const TcpConnectionPtr&)> CloseCallback;
typedef boost::function<void (const TcpConnectionPtr&)> WriteCompleteCallback;
typedef boost::function<void (const TcpConnectionPtr&,
                              Buffer*)> MessageCallback;

}//namespace net


















