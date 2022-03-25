/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configParser.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iltafah <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/21 21:44:33 by iltafah           #+#    #+#             */
/*   Updated: 2022/03/23 22:04:05 by iltafah          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "./configParser.hpp"

configParser::configParser(char *configFileName) //args and their count
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
						_tokensList.push_back(tokenNode);
						std::cout << tokenNode.data << std::endl;
						word.clear();
					}

					/***************************************************************/
					/* now check if the byte is one of the following '{'  '}'  ';' */
					/* if yes store it                                             */
					/***************************************************************/
					if (byte == '{' || byte == '}' || byte == ';')
					{
						tokenNode.data = byte;
						// tokenNode.type = ;
						_tokensList.push_back(tokenNode);
						std::cout << tokenNode.data << std::endl;
					}
					break ;
				}
				word += byte;
				configFile.get(byte);
			}
		}
	}
	// std::cout << word << std::endl;
	//throw some error
	//else
}

configParser::~configParser()
{
}
