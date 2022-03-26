# include "_server_.hpp"

webServer::webServer(std::vector<SData> _serv) : _servers_(_serv)
{
}

void	webServer::_start_()
{
	// for each server create it's own socket
	for (size_t i = 0; i < _servers_.size(); i++)
	{
		// create instance of a class socket and add it the socket list:
		this->_sockets_.push_back(_socket_());
		// for the port and host ,
		//u need to get them from parser data _servers_[i].getport and host
		if (this->_sockets_[i]._create_(80, "144.33.44.5") == -1)
			continue;
		
		//listen to socket:
		if (this->_sockets_[i]._listen_() < 0)
		{
			// throw an exception and kill th socket
		}
	}
}