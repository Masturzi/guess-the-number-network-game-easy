#include <iostream>
#include <string>
#include <cstdlib>
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

int main()
{
    // Create a socket
    int sockfd = std::socket(AF_INET, SOCK_STREAM, 0);

    // Connect to the server
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(369);
    addr.sin_addr.s_addr = std::inet_addr("127.0.0.1");
    std::connect(sockfd, (struct sockaddr*)&addr, sizeof(addr));

    // Loop until the user guesses the number
    while (true)
    {
        // Prompt the user to enter a guess
        std::cout << "Enter a guess: ";
        int guess;
        std::cin >> guess;

        // Send the guess to the server
        char buffer[1024];
        std::sprintf(buffer, "%d", guess);
        std::write(sockfd, buffer, std::strlen(buffer));

        // Read the response from the server
        std::memset(buffer, 0, sizeof(buffer));
        int n = std::read(sockfd, buffer, sizeof(buffer));

        // Print the response from the server
        std::cout << buffer;

        // Check if the user won
        if (std::strstr(buffer, "You won") != NULL)
        {
            // Close the socket
            std::close(sockfd);

            // Exit the loop
            break;
        }
    }

    return 0;
}
