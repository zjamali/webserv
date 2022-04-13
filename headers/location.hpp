/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iltafah <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/29 11:44:42 by iltafah           #+#    #+#             */
/*   Updated: 2022/04/12 07:02:32 by iltafah          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
# define LOCATION_HPP

# include <iostream>
# include <vector>
# include <map>
# include <set>

class location
{
	private:
		//location path
		std::string _path;
		
		//autoindex {bool}
		bool _autoIndex;

		//root path
		std::string _root;

		//indices {maybe a vector of strings}
		std::set<std::string> _indices;

		//allow methods {better to use map}
		std::map<std::string, bool> _allowedMethods;
		
		//return code and path
		bool _isRedirection;
		std::pair<int, std::string> _returnData;

		//fastCgiPass
		bool _isCgi;
		std::string _cgiPath;

		//upload
		bool _isUploadEnable;
		std::string _uploadPath;

	public:
		location();
		~location();

	public:
		void setPath(std::string path);
		std::string getPath();

		void setRoot(std::string givenRoot);
		std::string getRoot();

		void setAutoIndex(bool var);
		bool getAutoIndex();

		void setIndices(std::string index);
		std::set<std::string> getIndices();

		void setAllowedMethods(std::string method, bool var);
		std::map<std::string, bool> getAllowedMethods();

		void setReturnData(std::pair<int, std::string> givenData);
		std::pair<int, std::string> getReturnData();

		void setIsRedirection(bool var);
		bool getIsRedirection();

		void setIsCgi(bool var);
		bool getIsCgi();

		void setCgiPath(std::string givenPath);
		std::string getCgiPath();

		void setIsUploadEnable(bool var);
		bool getIsUploadEnable();

		void setUploadPath(std::string givenPath);
		std::string getUploadPath();
};

#endif
