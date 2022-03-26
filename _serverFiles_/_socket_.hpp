#ifndef _SOCKET_HPP
#define _SOCKET_HPP

# include "_UltimateHeader_.hpp"

// Socket class :
class _socket_{

	private:
		struct sockaddr_in	_addr_;
		size_t	_addrSize_;
		int	_fdSocket_;
	
	public:
		_socket_();
		int	_create_(int p, std::string h);
		int	_listen_();
		void	_bind_(int p, std::string h);
		const int& getSocket() const;
		struct sockaddr_in getSockAddress() const;

};

#endif // !_socket_.hpp
