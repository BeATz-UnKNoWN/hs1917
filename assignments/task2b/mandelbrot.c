/*
 * Andrew Timkov
 * Roman Ozerski
 * UNSW Tutorials
 * Date: 18/4/17
 * mandelbrot
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>

#include "mandelbrot.h"

#define MAX_STEPS 256
#define MAX_DISTANCE 2
#define SIZE 512
#define HALF_SIZE 256
#define BYTES_PER_PIXEL 3
#define HEADER_SIZE 54
#define PIXEL_START 54

#define SIMPLE_SERVER_VERSION 1.0
#define REQUEST_BUFFER_SIZE 1000
#define DEFAULT_PORT 1917
#define NUMBER_OF_PAGES_TO_SERVE 1000
#define FILE_NAME_START 5

#define NEGATIVE -1
#define POSITIVE 1
#define STOP -1

#define FALSE 0
#define TRUE 1

typedef struct _complexNum {
    double real;
    double imaginary;
} complexNum;

typedef struct _triordinate {
   double x;
   double y;
   int z;
} triordinate;

typedef unsigned char byte;

//Mandelbrot Prototypes
complexNum squareComplex (complexNum z);
complexNum addComplex (complexNum z1, complexNum z2);
void drawBrot (double real, double imaginary, int zoom, byte *bmp);
unsigned char stepsToBlue (int steps);
unsigned char stepsToGreen (int steps);
unsigned char stepsToRed (int steps);

//BMP Server Prototypes
void startServer (void);
int waitForConnection (int serverSocket);
int makeServerSocket (int portno);
void serveBMP (int socket, char *request);
void serveHTML (int socket);

//Extract Prototypes
triordinate extract (char *message);
double myAtoD (char *message);
long myAtoL (char *message);
int asciiToDigit (char asciiValue);

int main (int argc, char *argv[]) {
    startServer();

    return EXIT_SUCCESS;
}

/*************************** MANDELBROT *******************************/
int escapeSteps (double x, double y) {
    complexNum c = {x, y};
    complexNum z = {0, 0};
    complexNum zSquared;

    int step = 0;
    int counter = 0;
    double distance = 0;

    while (step < MAX_STEPS) {
        if (distance < MAX_DISTANCE) {
            zSquared = squareComplex(z);
            z = addComplex(zSquared, c); //z = z^2 + c
            distance = sqrt(pow(z.real, 2) + pow(z.imaginary, 2));
            counter++;
        }
        step++;
    }

    return counter;
}

//(a+b)^2 = a^2 + 2ab + b^2
//i^2 is negative
complexNum squareComplex (complexNum z) {
    complexNum newZ;

    newZ.real = pow(z.real, 2) - pow(z.imaginary, 2);
    newZ.imaginary = 2 * z.real * z.imaginary; //2ab

    return newZ;
}

//add real numbers and add imaginary numbers
complexNum addComplex (complexNum z1, complexNum z2) {
    complexNum newZ;
    newZ.real = z1.real + z2.real; //add real components
    newZ.imaginary = z1.imaginary + z2.imaginary; //add imag components

    return newZ;
}

void drawBrot (double real, double imaginary, int zoom, byte *bmp) {
    //distance between pixels is 2^-zoom
    double scale = pow(2, (zoom * -1));
    //create possible range of values
    double startReal = real - (256 * scale);
    double startImaginary = imaginary - (256 * scale);
    double endReal = real + (256 * scale);
    double endImaginary = imaginary + (256 * scale);
    complexNum current = {startReal, startImaginary};
    complexNum end = {endReal, endImaginary};

    //set counter to just after the header
    int i = PIXEL_START;

    //use '<' to go until "end" (not including), else there is segfault
    while (current.imaginary < end.imaginary) {
        while (current.real < end.real) {
            int steps = escapeSteps(current.real, current.imaginary);
            bmp[i] = stepsToBlue(steps);
            i++;
            bmp[i] = stepsToGreen(steps);
            i++;
            bmp[i] = stepsToRed(steps);
            i++;
            //add scaled pixel distance
            current.real += scale;
        }
        current.imaginary += scale;
        //reset current.real
        current.real = startReal;
    }

    return;
}

