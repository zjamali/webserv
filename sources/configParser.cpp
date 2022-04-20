/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iltafah <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/04/19 23:59:35 by iltafah           #+#    #+#             */
/*   Updated: 2022/04/20 01:36:24 by iltafah          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/configParser.hpp"

#ifndef DEBUG
#define DEBUG false
#endif

/*
** ************************************************************************** **
									Getters
** ************************************************************************** **
*/

const std::vector<serverData> &configParser::getServers() const
{
	return (_servers);
}

/*
** ************************************************************************** **
								Tokenization
** ************************************************************************** **
*/

void configParser::startTokenization(char *configFileName)
{
	std::ifstream configFile(configFileName);
	token tokenNode;
	type tokenType(name);
	std::string word;
	char byte;
	size_t line(1);

	if (configFile.is_open())
	{
		/*********************/
		/* reading file time */
		/*********************/
		while (configFile.good())
		{
			configFile.get(byte);
			/************************************/
			/*			skip spaces				*/
			/************************************/
			if (isspace(byte) == true)
			{
				if (byte == '\n')
					line++;
				while (configFile.good())
				{
					configFile.get(byte);
					if (isspace(byte))
					{
						if (byte == '\n')
							line++;
						continue;
					}
					break;
				}
			}

			/**********************************/
			/* another loop for reading words */
			/**********************************/
			while (configFile.good())
			{
				/************************************/
				/*			skip hashtags			*/
				/************************************/
				if (byte == '#')
				{
					while (configFile.good())
					{
						configFile.get(byte);
						if (byte != '\n')
							continue;
						break;
					}
				}

				/*************************************************************************/
				/* so here if I encounter spaces or semicolon or brackets store the word */
				/* and if I found a semicolon or brackets store it also 			     */
				/*************************************************************************/
				if (isspace(byte) || byte == '{' || byte == '}' || byte == ';')
				{
					/*********************************************************************************/
					/* store word if it is not empty, it can be empty if this is the first iteration */
					/*********************************************************************************/
					if (!word.empty())
					{
						tokenNode.data = word;
						tokenNode.type = tokenType;
						tokenNode.line = line;
						_tokensList.push_back(tokenNode);
						word.clear();
					}

					/***************************************************************/
					/* now check if the byte is one of the following '{'  '}'  ';' */
					/* if yes store it                                             */
					/***************************************************************/
					if (byte == '{' || byte == '}' || byte == ';')
					{
						// determine token type
						if (byte == '{')
							tokenType = openingCurlyBracket;
						else if (byte == '}')
							tokenType = closingCurlyBracket;
						else if (byte == ';')
							tokenType = semicolon;

						tokenNode.data = byte;
						tokenNode.type = tokenType;
						tokenNode.line = line;
						_tokensList.push_back(tokenNode);
					}

					// determine next token type
					if (byte == '{' || byte == '}' || byte == ';')
						tokenType = name;
					else if (byte == ' ')
						tokenType = parameter;
					else if (byte == '\n')
						line++;

					break;
				}
				word += byte;
				configFile.get(byte);
			}
		}
	}
	else
		throw(std::runtime_error("File couldn't be opened!!"));

	// Debugging Time
	if (DEBUG)
	{
		std::list<token>::iterator it = _tokensList.begin();

		while (it != _tokensList.end())
		{
			std::cout << "-----------------------------------------------------------" << std::endl;
			std::cout << "\r\t\t\t\t| ";
			std::cout << ((*it).type == name ? "name" : (*it).type == parameter			? "parameter"
													: (*it).type == openingCurlyBracket ? "openingCurlyBracket"
													: (*it).type == closingCurlyBracket ? "closingCurlyBracket"
																						: "semicolon");
			std::cout << "\r" << (*it).data << std::endl;
			it++;
		}
	}
}

/*
** ************************************************************************** **
							CheckSyntaxErrors
** ************************************************************************** **
*/

std::string strToLower(std::string &str)
{
	std::string lowerStr = "";

	for (int i = 0; str[i]; i++)
		lowerStr += tolower(str[i]);
	return (lowerStr);
}

