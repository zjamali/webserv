/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iltafah <marvin@42.fr>                     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/03/21 23:41:00 by iltafah           #+#    #+#             */
/*   Updated: 2022/04/13 07:22:57 by iltafah          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../headers/_UltimateHeader_.hpp"

int main(int argc, char **argv)
{
	if (argc == 2)
	{
		try
		{
			configParser config(argv[1]);
			webServer	ws(config);
		}
		catch(const std::exception& e)
		{
			std::cerr << e.what() << '\n';
		}
	}
	else
		std::cout << "Error, you should pass a config file" << std::endl;
	return (0);
}