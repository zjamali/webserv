/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iltafah <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/21 21:44:33 by iltafah           #+#    #+#             */
/*   Updated: 2022/04/11 01:29:36 by iltafah          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/configParser.hpp"

#ifndef DEBUG
# define DEBUG false
#endif


//I know the following foking ugly cood is foking ugly but don't worry I will butterfly it 
//I know the following foking ugly cood is foking ugly but don't worry I will butterfly it 
//I know the following foking ugly cood is foking ugly but don't worry I will butterfly it 
//I know the following foking ugly cood is foking ugly but don't worry I will butterfly it 
//I know the following foking ugly cood is foking ugly but don't worry I will butterfly it 
//I know the following foking ugly cood is foking ugly but don't worry I will butterfly it 
//I know the following foking ugly cood is foking ugly but don't worry I will butterfly it 


const std::vector<serverData> &configParser::getServers() const
{
	return (_servers);
}

/*
** ************************************************************************** **
							Tokenization
** ************************************************************************** **
*/

void	configParser::startTokenization(char *configFileName)
{
	std::ifstream configFile(configFileName);
	token	tokenNode;
	type	tokenType;
	std::string word;
	char		byte;


//firstly the type will be a name
tokenType = name;

	if (configFile.is_open())
	{
		///////////////////////
		/* reading file time */
		///////////////////////
		while (configFile.good())
		{
			configFile.get(byte);
			if (isspace(byte) == true)
			{
				//skip spaces and empty lines
				while (configFile.good())
				{
					configFile.get(byte);
					if (isspace(byte))
						continue ;
					break ;
				}
			}


			/**********************************/
			/* another loop for reading words */
			/**********************************/
			while (configFile.good())
			{
				//skip hashtags
				if (byte == '#')
				{
					while (configFile.good())
					{
						configFile.get(byte);
						if (byte != '\n')
							continue ;
						break ;
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
						//determine token type
						tokenNode.data = word;
						tokenNode.type = tokenType;
						_tokensList.push_back(tokenNode);
						word.clear();
					}

					/***************************************************************/
					/* now check if the byte is one of the following '{'  '}'  ';' */
					/* if yes store it                                             */
					/***************************************************************/
					if (byte == '{' || byte == '}' || byte == ';')
					{
						//determine token type
						if (byte == '{')
							tokenType = openingCurlyBracket;
						else if (byte == '}')
							tokenType = closingCurlyBracket;
						else if (byte == ';')
							tokenType = semicolon;

						tokenNode.data = byte;
						tokenNode.type = tokenType;
						_tokensList.push_back(tokenNode);
					}

					//determine next token type if it is a word
					if (byte == '{' || byte == '}' || byte == ';')
						tokenType = name;
					else if (byte == ' ')
						tokenType = parameter;
					
					break ;
				}
				word += byte;
				configFile.get(byte);
			}
		}
	}
	//else
		//throw some error

	if (DEBUG)
	{
		std::list<token>::iterator it = _tokensList.begin();

		while (it != _tokensList.end())
		{
			std::cout << "-----------------------------------------------------------" << std::endl;
			std::cout << "\r\t\t\t\t| " ;
			std::cout << ((*it).type == name ? "name" : 
				(*it).type == parameter ? "parameter" : 
					(*it).type == openingCurlyBracket ? "openingCurlyBracket" : 
						(*it).type == closingCurlyBracket ? "closingCurlyBracket" : 
							"semicolon");
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

void	configParser::checkAutoIndexSyntax(std::list<token>::iterator &it, location &loc)
{
	if ((*it).type == parameter)
	{
		if (strToLower((*it).data) != "on" && strToLower((*it).data) != "off")
			throw (std::runtime_error("invalid madafaka value" + (*it).data + "in `autoindex` directive, it must be `on` or `off` madafaka"));
		
		if (strToLower((*it).data) == "on")
			loc.setAutoIndex(true);

		it++;
		if ((*it).type != semicolon)
			throw (std::runtime_error("unexpected madafaka `" + (*it).data + "`"));
	}
	else
		throw (std::runtime_error("unexpected madafaka `" + (*it).data + "`"));
}

void	configParser::checkIndexSyntax(std::list<token>::iterator &it, location &loc)
{
	if ((*it).type == parameter)
	{
		loc.setIndices((*it).data);
		it++;
		if ((*it).type != semicolon)
			throw (std::runtime_error("unexpected madafaka `" + (*it).data + "`"));
	}
	else
		throw (std::runtime_error("unexpected madafaka `" + (*it).data + "`"));
}

void	configParser::checkAllowedMethodsSyntax(std::list<token>::iterator &it, location &loc)
{
	if ((*it).type == parameter)
	{
		int count;
		for (count = 0; it != _tokensList.end() && count < 3; count++, ++it)
		{
			if ((*it).type == semicolon)
				break ;
			if ((*it).type == parameter)
			{
				if ((*it).data != "GET" && (*it).data != "POST" && (*it).data != "DELETE")
					throw (std::runtime_error("wtf is this method ?!?!?! `" + (*it).data + "`, go learn some foking English"));
				loc.setAllowedMethods((*it).data, true);
			}
			else
				throw (std::runtime_error("unexpected madafaka `" + (*it).data + "`"));
		}
		if (count == 0 || (*it).type != semicolon)
			throw (std::runtime_error("Unexpected foking `" + (*it).data + "`"));
	}
	else
		throw (std::runtime_error("unexpected madafaka `" + (*it).data + "`"));
}

void	configParser::checkReturnSyntax(std::list<token>::iterator &it, location &loc) // from 0 to 999 as in nginx ?!
{
	char *ptr;
	int	errorCode;
	std::string errorPath;

	if ((*it).type == parameter)
	{
		errorCode = strtol((*it).data.c_str(), &ptr, 10);
		if (*ptr != '\0')
			throw (std::runtime_error("WTF!!! what is this foking return code `" + (*it).data + "`"));
		it++;
		if ((*it).type == parameter)
		{
			errorPath = (*it).data;
			it++;
			if ((*it).type != semicolon)
				throw (std::runtime_error("unexpected madafaka `" + (*it).data + "`"));
		}
		else
			throw (std::runtime_error("unexpected madafaka `" + (*it).data + "`"));
	}
	else
		throw (std::runtime_error("unexpected madafaka `" + (*it).data + "`"));
	
	loc.setIsRedirection(true);
	loc.setReturnData(std::make_pair<int, std::string>(errorCode, errorPath));
}

void	configParser::checkFastcgiPassSyntax(std::list<token>::iterator &it, location &loc)
{
	if ((*it).type == parameter)
	{
		loc.setIsCgi(true);
		loc.setCgiPath((*it).data);
		it++;
		if ((*it).type != semicolon)
			throw (std::runtime_error("unexpected madafaka `" + (*it).data + "`"));
	}
	else
		throw (std::runtime_error("unexpected madafaka `" + (*it).data + "`"));
}

void	configParser::checkUploadEnableSyntax(std::list<token>::iterator &it, location &loc)
{
	if ((*it).type == parameter)
	{
		if (strToLower((*it).data) != "on" && strToLower((*it).data) != "off")
			throw (std::runtime_error("invalid madafaka value" + (*it).data + "in `upload_enable` directive, it must be `on` or `off` madafaka"));
		if (strToLower((*it).data) == "on")
			loc.setIsUploadEnable(true);
		it++;
		if ((*it).type != semicolon)
			throw (std::runtime_error("unexpected madafaka `" + (*it).data + "`"));
	}
	else
		throw (std::runtime_error("unexpected madafaka `" + (*it).data + "`"));
}

void	configParser::checkUploadStoreSyntax(std::list<token>::iterator &it, location &loc)
{
	if ((*it).type == parameter)
	{
		loc.setUploadPath((*it).data);
		it++;
		if ((*it).type != semicolon)
			throw (std::runtime_error("unexpected madafaka `" + (*it).data + "`"));
	}
	else
		throw (std::runtime_error("unexpected madafaka `" + (*it).data + "`"));
}

void	configParser::checkLocationSyntax(std::list<token>::iterator &it, serverData &server)
{
	location loc;

	if ((*it).type == parameter)
	{
		loc.setPath((*it).data);
		it++;
		if ((*it).type == openingCurlyBracket)
		{
			while (it != _tokensList.end())
			{
				if ((*it).type == closingCurlyBracket)
					break ;
				else if ((*it).type == name)
				{
					if ((*it).data == "autoindex")
						checkAutoIndexSyntax(++it, loc);
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
						throw (std::runtime_error("holy shit I don't know this directive [" + (*it).data + "] are u insane ?"));
				}
				it++;
			}
			if ((*it).type != closingCurlyBracket)
				throw (std::runtime_error("for god sake ma sed 3lina had location, use this '}'"));
		}
		else
			throw (std::runtime_error("u forgot the focking `{` don't forget it again"));
	}
	else
		throw (std::runtime_error("u madafaka have forgotten an argument and that argument is the focking path"));

	server.setLocations(loc);
}

void	configParser::checkListenSyntax(std::list<token>::iterator &it, serverData &server)
{
	int	port = 0;
	char *ptr = NULL;

	if ((*it).type == parameter)
	{
		port = strtol((*it).data.c_str(), &ptr, 10);
		if (*ptr != '\0' || port <= 0 || port > 65535)
			throw (std::runtime_error("unexpected port `" + (*it).data + "`"));
		server.setPorts(port);
		it++;
		if ((*it).type != semicolon)
			throw (std::runtime_error("unexpected madafaka `" + (*it).data + "`"));
	}
	else
		throw (std::runtime_error("unexpected madafaka `" + (*it).data + "`"));
}

void	configParser::checkHostSyntax(std::list<token>::iterator &it, serverData &server)
{
	if ((*it).type == parameter)
	{
		size_t pos = 0;
		size_t oldPos = 0;
		int octet = 0;
		int count = 0;
		char *ptr;

		while ((pos = (*it).data.find(".", pos)) != std::string::npos)
		{
			octet = strtol((*it).data.substr(oldPos, pos).c_str(), &ptr, 10);
			count++;
			std::cout << "'" << (*it).data.substr(oldPos, pos - oldPos) << "' old pos : " << oldPos << ", pos : " << pos << std::endl;
			if (/* *ptr != '\0' || */octet < 0 || octet > 255 || count > 4)
				throw (std::runtime_error("host not found in \"" + (*it).data + "\" of the \"listen\" directive"));
			pos++;
			oldPos = pos;
		}
		
		std::cout << "'" << (*it).data.substr(oldPos, pos - oldPos) << "' old pos : " << oldPos << ", pos : " << pos << std::endl;
		octet = strtol((*it).data.substr(oldPos, pos).c_str(), &ptr, 10);
		count++;
		// std::cout << "count : " << count << std::endl;
		if (/* *ptr != '\0' || */octet < 0 || octet > 255 || count > 4)
				throw (std::runtime_error("host not found in \"" + (*it).data + "\" of the \"listen\" directive"));


		server.setHost((*it).data);
		it++;
		if ((*it).type != semicolon)
			throw (std::runtime_error("unexpected madafaka `" + (*it).data + "`"));
	}
	else
		throw (std::runtime_error("unexpected madafaka `" + (*it).data + "`"));
}

void	configParser::checkServerNameSyntax(std::list<token>::iterator &it, serverData &server)
{
	if ((*it).type == parameter)
	{
		server.setServerNames((*it).data);
		it++;
		if ((*it).type != semicolon)
			throw (std::runtime_error("unexpected madafaka `" + (*it).data + "`"));
	}
	else
		throw (std::runtime_error("unexpected madafaka `" + (*it).data + "`"));
}

void	configParser::checkRootSyntax(std::list<token>::iterator &it, serverData &server)
{
	if ((*it).type == parameter)
	{
		server.setRoot((*it).data);
		it++;
		if ((*it).type != semicolon)
			throw (std::runtime_error("unexpected madafaka `" + (*it).data + "`"));
	}
	else
		throw (std::runtime_error("unexpected madafaka `" + (*it).data + "`"));
}

void	configParser::checkErrorPageSyntax(std::list<token>::iterator &it, serverData &server)
{
	char *ptr = NULL;
	int errorCode = 0;
	std::string errorPath;

	if ((*it).type == parameter)
	{
		errorCode = std::strtol((*it).data.c_str(), &ptr, 10);
		if (*ptr != '\0') //Don't forget to check errorCode range
			throw (std::runtime_error("unexpected errorCode `" + (*it).data + "`"));
		it++;
		if ((*it).type == parameter)
		{
			errorPath = (*it).data;
			it++;
			if ((*it).type != semicolon)
				throw (std::runtime_error("unexpected madafaka `" + (*it).data + "`"));
		}
		else
			throw (std::runtime_error("unexpected madafaka `" + (*it).data + "`"));
	}
	else
		throw (std::runtime_error("unexpected madafaka `" + (*it).data + "`"));
	server.setErrorPages(errorCode, errorPath);
}

void	configParser::checkMaxBodySizeSyntax(std::list<token>::iterator &it, serverData &server)
{
	int size = 0;
	char *ptr = NULL;

	if ((*it).type == parameter)
	{
		size = strtol((*it).data.c_str(), &ptr, 10);
		if ((*ptr != 'm' && *ptr != 'M') || *(ptr + 1) != '\0')
			throw (std::runtime_error("unexpected client max body size `" + (*it).data + "`"));
		server.setClientMaxBodySize(size);
		it++;
		if ((*it).type != semicolon)
			throw (std::runtime_error("unexpected madafaka `" + (*it).data + "`"));
	}
	else
		throw (std::runtime_error("unexpected madafaka `" + (*it).data + "`"));
}

void	configParser::checkServerSyntax(std::list<token>::iterator &it)
{
	serverData server;

	if ((*it).type != openingCurlyBracket)
		throw (std::runtime_error("hohohoho and how I will determine the server content without '{' idiot"));//throw an error

	while (++it != _tokensList.end())
	{
		if ((*it).type == closingCurlyBracket)
			break ;
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
				checkRootSyntax(++it, server);
			else if ((*it).data == "error_page")
				checkErrorPageSyntax(++it, server);
			else if ((*it).data == "client_max_body_size")
				checkMaxBodySizeSyntax(++it, server);
			else
				throw (std::runtime_error("holy shit I don't know this directive [" + (*it).data + "] are u insane ?"));//throw an error
		}
		else if ((*it).type == openingCurlyBracket)
			throw (std::runtime_error("why the fuck there is another opening curly bracket ?"));//throw an error
	}

	if ((*it).type != closingCurlyBracket)
		throw (std::runtime_error("for god sake ma sed 3lina had server use this '}'"));//throw an error

	_servers.push_back(server);

	// I think you need to increment the iterator to skip the closing curly bracket
	it++; // skip the closing curly bracket

	// std::cout << (*(_servers[0].getErrorPages().begin())).first << std::endl;
	// std::cout << (*(_servers[0].getErrorPages().begin())).second << std::endl;
	// std::cout << *(_servers[0].getServerNames().begin()) << std::endl;
	// std::cout << (_servers[0].getClientMaxBodySize()) << std::endl;
	// std::cout << *(_servers[0].getPorts().begin()) << std::endl;
	// std::cout << *(++_servers[0].getPorts().begin()) << std::endl;
}

void	configParser::checkSyntaxAndFillData()
{
	std::list<token>::iterator it = _tokensList.begin();

	while (it != _tokensList.end())
	{
		if ((*it).data == "server")
			checkServerSyntax(++it);//check server syntax
		else
			throw(std::runtime_error("unknown directive \"" + (*it).data + "\""));//throw an error
	}
}

configParser::configParser(char *configFileName) //args and their count or just file name :3
{
	//don't forget to check the extention of the given file
	startTokenization(configFileName);
	checkSyntaxAndFillData();
	// createServers();
}

configParser::~configParser()
{
}




//************************************//
//**	  must check this case		**//
//************************************//
//listen 80#hello world