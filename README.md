# mini-shell

### 1. Commande pour terminer le shell
```
shell> quit
```

### 2. Interprétation de commande simple
* ls
* cat
* clear
* ...

### 3. Commande avec redirection d’entrée ou de sortie
```
shell> ls > test.txt
```

### 4. Gestion des erreurs


### 5. Séquence de commandes composée de deux commandes reliées par un tube
```
ls | wc -l
```

### 6. Séquence de commandes composée de plusieurs commandes et plusieurs tubes
```
 ls -l | grep 2018 | wc -l
```

### 7. Exécution de commandes en arrière-plan

```
shell> gedit test.txt &
```

### 8. Gestion des zombis
