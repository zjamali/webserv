/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   configParser.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iltafah <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/21 21:44:02 by iltafah           #+#    #+#             */
/*   Updated: 2022/04/06 14:02:37 by iltafah          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIGPARSER_HPP
# define CONFIGPARSER_HPP

# include <iostream>
# include <string>
# include <list>
# include <fstream>
# include "serverData.hpp"

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
		std::vector<serverData> _servers;
	
	private:
		void checkServerSyntax(std::list<token>::iterator &it);
		void checkHostSyntax(std::list<token>::iterator &it, serverData &server);
		void checkRootSyntax(std::list<token>::iterator &it, serverData &server);
		void checkListenSyntax(std::list<token>::iterator &it, serverData &server);
		void checkLocationSyntax(std::list<token>::iterator &it, serverData &server);
		void checkErrorPageSyntax(std::list<token>::iterator &it, serverData &server);
		void checkServerNameSyntax(std::list<token>::iterator &it, serverData &server);
		void checkMaxBodySizeSyntax(std::list<token>::iterator &it, serverData &server);

		void checkIndexSyntax(std::list<token>::iterator &it, location &loc);
		void checkReturnSyntax(std::list<token>::iterator &it, location &loc);
		void checkAutoIndexSyntax(std::list<token>::iterator &it, location &loc);
		void checkFastcgiPassSyntax(std::list<token>::iterator &it, location &loc);
		void checkUploadStoreSyntax(std::list<token>::iterator &it, location &loc);
		void checkUploadEnableSyntax(std::list<token>::iterator &it, location &loc);
		void checkAllowedMethodsSyntax(std::list<token>::iterator &it, location &loc);

	public:
		configParser(char *configFileName);
		void startTokenization(char *configFileName);
		void checkSyntaxAndFillData();
		const std::vector<serverData> &getServers() const;
		~configParser();
};

#endif
