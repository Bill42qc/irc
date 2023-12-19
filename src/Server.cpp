#include "Server.hpp"

const char *WELCOME_MSG = "Welcome into FT_IRC by bmarttin, pbergero and rofontai";
const int MAX_CONNECTIONS = 100;
const int BUFFER_SIZE = 2048;
static bool exiting = false;

Server::Server(){
}

Server::~Server(){
}

// bool isAuthValid (Client &client)
// {

// }

void Server::handleClientInput(int i) {
    Client &client = getClient(i);
    int clientSocket = client.getClientSocket();
    char buffer[BUFFER_SIZE];
    bzero(buffer, sizeof(buffer));

    ssize_t bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);

    if (bytesRead > 0) {
        client.catMSG(buffer);
        
        if (buffer[bytesRead - 2] == 13) {
           
            std::cout << "Received data client: " << client.getMSG() << std::endl;
			std::string tmpStr = client.getMSG();

			// Remove newline characters '\n' before splitting on '\r'
            removeNewlines(tmpStr);
            std::vector<std::string> commandChain = splitString(tmpStr, '\r'); 

            for (size_t i = 0; i < commandChain.size(); i++) {
				addSpaceAfterKeywords(commandChain[i]);
                std::cout << "command chain [" << i << "]: " << commandChain[i] << std::endl;

                parsMsg(commandChain[i], client);
            }

            try {
                ACommand *cmd = commandFactory(client);
                cmd->exe();
                delete cmd;
            } catch (std::exception &) {
                // Doing nothing is fine; we just stop doing useless stuff
            }
            client.resetMSG();
        }
    } else if (bytesRead == 0) {
        // Connection closed by the client
        std::cout << "Connection closed by client." << std::endl;
        close(clientSocket);
        removeClient(i);
        pollfd_.erase(pollfd_.begin() + i + 1);
    } else {
        throw RecvException();
    }
}

///@brief
/*accept a new connection and create the socket for that connection.
Than the socket is added into the vector of socket*/
void Server::receiveNewConnection(){
	int clientSocket;
	sockaddr_in clientAddress;
	socklen_t clientAddressSize = sizeof(clientAddress);
	struct pollfd clientfd;

	// Accept a connection from a client
	clientSocket = accept(socket_, (struct sockaddr*)&clientAddress, &clientAddressSize);
	if (clientSocket == -1) {
		std::cerr << "Error accepting connection." << std::endl;
		throw AcceptException();
	}

	clientfd.fd = clientSocket;
	clientfd.events = POLLIN;
	//add the socket of the new client to the vector
	pollfd_.push_back(clientfd);

	Client client(clientSocket);
	addClient(client);
}

///@brief
//create the socket use for connection
void Server::createSocket()
{
	socket_ = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_ == -1) {
		std::cerr << "Error creating socket." << std::endl;
	   throw SocketException();
	int i = 1;
	if (setsockopt(socket_, SOL_SOCKET, SO_REUSEADDR, &i, sizeof(i)))
		throw SocketException();

	if (fcntl(socket_, F_SETFL, O_NONBLOCK) == -1)
		throw SocketException();
	}
}

