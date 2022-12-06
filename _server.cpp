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

int main()
{
    // Generate a random number to be guessed
    std::srand(std::time(NULL));
    int number = std::rand() % 100 + 1;

    // Create a socket
    int sockfd = std::socket(AF_INET, SOCK_STREAM, 0);

    // Bind the socket to an IP address and port
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(369);
    addr.sin_addr.s_addr = INADDR_ANY;
    std::bind(sockfd, (struct sockaddr*)&addr, sizeof(addr));

    // Listen for incoming connections
    std::listen(sockfd, 5);

    // Accept an incoming connection
    int clientfd = std::accept(sockfd, NULL, NULL);

    // Loop until the client guesses the number
    while (true)
    {
        // Read a guess from the client
        char buffer[1024];
        int n = std::read(clientfd, buffer, sizeof(buffer));
        int guess = std::atoi(buffer);

        // Check if the guess is correct
        if (guess == number)
        {
            // Send a message to the client indicating that they won
            const char* response = "You won!\n";
            std::write(clientfd, response, std::strlen(response));

            // Close the client socket
            std::close(clientfd);

            // Exit the loop
            break;
        }
        else if (guess < number)
        {
            // Send a message to the client indicating that their guess is too low
            const char* response = "Your guess is too low.\n";
            std::write(clientfd, response, std::strlen(response));
        }
        else
        {
            // Send a message to the client indicating that their guess is too high
            const char* response = "Your guess is too high.\n";
            std::write(clientfd, response, std::strlen(response));
        }
    }

    // Close the server socket
    std::close(sockfd);

    return 0;
}
