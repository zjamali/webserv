/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverData.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iltafah <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/29 11:36:55 by iltafah           #+#    #+#             */
/*   Updated: 2022/04/05 16:01:07 by iltafah          ###   ########.fr       */
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
		
		//client_max_body_size
		bool _client_max_body_size;
		
		//map of error pages
		std::map<bool, std::string> _errorPages;
		

	public:
		serverData();
		~serverData();

	public:
		// void setRoot(std::string givenRoot);
		void setHost(std::string givenHost);
		void setPorts(int givenPort);
		std::set<int> getPorts();
		std::string getHost();
		void setRoot(std::string givenRoot);
		std::string getRoot();
		// void setServerNames();
		// void setClientMaxBodySize(bool var);
		// void setErrorPages();
};

#endif
