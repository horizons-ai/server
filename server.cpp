#include <iostream>
#include <fstream>
#include <vector>
#include <iomanip>  // Para std::hex y std::setfill
#include <cstdint> // Para uint8_t, uint32_t
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h>

#define PORT 20777
#define BUFFER_SIZE 1500

using namespace std;

int main() {
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    uint8_t buffer[BUFFER_SIZE];
    socklen_t addr_len = sizeof(client_addr);

    ofstream file("output.txt", ios::app);
    if (!file.is_open()) {
        cerr << "Error al abrir el archivo" << endl;
        return EXIT_FAILURE;
    }

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Error al crear el socket");
        return EXIT_FAILURE;
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Error al enlazar");
        close(sockfd);
        return EXIT_FAILURE;
    }

    cout << "Servidor UDP en el puerto " << PORT << endl;

    while (true) {
        int n = recvfrom(sockfd, buffer, BUFFER_SIZE, 0, 
                         (struct sockaddr *)&client_addr, &addr_len);
        if (n < 0) {
            perror("Error al recibir datos");
            continue;
        }

        // Escribir la trama leída en el archivo
        for (int i = 0; i < n; i++) {
            file << hex << setw(2) << setfill('0') 
                 << static_cast<int>(buffer[i]);
        }
        file << '\n'; // Salto de línea después de cada trama
        file.flush();
    }

    file.close();
    close(sockfd);
    return 0;
}
