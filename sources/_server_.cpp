/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _server_.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdait-m <abdait-m@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/26 20:43:19 by abdait-m          #+#    #+#             */
/*   Updated: 2022/04/13 23:37:50 by abdait-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "../headers/_server_.hpp"

webServer::webServer(configParser& _cp_)
{
	this->_servers_.assign(_cp_.getServers().begin(), _cp_.getServers().end());
	this->_start_();
}

webServer::~webServer()
{
	
}

void	webServer::_buildASocket_() 
{
	/* code */
	// creating a socket this socket is used for accepting connections not for exchanging the data{ :
	if ((this->_socket_ = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		throw (std::runtime_error("Socket creation error !"));
	// Set the server socket to non-blocking mode .
	// file descriptors are by default blocking , so need to change it to non-blocking mode
	// we do that by taking the fd of first arg and set its flag to non-blocking mode using fcntl
	if (fcntl(this->_socket_, F_SETFL, O_NONBLOCK) == -1)
		throw (std::runtime_error("Non-blocking error for socket " + std::to_string(this->_socket_)+ "!"));
	this->_option_ = true;
	if (setsockopt(this->_socket_, SOL_SOCKET, SO_REUSEADDR, &this->_option_, sizeof(socklen_t)))
		throw (std::runtime_error("Socket option error !"));
	// binding the socket:
	this->_addrSize_ = sizeof(this->_Saddr_);
	std::memset(&this->_Saddr_, 0, this->_addrSize_);
	this->_Saddr_.sin_family = AF_INET;
	this->_Saddr_.sin_port = htons(this->_currPort_);
	this->_Saddr_.sin_addr.s_addr = htonl(INADDR_ANY);
	if (bind(this->_socket_, (struct sockaddr*)&this->_Saddr_, this->_addrSize_) == -1)
		throw std::runtime_error("Binding Error [ Socket " + std::to_string(this->_socket_) + " and Host " + this->_host_ + " ] !");
	// listenning:
	if (listen(this->_socket_, 0))
		throw (std::runtime_error("Listenning error for socket [ " + std::to_string(this->_socket_) + " ]"));
	FD_SET(this->_socket_, &this->_setFDs_);
	// this is for select function we turn in always the max fd.
	this->_maxSfd_ = (this->_socket_ > this->_maxSfd_) ? this->_socket_ : this->_maxSfd_;
	this->_socketFds_.push_back(this->_socket_);
	// }
	
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
			// error check :
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
	// waiting for the connection :
	struct timeval _time_ = {1, 0};
	while(true)
	{
		FD_ZERO(&this->_readfds_);
		this->_readfds_ = this->_setFDs_;
		// On successful return, each set is modified such that 
		// it contains only the file descriptors that are ready for I/O of the type delineated by that set
		// the first parameter in select should be the highest-numbered file descriptor in any of the three sets
		// plus 1 so we check the max fd num
		// we use FD_ISSET to check for the fds that are ready 
		//select uses descriptor sets, typically an array of integers,
		// with each bit in each integer corresponding to a descriptor. FD_ZERO turns off all the bits of the fds (= 0)
		// and every fd that is ready for write or read select turns on its bit () 
		// usleep(2000);
		int _selectRet_ = select(this->_maxSfd_ + 1, &this->_readfds_, &this->_writefds_, NULL, &_time_);
		if (_selectRet_ > 0)
		{
			for (int _fdsocket = 0; _fdsocket < this->_maxSfd_ + 1; _fdsocket++)
			{
				// look for the ready discriptors (socket)
				if (FD_ISSET(_fdsocket, &this->_readfds_))
				{
					bool _newC_ = false;
					for (std::vector<int>::iterator it = this->_socketFds_.begin(); it != _socketFds_.end(); it++)
					{
						// if the server socket is the ready one so we need to create the new connection with client
						if (_fdsocket == *it)
						{
							_newC_ = true;
							break ;
						}
					}
					// check for _newC_ : accepting the client connection ...
					if (_newC_)
					{
						// accept the new connection and get the socket for exchanging the data 
						int _acceptedS_ = accept(_fdsocket, (struct sockaddr*)&this->_Caddr_, &this->_addrSize_);
						if (_acceptedS_ == -1)
							throw (std::runtime_error("The connection from the client is rejected !"));
						std::cout << "New connection established [ Server socket " << _fdsocket << " | Client socket "<<" | " << inet_ntoa(this->_Caddr_.sin_addr)<< ":"<< this->_Caddr_.sin_port << std::endl;
						if (fcntl(_acceptedS_, F_SETFL, O_NONBLOCK) == -1)
							throw (std::runtime_error("Non-blocking error for socket" + std::to_string(_acceptedS_) + "!"));
						FD_SET(_acceptedS_, &this->_setFDs_);
						FD_SET(_acceptedS_, &this->_writefds_);
						// set the max fd
						this->_maxSfd_ = (_acceptedS_ > this->_maxSfd_) ? _acceptedS_ : this->_maxSfd_;
						this->_clientsInfos_.insert(std::make_pair(_acceptedS_, ""));
						std::map<int, int>::iterator i = this->_clientServer_.find(_acceptedS_);
						if (i != this->_clientServer_.end()) 
							i->second = _fdsocket; // set server socket to the client node 
						else
							this->_clientServer_.insert(std::make_pair(_acceptedS_, _fdsocket)); // create a new node of client server
						
					}
					else // for reading the request
					{
						int _acceptedS_ = _fdsocket;
						// handling the request that sent and return the correct response :
						char _buffer_[BUFSIZE + 1];
						bzero(_buffer_, sizeof(_buffer_));
						// read from client socket 
						int _rVal_ = recv(_acceptedS_, _buffer_, BUFSIZE, 0);
						std::cout << "Receive data from client with socket : " << _acceptedS_ << " - IP = " << inet_ntoa(this->_Caddr_.sin_addr) << ":" << ntohs(this->_Caddr_.sin_port) << std::endl;
						if (_rVal_ > 0)
						{
							_buffer_[_rVal_] = '\0';
							// look for the client socket inside the map and add the body to its mapped value:
							std::map<int, std::string>::iterator _it = this->_clientsInfos_.find(_acceptedS_);
							if (_it != this->_clientsInfos_.end())
								_it->second.append(_buffer_, _rVal_);
							// std::cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n";
							// std::cout << _buffer_ << std::endl;
							// std::cout << _rVal_ << std::endl;
							// std::cout << "^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^\n";
							
							/* FOR HANDLING CHUNKED TRANSFER-CODING
								length := 0
								read chunk-size, chunk-extension (if any) and CRLF
								while (chunk-size > 0) {
									read chunk-data and CRLF
									append chunk-data to entity-body
									length := length + chunk-size
									read chunk-size and CRLF
								}
								read entity-header
								while (entity-header not empty) {
									append entity-header to existing header fields
									read entity-header
								}
								Content-Length := length
								Remove "chunked" from Transfer-Encoding
							*/
							/*
								Example of chunked body:
									4\r\n        (bytes to send)
									Wiki\r\n     (data)
									6\r\n        (bytes to send)
									pedia \r\n   (data)
									E\r\n        (bytes to send)
									in \r\n
									\r\n
									chunks.\r\n  (data)
									0\r\n        (final byte - 0)
									\r\n         (end message)
							*/
							if (this->_handleRequest_(_it->second, _acceptedS_))
							{
								if (this->_chunkedReq_)
									_it->second = this->_handleChunkedRequest_(_it->second);
								// send the request data , the request call is in here
								// request part needs a setter and a default constructor
								std::cout << "SERVER[buffer] ==> &&&&&&&&&&&&&&&&&&&&& START &&&&&&&&&&&&&&&&&&&&&&&&&&&&\n";
								std::cout << _it->second << std::endl;
								std::cout << "SERVER[buffer] ==> &&&&&&&&&&&&&&&&&&&&& END &&&&&&&&&&&&&&&&&&&&&&&&&&&&\n";
								this->_requestObj_.setBuffer(_it->second);
								this->_requestObj_.initRequest();
								// this->_requestObj_.print();
								// this->_requestObj_.setHost();
								// this->_requestObj_.setPort()
								//this->_request_.start();
								if (FD_ISSET(_acceptedS_, &this->_writefds_))
									this->_handleResponse_(_acceptedS_); 
								_it->second = "";
							}
						}
						else if (_rVal_ == 0) // socket shutdown
						{
							close(_acceptedS_);
							FD_CLR(_acceptedS_, &this->_setFDs_);
							FD_CLR(_acceptedS_, &this->_writefds_);
							this->_clientsInfos_.erase(_acceptedS_);
							std::cout << "The stream socket [ " << _acceptedS_ << " ] disconnected !" << std::endl;
						}
						else // >
							return ;
					}
				}
			}
		}
		if (_selectRet_ == -1)
			throw (std::runtime_error("Select Error !"));
	}
}

void	webServer::_getClientMaxBodySize_(int&	_clientSocket_)
{
	// first get the socket of the server that linked with the socket client :
	std::map<int, int>::iterator iter = this->_clientServer_.find(_clientSocket_);
	struct sockaddr_in _addr_;
	std::memset(&_addr_, 0, sizeof(_addr_));
	if (getsockname(iter->second, (struct sockaddr *)&_addr_, &this->_addrSize_) == -1)
		throw (std::runtime_error("Unable to fetch server informations !"));
	for(std::vector<serverData>::iterator iter = this->_servers_.begin(); iter != this->_servers_.end(); iter++)
	{
		// get host from the server and check with iter hosts iter->get_host() == inet_ntoa(_addr_.sin_addr)
		if (iter->getHost() == inet_ntoa(_addr_.sin_addr))
		{
			this->_ports_ = iter->getPorts(); // get ports from the current server iter->_get_ports()
			for(std::set<int>::iterator port = this->_ports_.begin(); port != this->_ports_.end(); port++)
			{
				// look for the port that we need *port == ntohs(_addr_.sin_port)
				if (*port == ntohs(_addr_.sin_port))
				{
					this->_respServer_ = (*iter);
					this->_respPort_  = *port;
					this->_clientMaxBodyS_ = this->_respServer_.getClientMaxBodySize() * 1024 * 1024;
					break;
					// also set the current port and current server ....
					// save this server and  port and return the max body size 
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
			// index = find("Content-length: ")
			size_t _bodySize_ = std::stoi(_reqHeaders_.substr((int)(_reqHeaders_.find("Content-Length: ")) + 16));
			std::string _reqBody_ = _buff.substr(_buff.find(D_CRLF) + 4);
			
			if (_bodySize_ > (size_t)this->_clientMaxBodyS_)
				return (true);
			else if (_reqBody_.length() < _bodySize_)//if we didn't copy all the body
				return (false);
			
		}
		return (true);
	}
	return (false);
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
		throw (std::runtime_error("Incorrect size !"));
	size_t _decimalSize_ = 0;
	std::stringstream hex(_chunkBuff);
	hex >> std::hex >> _decimalSize_;
	return (_decimalSize_);
}

std::string	webServer::_handleChunkedRequest_(std::string& _reqbuff)
{
	// get the body :
	std::string _reqFixed_ = _reqbuff.substr(0, _reqbuff.find(D_CRLF) + 4);
	std::string _reqBody_ = _reqbuff.substr(_reqbuff.find(D_CRLF) + 4);
	size_t i = 0, _bodysize_ = 0;
	while (i < _reqBody_.size())
	{
		// store the unchunked strings in tmp
		std::string tmp = _reqBody_.substr(i, std::string::npos);
		size_t _tmpLength_ = 0;
		std::string line("");
		std::stringstream _Sbody_(tmp);
		std::getline(_Sbody_, line);
		i += line.length() + 1;
		_tmpLength_ = this->_getHexSizeOfChunk_(line);
		_reqFixed_.append(_reqBody_.c_str() + i, _tmpLength_);
		i+= _tmpLength_ + 2; // 2 for \r and \n
		_bodysize_ += _tmpLength_;
	}
	this->_chunkedReq_ = false;// chunked data is fixed
	(void)_bodysize_;
	return (_reqFixed_);
}

void	webServer::_handleResponse_(int& _acceptedS_)
{
	// CAll the respone class here // port && server
	// std::cout << "***********************************************\n";
	// this->_requestObj_.print();
	// std::cout << "***********************************************\n";
	HttpResponse	_responseObj_(this->_requestObj_, this->_respServer_);
	std::string _response_("");
	// _responseObj_.print();
	_response_.append(_responseObj_.getResponse());
	// std::cout << std::endl <<"++++++++++++++++++++++++++\n"<< _response_ << std::endl;
	if (send(_acceptedS_, _response_.c_str(), _response_.length(), 0) != (ssize_t)_response_.length())
		throw (std::runtime_error("Response Error for [ Socket : "+std::to_string(_acceptedS_) + " ]"));
	// std::cout << "----------------------- RESPONSE ---------------------------" << std::endl;
	// std::cout << _response_ << std::endl;
	// std::cout << "------------------------------------------------------------" << std::endl;
	// check for Header connection and close the socket 
	// if (true) //if the option of connection is close :
	// {
	// 	std::cout << "Socket [ " << _acceptedS_ << " ] disconnected !" << std::endl;
	// 	close(_acceptedS_);
	// 	FD_CLR(_acceptedS_, &_writefds_);
	// 	FD_CLR(_acceptedS_, &_setFDs_);
	// }
	// clear all for request and response ....
}

void webServer::_testgit_(void)
{
	std::cout << "hahahah";
}