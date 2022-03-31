/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _server_.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdait-m <abdait-m@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/14 16:29:15 by abdait-m          #+#    #+#             */
/*   Updated: 2022/03/31 07:47:45 by abdait-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef _SERVER_HPP
#define _SERVER_HPP

# include "_UltimateHeader_.hpp"


// Parsing data :
class SData{
	
	private:
		int	_port_;
		std::string	_host_;
		std::vector<std::string> _names_;
		std::vector<std::string> _locs_;
		
	public:
		int const& getPort() const { return (this->_port_);}
		const std::string& getHost() const { return (this->_host_); }
		// .......
};

class webServer{
	
	private:
		int	_socket_; // current socket of the server
		bool _option_; // for setsocketopt()
		int _currPort_; // the port in use
		std::vector<int>	_socketFds_; // Holds all the sockets
		std::vector<int> _ports_; // holds all the ports
		
		// fd_sets for select():
		fd_set	_readfds_;
		fd_set	_writefds_;
		fd_set	_setFDs_;
		
		int		_maxSfd_; // first argument of select()
		std::string _host_;
		std::vector<SData>	_servers_; // the server from the parser
		
		// socket structures for client and server :
		struct sockaddr_in	_Saddr_;
		struct sockaddr_in	_Caddr_;
		socklen_t			_addrSize_;
		
		// Clients sockets and request data :
		std::map<int, std::string> _clientsInfos_;
		
		//
		std::map<int, int> _clientServer_;
		// std::vector<_socket_> _sockets_;
		webServer() { }
		webServer&	operator = (const webServer& );
	
	public:
		webServer(std::vector<SData>	_serv);
		~webServer();
		
		void	_start_();
		
};


#endif // !_server_.hpp
