#include <iostream>
#include <string>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sstream>
#include <algorithm>

// A very simple network game where the server generates 
// a random number and the client
// tries to guess the number.
//
// Author: Don P
// Date: December 6, 2022

using std::socket;
using std::cout;
using std::cin;
using std::endl;
using std::stringstream;
using std::fill;

int main()
{
    // Create a socket
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    // Connect to the server
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(369);
    addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    connect(sockfd, (struct sockaddr*)&addr, sizeof(addr));

    // Loop until the user guesses the number
    while (true)
    {
        // Prompt the user to enter a guess
        cout << "Enter a guess: ";
        int guess;
        cin >> guess;

        // Send the guess to the server
        char buffer[1024];
        stringstream ss;
        ss << guess;
        write(sockfd, ss.str().c_str(), ss.str().length());

        // Read the response from the server
        fill(buffer, buffer + sizeof(buffer), 0);
        int n = read(sockfd, buffer, sizeof(buffer));

        // Print the response from the server
        cout << buffer;

        // Check if the user won
        if (strstr(buffer, "You won") != NULL)
        {
            // Close the socket
            close(sockfd);

            // Exit the loop
            break;
        }
    }

    return 0;
}
