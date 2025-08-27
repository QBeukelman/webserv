/* ************************************************************************** */
/*                                                                            */
/*                                                        ::::::::            */
/*   Config.hpp                                         :+:    :+:            */
/*                                                     +:+                    */
/*   By: dkolodze <dkolodze@student.codam.nl>         +#+                     */
/*                                                   +#+                      */
/*   Created: 2025/08/18 19:25:34 by dkolodze      #+#    #+#                 */
/*   Updated: 2025/08/25 09:54:22 by hein          ########   odam.nl         */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <string>
#include <vector>

typedef struct	Socket
{
	std::string address;
	int			port;
} 				Socket;

typedef struct	Location
{
	std::string		path;
	std::string		root;			/*overwrite*/
	unsigned int	allowedMethods; /*bitmask check*/
	bool			autoindex;
	std::string		upload;
	std::string		cgi;

}				Location;

enum HttpMethodMask
{
	METHOD_GET = 1 << 0,
	METHOD_POST = 1 << 1,
	METHOD_DELETE = 1 << 2
};

class Config
{
  public:

	Socket		listen;
	std::string name;
	std::string	root;
	std::vector<Location> locations;


  private:
};

#endif // CONFIG_HPP
