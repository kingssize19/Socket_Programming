#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>


#define PORT 8080
#define BUFFER_SIZE 1024


int main() {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[BUFFER_SIZE] = {0};
    char *response = "Mesajin alindi, hos geldin!";

    // 1. ADIM : Soket Oluşturma
    // AF_INET : IPv4, SOCK_STREAM : TCP
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Soket olusturulmadi.");
        exit(EXIT_FAILURE);
    }


    // 2. ADIM : Port Ayarları (Zorunlu değil ama önerilir)
    // Sunucuyu kapattıktan hemen sonra tekrar açabilmek için portu serbest bırakır.
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt hatasi");
        exit(EXIT_FAILURE);
    }

    // 3. ADIM : Adres ve Port Bağlama (Bind)
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;		// Herhangi bir arayüzden gelen bağlantıyı kabul et
    address.sin_port = htons(PORT);			// Port numarasını ağ formatına çevir

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind islemi basarisiz");
        exit(EXIT_FAILURE);
    }


    // 4. ADIM : Dinlemeye Başla (listen)
    if (listen(server_fd, 3) < 0) {
        perror("Dİnleme hatasi");
        exit(EXIT_FAILURE);
    }

    printf("Sunucu %d portunda dinliyor... (Baglanti bekleniyor)\n", PORT);


    // 5. ADIM : Bağlantı Kabul Et (Accept) - KOD BURADA BLOKLANIR/BEKLER
    if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0) {
        perror("Kabul etme hatasi");
        exit(EXIT_FAILURE);
    }


    // 6. ADIM : Veri Okuma
    int valread = read(new_socket, buffer, BUFFER_SIZE);
    printf("Istemciden gelen mesaj : %s\n", buffer);

    // 7. ADIM : Cevap Gönderme
    send(new_socket, response, strlen(response), 0);
    printf("Cevap gönderildi.\n");

    // 8. ADIM : Kapatma
    close(new_socket);
    close(server_fd);
    return 0;
}
