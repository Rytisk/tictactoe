#ifdef _WIN32
	#include <winsock2.h>
	#define socklen_t int
#else
	#include <sys/socket.h>
	#include <sys/types.h>
	#include <netinet/in.h>
	#include <sys/select.h>
	#include <arpa/inet.h>
	#include <unistd.h>
#endif
#include <string>

using namespace std;

class Client
{
private:
	void PrintBoard(char board[]);
	void CloseConnection(int s_socket);
	struct sockaddr_in servaddr;
	int s_socket;
public:
	void Init();
	int Connect();
	int CreateSocket();
	void Run();
	int SetAddress(unsigned int port, string ip);
	void Exit();
};