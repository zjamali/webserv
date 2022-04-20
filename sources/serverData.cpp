/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverData.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iltafah <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/29 11:36:44 by iltafah           #+#    #+#             */
/*   Updated: 2022/04/20 01:33:39 by iltafah          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/serverData.hpp"

serverData::serverData() : _client_max_body_size(1) //default root is "/etc/nginx/html", //default server name ""
{	
}

serverData::~serverData()
{
}

bool serverData::setPorts(int givenPort)
{
	std::pair<std::set<int>::iterator, bool> ret = _ports.insert(givenPort);
	
	return (ret.second);
}

std::set<int> serverData::getPorts()
{
	return (_ports);
}

void serverData::setHost(std::string givenHost)
{
	_host = givenHost;
}

std::string serverData::getHost()
{
	return (_host);
}

void serverData::setRoot(std::string givenRoot)
{
	_root = givenRoot;
}

std::string serverData::getRoot()
{
	return (_root);
}

void serverData::setClientMaxBodySize(int givenSize)
{
	_client_max_body_size = givenSize;
}

int serverData::getClientMaxBodySize()
{
	return (_client_max_body_size);
}

void serverData::setServerNames(std::string serverName)
{
	_serverNames.insert(serverName);
}

std::set<std::string> serverData::getServerNames()
{
	return (_serverNames);
}

void serverData::setErrorPages(int errorCode, std::string errorPath)
{
	_errorPages.insert(std::make_pair(errorCode, errorPath));
}

std::map<int, std::string> serverData::getErrorPages()
{
	return (_errorPages);
}

void serverData::setLocations(location loc)
{
	_locations.push_back(loc);
}

std::vector<location> serverData::getLocations()
{
	return (_locations);
}
