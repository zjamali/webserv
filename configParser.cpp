/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iltafah <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/21 21:44:33 by iltafah           #+#    #+#             */
/*   Updated: 2022/04/05 00:28:39 by iltafah          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./configParser.hpp"

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
				if (isspace(byte) || byte == '{' || byte == '}' || byte == ';' || byte == '#')
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

void	configParser::checkAutoIndexSyntax(std::list<token>::iterator &it)
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

void	configParser::checkIndexSyntax(std::list<token>::iterator &it)
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

void	configParser::checkAllowMethodsSyntax(std::list<token>::iterator &it)
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

void	configParser::checkReturnSyntax(std::list<token>::iterator &it)
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

void	configParser::checkFastcgiPassSyntax(std::list<token>::iterator &it)
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

void	configParser::checkUploadEnableSyntax(std::list<token>::iterator &it)
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

void	configParser::checkUploadStoreSyntax(std::list<token>::iterator &it)
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

void	configParser::checkLocationSyntax(std::list<token>::iterator &it)
{
	if ((*it).type == parameter)
	{
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
						checkAutoIndexSyntax(++it);
					else if ((*it).data == "index")
						checkIndexSyntax(++it);
					else if ((*it).data == "allow_methods")
						checkAllowMethodsSyntax(++it);
					else if ((*it).data == "return")
						checkReturnSyntax(++it);
					else if ((*it).data == "fastcgi_pass")
						checkFastcgiPassSyntax(++it);
					else if ((*it).data == "upload_enable")
						checkUploadEnableSyntax(++it);
					else if ((*it).data == "upload_store")
						checkUploadStoreSyntax(++it);
					else
						throw (std::runtime_error("holy shit I don't know this directive [" + (*it).data + "] are u insane ?"));
				}
				it++;
			}
			if ((*it).type != closingCurlyBracket)
				throw (std::runtime_error("for god sake ma sed 3lina had location, use this '}'"));//throw an error
		}
		else
			throw (std::runtime_error("u forgot the focking `{` don't forget it again"));
	}
	else
		throw (std::runtime_error("u madafaka have forgotten an argument and that argument is the focking path"));
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
	// I think you need to increment the iterator to skip the closing curly bracket
	//it++;
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

void	fillServersData()
{
	
}

configParser::configParser(char *configFileName) //args and their count or just file name :3
{
	//don't forget to check the extention of the given file
	startTokenization(configFileName);
	checkSyntaxErrors();
	fillServersData();
}

configParser::~configParser()
{
}




//************************************//
//**	  must check this case		**//
//************************************//
//listen 80#hello world
