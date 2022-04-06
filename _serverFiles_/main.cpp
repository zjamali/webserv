/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abdait-m <abdait-m@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/26 20:43:31 by abdait-m          #+#    #+#             */
/*   Updated: 2022/03/26 20:43:32 by abdait-m         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "_server_.hpp"

// class SData{
	
// 	private:
// 		int	_port_;
// 		std::string	_host_;
// 		std::vector<std::string> _names_;
// 		std::vector<std::string> _locs_;
// };

int main(int argc, char *argv[])
{
	// getter for vector of servers from the parser....
	std::vector<SData>	parsingData;
	webServer server(parsingData);

	server._start_();
}