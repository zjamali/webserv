# include "_socket_.hpp"

_socket_::_socket_() { }

int		_socket_::_create_(int _port_, std::string _host_)
{
	// Creating socket :
	if ((this->_fdSocket_ = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	{
		// creation error throw an exception
		return (-1);
	}
	this->_addrSize_ = sizeof(this->_addr_);
	this->_bind_(_port_, _host_);
	return (this->_fdSocket_);
}

void	_socket_::_bind_(int _p, std::string _h)
{
	this->_addr_.sin_family = AF_INET;
	this->_addr_.sin_port = htons(_p);
	this->_addr_.sin_addr.s_addr = htonl(INADDR_ANY);
	// setSockopt ??
	if (bind(this->_fdSocket_ , (struct sockaddr *)&this->_addr_, this->_addrSize_))
	{
		// throw an exception for binding fail
	}
}

int		_socket_::_listen_()
{
	int val = (this->_fdSocket_ > 0) ? listen(this->_fdSocket_, 0) : 0;
	return (val);
}

const int&	_socket_::getSocket() const { return (this->_fdSocket_);}
struct sockaddr_in _socket_::getSockAddress() const{ return (this->_addr_); }