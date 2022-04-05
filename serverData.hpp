/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverData.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iltafah <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/29 11:36:55 by iltafah           #+#    #+#             */
/*   Updated: 2022/04/05 17:36:56 by iltafah          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERDATA_HPP
# define SERVERDATA_HPP

# include "./location.hpp"
# include <iostream>
# include <vector>
# include <set>
# include <map>

class serverData
{
	private:

		//client_max_body_size
		int _client_max_body_size;

		//root path
		std::string _root;
		
		//server host
		std::string _host;
		
		//ports that the server listen on {vector ???}
		std::set<int> _ports;
		
		//vector of locations
		std::vector<location> _locations;
		
		//server names
		std::set<int> _serverNames;
		
		//map of error pages
		std::map<bool, std::string> _errorPages;

	public:
		serverData();
		~serverData();

	public:
		void setHost(std::string givenHost);
		void setPorts(int givenPort);

		std::set<int> getPorts();
		std::string getHost();

		void setRoot(std::string givenRoot);
		std::string getRoot();

		void setClientMaxBodySize(int givenSize);
		int getClientMaxBodySize();
		// void setServerNames();
		// void setErrorPages();
};

#endif