/**************************** BMP SERVER ******************************/
void startServer (void) {
      
    printf ("************************************\n");
    printf ("Starting simple server %f\n", SIMPLE_SERVER_VERSION);
    printf ("Serving bmps since 2012\n");   
   
    int serverSocket = makeServerSocket (DEFAULT_PORT);   
    printf ("Access this server @ http://localhost:%d/\n", DEFAULT_PORT);
    printf ("************************************\n");
   
    char request[REQUEST_BUFFER_SIZE];
   
    int numberServed = 0;
    while (numberServed < NUMBER_OF_PAGES_TO_SERVE) {
      
        printf ("*** So far served %d pages ***\n", numberServed);
          
        int connectionSocket = waitForConnection (serverSocket);
        // wait for a request to be sent from a web browser, open a new
        // connection for this conversation
          
        // read the first line of the request sent by the browser  
        int bytesRead;
        bytesRead = read (connectionSocket, request, (sizeof request)-1);
        assert (bytesRead >= 0); 
        //were we able to read any data from the connection?
                
        //print entire request to the console 
        printf (" *** Received http request ***\n %s\n", request);
          
        //send the browser a simple html page using http
        printf (" *** Sending http response ***\n");
        //if filename is blank (not specified), load almondbread
        //else generate mandelbrot image
        if (request[FILE_NAME_START] == ' ') {
            serveHTML(connectionSocket); 
        } else {
            serveBMP(connectionSocket, request);
        }    
          
        //close the connection after sending the page- keep aust beautiful
        close(connectionSocket);
          
        numberServed++;
    } 
   
    //close the server connection after we are done- keep aust beautiful
    printf ("** shutting down the server **\n");
    close (serverSocket);

    return;
}

void serveBMP (int socket, char *request) {
    char *message;
   
    //first send the http response header
   
    //(if you write stings one after another like this on separate
    //lines the c compiler kindly joins them togther for you into
    //one long string)
    message = "HTTP/1.0 200 OK\r\n"
              "Content-Type: image/bmp\r\n"
              "\r\n";
    printf ("about to send=> %s\n", message);
    write (socket, message, strlen (message));
   
    triordinate input = extract(request);
   
    //bmp array with hardcoded header
    byte bmp[HEADER_SIZE + (SIZE * SIZE * BYTES_PER_PIXEL)] = {
        0x42,0x4d,0x36,0x00,0x0c,0x00,0x00,0x00,
        0x00,0x00,0x36,0x00,0x00,0x00,0x28,0x00,
        0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x02,
        0x00,0x00,0x01,0x00,0x18,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x0c,0x00,0x13,0x0b,
        0x00,0x00,0x13,0x0b,0x00,0x00,0x00,0x00,
        0x00,0x00,0x00,0x00,0x00,0x00,
    };
    
    drawBrot(input.x, input.y, input.z, bmp);
    
    write(socket, bmp, sizeof(bmp));
    return;
}

void serveHTML (int socket) {
    char *message;
    
    //Send the http response header
    message = "HTTP/1.0 200 OK\r\n"
              "Content-Type: text/html\r\n"
              "\r\n";
    write (socket, message, strlen (message));
    
    //display almondbread viewer by linking JS file
    char *content = "<!DOCTYPE html>\n"
                    "<script src=\"http://almondbread.cse.unsw.edu.au/tiles.js\"></script>"
                    "\n";
    write (socket, content, strlen (content));
}
    
    


//start the server listening on the specified port number
int makeServerSocket (int portNumber) { 
   
    //create socket
    int serverSocket = socket (AF_INET, SOCK_STREAM, 0);
    assert (serverSocket >= 0);   
    //error opening socket
   
    //bind socket to listening port
    struct sockaddr_in serverAddress;
    memset ((char *) &serverAddress, 0,sizeof (serverAddress));
   
    serverAddress.sin_family      = AF_INET;
    serverAddress.sin_addr.s_addr = INADDR_ANY;
    serverAddress.sin_port        = htons (portNumber);
   
    //let the server start immediately after a previous shutdown
    int optionValue = 1;
    setsockopt (
        serverSocket,
        SOL_SOCKET,
        SO_REUSEADDR,
        &optionValue, 
        sizeof(int)
    );

    int bindSuccess = 
        bind (
            serverSocket, 
            (struct sockaddr *) &serverAddress,
            sizeof (serverAddress)
        );
   
    assert (bindSuccess >= 0);
    //if this assert fails wait a short while to let the operating 
    //system clear the port before trying again
   
    return serverSocket;
}