void configParser::checkAutoIndexSyntax(std::list<token>::iterator &it, location &loc)
{
	if ((*it).type == parameter)
	{
		if (strToLower((*it).data) != "on" && strToLower((*it).data) != "off")
			throw(std::runtime_error("invalid value of \"autoindex\" directive in " + _configFileName + ":" + std::to_string((*it).line) + "\nIt must be one of [on, off]"));

		if (strToLower((*it).data) == "on")
			loc.setAutoIndex(true);

		it++;
		if ((*it).type != semicolon)
			throw(std::runtime_error("invalid number of arguments in \"autoindex\" directive in " + _configFileName + ":" + std::to_string((*it).line)));
	}
	else
		throw(std::runtime_error("invalid number of arguments in \"autoindex\" directive in " + _configFileName + ":" + std::to_string((*it).line)));
}

void configParser::checkIndexSyntax(std::list<token>::iterator &it, location &loc)
{
	if ((*it).type == parameter)
	{
		loc.setIndices((*it).data);
		it++;
		if ((*it).type != semicolon)
			throw(std::runtime_error("invalid number of arguments in \"index\" directive in " + _configFileName + ":" + std::to_string((*it).line)));
	}
	else
		throw(std::runtime_error("invalid number of arguments in \"index\" directive in " + _configFileName + ":" + std::to_string((*it).line)));
}

void configParser::checkAllowedMethodsSyntax(std::list<token>::iterator &it, location &loc)
{
	if ((*it).type == parameter)
	{
		int count;
		for (count = 0; it != _tokensList.end() && count < 3; count++, ++it)
		{
			if ((*it).type == semicolon)
				break;
			if ((*it).type == parameter)
			{
				if ((*it).data != "GET" && (*it).data != "POST" && (*it).data != "DELETE")
					throw(std::runtime_error("invalid value of \"allow_methods\" directive in " + _configFileName + ":" + std::to_string((*it).line) + "\nIt must be one of [GET, POST, DELETE]"));
				loc.setAllowedMethods((*it).data, true);
			}
			else
				throw(std::runtime_error("invalid number of arguments in \"allow_methods\" directive in " + _configFileName + ":" + std::to_string((*it).line)));
		}
		if (count == 0 || (*it).type != semicolon)
			throw(std::runtime_error("invalid number of arguments in \"allow_methods\" directive in " + _configFileName + ":" + std::to_string((*it).line)));
	}
	else
		throw(std::runtime_error("invalid number of arguments in \"allow_methods\" directive in " + _configFileName + ":" + std::to_string((*it).line)));
}

void configParser::checkReturnSyntax(std::list<token>::iterator &it, location &loc)
{
	char *ptr;
	int errorCode;
	std::string errorPath;

	if ((*it).type == parameter)
	{
		errorCode = strtol((*it).data.c_str(), &ptr, 10);
		if (*ptr != '\0')
			throw(std::runtime_error("invalid return code \"" + (*it).data + "\" in " + _configFileName + ":" + std::to_string((*it).line)));
		it++;
		if ((*it).type == parameter)
		{
			errorPath = (*it).data;
			it++;
			if ((*it).type != semicolon)
				throw(std::runtime_error("invalid number of arguments in \"return\" directive in " + _configFileName + ":" + std::to_string((*it).line)));
		}
		else
			throw(std::runtime_error("invalid number of arguments in \"return\" directive in " + _configFileName + ":" + std::to_string((*it).line)));
	}
	else
		throw(std::runtime_error("invalid number of arguments in \"return\" directive in " + _configFileName + ":" + std::to_string((*it).line)));

	loc.setIsRedirection(true);
	loc.setReturnData(std::make_pair(errorCode, errorPath));
}

void configParser::checkFastcgiPassSyntax(std::list<token>::iterator &it, location &loc)
{
	if ((*it).type == parameter)
	{
		loc.setIsCgi(true);
		loc.setCgiPath((*it).data);
		it++;
		if ((*it).type != semicolon)
			throw(std::runtime_error("invalid number of arguments in \"fastcgi_pass\" directive in " + _configFileName + ":" + std::to_string((*it).line)));
	}
	else
		throw(std::runtime_error("invalid number of arguments in \"fastcgi_pass\" directive in " + _configFileName + ":" + std::to_string((*it).line)));
}

