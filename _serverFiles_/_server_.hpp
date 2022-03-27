/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _server_.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdait-m <abdait-m@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/14 16:29:15 by abdait-m          #+#    #+#             */
/*   Updated: 2022/03/26 20:42:27 by abdait-m         ###   ########.fr       */
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
		std::vector<SData>	_servers_;
		struct sockaddr_in	_addr_;
		size_t				_addrSize_;
		std::vector<_socket_> _sockets_;
		
		webServer() { }
	
	public:
		webServer(std::vector<SData>	_serv);
		void	_start_();
		
};


#endif // !_server_.hpp
