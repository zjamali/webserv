/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _server_.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdait-m <abdait-m@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/26 20:43:19 by abdait-m          #+#    #+#             */
/*   Updated: 2022/04/01 09:40:30 by abdait-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "_server_.hpp"

webServer::webServer(std::vector<SData> _serv) : _servers_(_serv)
{
}

void	webServer::_buildASocket_() 
{
	/* code */
	// creating a socket this socket is used for accepting connections not for exchanging the data{ :
	if ((this->_socket_ = socket(AF_INET, SOCK_STREAM, 0)) == -1)
		throw (std::runtime_error("Socket creation error !"));
	// for non-blocking
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
	for (std::vector<SData>::iterator serv = this->_servers_.begin(); serv != this->_servers_.end(); serv++)
	{
		this->_ports_ = {}; // get ports from the server
		this->_host_ = "23.23.23.23"; // get host from the server
		for (std::vector<int>::iterator port = this->_ports_.begin(); port!=this->_ports_.end(); port++)
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
	struct timeval _time_ = {1, 1};
	while(true)
	{
		FD_ZERO(&this->_readfds_);
		this->_readfds_ = this->_setFDs_;
		// On successful return, each set is modified such that 
		// it contains only the file descriptors that are ready for I/O of the type delineated by that set
		// the first parameter in select should be the highest-numbered file descriptor in any of the three sets
		// plus 1 so we check the max fd num
		// we use FD_ISSET to check for the fds that are ready 
		int _selectRet_ = select(this->_maxSfd_ + 1, &this->_readfds_, &this->_writefds_, NULL, &_time_);
		if (_selectRet_ > 0)
		{
			for (int _fdsocket = 0; _fdsocket < this->_maxSfd_ + 1; _fdsocket++)
			{
				if (FD_ISSET(_fdsocket, &this->_readfds_))
				{
					bool is_connected = false;
					for (std::vector<int>::iterator it = this->_socketFds_.begin(); it != _socketFds_.end(); it++)
					{
						if (_fdsocket == *it)
						{
							is_connected = true;
							break ;
						}
					}
					// check for is_connected : accepting the client connection ...
					if (is_connected)
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
					else // for write 
					{
						int _acceptedS_ = _fdsocket;
						// handling the request that sent and return the correct response :
						char _buffer_[1024];
						bzero(_buffer_, sizeof(_buffer_));
						// read from client socket 
						int _rVal_ = recv(_acceptedS_, _buffer_, 1024, 0);
						std::cout << "Receive data from client with socket : " << _acceptedS_ << " - IP = " << inet_ntoa(this->_Caddr_.sin_addr) << ":" << ntohs(this->_Caddr_.sin_port) << std::endl;
						if (_rVal_ > 0)
						{
							_buffer_[_rVal_] = '\0';
							// look for the client socket inside the map and add the body to its mapped value:
							std::map<int, std::string>::iterator _it = this->_clientsInfos_.find(_acceptedS_);
							if (_it != this->_clientsInfos_.end())
								_it->second += _buffer_;
						}
						else if (_rVal_ == 0) // socket shutdown
						{
							close(_acceptedS_);
							FD_CLR(_acceptedS_, &this->_setFDs_);
							FD_CLR(_acceptedS_, &this->_writefds_);
							this->_clientsInfos_.erase(_acceptedS_);
							std::cout << "The stream socket [ " << _acceptedS_ << " ] disconnected !" << std::endl;
						}
						else // 
							return ;
					}
				}
			}
		}
		if (_selectRet_ == -1)
			throw (std::runtime_error("Select Error !"));
	}
}

int	webServer::_getClientMaxBodySize_(int&	_clientSocket_)
{
	// first get the socket of the server that linked with the socket client :
	std::map<int, int>::iterator iter = this->_clientServer_.find(_clientSocket_);
	struct sockaddr_in _addr_;
	std::memset(&_addr_, 0, sizeof(_addr_));
	if (getsockname(iter->second, (struct sockaddr *)&_addr_, &this->_addrSize_) == -1)
		throw (std::runtime_error("Unable to fetch server informations !"));
	for(std::vector<SData>::iterator iter = this->_servers_.begin(); iter != this->_servers_.end(); iter++)
	{
		// get host from the server and check with iter hosts iter->get_host() == inet_ntoa(_addr_.sin_addr)
		if (true)
		{
			// this->_ports_ =  // get ports from the current server iter->_get_ports()
			for(std::vector<int>::iterator port = this->_ports_.begin(); port != this->_ports_.end(); port++)
			{
				// look for the port that we need *port == ntohs(_addr_.sin_port)
				if (true)
				{
					return (1); // return the client body size of this server (*iter)->get_client_max_body_size()
				}
			}
		}
	}
	return (0);
}