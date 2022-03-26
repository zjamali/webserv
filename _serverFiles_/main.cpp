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