// From: www.thegeekstuff.com/2011/12/c-socket-programming

/*
server has to be running the entire time, otherwise the clinets don't have a 
connectino point to go to

first compoile server "gcc server.c -o server"
then compoile clients "gcc client.c -o client"
then run server "./server"
and client "./client 127.0.0.1"
tthen it works, cleitn goes to completion, server stays open
but have to put in the 60 second timer
*/
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <time.h> 
#include <iostream>

using namespace std;

void Trans( int n );    // Forward declarations of the provided functions without using header file
void Sleep( int n );
/*
    I made these global so then I can have the main function cleaner by utilizing
    the void methods
*/

// listenfd is a socket
// socket is like adoor
// both sides make door
// then later we say "these are the same door"
int listenfd = 0, connfd = 0;
// internet socket address
struct sockaddr_in serv_addr; 
int portNum;
char sendBuff[1025];


void setup( int argc, char *argv[] )
{
    if( argc != 2 )     // maybe later i should change these to ask again? possible?
    {
        cout << "Invalid input arguments" << endl;
        exit( EXIT_FAILURE );
    }

    portNum = stoi( argv[1] );

    if( portNum < 5000 or portNum > 64000 )
    {
        cout << "Port number must be between 5000 and 64,000" << endl;
        exit( EXIT_FAILURE );
    } else {
        cout << "Port number selected: " << portNum << endl;
    }
    // set the memory block needed by the server
    // ( Pointer to the block of memory to fill
    // , Value to be set. The value is passed as an int, 
    // but the function fills the block of memory using the 
    // unsigned char conversion of this value, 
    // size )
    // sets all addresess within that block to 0 i think
    memset( &serv_addr, '0', sizeof( serv_addr ) );
    memset( sendBuff, '0', sizeof( sendBuff ) ); 



    // where we create the socket door for the server
    // creats an endpoint connection for communication and returns
    // a file descriptor that refers to that endpoint
    // maybe like a pointer to it?
    listenfd = socket( AF_INET, SOCK_STREAM, 0 );

    /*
    setting the type of interenet addresses as the IPv4 IP adresses
    */
    serv_addr.sin_family = AF_INET;

    /*
    tell the server to listen to any/all interfaces
    */
    serv_addr.sin_addr.s_addr = htonl( INADDR_ANY );

    serv_addr.sin_port = htons(portNum);

    /*
    binds all the socket info to the currently socket,
    like sends the address to the socket
    */
    bind( listenfd, ( struct sockaddr* ) &serv_addr, sizeof( serv_addr ) ); 


    /*
    this server/socket can only allow this many ppl
    enables the thing to use accept()
    10 is the length of the cue of incoming messages 
    */
    listen( listenfd, 10 );       // 10 is max num of clients
}


void serverWhileLoop()
{
    time_t ticks; 
    while( 1 )  // servser goes foever
    {
        /*
        extracts the firs tconnection request on the queue of pending 
        connections on the listening socket
        */
        connfd = accept( listenfd, ( struct sockaddr* ) NULL, NULL ); 

        /*
        sending the date time to the buffer to be read by the client
        */
        ticks = time( NULL ); // time null returns current calendar time
        // (pointer to buffer, max num of bytes, string has
        // at most n-1 length, leaving char for terminating null char, 
        // format string , additional according to format buffer)
        snprintf( sendBuff, sizeof( sendBuff ), "%.24s\r\n", ctime( &ticks ) );
        // writes to a file referred to by the first arg
        // writes what's in sendbuffer to the socket because sendbuff
        // is local, socket is what's the 'sharebuffer'
        write( connfd, sendBuff, strlen( sendBuff ) ); 

        close( connfd );
        sleep( 1 );

     }
}


int main(int argc, char *argv[])
{
    setup( argc, argv );
    time_t ticks; 
    serverWhileLoop();
}