///@brief
//bind the socket to the ports given the the server
void Server::bindSocket(){
	sockaddr_in serverAddress;

	serverAddress.sin_family = AF_INET;
	serverAddress.sin_port = htons(port_);
	serverAddress.sin_addr.s_addr = INADDR_ANY;

	// Bind the socket to the specified address and port
	if (bind(socket_, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
		close(socket_);
		throw BindingException();
	}
}

///@brief
//listen to the socket for incomming input
void Server::listenSocket(){
	if (listen(socket_, MAX_CONNECTIONS) == -1) {
		close(socket_);
		throw ListenException();
		}

	std::cout << "Server listening on port " << port_ << "..." << std::endl;
}

void signalHandler(int sig){
	(void)sig;
	exiting = true;
}

///@brief
//Main loop of the server
//Poll an array of fd and wait for event to happen.
//When event happen handle the data send or create new client
void Server::run(){
	struct pollfd serverfd;

	serverfd.fd = socket_;
	serverfd.events = POLLIN;
	pollfd_.push_back(serverfd);

	while (exiting == false) {
		int result = poll(pollfd_.data(), pollfd_.size(), 50);
		if (result == -1 && errno != EINTR)
			throw PollException();
		else if (result > 0) {
			for (size_t i = 0; i < pollfd_.size(); ++i) {
				try {
					if (pollfd_[i].revents & POLLIN) {
						if (pollfd_[i].fd == socket_) {
							receiveNewConnection();
							}
						else {
							handleClientInput(i - 1);
						}
					}
				}
				catch (std::exception& e) {
				std::cout << e.what() << std::endl;
				removeClient(i - 1);
				}
			}
		}
	}
}

///@brief
//init the socket needed for the server
///@param
//port : the port number that will be listen to
///@param
//password : the password needed to connect to the server
void Server::init(const std::string &port, const std::string &password){
	port_ = std::stoi(port);
	serverPassword_ = password;
	createSocket();
	bindSocket();
	listenSocket();
	signal(SIGINT, signalHandler);
}

void Server::shutdown(){
	std::cout << "shutting down server" << std::endl;
	for (unsigned int i = 0; i < clientVector_.size(); ++i){
		removeClient(i);
	}
	close(pollfd_[0].fd); //closing server socket

	pollfd_.clear();
	clientVector_.clear();
}

///@brief
//Add a client to the server
///@param
//client : the client added to the server
void Server::addClient(Client &client){
	clientVector_.push_back(client);
}

///@brief
//remove a client from the server
///@param
//i : the index of the client in the vector
void Server::removeClient(int i){
	clientVector_[i].closeSocket();
	if (i >= 0 && i < static_cast<int>(clientVector_.size()))
		 clientVector_.erase(clientVector_.begin() + i);
}


///@brief
//add a channel to the map
void Server::addChannel(Channel &channel){
	channelVector_.push_back(channel);
}

Channel &Server::getChannel(std::string &name){
	for (unsigned long i = 0;  i < channelVector_.size(); ++i){
		if (channelVector_[i] == name){
			return channelVector_[i];
		}
	}
	throw (std::runtime_error("channel does not exist"));
}

void Server::broadcastMessage(const std::string& message, Client &client) {
	for (size_t i = 0; i < clientVector_.size(); ++i) {
		if (client != clientVector_[i]) {
			// Send the message to all clients except the sender
			clientVector_[i].send(client.getUserName() + ": ");
			clientVector_[i].send(message);
			clientVector_[i].send("\n");
		}
	}
}

Client &Server::getClientByHostName(std::string name){
		for (unsigned long i = 0; i < clientVector_.size(); ++i){
		if (clientVector_[i].getHostName() == name)
			return clientVector_[i];
	}
	throw std::runtime_error("Client not found");

}

Client &Server::getClientByUserName(std::string name){
	for (unsigned long i = 0; i < clientVector_.size(); ++i){
		if (clientVector_[i].getUserName() == name)
			return clientVector_[i];
	}
	throw std::runtime_error("Client not found");
}

Client &Server::getClientByNickName(std::string name){
	for (unsigned long i = 0; i < clientVector_.size(); ++i){
		if (clientVector_[i].getNickName() == name)
			return clientVector_[i];
	}
	throw std::runtime_error("Client not found");
}

bool Server::checkClientByNickName(std::string name){
	for (unsigned long i = 0; i < clientVector_.size(); ++i){
		if (clientVector_[i].getNickName() == name)
			return true;
	}
	return false;
}

void Server::handlePing(Client &client) {
	std::string message = client.getMSG();

		// Extract the PING message content
		std::string pingContent = message.substr(5);

		// Send a PONG response back to the client
		std::string pongResponse = "PONG " + pingContent + CRLF;
		client.send(pongResponse);

		std::cout <<RED "Sent PONG to client: " WHT<< client.getUserName() << pongResponse << std::endl;

}
