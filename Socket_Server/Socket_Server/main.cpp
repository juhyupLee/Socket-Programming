#include "server_TCP_1.h"
#include "udp_server_1.h"
#include "Multiplexing.h"
#include "thread_create_1.h"
#include "CriticalSection_1.h"
#include "Semaphore_1.h"
#include "AsyncServer.h"
int main(int argc, char* argv[])
{

	//Server_UDP_1(argc, argv);
	//Multiplexing_1(argc, argv);
	//Thread_Create_1(argc, argv);
	//Critical_Section_1(argc, argv);
	//Semaphroe_1(argc, argv);
	AsyncServer(argc, argv);



}