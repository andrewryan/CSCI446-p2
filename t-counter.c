// Kyle Kruskamp
// Andrew Ryan
// CSCI 446
// Spring 2017

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

/***************************************************************
* Some Requirements --cd
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

#define host "www.ecst.csuchico.edu"
#define SERVER_PORT "80"
#define REQUEST "GET /~ehamouda/file.html HTTP/1.0\n\n"
#define MAX_LINE 1000  // determines buffer size for recv()


int main(int argc, char *argv[])
{
  struct addrinfo hints;
  struct addrinfo *rp, *result;
  int s;
  char *len;
  char *value;

  if (argc > 3){
	  perror("Too many command line arguments given.");
    return -1;
  }
  if (argc < 3){
	  perror("Not enough command line arguments were given.");
    return -1;
  }
  //storing command line arguments to be used in readchunk
  len = argv[1];
  value = argv[2];
  printf("command line byte length = ");
  printf("%s\n",len);
  printf("command line search value = ");
  printf("%s\n",value);

  /* Translate host name into peer's IP address */
  memset(&hints, 0, sizeof(hints));
  hints.ai_family = AF_INET;
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = 0;
  hints.ai_protocol = 0;

  if ((s = getaddrinfo(host, SERVER_PORT, &hints, &result)) != 0 ) {
    fprintf(stderr, "%s: getaddrinfo: %s\n", argv[0], gai_strerror(s));
    return -1;
  }

  /* Iterate through the address list and try to connect */
  for (rp = result; rp != NULL; rp = rp->ai_next) {
    if ((s = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol)) == -1 ) {
  	  continue;
    }
    //connect to socket
    if (connect(s, rp->ai_addr, rp->ai_addrlen) != -1) {
      printf("host is: " host"\n");
      printf("port is: " SERVER_PORT"\n");
      printf("connect successful\n");
  	  break;
    }
  }

  if (rp == NULL) {
    perror("stream-talk-client: connect");
    return -1;
  }
  freeaddrinfo(result);

  char *message;
  char server_reply[MAX_LINE];
  message = REQUEST;
  //test for request fail to the server
  if( send(s , message , *len , 0) < 0){
    perror("Send failed");
    return -1;
  }
  //send a request to the server to return the content from the webpage
  send(s , message , *len , 0);
  printf("Data Send\n");

  /* Receive a reply from the server   // want to call readchunk here before recv(),then repeatedly call recv() inside of function  */
  // int retval;
  // retval = readchunk(s, server_reply, len, value);
  // printf("%i\n", retval);

  recv(s , server_reply , sizeof(server_reply) , 0);

  printf("Reply received\n");
  printf("%s\n", server_reply);    // used to print out entire request from server

  /*  this works to search for the value but as is it only finds the first occurrence and then
      prints out everything after it as well   */
  // char *retval;
  // retval = strstr(server_reply, value);
  // printf("results from ret: ");
  // printf("%s\n",retval);

  close(s);
  return 0;
}

/* *********************************************************************************************
// call readchunk
// ssize_t len is the value read in from the command line
// server_reply is the buffer returned from the server
// value is what we are looking for form the command line
// call recv
// find matches to command line arg for value
// increment count if found
// check to see if size length equals the passed in size_t len
// after both lengths match, all bytes have been read in
// return counter
********************************************************************************************  */
int count = 0;  // needs to return a count of how many times the given value occured
int readchunk( int s, void *server_reply, size_t len, char *value )
{
  recv(s , server_reply , len , 0);
  int size;
  size = sizeof("%i", *server_reply);     // use size to save the server_reply length to a variable
  while(size < len){                          //so you can compare it to the length of server reply to
    if(strstr(server_reply, value) != NULL){      //see if the entire reply has been read through
      count++;
    }
    size = len - size;
  }
  return count;
/* Define readchunck to return exactly len bytes unless an error occurs or the socket closes.
*/

}


// these were the provided examples for readchunk *************************************
// ssize_t readchunck( int sockfd, void *buf, size_t len );
// ssize_t readchunck( int sockfd, void *buf, size_t len )
// {
// /* Define readchunck to return exactly len bytes unless an error occurs or the socket closes.
// */
//
// }
