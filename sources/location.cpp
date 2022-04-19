/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iltafah <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/29 11:45:44 by iltafah           #+#    #+#             */
/*   Updated: 2022/04/13 22:57:29 by iltafah          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/location.hpp"

location::location() : _autoIndex(false), _isRedirection(false), _isCgi(false), _isUploadEnable(false)
{
	_allowedMethods.insert(std::make_pair("GET", true));
	_allowedMethods.insert(std::make_pair("POST", false));
	_allowedMethods.insert(std::make_pair("DELETE", false));
}

location::~location()
{
}

void	location::setAutoIndex(bool var)
{
	_autoIndex = var;
}

bool	location::getAutoIndex()
{
	return (_autoIndex);
}

void	location::setIndices(std::string index)
{
	_indices.insert(index);
}

std::set<std::string> location::getIndices()
{
	return (_indices);
}

void location::setAllowedMethods(std::string method, bool var)
{
	_allowedMethods[method] = var;
}

std::map<std::string, bool> location::getAllowedMethods()
{
	return (_allowedMethods);
}

void location::setReturnData(std::pair<int, std::string> givenData)
{
	_returnData = givenData;
}

std::pair<int, std::string> location::getReturnData()
{
	return (_returnData);
}

void location::setIsRedirection(bool var)
{
	_isRedirection = var;
}

bool location::getIsRedirection()
{
	return (_isRedirection);
}

void location::setPath(std::string givenPath)
{
	_path = givenPath;
}

std::string location::getPath()
{
	return (_path);
}

void location::setIsCgi(bool var)
{
	_isCgi = var;
}

bool location::getIsCgi()
{
	return (_isCgi);
}

void location::setCgiPath(std::string givenPath)
{
	_cgiPath = givenPath;
}

std::string location::getCgiPath()
{
	return (_cgiPath);
}

void location::setIsUploadEnable(bool var)
{
	_isUploadEnable = var;
}

bool location::getIsUploadEnable()
{
	return (_isUploadEnable);
}

void location::setUploadPath(std::string givenPath)
{
	_uploadPath = givenPath;
}

std::string location::getUploadPath()
{
	return (_uploadPath);
}

void location::setRoot(std::string givenRoot)
{
	_root = givenRoot;
}

std::string location::getRoot()
{
	return (_root);
}