//wait for a browser to request a connection,
//returns the socket on which the conversation will take place
int waitForConnection (int serverSocket) {
    //listen for a connection
    const int serverMaxBacklog = 10;
    listen (serverSocket, serverMaxBacklog);
   
    //accept the connection
    struct sockaddr_in clientAddress;
    socklen_t clientLen = sizeof (clientAddress);
    int connectionSocket = 
        accept (
            serverSocket, 
            (struct sockaddr *) &clientAddress, 
            &clientLen
        );
   
    assert (connectionSocket >= 0);
    //error on accept
   
    return (connectionSocket);
}

/****************************** EXTRACT *******************************/
triordinate extract(char *message) {
    triordinate output = {0,0,0};

    //variables to determine if x, y, or z is being processed
    int landX = FALSE;
    int landY = FALSE;
    int landZ = FALSE;
  
    //indexes for x, y and z strings
    int xIndex = 0;
    int yIndex = 0;
    int zIndex = 0;
  
    char xString[sizeof(double)] = {0};
    char yString[sizeof(double)] = {0};
    char zString[sizeof(long)] = {0};
  
    int messageIndex = 0;
    int spaceCount = 0;
    
    while (messageIndex < strlen(message) && spaceCount < 3) {
        //add to count tally, to stop processing string
        if (message[messageIndex] == ' ') {
            spaceCount++;
            messageIndex++;
        //check for underscores
        //increment messageIndex by 2 to skip checking of x/y/z
        } else if (message[messageIndex] == '_') {
            //control whether x, y, or z is to be processed
            if (landX == FALSE) {
                landX = TRUE;
                messageIndex++;
            } else if (landY == FALSE) {
                landY = TRUE;
                messageIndex++;
            } else {
                landZ = TRUE;
                messageIndex++;
            }
        } else { //check for digits
            if (landZ == TRUE) {
                //check for the . in .bmp
                if (message[messageIndex] == '.') {
                    //will end while loop during next condition test
                    messageIndex = strlen(message);
                } else {
                    zString[zIndex] = message[messageIndex];
                    zIndex++;
                }
            } else if (landY == TRUE) {
                yString[yIndex] = message[messageIndex];
                yIndex++;
            } else if (landX == TRUE) {
                xString[xIndex] = message[messageIndex];
                xIndex++;
            }
        }
        messageIndex++;
    }

    output.x = myAtoD(xString);
    output.y = myAtoD(yString);
    output.z = myAtoL(zString);

    return output;
  }
 
double myAtoD (char *message) {
    double value = 0;
    int sign = POSITIVE;

    int index = 0;

    //checks if value in string is negative
    if (message[index] == '-') {
        sign = NEGATIVE;
        index++;
    }

    //loops through all numbers before dec. point
    while (message[index] >= '0' && message[index] <= '9') {
        int digit = asciiToDigit(message[index]);
        //multiply prev value by 10 and add new digit
        value = (value * 10) + digit;
        index++;
    }

    //loops through all numbers after dec. point
    if (message[index] == '.') {
        int power = -1;
        index++;
        while (message[index] >= '0' && message[index] <= '9') {
            int digit = asciiToDigit(message[index]);
            //multiply digit by the negative power, to get dec. pl.
            //add to value
            value += (digit * pow(10, power));
            //decrease the power
            power--;
            index++;
        }
    }

    value *= sign;

    return value;
}
 
long myAtoL (char *message) {
    long value = 0;
    int sign = POSITIVE;

    int index = 0;

    //checks if value in string is negative
    if (message[index] == '-') {
        sign = NEGATIVE;
        index++;
    }

    //loops through all numerical values in the string
    while (message[index] >= '0' && message[index] <= '9') {
        int digit = asciiToDigit(message[index]);
        value = (value * 10) + digit;
        index++;
    }

    value *= sign;

    return value;
}

//converts ascii value in string to number 0-9
int asciiToDigit (char asciiValue) {
    int digit = 0;

    digit = asciiValue - '0';

    return digit;
}