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
2. **addr (&address) :** struct sockaddr_in tipindeki adres yapısı. (Fonksiyon **struct sockaddr\*** beklediği için **(struct sockaddr *)** ile tip dönüşümü-cast-yapılır).
   * İçindeki **INADDR_ANY :** Sunucunun tüm ağ kartlarından (Wi-Fi, Ethernet vb.) gelen istekleri kabul etmesini sağlar.
3. **addrlen (sizeof(address)) :** Adres yapısının boyutu.

**Dönen Değer :**
* **0 :** Başarılı
* **-1 :** Hata (Genelde "Address already in use" hatası burada alınır).

## 5. listen()

Soketi "pasif" moda alır ve gelen bağlantıları kuyruğa atmaya başlar.

```c
listen(server_fd, 3);
```

**PARAMETRELER :**

1. **sockfd (server_fd) :** Dinleyecek soket.
2. **backlog (3) :** Bekleme Kuyruğu Uzunluğu
   * Eğer sunucu şu an bir istemciyle meşgulse ve o an 2. bir istemci gelirse kuyruğa alınır.
   * Bu sayı (3), kuyrukta en fazla kaç kişinin bekleyeceğini belirtir. 4. kişi gelirse "Connection Refused" hatası alır. Modern sistemlerde bu sayı genelde daha yüksek tutulur.



## 6. accept()

Gelen bağlantı isteğini kabul eder. Bu fonksiyon **BLOKLAYICIDIR**. Yani birisi bağlanana kadar kod bu satırda durur.

```c
new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
```

**PARAMETRELER :**

1. **sockfd (server_fd) :** "Dinleyen" ana soket.
2. **addr (&address) :** Burası boş gönderilir, fonksiyon döndüğünde içi bağlanan istemcinin IP ve Port bilgileriyle doldurulur. "Kim arıyor?" sorusunun cevabı buraya yazılır.
3. **addrlen :** Adres yapısının boyutu (Pointer olarak verilir çünkü fonksiyon boyutu güncelleyebilir).

**Dönen Değer (new_socket) :**

* **İşte püf nokta burası!** **accept**, yepyeni bir dosya tanımlayıcısı (socket ID) döndürür.
* **server_fd :** Hala kapıda bekleyip yeni müşteri aramaya devam eder.
* **new_socket :** Sadece şu anki müşteriyle konuşmak için kullanılan özel hattır. read/send işlemlerinde bu kullanılır.



## 7. read()

Soketten veri okur. Linux'ta soketler dosya gibi olduğu için standart dosya okuma fonksiyonu kullanır.  (Alternatifi : recv())

```c
int valread = read(new_socket, buffer, BUFFER_SIZE);
```

**PARAMETRELER :** 

1. **fd (new_socket) :** Hangi bağlantıdan okuyacağız? (Dikkat: server_fd değil.)
2. **buf (buffer) :** Okunan verinin yazılacağı bellek alanı (char dizisi).
3. **count (BUFFER_SIZE) :** Maksimum kaç byte okunacak? (Buffer taşmasın diye sınır koyarız) 

**Dönen Değer (valread) :**

* **> 0 :** Okunan byte sayısı (Örn: "Selam" giderse 5 döner).
* **0 :** Bağlantı koptu. (İstemci close() yaptıysa 0 döner). DÖngüyü kırmak için bu kontrol şarttır.
* **-1 :** Hata



## 8. send()

Sokete veri yazar. (ALternatifi: write()).

```c
send(new_socket, msg, strlen(msg), 0);
```

**PARAMETRELER :** 

1. **sockfd (new_socket) :** Kime göndereceğiz?
2. **buf (msg) :** Gönderilecek veri.
3. **len (strlen(msg)) :** Kaç bayt gönderilecek?
4. **flags (0) :** Gönderim seçenekleri. Genelde 0 kullanılır.


## 9. memset() ve strcspn() 

Ağ programlamada veri temizliği hayati önem taşır.

* **memset(buffer, 0, BUFFER_SIZE) :**
  * Buffer'ı her turda temizleriz. Temizlemezsek, önceki turdan kalan veriler ("Merhaba" -> "Naber") birleşip "Naberba" gibi anlamsız yazılar oluşturabilir (Ghosting).
* **buffer\[strcspn(buffer, "\n")\] = 0; :**
  * Terminalden veri girerken Enter tuşuna bastığınızda sona görünmez bir \n (yeni satır) karakteri eklenir. strcmp yaparken "cikis" ile "cikis\n" birbirine eşit değildir. Bu fonksiyon sondaki o \n karakterini bulur ve siler (null terminator \0 yapar).












































