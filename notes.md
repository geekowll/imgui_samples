#Giriş
---

ImGui ile alakalı basit örnekler ve öğrenme sürecinde edindiğim bilgileri burada basit notlar halinde açıklayacağım.

# ImGui

* ImGui::Begin
    * bir window açacaktır. kendine ait bir context bilgisi olduğunu düşünüyorum.
    * bu window üzerinde çizim yapılır.
    * bunu bir qt widget gibi düşünebiliriz. her şey aynı widget üzerinde impl. edilir.
    
* Id
    * Panel, window, child, ... bunların ilk parametrelerinde "name","id" keywordleri geçiyorsa kullanılabilir.
    * ##, ###,####... şeklinde "aynı" begin() içinde componentlar gruplanabilir. Yani aynı id aynı yerde implement edilir.
