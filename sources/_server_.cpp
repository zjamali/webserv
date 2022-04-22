/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _server_.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdait-m <abdait-m@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/26 20:43:19 by abdait-m          #+#    #+#             */
/*   Updated: 2022/04/22 13:43:38 by abdait-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../headers/_server_.hpp"

webServer::webServer(configParser& _cp_):_socket_(0), _option_(false), _currPort_(0), _clientMaxBodyS_(0), _maxSfd_(0), _host_(""), _addrSize_(0), _chunkedReq_(false)
{
	this->_servers_.assign(_cp_.getServers().begin(), _cp_.getServers().end());
	this->_start_();
}

webServer::~webServer()
{
	for (std::vector<int>::iterator itC = this->_socketFds_.begin(); itC != this->_socketFds_.end(); ++itC)
		close((*itC));
}

void	webServer::_buildASocket_() 
{
	if ((this->_socket_ = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		throw (std::runtime_error("\033[1;31m----> [ERROR] Socket creation error !\033[0;37m"));
	if (fcntl(this->_socket_, F_SETFL, O_NONBLOCK) == -1)
		throw (std::runtime_error("\033[1;31m----> [ERROR] Non-blocking error for socket " + std::to_string(this->_socket_)+ "!\033[0;37m"));
	this->_option_ = 1;
	if (setsockopt(this->_socket_, SOL_SOCKET, SO_REUSEADDR, &this->_option_, sizeof(socklen_t)) == -1)
		throw (std::runtime_error("\033[1;31m----> [ERROR] Socket option error !\033[0;37m"));
	this->_addrSize_ = sizeof(this->_Saddr_);
	std::memset(&this->_Saddr_, 0, this->_addrSize_);
	this->_Saddr_.sin_family = AF_INET;
	this->_Saddr_.sin_port = htons(this->_currPort_);
	this->_Saddr_.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(this->_socket_, (struct sockaddr*)&this->_Saddr_, this->_addrSize_) == -1)
		throw std::runtime_error("\033[1;31m----> [ERROR] Binding Error [ Socket " + std::to_string(this->_socket_) + " and Host " + this->_host_ + ":" + std::to_string(this->_currPort_) + " ] !\033[0;37m");
	if (listen(this->_socket_, 2048))
		throw (std::runtime_error("\033[1;31m----> [ERROR] Listenning error for socket [ " + std::to_string(this->_socket_) + " ]\033[0;37m"));
	FD_SET(this->_socket_, &this->_setFDs_);
	this->_maxSfd_ = (this->_socket_ > this->_maxSfd_) ? this->_socket_ : this->_maxSfd_;
	this->_socketFds_.push_back(this->_socket_);
	std::cout << "\033[1;30m---> Socket " << this->_socket_ << " bind to " + this->_host_ + ":" << this->_currPort_ << "\033[0;37m\n";
}

void	webServer::_acceptingClientConnection_(int& _fdsocket)
{
	int _acceptedS_ = accept(_fdsocket, (struct sockaddr*)&this->_Caddr_, &this->_addrSize_);
	if (_acceptedS_ == -1)
		throw (std::runtime_error("\033[1;31m----> [ERROR] The connection from the client is rejected !"));
	std::cout << "\033[1;30m---> New connection established [ Server socket " << _fdsocket << " | Client socket "<< _acceptedS_ <<" | " << inet_ntoa(this->_Caddr_.sin_addr)<< ":"<< this->_Caddr_.sin_port << "\033[0;37m\n";
	if (fcntl(_acceptedS_, F_SETFL, O_NONBLOCK) == -1)
		throw (std::runtime_error("\033[1;31m----> [ERROR] Non-blocking error for socket" + std::to_string(_acceptedS_) + "!\033[0;37m"));
	FD_SET(_acceptedS_, &this->_setFDs_);
	FD_SET(_acceptedS_, &this->_writefds_);
	// set the max fd
	this->_maxSfd_ = (_acceptedS_ > this->_maxSfd_) ? _acceptedS_ : this->_maxSfd_;
	this->_clientsInfos_.insert(std::make_pair(_acceptedS_, ""));
	std::map<int, int>::iterator it = this->_clientServer_.find(_acceptedS_);
	if (it != this->_clientServer_.end()) 
		it->second = _fdsocket;
	else
		this->_clientServer_.insert(std::make_pair(_acceptedS_, _fdsocket));
}

bool	webServer::_checkServerSocket_(int& _fdsocket)
{
	for (std::vector<int>::iterator it = this->_socketFds_.begin(); it != _socketFds_.end(); it++)
	{
		if (_fdsocket == *it)
			return (true);
	}
	return (false);
}

void	webServer::_closeSocket_(int& _acceptedS_)
{
	close(_acceptedS_);
	FD_CLR(_acceptedS_, &this->_setFDs_);
	FD_CLR(_acceptedS_, &this->_writefds_);
	this->_clientsInfos_.erase(_acceptedS_);
	std::cout << "\033[1;30m---> Client with Socket [ " << _acceptedS_ << " ] disconnected !\033[0;37m" << std::endl;
}

void	webServer::_handlingClientConnection_(int& _fdsocket)
{
	int _acceptedS_ = _fdsocket;
	char _buffer_[BUFSIZE + 1];
	bzero(_buffer_, sizeof(_buffer_));
	int _rVal_ = recv(_acceptedS_, _buffer_, BUFSIZE, 0);
	if (_rVal_ > 0)
	{
		_buffer_[_rVal_] = '\0';
		std::cout << "\033[1;30m---> Receive data from client with socket : " << _acceptedS_ << " - IP = " << inet_ntoa(this->_Caddr_.sin_addr) << ":" << ntohs(this->_Caddr_.sin_port) << "\033[0;37m\n";
		std::map<int, std::string>::iterator _it = this->_clientsInfos_.find(_acceptedS_);
		if (_it != this->_clientsInfos_.end())
			_it->second.append(_buffer_, _rVal_);
		if (this->_handleRequest_(_it->second, _acceptedS_))
		{
			if (this->_chunkedReq_)
			{
				try
				{
					_it->second = this->_handleChunkedRequest_(_it->second);
				}
				catch(const std::exception& e)
				{
					std::cerr << e.what() << std::endl;
				}
			}
			HttpRequest _newReq_;
			_newReq_.setBuffer(_it->second);
			_newReq_.initRequest();
			this->_handleResponse_(_acceptedS_, _newReq_); 
			_it->second.clear();
		}
	}
	else if (_rVal_ == 0)
		this->_closeSocket_(_acceptedS_);
	else if (_rVal_ == -1)
		std::cout << "\033[1;31m----> [ERROR] An error occurred while reading data from socket [ " << _acceptedS_ <<" ]\033[0;37m\n";
}

void	webServer::_holdForConnections_()
{
	std::cout << "\033[1;30m---> Waiting for incoming connections \033[0;37m\n";
	while(true)
	{
		FD_ZERO(&this->_readfds_);
		this->_readfds_ = this->_setFDs_;
		struct timeval _time_ = {1, 0};
		int _selectRet_ = select(this->_maxSfd_ + 1, &this->_readfds_, &this->_writefds_, NULL, &_time_);
		if (_selectRet_ > 0)
		{
			for (int _fdsocket = 0; _fdsocket < this->_maxSfd_ + 1; _fdsocket++)
			{
				if (FD_ISSET(_fdsocket, &this->_readfds_))
				{
					bool _newC_ = this->_checkServerSocket_(_fdsocket);

					if (_newC_)
						this->_acceptingClientConnection_(_fdsocket);
					else 
						this->_handlingClientConnection_(_fdsocket);
				}
			}
		}
		else if (_selectRet_ == -1)
			throw (std::runtime_error("\033[1;31m----> [ERROR] Select Error !\033[0;37m"));
	}
}

void	webServer::_start_()
{ 
	FD_ZERO(&_setFDs_);
	FD_ZERO(&_writefds_);
	for (std::vector<serverData>::iterator serv = this->_servers_.begin(); serv != this->_servers_.end(); serv++)
	{
		this->_ports_ = serv->getPorts();
		this->_host_ = serv->getHost();
		for (std::set<int>::iterator port = this->_ports_.begin(); port!=this->_ports_.end(); port++)
		{
			this->_currPort_ = *port;
			try
			{
				this->_buildASocket_();
			}
			catch(const std::exception& e)
			{
				close(this->_socket_);
				std::cerr << e.what() << '\n';
			}
		}
	}
	this->_holdForConnections_();
}

void	webServer::_getClientMaxBodySize_(int&	_clientSocket_)
{
	std::map<int, int>::iterator iter = this->_clientServer_.find(_clientSocket_);
	struct sockaddr_in _addr_;
	std::memset(&_addr_, 0, sizeof(_addr_));
	if (getsockname(iter->second, (struct sockaddr *)&_addr_, &this->_addrSize_) == -1)
		throw (std::runtime_error("\033[1;31m----> [ERROR] Unable to fetch server informations !\033[0;37m"));
	for(std::vector<serverData>::iterator iter = this->_servers_.begin(); iter != this->_servers_.end(); iter++)
	{
		if (iter->getHost() == inet_ntoa(_addr_.sin_addr))
		{
			this->_ports_ = iter->getPorts();
			for(std::set<int>::iterator port = this->_ports_.begin(); port != this->_ports_.end(); port++)
			{
				if (*port == ntohs(_addr_.sin_port))
				{
					this->_respServer_ = (*iter);
					this->_respPort_  = *port;
					this->_clientMaxBodyS_ = this->_respServer_.getClientMaxBodySize() * 1024 * 1024;
					break;
				}
			}
		}
	}
}

bool	webServer::_handleRequest_(std::string& _buff, int _acceptedS_)
{
	// max body size:
	this->_getClientMaxBodySize_(_acceptedS_);
	if (_buff.find(D_CRLF) != std::string::npos)
	{
		std::string _reqHeaders_ = _buff.substr(0, _buff.find(D_CRLF) + 4);
		if (_reqHeaders_.find("Transfer-Encoding: chunked") != std::string::npos)
		{
			this->_chunkedReq_ = true;
			if (_buff.find(LAST_CHUNK) != std::string::npos)
				return (true);
			return (false);
		}
		else if (_reqHeaders_.find("Content-Length") != std::string::npos)
		{
			size_t _bodySize_ = std::stoi(_reqHeaders_.substr((int)(_reqHeaders_.find("Content-Length: ")) + 16));
			std::string _reqBody_ = _buff.substr(_buff.find(D_CRLF) + 4);
			
			if (_bodySize_ > (size_t)this->_clientMaxBodyS_)
				return (true);
			else if (_reqBody_.length() < _bodySize_)
				return (false);
			
		}
		return (true);
	}
	return (false);
}

void	webServer::_dropSocket_(int& fd)
{
	close(fd);
	FD_CLR(fd, &_writefds_);
	FD_CLR(fd, &_setFDs_);
}

bool	webServer::_NonHexChar_(std::string& _check)
{
	std::string hex = "0123456789abcdefABCDEF";
	for(size_t i = 0; i < _check.size(); i++)
		if (hex.find(_check[i]) == std::string::npos)
			return (true);
	return (false);
}

size_t	webServer::_getHexSizeOfChunk_(std::string&	_chunkBuff)
{
	_chunkBuff.pop_back();
	if (_NonHexChar_(_chunkBuff))
		throw (std::runtime_error("\033[1;31m----> [ERROR] Incorrect chunk size !\033[0;37m"));
	size_t _decimalSize_ = 0;
	std::stringstream hex(_chunkBuff);
	hex >> std::hex >> _decimalSize_;
	return (_decimalSize_);
}

std::string	webServer::_handleChunkedRequest_(std::string& _reqbuff)
{
	std::string _reqFixed_ = _reqbuff.substr(0, _reqbuff.find(D_CRLF) + 4);
	std::string _reqBody_ = _reqbuff.substr(_reqbuff.find(D_CRLF) + 4);
	size_t i = 0, _bodysize_ = 0;
	while (i < _reqBody_.size())
	{
		std::string tmp = _reqBody_.substr(i, std::string::npos);
		size_t _tmpLength_ = 0;
		std::string line("");
		std::stringstream _Sbody_(tmp);
		std::getline(_Sbody_, line);
		i += line.length() + 1;
		_tmpLength_ = this->_getHexSizeOfChunk_(line);
		_reqFixed_.append(_reqBody_.c_str() + i, _tmpLength_);
		i+= _tmpLength_ + 2;
		_bodysize_ += _tmpLength_;
	}
	this->_chunkedReq_ = false;
	return (_reqFixed_);
}

void	webServer::_handleResponse_(int& _acceptedS_, HttpRequest& _newReq_)
{
	if (FD_ISSET(_acceptedS_, &this->_writefds_))
	{
		HttpResponse	_responseObj_(_newReq_, this->_respServer_);
		std::string _response_("");
		size_t		_respSize_ = 0;
		_response_.append(_responseObj_.getResponse());
		_respSize_ = _response_.length();
		ssize_t	send_ret = send(_acceptedS_, _response_.c_str(), _respSize_, 0);
		if (send_ret == -1)
			std::cout << "\033[1;33m----> [ERROR] Can't send response !\033[0;37m" << std::endl;
		else if (send_ret == 0)
			std::cout << "\033[1;33m----> Response Buffer is empty 0 bytes was passed to send !\033[0;37m" << std::endl;
		_dropSocket_(_acceptedS_);
	}
}