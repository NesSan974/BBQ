# 🥓 BBQ (Batch Buffer Queue)
```text
__/\\\\\\\\\\\\\____/\\\\\\\\\\\\\__________/\\\_______        
 _\/\\\/////////\\\_\/\\\/////////\\\_____/\\\\/\\\\____       
  _\/\\\_______\/\\\_\/\\\_______\/\\\___/\\\//\////\\\__      
   _\/\\\\\\\\\\\\\\__\/\\\\\\\\\\\\\\___/\\\______\//\\\_     
    _\/\\\/////////\\\_\/\\\/////////\\\_\//\\\______/\\\__    
     _\/\\\_______\/\\\_\/\\\_______\/\\\__\///\\\\/\\\\/___   
      _\/\\\_______\/\\\_\/\\\_______\/\\\____\////\\\//_____   
       _\/\\\\\\\\\\\\\/__\/\\\\\\\\\\\\\/________\///\\\\\\__   
        _\/////////////____\/////////////____________\//////___  
```

`Slant Relief - https://patorjk.com/software/taag/#p=testall&f=Graffiti&t=BBQ&x=none&v=4&h=4&w=80&we=false`

## Objectif

Faire un tableau / Queue circulaire de taille dynamique, contenant des items de taille dynamique.  
une sorte de buffer circulaire extensible.
Ex :
```
[ [Item1 : 1 octet] [Item2 : 12 octet] [Item3 : 55 octet] ]
```

- Ajout d'un item de 5 octet

```
[ [Item1 : 1 octet] [Item2 : 12 octet] [Item3 : 55 octet] [Item4 : 5octet] ]
```


https://www.geeksforgeeks.org/c/queue-in-c/
https://www.geeksforgeeks.org/dsa/introduction-to-circular-queue/
