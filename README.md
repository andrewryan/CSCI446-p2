# CSCI446-p2
Input and output examples:

$ ./t-counter 100 "socket"

Number of socket instances: 20

$ ./t-counter 10 "class"

Number of class instances: 20

This program establishes a socket connection using C and can be used to count occurences of a specific element in an HTML file. Two command line arguments are given. The first is an integer that represents a byte count for how many bytes of the html file to read. The second is an element that can be used to compare to the html file to determine how many times that element appears. Once the socket is connected a request is sent, and what is returned is how many times that specific command line argument appeared in the HTML file.
