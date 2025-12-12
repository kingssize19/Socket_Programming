## Server Side

### socket()

Bu fonksiyon iletişim kanalının **uç noktasını (endpoint)** oluşturur.

```c
server_fd = socket(AF_INET, SOCK_STREAM, 0);
```
**1. domain (AF_INET) :** İletişim ailesini belirler.
* **AF_INET :** IPv4 kullanacağız (Örn: 192.168.10.10)
* **AF_INET6 :** IPv6 kullanılacağı zaman bu seçilir.
* **AF_UNIX :** Sadece aynı bilgisayar içindeki süreçler konuşacaksa bu seçilir.

**2. type (SOCK_STREAM) :** İletişim tipini belirler.
* **SOCK_STREAM :** **TCP** protokolü. Güvenilir, sıralı, çift yönlü bağlantı.
* **SOCK_DGRAM :** **UDP** protokolü. Hızlı ama güvensiz, bağlantısız.

**3. protocol (0) :** Genelde **0** verilir. İşletim sistemi, **SOCK_STREAM** için otomatik olarak TCP'yi, **SOCK_DGRAM** için UDP'yi seçer.

**Dönen Değer :**
* **Başarılıysa :** **File Descriptor** döner (Pozitif bir tam sayı.)
* **Başarısızsa :** -1 döner.




