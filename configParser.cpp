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
}

void	configParser::checkSyntaxErrors()
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

configParser::configParser(char *configFileName) //args and their count or just file name :3
{
	startTokenization(configFileName);
	checkSyntaxErrors();
}

configParser::~configParser()
{
}