void configParser::checkUploadEnableSyntax(std::list<token>::iterator &it, location &loc)
{
	if ((*it).type == parameter)
	{
		if (strToLower((*it).data) != "on" && strToLower((*it).data) != "off")
			throw(std::runtime_error("invalid value \"" + (*it).data + "\" in " + _configFileName + ":" + std::to_string((*it).line)));
		if (strToLower((*it).data) == "on")
			loc.setIsUploadEnable(true);
		it++;
		if ((*it).type != semicolon)
			throw(std::runtime_error("invalid number of arguments in \"upload_enable\" directive in " + _configFileName + ":" + std::to_string((*it).line)));
	}
	else
		throw(std::runtime_error("invalid number of arguments in \"upload_enable\" directive in " + _configFileName + ":" + std::to_string((*it).line)));
}

void configParser::checkUploadStoreSyntax(std::list<token>::iterator &it, location &loc)
{
	if ((*it).type == parameter)
	{
		loc.setUploadPath((*it).data);
		it++;
		if ((*it).type != semicolon)
			throw(std::runtime_error("invalid number of arguments in \"upload_store\" directive in " + _configFileName + ":" + std::to_string((*it).line)));
	}
	else
		throw(std::runtime_error("invalid number of arguments in \"upload_store\" directive in " + _configFileName + ":" + std::to_string((*it).line)));
}

void configParser::checkLocationRootSyntax(std::list<token>::iterator &it, location &loc)
{
	if ((*it).type == parameter)
	{
		loc.setRoot((*it).data);
		it++;
		if ((*it).type != semicolon)
			throw(std::runtime_error("invalid number of arguments in \"root\" directive in " + _configFileName + ":" + std::to_string((*it).line)));
	}
	else
		throw(std::runtime_error("invalid number of arguments in \"root\" directive in " + _configFileName + ":" + std::to_string((*it).line)));
}

void configParser::checkLocationSyntax(std::list<token>::iterator &it, serverData &server)
{
	location loc;

	if ((*it).type == parameter)
	{
		loc.setPath((*it).data);
		it++;
		if ((*it).type == openingCurlyBracket)
		{
			it++;
			while (it != _tokensList.end())
			{
				if ((*it).type == closingCurlyBracket)
					break;
				else if ((*it).type == name)
				{
					if ((*it).data == "autoindex")
						checkAutoIndexSyntax(++it, loc);
					else if ((*it).data == "root")
						checkLocationRootSyntax(++it, loc);
					else if ((*it).data == "index")
						checkIndexSyntax(++it, loc);
					else if ((*it).data == "allow_methods")
						checkAllowedMethodsSyntax(++it, loc);
					else if ((*it).data == "return")
						checkReturnSyntax(++it, loc);
					else if ((*it).data == "fastcgi_pass")
						checkFastcgiPassSyntax(++it, loc);
					else if ((*it).data == "upload_enable")
						checkUploadEnableSyntax(++it, loc);
					else if ((*it).data == "upload_store")
						checkUploadStoreSyntax(++it, loc);
					else
						throw(std::runtime_error("an unknown directive \"" + (*it).data + "\" within location context in " + _configFileName + ":" + std::to_string((*it).line)));
				}
				else if ((*it).type == openingCurlyBracket)
					throw(std::runtime_error("multiple `{` within location context in " + _configFileName + ":" + std::to_string((*it).line)));
				it++;
			}
			if ((*it).type != closingCurlyBracket)
				throw(std::runtime_error("location must be closed with `}` in " + _configFileName + ":" + std::to_string((*it).line)));
		}
		else
			throw(std::runtime_error("missing `{` for a location in " + _configFileName + ":" + std::to_string((*it).line)));
	}
	else
		throw(std::runtime_error("invalid number of arguments in \"location\" directive in " + _configFileName + ":" + std::to_string((*--it).line)));

	server.setLocations(loc);
}

void configParser::checkListenSyntax(std::list<token>::iterator &it, serverData &server)
{
	int port = 0;
	char *ptr = NULL;

	if ((*it).type == parameter)
	{
		port = strtol((*it).data.c_str(), &ptr, 10);
		if (*ptr != '\0' || port <= 0 || port > 65535)
			throw(std::runtime_error("invalid port in \"" + (*it).data + "\" of the \"listen\" directive in " + _configFileName + ":" + std::to_string((*it).line)));
		if (server.setPorts(port) == false)
			throw(std::runtime_error("duplicated port within a single server in \"listen\" directive in " + _configFileName + ":" + std::to_string((*--it).line)));
		it++;
		if ((*it).type != semicolon)
			throw(std::runtime_error("invalid number of arguments in \"listen\" directive in " + _configFileName + ":" + std::to_string((*--it).line)));
	}
	else
		throw(std::runtime_error("invalid number of arguments in \"listen\" directive in " + _configFileName + ":" + std::to_string((*--it).line)));
}

