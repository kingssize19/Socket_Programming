#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 8080
#define BUFFER_SIZE 1024

int main() {
    int sock = 0, valread;
    struct sockaddr_in serv_addr;
    char *hello = "Merhaba Sunucu! Ben Client.";
    char buffer[BUFFER_SIZE] = {0};

    // 1. ADIM : Soket Oluşturma
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Soket oluşturma hatası \n");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);


    // 2. ADIM : IP Adresini Çevirme
    // 127.0.0.1 (Localhost) adresini binary formata çeviriyoruz.
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        printf("\nGeçersiz Adres / Adres Desteklenmiyor \n");
        return -1;
    }

    // 3. ADIM: Bağlanma (Connect)
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nBağlantı Başarısız \n");
        return -1;
    }

    // 4. ADIM: Mesaj Gönderme
    send(sock, hello, strlen(hello), 0);
    printf("Mesaj sunucuya gonderildi.\n");

    // 5. ADIM: Cevap Okuma
    valread = read(sock, buffer, BUFFER_SIZE);
    printf("Sunucudan gelen cevap: %s\n", buffer);

    // 6. ADIM: Kapatma
    close(sock);
    return 0;
}
