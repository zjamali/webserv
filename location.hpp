/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iltafah <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/29 11:44:42 by iltafah           #+#    #+#             */
/*   Updated: 2022/04/06 01:25:07 by iltafah          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
# define LOCATION_HPP

# include <iostream>
# include <vector>
# include <map>

class location
{
	private:
		//location path
		std::string path;
		
		//autoindex {bool}
		bool autoIndex;
		
		//indices {maybe a vector of strings}
		std::vector<std::string> indices;

		//allow methods {better to use map}
		std::map<bool, std::string> allowedMethods;
		
		//return code and path
		std::pair<int, std::string> returnData;

		//fastCgiPass
		bool isCgi;
		std::string cgiPath;

		//upload
		bool isUploadEnable;
		std::string uploadPath;

	public:
		location();
		~location();
};

#endif