void configParser::checkHostSyntax(std::list<token>::iterator &it, serverData &server)
{
	if ((*it).type == parameter)
	{
		size_t pos = 0;
		size_t oldPos = 0;
		int octet = 0;
		int count = 0;
		char *ptr;

		///////////////////////////////////////////////////////////////////////
		//*							Split Host								*//
		///////////////////////////////////////////////////////////////////////
		std::vector<std::string> splittedHost;
		while ((pos = (*it).data.find(".", pos)) != std::string::npos)
		{
			splittedHost.push_back((*it).data.substr(oldPos, pos - oldPos));
			pos++;
			oldPos = pos;
		}
		splittedHost.push_back((*it).data.substr(oldPos, pos - oldPos));

		///////////////////////////////////////////////////////////////////////
		//*							Check Host								*//
		///////////////////////////////////////////////////////////////////////
		std::vector<std::string>::iterator vecit = splittedHost.begin();
		while (vecit != splittedHost.end())
		{
			octet = strtol((*vecit).c_str(), &ptr, 10);
			if (*ptr != '\0' || octet < 0 || octet > 255)
				throw(std::runtime_error("host not found in \"" + (*it).data + "\" of the \"listen\" directive in " + _configFileName + ":" + std::to_string((*it).line)));
			count++;
			vecit++;
		}
		if (count != 4)
			throw(std::runtime_error("host not found in \"" + (*it).data + "\" of the \"listen\" directive in " + _configFileName + ":" + std::to_string((*it).line)));

		server.setHost((*it).data);
		it++;
		if ((*it).type != semicolon)
			throw(std::runtime_error("invalid number of arguments in \"host\" directive in " + _configFileName + ":" + std::to_string((*--it).line)));
	}
	else
		throw(std::runtime_error("invalid number of arguments in \"host\" directive in " + _configFileName + ":" + std::to_string((*--it).line)));
}

void configParser::checkServerNameSyntax(std::list<token>::iterator &it, serverData &server)
{
	if ((*it).type == parameter)
	{
		server.setServerNames((*it).data);
		it++;
		if ((*it).type != semicolon)
			throw(std::runtime_error("invalid number of arguments in \"server_name\" directive in " + _configFileName + ":" + std::to_string((*--it).line)));
	}
	else
		throw(std::runtime_error("invalid number of arguments in \"server_name\" directive in " + _configFileName + ":" + std::to_string((*--it).line)));
}

void configParser::checkServerRootSyntax(std::list<token>::iterator &it, serverData &server)
{
	if ((*it).type == parameter)
	{
		server.setRoot((*it).data);
		it++;
		if ((*it).type != semicolon)
			throw(std::runtime_error("invalid number of arguments in \"root\" directive in " + _configFileName + ":" + std::to_string((*--it).line)));
	}
	else
		throw(std::runtime_error("invalid number of arguments in \"root\" directive in " + _configFileName + ":" + std::to_string((*--it).line)));
}

void configParser::checkErrorPageSyntax(std::list<token>::iterator &it, serverData &server)
{
	char *ptr = NULL;
	int errorCode = 0;
	std::string errorPath;

	if ((*it).type == parameter)
	{
		errorCode = std::strtol((*it).data.c_str(), &ptr, 10);
		if (*ptr != '\0' || errorCode < 300 || errorCode > 599)
			throw(std::runtime_error("value \"" + (*it).data + "\" must be between 300 and 599 in " + _configFileName + ":" + std::to_string(((*it).line))));
		it++;
		if ((*it).type == parameter)
		{
			errorPath = (*it).data;
			it++;
			if ((*it).type != semicolon)
				throw(std::runtime_error("invalid number of arguments in \"error_page\" directive in " + _configFileName + ":" + std::to_string((*--it).line)));
		}
		else
			throw(std::runtime_error("invalid number of arguments in \"error_page\" directive in " + _configFileName + ":" + std::to_string((*--it).line)));
	}
	else
		throw(std::runtime_error("invalid number of arguments in \"error_page\" directive in " + _configFileName + ":" + std::to_string((*--it).line)));
	server.setErrorPages(errorCode, errorPath);
}

