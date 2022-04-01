/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iltafah <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/21 21:44:02 by iltafah           #+#    #+#             */
/*   Updated: 2022/03/23 01:07:31 by iltafah          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGPARSER_HPP
# define CONFIGPARSER_HPP

# include <iostream>
# include <string>
# include <list>
# include <fstream>

enum type { name, parameter, semicolon, openingCurlyBracket, closingCurlyBracket };

struct token
{
	std::string data;
	type		type;
};

class configParser
{
	private:
		std::list<token> _tokensList;
	
	private:
		void checkHostSyntax(std::list<token>::iterator &it);
		void checkRootSyntax(std::list<token>::iterator &it);
		void checkServerSyntax(std::list<token>::iterator &it);
		void checkListenSyntax(std::list<token>::iterator &it);
		void checkLocationSyntax(std::list<token>::iterator &it);
		void checkErrorPageSyntax(std::list<token>::iterator &it);
		void checkServerNameSyntax(std::list<token>::iterator &it);
		void checkMaxBodySizeSyntax(std::list<token>::iterator &it);

	public:
		configParser(char *configFileName);
		void startTokenization(char *configFileName);
		void checkSyntaxErrors();
		~configParser();
};

#endif
