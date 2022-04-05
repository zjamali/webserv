/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverData.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iltafah <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/29 11:36:44 by iltafah           #+#    #+#             */
/*   Updated: 2022/04/05 16:02:29 by iltafah          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "serverData.hpp"

serverData::serverData()
{
}

serverData::~serverData()
{
}

void serverData::setPorts(int givenPort)
{
	_ports.insert(givenPort);
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