void configParser::checkMaxBodySizeSyntax(std::list<token>::iterator &it, serverData &server)
{
	int size = 0;
	char *ptr = NULL;

	if ((*it).type == parameter)
	{
		size = strtol((*it).data.c_str(), &ptr, 10);
		if ((*ptr != 'm' && *ptr != 'M') || *(ptr + 1) != '\0')
			throw(std::runtime_error("unexpected client max body size `" + (*it).data + "`"));
		server.setClientMaxBodySize(size);
		it++;
		if ((*it).type != semicolon)
			throw(std::runtime_error("invalid number of arguments in \"MaxBodySize\" directive in " + _configFileName + ":" + std::to_string((*--it).line)));
	}
	else
		throw(std::runtime_error("invalid number of arguments in \"MaxBodySize\" directive in " + _configFileName + ":" + std::to_string((*--it).line)));
}

void configParser::checkServerSyntax(std::list<token>::iterator &it)
{
	serverData server;

	if ((*it).type != openingCurlyBracket)
		throw(std::runtime_error(("missing `{` for a server in " + _configFileName + ":" + std::to_string((*it).line))));

	while (++it != _tokensList.end())
	{
		if ((*it).type == closingCurlyBracket)
			break;
		else if ((*it).type == name)
		{
			if ((*it).data == "location")
				checkLocationSyntax(++it, server);
			else if ((*it).data == "listen")
				checkListenSyntax(++it, server);
			else if ((*it).data == "host")
				checkHostSyntax(++it, server);
			else if ((*it).data == "server_name")
				checkServerNameSyntax(++it, server);
			else if ((*it).data == "root")
				checkServerRootSyntax(++it, server);
			else if ((*it).data == "error_page")
				checkErrorPageSyntax(++it, server);
			else if ((*it).data == "client_max_body_size")
				checkMaxBodySizeSyntax(++it, server);
			else
				throw(std::runtime_error("an unknown directive \"" + (*it).data + "\" within server context in " + _configFileName + ":" + std::to_string((*it).line)));
		}
		else if ((*it).type == openingCurlyBracket)
			throw(std::runtime_error("multiple `{` within server context in " + _configFileName + ":" + std::to_string((*it).line)));
	}

	if ((*it).type != closingCurlyBracket)
		throw(std::runtime_error("Server must be closed with `}` in " + _configFileName + ":" + std::to_string((*--it).line)));

	if (server.getHost().empty() == true)
		throw(std::runtime_error("there is no host within the server that ends in line : `" + std::to_string((*it).line) + "` please specify one"));
	else if (server.getPorts().empty() == true)
		throw(std::runtime_error("there is no port within the server that ends in line : `" + std::to_string((*it).line) + "` please specify one"));
	else if (server.getRoot().empty() == true)
		throw(std::runtime_error("there is no root within the server that ends in line : `" + std::to_string((*it).line) + "` please specify one"));
	_servers.push_back(server);

	it++; // skip the closing curly bracket
}

void configParser::checkSyntaxAndFillData()
{
	std::list<token>::iterator it = _tokensList.begin();

	while (it != _tokensList.end())
	{
		if ((*it).data == "server")
			checkServerSyntax(++it);
		else
			throw(std::runtime_error("Unknown directive \"" + (*it).data + "\" in " + _configFileName + ":" + std::to_string((*it).line)));
	}
}

void configParser::checkFileExtension(std::string configFileName)
{
	std::string fileName(configFileName);
	size_t found = fileName.find('.');
	std::string extension;

	if (found != std::string::npos)
	{
		if ((extension = fileName.substr(found)) != ".conf")
			throw(std::runtime_error("Error in the extension of the given file, it must end with \".conf\" rathar than \"" + extension + "\""));
	}
	else
		throw(std::runtime_error("Error in the extension of the given file, it must end with \".conf\" rathar than \"\""));
}

configParser::configParser(char *configFileName) : _configFileName(configFileName)
{
	checkFileExtension(std::string(configFileName));
	startTokenization(configFileName);
	checkSyntaxAndFillData();
}

configParser::~configParser()
{
}
