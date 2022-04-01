/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iltafah <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/21 21:44:33 by iltafah           #+#    #+#             */
/*   Updated: 2022/03/29 13:24:55 by iltafah          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./configParser.hpp"

#ifndef DEBUG
# define DEBUG false
#endif

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

			if (byte == '#')
			{
				//skip hashtags
				while (configFile.good())
				{
					configFile.get(byte);
					if (byte != '\n')
						continue ;
					break ;
				}
			}

			/**********************************/
			/* another loop for reading words */
			/**********************************/
			while (configFile.good())
			{
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
						// std::cout << tokenNode.data << "     ";
						// std::cout << (tokenNode.type == name ? "name" : tokenNode.type == parameter ? "parameter" : tokenNode.type == openingCurlyBracket ? "openingCurlyBracket" : tokenNode.type == closingCurlyBracket ? "closingCurlyBracket" : "semicolon") << std::endl;
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
						// std::cout << tokenNode.data << "    ";
						// std::cout << (tokenNode.type == name ? "name" : tokenNode.type == parameter ? "parameter" : tokenNode.type == openingCurlyBracket ? "openingCurlyBracket" : tokenNode.type == closingCurlyBracket ? "closingCurlyBracket" : "semicolon") << std::endl;
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

void	configParser::checkLocationSyntax(std::list<token>::iterator &it)
{

}

void	configParser::checkListenSyntax(std::list<token>::iterator &it)
{
	if ((*it).type == parameter)
	{
		it++;
		if ((*it).type != semicolon)
			throw (std::runtime_error("unexpected madafaka `" + (*it).data + "`"));
	}
	else
		throw (std::runtime_error("unexpected madafaka `" + (*it).data + "`"));
}

void	configParser::checkHostSyntax(std::list<token>::iterator &it)
{
	if ((*it).type == parameter)
	{
		it++;
		if ((*it).type != semicolon)
			throw (std::runtime_error("unexpected madafaka `" + (*it).data + "`"));
	}
	else
		throw (std::runtime_error("unexpected madafaka `" + (*it).data + "`"));
}

void	configParser::checkServerNameSyntax(std::list<token>::iterator &it)
{
	if ((*it).type == parameter)
	{
		it++;
		if ((*it).type != semicolon)
			throw (std::runtime_error("unexpected madafaka `" + (*it).data + "`"));
	}
	else
		throw (std::runtime_error("unexpected madafaka `" + (*it).data + "`"));
}

void	configParser::checkRootSyntax(std::list<token>::iterator &it)
{
	if ((*it).type == parameter)
	{
		it++;
		if ((*it).type != semicolon)
			throw (std::runtime_error("unexpected madafaka `" + (*it).data + "`"));
	}
	else
		throw (std::runtime_error("unexpected madafaka `" + (*it).data + "`"));
}

void	configParser::checkErrorPageSyntax(std::list<token>::iterator &it)
{
	if ((*it).type == parameter)
	{
		it++;
		if ((*it).type == parameter)
		{
			it++;
			if ((*it).type != semicolon)
				throw (std::runtime_error("unexpected madafaka `" + (*it).data + "`"));
		}
		else
			throw (std::runtime_error("unexpected madafaka `" + (*it).data + "`"));
	}
	else
		throw (std::runtime_error("unexpected madafaka `" + (*it).data + "`"));
}

void	configParser::checkMaxBodySizeSyntax(std::list<token>::iterator &it)
{
	if ((*it).type == parameter)
	{
		it++;
		if ((*it).type != semicolon)
			throw (std::runtime_error("unexpected madafaka `" + (*it).data + "`"));
	}
	else
		throw (std::runtime_error("unexpected madafaka `" + (*it).data + "`"));
}

void	configParser::checkServerSyntax(std::list<token>::iterator &it)
{
	if ((*it).type != openingCurlyBracket)
		throw (std::runtime_error("hohohoho and how I will determine the server content without '{' idiot"));//throw an error

	while (++it != _tokensList.end())
	{
		if ((*it).type == closingCurlyBracket)
			break ;
		else if ((*it).type == name)
		{
			if ((*it).data == "location")
				checkLocationSyntax(++it);
			else if ((*it).data == "listen")
				checkListenSyntax(++it);
			else if ((*it).data == "host")
				checkHostSyntax(++it);
			else if ((*it).data == "server_name")
				checkServerNameSyntax(++it);
			else if ((*it).data == "root")
				checkRootSyntax(++it);
			else if ((*it).data == "error_page")
				checkErrorPageSyntax(++it);
			else if ((*it).data == "client_max_body_size")
				checkMaxBodySizeSyntax(++it);
			else
				throw (std::runtime_error("holy shit I don't know this directive [" + (*it).data + "] are u insane ?"));//throw an error
		}
		else if ((*it).type == openingCurlyBracket)
			throw (std::runtime_error("why the fuck there is another opening curly bracket ?"));//throw an error
	}

	if ((*it).type != closingCurlyBracket)
		throw (std::runtime_error("for god sake ma sed 3lina had server use this '}'"));//throw an error
}

void	configParser::checkSyntaxErrors()
{
	std::list<token>::iterator it = _tokensList.begin();

	// name parameter semicolon
	// name parameter parameter semicolon
	// name parameter obrackets ... cbrackets

	// server { }
	// location /path { }
	// listen 80 ;
	// host 0.0.0.0 ;
	// server_name www.example.com ;
	// root /path ;
	// error_page 404 /error_page ;

	while (it != _tokensList.end())
	{
		if ((*it).data == "server")
			checkServerSyntax(++it);//check server syntax
		else
			std::cout << "tada" << std::endl ;//throw an error
		break ;//remove this break ;
	}
}

configParser::configParser(char *configFileName) //args and their count or just file name :3
{
	//don't forget to check the extention of the given file
	startTokenization(configFileName);
	checkSyntaxErrors();
}

configParser::~configParser()
{
}
