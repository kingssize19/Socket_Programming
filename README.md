# C ile Soket Programlama

## Soket Programlama Nedir?

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



