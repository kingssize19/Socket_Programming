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
