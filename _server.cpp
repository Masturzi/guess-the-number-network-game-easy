#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

// A very simple network game where the server generates 
// a random number and the client
// tries to guess the number.
//
// Author: Don P
// Date: December 6, 2022

using std::srand;
using std::random_device;
using std::uniform_int_distribution;
using std::ostream;
using std::cout;
using std::endl;

int main()
{
    // Generate a random number to be guessed
    random_device rd;
    srand(rd());
    uniform_int_distribution<int> dist(1, 100);
    int number = dist(rd);

    // Create a socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // Bind the socket to an IP address and port
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(369);
    addr.sin_addr.s_addr = INADDR_ANY;
    bind(sockfd, (struct sockaddr*)&addr, sizeof(addr));

    // Listen for incoming connections
    listen(sockfd, 5);

    // Accept an incoming connection
    int clientfd = accept(sockfd, NULL, NULL);

    // Loop until the client guesses the number
    while (true)
    {
        // Read a guess from the client
        char buffer[1024];
        int n = read(clientfd, buffer, sizeof(buffer));
        int guess = atoi(buffer);

        // Check if the guess is correct
        if (guess == number)
        {
            // Send a message to the client indicating that they won
            cout << "You won!" << endl;

            // Close the client socket
            close(clientfd);

            // Exit the loop
            break;
        }
        else if (guess < number)
        {
            // Send a message to the client indicating that their guess is too low
            cout << "Your guess is too low." << endl;
        }
        else
        {
            // Send a message to the client indicating that their guess is too high
            cout << "Your guess is too high." << endl;
        }
    }

    // Close the server socket
    close(sockfd);

    return 0;
}
