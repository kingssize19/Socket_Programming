# C ile Soket Programlama

## 1. Soket Programlama Nedir?

Soket programlama, bir ağ üzerindeki iki farklı düğümü (node) birbirine bağlayarak aralarında iletişim kurulmasını sağlayan bir yöntemdir. Temel olarak, bilgisayarların veya cihazların birbirleriyle konuşmasını sağlayan altyapıdır.

## Nasıl Çalışır? (Sunucu ve İstemci Mimarisi)

Bu iletişim modelinde iki ana taraf bulunur:

**1. Sunucu (Server) :**
  * Bir soket, belirli bir IP adresinde ve belirli bir port üzerinde "dinleme" moduna geçer.
  * Bağlantı taleplerini bekleyen taraftadır.

**2. İstemci (Client) :**
 * Diğer soket ise bağlantı kurmak için sunucuya "uzanır" (reach out).
 * İletişimi başlatan taraftır.

**Özetle :** Sunucu, dinleyici soketi oluşturur; İstemci ise sunucuya ulaşarak bağlantıyı başlatır.

## Kullanım Alanları

Soket programlama, modern ağ iletişiminin temel taşıdır ve şu alanlarda yaygın olarak kullanılır:
* **Anlık Mesajlaşma Uygulamaları :** WhatsApp veya Telegram gibi uygulamalar, mesajları anında iletmek için soketleri kullanır.
* **İkili Veri Akışı (Binary Streaming) :** Dosya transferleri veya veri paketlerinin iletimi.
* **Belge İşbirliği :** Google Docs gibi aynı anda birden fazla kişinin düzenleme yaptığı sistemler.
* **Çevrimiçi Yayın Platformları :** YouTube veya Netflix gibi video akış servisleri veriyi iletmek için bu teknolojiden faydalanır.


## Büyük Resim

Kodlara dalmadan önce, bu sürecin bir "telefon görüşmesi" gibi olduğunu hayal edin.
* **Server :** Telefonu kuran, fişe takan ve çalmasını bekleyen taraftır.
* **Client :** Numarayı çevirip aramayı başlatan taraftır.


## 2. Kritik Veri Yapıları 

C dilinde soket programlamanın kalbi **struct sockaddr_in** yapısıdır. Kodlarda sürekli göreceğiniz bu yapı, "Kimi arıyorum?" veya "Ben kimim?" sorusunun cevabıdır.

```c
struct sockaddr_in {
    short             sin_family;        // Adres ailesi (Genelde AF_INET -> IPv4)
    unsigned short    sin_port;          // Port numarası (Örnek: 8080)
    struct in_addr    sin_addr;          // IP Adresi
    char              sin_zero[8];       // Padding - yapıyı standart boyuta getirmek için
}
```

**Byte Ordering :** Ağ trafiği "Big Endian" formatını kullanır. Bilgisayarınız ise muhtemelen "Little Endian"'dır. Bu yüzden port numaralarını atarken **htons()** (Host to Network Short)fonksiyonunu kullanarak çeviri yapmak **zorunludur**. Yapmazsanız port 8080 (0x1F90) ağda bambaşka bir sayı (0x901F -> 36895) olarak görünür. 

## 3. Sunucu Tarafı (Server)

Sunucu tarafı daha karmaşıktır çünkü bağlantıyı karşılamak zorundadır. 

**ADIM 1: Soket Oluşturma (Socket Creation)**
Telefon cihazını satın almak gibidir. Henüz hat bağlı değildir.

```c
int server_fd;
// socket (DOMAIN, TYPE, PROTOCOL)
if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    perror("Soket olusturulmadi.");
    exit(EXIT_FAILURE);
}
```
* **AF_INET :** IPv4 kullanacağımızı belirtir.
* **SOCK_STREAM :** TCP Protokolü (Güvenilir, sıralı veri). UDP için **SOCK_DGRAM** kullanılır.
* **0 :** Varsayılan protokol (TCP için IP).
* **Dönen Değer (server_fd) :** İşletim sisteminde her şey bir dosyadır. Bu "File Descriptor", bu soketi temsil eden bir tam sayıdır. 


**ADIM 2: Ayarları Yapılandırma (Setsockopt)**
Bu adım opsiyonel görünse de pratikte çok önemlidir. Sunucuyu kapatıp hemen tekrar açtığınızda "Address already in use" hatası almamak için portu serbest bırakır.

```c
int opt = 1
if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
    perror("setsockopt hatasi");
    exit(EXIT_FAILURE);
}
```


**ADIM 3: Adresi Bağlama (Bind)**
Telefon numarasını hatta tanımlamak gibidir. "Ben bu IP ve Port'ta çalışacağım" demektir.

```c
struct sockaddr_in address;
address.sin_family = AF_INET;
adress.sin_addr.s_addr = INADDR_ANY;        // Makinedeki tüm IP'leri kabul et.
address.sin_port = htons(8080);             // Port 8080 (Ağ formatına çevrildi!)

if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
    perror("Bind basarisiz.");
    exit(EXIT_FAILURE);
}
```
* **INADDR_ANY :** Sunucunun birden fazla ağ kartı varsa (Wi-Fi, Ethernet), hangisinden gelirse gelsin kabul et demektir.


**ADIM 4: Dinleme (Listen)**
Telefonun çalmasını beklemeye başlamaktır.

```c
if (listen(server_fd, 3) < 0) {        // 3: Bekleme kuyruğu (Backlog)
    perror("Listen basarisiz.");
    exit(EXIT_FAILURE);
}
```
* **Backlog (3) :** Sunucu o an meşgulken sırada bekleyebilecek maksimum istemci sayısı.


**ADIM 5: Kabul Etme (Accept)**
Telefon çaldığında ahizeyi kaldırmaktır.

```c
int new_socket;
int addrlen = sizeof(address);
if ((new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen)) < 0) {
    perror("Accept basarisiz");
    exit(EXIT_FAILURE);
}
```
* **accept()** fonksiyonu bloklayıcıdır. Yani bir istemci gelene kadar kod burada durur ve ilerlemez. Bağlantı geldiğinde **YENİ** bir soket ID'si (new_socket) döndürür.
* **server_fd :** Kapıdaki güvenlik görevlisidir, sadece geleni karşılar.
* **new_socket :** İçerdeki rehberdir, o müşteriyle özel ilgilenir.


## 4. İstemci Tarafı (Client)















































