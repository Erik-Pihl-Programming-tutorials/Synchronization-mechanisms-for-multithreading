# Semaforer i C och C++
Användning av binära och räknande semaforer för att skydda delade resurser i flertrådade program.
Via semaforer låses resurserna så att endast en tråd har åtkomst till en given resurs åt gången, 
exempelvis terminalen eller globala variabler.

Semaforer sägs vara reserverade (motsvarar låst mutex) eller tillgängliga (motsvarar olåst). 
Semaforer är oftast binära, där 0 = tillgänglig och 1 = reserverad. 
Det finns också uppräknande semaforer, som kan användas om exempelvis fem resurser finns tillgängliga. 
Först när alla fem semaforer är reserverade och en tråd försöker reservera en sjätte semaforer sker väntan 
på att en av de fem reserverade semaforerna blir tillgänglig.

En nackdel med semaforer är att vilken tråd som helst kan ta bort en potentiell reservation, till skillnad mot mutex,
där bara den tråden som låste kan låsa upp. En fördel är dock att man vid användning av multipla binära semaforer kan 
reservera enkelt via separata ID:s i stället för att implementera multipla objekt. Ytterligare en fördel är att man
kan använda räknande semaforer om man har multipla likartade resurs som kan delas mellan trådarna, vilket inte är
möjligt med en mutex, som enbart kan fungera likt en binär semafor (fast något säkrare).

I denna katalog har kod implementerats både för C och C++ via inkluderingsfilen semaphore.h:
    - Binära semaforer är implementerade via funktioner binary_semaphore_take samt binary_semaphore_release,
      som fungerar både i C och C++. 
    - För uppräknande semaforer har två olika interface implementerats. I C används strukten counting_semaphore,
      som deklareras internt i motsvarande källkodsfil semaphore.c I C++ används i stället ett klasstemplate med samma namn, 
      där det totala antalet resurser anges när semaforen skapas och antalet reserverade resurser hålls privat i klassen.

Tre körbara filer skapas vid kompilering:
    - run_binary_semaphore_example_c    : Kör C-program innehållande binära semaforer.
    - run_counting_semaphore_example_c  : Kör C-program innehållande räknande semaforer.
    - run_counting_semaphore_example_cpp: Kör C++-program innehållande räknande semaforer.

Information om mutex samt startkod för main-filerna kan laddas ned här:
https://github.com/Erik-Pihl-Programming-tutorials/Synchronization-mechanisms-for-multithreading/tree/main/mutex

Se video tutorial här:
https://youtu.be/4MA4SBYyaIA
