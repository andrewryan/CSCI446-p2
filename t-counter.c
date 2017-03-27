/*
* Kyle Kruskamp
* Andrew Ryan
* CSCI 446
* Spring 2017
*
* This template provides the framework you need for program 1. Complete the functions
* defined and create any you feel are neccessary. stream-talk-client.c on Learn will
* prove useful.
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <resolv.h>
#include <netinet/in.h>

// #define host "www.ecst.csuchico.edu"
// #define SERVER_PORT "80"
// #define REQUEST "GET /~ehamouda/file.html HTTP/1.0\n\n"

//send request "get /~....", recv the buffer(data from webpage)  /// this reads in data from that webpage
//then manipulate it to be able to use readchunk

/***************************************************************
* Arguments: same as recv(2)
* Return:-1 on error or bytes received
*
* Receive len bytes of data into array buf from socket sockfd. Always receive len bytes
* of data unless the socket closes. The return value will fall into one of three cases:
*   (1) on success the return value equals len
*   (2) on error the return value equals -1
*   (3) if the socket closes without receiving len bytes, then return the number
*       actually received, which may be zero
***************************************************************/
// ssize_t readchunck( int sockfd, void *buf, size_t len );

#define host "www.ecst.csuchico.edu"
#define SERVER_PORT "80"
#define REQUEST "GET /~ehamouda/file.html HTTP/1.0\n\n"

//stream-talk-client.c given example
// #define SERVER_PORT "5432"
#define MAX_LINE 1000

int main(int argc, char *argv[])
{
  struct addrinfo hints;
  struct addrinfo *rp, *result;
  // char *host;
  char buf[MAX_LINE];
  int s;
  int len;


  // ************ used for testing how many command line args were passed
  // int count;
  // if (argc > 1){
  //     for (count = 1; count < argc; count++){
	//       printf("argv[%d] = %s\n", count, argv[count]);
	//     }
  // }
  if (argc > 3){
	  perror("Too many command line arguments given.");
    exit(1);
  }
  if (argc < 3){
	  perror("Not enough command line arguments were given.");
    exit(1);
  }
  /* Translate host name into peer's IP address */
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = 0;
  hints.ai_protocol = 0;

  if ((s = getaddrinfo(host, SERVER_PORT, &hints, &result)) != 0 ) {
    fprintf(stderr, "%s: getaddrinfo: %s\n", argv[0], gai_strerror(s));
    exit(1);
  }

  /* Iterate through the address list and try to connect */
  for (rp = result; rp != NULL; rp = rp->ai_next) {
    if ((s = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol)) == -1 ) {
	continue;
      }

    if (connect(s, rp->ai_addr, rp->ai_addrlen) != -1) {
  printf("host is: " host"\n");
  printf("port is: " SERVER_PORT"\n");
  // printf(hints"\n");
  printf("connect successful\n");
	break;
      }

    // close(s);
  }

  if (rp == NULL) {
    perror("stream-talk-client: connect");
    exit(1);
  }

  freeaddrinfo(result);

  /* Main loop: get and send lines of text */
  // while (fgets(buf, sizeof(buf), stdin)) {
  //     buf[MAX_LINE-1] = '\0';
  //     len = strlen(buf) + 1;
  //     send(s, buf, len, 0);  /* Call readchunck in here ... send request to webpage */
  //   }

      char *message , server_reply[1000];
      //Send some data
      message = REQUEST;
      if( send(s , message , strlen(message) , 0) < 0)
      {
          perror("Send failed");
          exit(1);
      }
      printf("Data Send\n");

      //Receive a reply from the server
      if( recv(s, server_reply , 2000 , 0) < 0)
      {
          perror("recv failed");
          exit(1);
      }
      printf("Reply received\n");
      printf(server_reply);



  close(s);
  printf("\n");

  return 0;

}

ssize_t readchunck( int sockfd, void *buf, size_t len )
{

/* Define readchunck to return exactly len bytes unless an error occurs or the socket closes.
*/

}





//*********** everything below was just stuff I was trying to use to set up the socket connection **************//



//works to get ip address
// int main(int argc , char *argv[])
// {
//   char *hostname = "www.ecst.csuchico.edu";
//   char ip[100];
//   struct hostent *he;
//   struct in_addr **addr_list;
//   int i;
//
//   if ( (he = gethostbyname( hostname ) ) == NULL)
//   {
//       //gethostbyname failed
//       herror("gethostbyname");
//       return 1;
//   }
//
//   //Cast the h_addr_list to in_addr , since h_addr_list also has the ip address in long format only
//   addr_list = (struct in_addr **) he->h_addr_list;
//
//   for(i = 0; addr_list[i] != NULL; i++)
//   {
//       //Return the first one;
//       strcpy(ip , inet_ntoa(*addr_list[i]) );
//   }
//
//   printf("%s : has ip address : %s" , hostname , ip);
// 	printf("\n");
//   return 0;
// }


