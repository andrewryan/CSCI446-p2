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
#define host "www.ecst.csuchico.edu"
#define SERVER_PORT "80"
#define REQUEST "GET /~ehamouda/file.html HTTP/1.0\n\n"
#define MAX_LINE 1001  /* Determines buffer size for recv(), 1001 to account for NULL pointer */

int readchunk( int s, char *server_reply, ssize_t len, char *value );

int main(int argc, char *argv[])
{
  struct addrinfo hints;
  struct addrinfo *rp, *result;
  int s;
  int len;
  char *value;

  if (argc > 3){
	  perror("Too many command line arguments given.");
    return -1;
  }
  if (argc < 3){
	  perror("Not enough command line arguments were given.");
    return -1;
  }
  /* Storing command line arguments to be used in readchunk */
  len = atoi(argv[1]);
  value = argv[2];
  if (len > 1000){
    perror("Chunk size is too large, max is 1000.");
    return -1;
  }

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
    /* Connect to socket */
    if (connect(s, rp->ai_addr, rp->ai_addrlen) != -1) {
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
  /* Tests for request fail to the server */
  if( send(s , message , len , 0) < 0){
    perror("Send failed");
    return -1;
  }
  /* Sends a request to the server to return the content from the webpage */
  send(s , message , strlen(message) , 0);

  /* Retval contains the number of matches returned from readchunk */
  int retval;
  retval = readchunk(s, server_reply, len, value);
  printf("Number of ");
  printf("%s",value);
  printf(" instances: ");
  printf("%i\n", retval);

  /* Closes the socket */
  close(s);
  return 0;
}

/* Readchunk uses a for loop to iterate through the server_reply, as long as the the server_reply does not equal
    the NULL pointer, if the value of the pointer to server_reply equals the pointer to the first value
    in the match, a while loop is used to continue to compare the buffer for a possible match, the variable
    j is used to iterate through the command line argument value to compare it to the buffer */

int readchunk( int s, char *server_reply, ssize_t len, char *value )
{
  /* Receives the buffer to be searched for a match */
  recv(s , server_reply , len , 0);
  if(strlen(server_reply) != len){
    perror("The server did not send the correct amount of bytes.");
    return -1;
  }
  //printf("%s\n",server_reply); // use this to test if the correct number of matches are found
  int i, j;
  int count = 0;
  char *match = value;
  int value_length = strlen(value);
  /* still need a loop to check if entire buffer has been read through,if
      it hasn't then return the number of bytes that was read through */
  for (i = 0, j = 0; server_reply[i] != '\0'; i++) {
    if (server_reply[i] == match[j]) {
      j++;
      while(server_reply[i] == match[j]){
        j++;
      }
      if(value_length == j){
        count++;
      }
    }
    else{
      j = 0;
    }
  }
  /* Returns the number of matches found to main */
  return count;
}
