# Mutex i C och C++
Användning av mutex för att skydda delade resurser i flertrådade program.
Via en mutex låses resurserna så att endast en tråd har åtkomst till en given
resurs åt gången, exempelvis terminalen eller globala variabler.

Mutex utgörs av ett objekt, som antingen är låst eller olåst. Endast den tråd som låste mutexen 
(för att reservera den delade resursen) har möjlighet att låsa upp den. Om en given tråd 
försöker reservera en resurs som redan är reserverad (mutex låst) får denna tråd vänta på att 
den blir ledig (mutex olåst) innan den får åtkomst till resursen. 
Därmed bromsas tråden upp tills resursen blir ledig.

Se video tutorial här:
https://youtu.be/jaI_mufDQ2I
