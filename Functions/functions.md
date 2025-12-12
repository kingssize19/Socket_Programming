# Server Side

## 1. socket()

Bu fonksiyon iletişim kanalının **uç noktasını (endpoint)** oluşturur.

```c
server_fd = socket(AF_INET, SOCK_STREAM, 0);
```

**PARAMETRELER :**

1. **domain (AF_INET) :** İletişim ailesini belirler.
* **AF_INET :** IPv4 kullanacağız (Örn: 192.168.10.10)
* **AF_INET6 :** IPv6 kullanılacağı zaman bu seçilir.
* **AF_UNIX :** Sadece aynı bilgisayar içindeki süreçler konuşacaksa bu seçilir.

2. **type (SOCK_STREAM) :** İletişim tipini belirler.
* **SOCK_STREAM :** **TCP** protokolü. Güvenilir, sıralı, çift yönlü bağlantı.
* **SOCK_DGRAM :** **UDP** protokolü. Hızlı ama güvensiz, bağlantısız.

3. **protocol (0) :** Genelde **0** verilir. İşletim sistemi, **SOCK_STREAM** için otomatik olarak TCP'yi, **SOCK_DGRAM** için UDP'yi seçer.

**Dönen Değer :**
* **Başarılıysa :** **File Descriptor** döner (Pozitif bir tam sayı.)
* **Başarısızsa :** -1 döner.


## 2. setsockopt()

Soket seviyesindeki ayarları değiştirmek için kullanılır. Portu hemen tekrar kullanabilmek içinde kullanılır.

```c
setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
```

**PARAMETRELER :**

1. **sockfd (server_fd) :** Ayar yapılacak soketin numarası.
2. **level (SOL_SOCKET) :** Ayarın hangi katmanda yapılacağı. **SOL_SOCKET**, "Soket API seviyesi" demektir (Protokolden bağımsız genel ayarlar).
3. **optname (SO_REUSEADDR) :** Hangi ayarı değiştiriyoruz? "Adresi Yeniden Kullanma" izni.
4. **optval (&opt) :** Ayarın yeni değeri, **opt = 1** olduğu için bu özelliği açıyoruz.
5. **optlen (sizeof(opt)) :** **optval** verisinin boyutu.


## 3. htons()

Bu bir sistem çağrısı değil, bir yardımcı fonksiyonudur ama ağ programlamada zorunludur.

```c
address.sin_port = htons(PORT);
```
* **Sorun :** Bilgisayarlar (Intel/AMD işlemciler) sayıları bellekte **Little Endian** (Ters sırada) tutar. Ağ protokolleri ise **Big Endian** (Düz sırada) bekler.
* **İşlevi :** 8080 sayısını alır, ağın anlayacağı bayt dizilimine çevirir. Eğer bunu yapmazsanız port numarası karşı tarafa bo 


## 4. bind()

Oluşturulan soketi (server_fd), fiziksel bir adres (IP) ve port ile eşleştirir.

```c
bind(server_fd, (struct sockaddr *)&address, sizeof(address));
```

**PARAMETRELER :**

1. **sockfd (server_fd) :** Hangi soket bağlanacak?
2. **addr (&address) :** struct sockaddr_in tipindeki adres yapısı. (Fonksiyon **struct sockaddr\***)














