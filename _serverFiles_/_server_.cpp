/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _server_.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdait-m <abdait-m@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/26 20:43:19 by abdait-m          #+#    #+#             */
/*   Updated: 2022/03/31 07:54:17 by abdait-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "_server_.hpp"

webServer::webServer(std::vector<SData> _serv) : _servers_(_serv)
{
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
				/* code */
				// creating a socket { :
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
				// waiting for the connection :
				struct timeval _time_ = {1, 1};
				while(true)
				{
					FD_ZERO(&this->_readfds_);
					this->_readfds_ = this->_setFDs_;
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
								// check for is_connected  and create new connection ...
								if (is_connected)
								{
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
								else
								{
									// handling the request that sent and return the correct response :
									
								}
							}
						}
					}
					if (_selectRet_ == -1)
						throw (std::runtime_error("Select Error !"));
				}
			}
			catch(const std::exception& e)
			{
				close(this->_socket_);
				std::cerr << e.what() << '\n';
			}
			
			
		}
	}
}