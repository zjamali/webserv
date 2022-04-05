/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverData.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iltafah <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/29 11:36:55 by iltafah           #+#    #+#             */
/*   Updated: 2022/04/05 00:44:23 by iltafah          ###   ########.fr       */
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
		
		//vector of locations
		std::vector<location> locations;
		
		//ports that the server listen on {vector ???}
		std::set<int> ports;
		
		//server host
		std::string host;
		
		//server names
		std::set<int> serverNames;
		
		//client_max_body_size
		bool client_max_body_size;
		
		//map of error pages
		std::map<bool, std::string> errorPages;
		
		//root path
		std::string root;

	public:
		serverData();
		~serverData();
};

#endif