//most progress
// int main(int argc , char *argv[])
// {
// 	printf("beginning");
//   int socket_desc;
//   struct sockaddr_in server;
//   char *message , server_reply[1000];
//
//   //Create socket
//   socket_desc = socket(AF_INET , SOCK_STREAM , 0);
//   if (socket_desc == -1)
//   {
//       printf("Could not create socket");
//   }
//
//   server.sin_addr.s_addr = inet_addr("74.125.235.20");
//   server.sin_family = AF_INET;
//   server.sin_port = htons( 80 );
//
//   //Connect to remote server
//   if (connect(socket_desc , (struct sockaddr *)&server , sizeof(server)) < 0)
//   {
//       puts("connect error");
//       return 1;
//   }
//
//   puts("Connected\n");
//
//   //Send some data
//   message = "GET / HTTP/1.1\r\n\r\n";
//   if( send(socket_desc , message , strlen(message) , 0) < 0)
//   {
//       puts("Send failed");
//       return 1;
//   }
//   puts("Data Send\n");
//
//   //Receive a reply from the server
//   if( recv(socket_desc, server_reply , 2000 , 0) < 0)
//   {
//       puts("recv failed");
//   }
//   puts("Reply received\n");
//   puts(server_reply);
// 	// read(socket_desc, server_reply , 2000);
// 	// close(socket_desc);
//   return 0;
// }


//to connect to a socket
// int main(int argc , char *argv[])
// {
//   printf("Could not create socket");
//   int socket_desc;
//   struct sockaddr_in server;
//
//   //Create socket
//   socket_desc = socket(AF_INET , SOCK_STREAM , 0);
//   if (socket_desc == -1)
//   {
//       printf("Could not create socket");
//   }
//
//   server.sin_addr.s_addr = inet_addr("74.125.235.20");
//   server.sin_family = AF_INET;
//   server.sin_port = htons( 80 );
//
//   //Connect to remote server
//   if (connect(socket_desc , (struct sockaddr *)&server , sizeof(server)) < 0)
//   {
//       puts("connect error");
//       return 1;
//   }
//
//   puts("Connected");
//   return 0;
// }






// int main(int argc , char *argv[])
// {
//     int sock;
//     struct sockaddr_in server;
//     char message[1000] , server_reply[2000];
//
//     //Create socket
//     sock = socket(AF_INET , SOCK_STREAM , 0);
//     if (sock == -1)
//     {
//         printf("Could not create socket");
//     }
//     puts("Socket created");
//
//     server.sin_addr.s_addr = inet_addr("127.0.0.1");
//     server.sin_family = AF_INET;
//     server.sin_port = htons( 80 );
//
//     //Connect to remote server
//     if (connect(sock , (struct sockaddr *)&server , sizeof(server)) < 0)
//     {
//         perror("connect failed. Error");
//         return 1;
//     }
//
//     puts("Connected\n");
//
//     //keep communicating with server
//     while(1)
//     {
//         printf("Enter message : ");
//         scanf("%s" , message);
//
//         //Send some data
//         if( send(sock , message , strlen(message) , 0) < 0)
//         {
//             puts("Send failed");
//             return 1;
//         }
//
//         //Receive a reply from the server
//         if( recv(sock , server_reply , 2000 , 0) < 0)
//         {
//             puts("recv failed");
//             break;
//         }
//
//         puts("Server reply :");
//         puts(server_reply);
//     }
//
//     close(sock);
//     return 0;
// }



// int main( )
// // int main( int argc, char *argv[] )
// {
// //convert hostname to ip address
// 	struct hostent* host;
// 	host = gethostbyname("www.ecst.csuchico.edu");
// 	printf("IP address = %s\n",
// 	  inet_ntoa(*(long*)host->h_addr_list[0]));
// // for selecting the port
// 	struct servent *srv;
// 	srv = getservbyname("http", "tcp");
// 	printf("%s: port=%d\n", srv->s_name, ntohs(srv->s_port));
// //create socket
// 	int sd;
// 	sd = socket(PF_INET, SOCK_STREAM, 0);
//
// 	struct sockaddr_in addr;
// 	memset(&addr, 0, sizeof(addr));    /* create & zero struct */
// 	addr.sin_family = AF_INET;    /* select internet protocol */
// 	addr.sin_port = srv->s_port;         /* set the port # */
// 	addr.sin_addr.s_addr =
// 	        *(long*)host->h_addr_list[0]; /* set the addr */
// 	connect(sd, &addr, sizeof(addr));         /* connect! */
//
//
// /* Call readchunck in here ... */
//
// 	return 0;
//}
