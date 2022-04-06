/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   _socket_.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdait-m <abdait-m@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/26 20:43:25 by abdait-m          #+#    #+#             */
/*   Updated: 2022/03/26 20:43:26 by abdait-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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
