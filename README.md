# TD's
Un d�p�t pour nos codes sources li�s aux Projets.  

## Pourquoi ?
- Pouvoir directement partager du code, sans besoin de Mail ou Facebook;
- Ca offre des pistes � ceux qui sont moins � l'aise, et �a reste pratique pour les autres;
- Apprendre (un peu) Git.
- Allez viens, on est bien !

## Au menu
- Comment c'est que �a marche ? (Git d�butant)
- Convention de nommage
- Pr�cautions
- Trucs cool � faire

## Comment c'est que �a marche ? (Git d�butant)
Mets-toi dans le dossier de ton choix: la commande suivante va cloner le d�p�t dans un nouveau sous-dossier `tds`.
- `git clone https://github.com/imac2018/project`
- `cd project`

Et en fait, d�s que tu commences/reprend un projet, tu cr�es le sous-dossier appropri� � ton nom, et tu bosses directement dedans.

Au d�but de chaque projet, tu fais :
- `cd <le_dossier_tds>`
- `git pull`

Avant de partir, tu fais :
- `cd <le_dossier_tds>` (revenir au dossier racine du d�p�t)
- `git add -A` (ajouter aux changement tous les fichiers modifi�s)
- `git commit -m "Ceci est un message qui explique mes changements."` (laisser un caillou derri�re soi)
- `git pull` (Recevoir les derniers changements depuis qu'on a touch� notre code)
- `git push origin master` (Envoyer les changements au serveur)

Bien s�r, il y a plus de d�tails � conna�tre mais globalement �a c'est suffisant pour commencer.

## Convention de nommage
Y'en a pas, on s'en fout.

Mais nommage conseill� pour pas foutre le bordel ^^ : `<nom_cours>/<nom_projet_et_initialnom>/<file>`

## Pr�cautions
La taille max. du d�p�t est 1 Go.
C'est fait pour contenir du code source, **et surtout pas** des images, de l'audio etc.

Si ton fichier est l�ger �a passe. Mais s'il est lourd, faut pas encombrer le d�p�t avec.
Cr�es donc un fichier `.gitignore` dans ton dossier et �cris-y chaque fichier qui doit �tre ignor� par Git, dans ce dossier l�.

Tu peux ins�rer des "wildcards" (jokers), genre :
```.gitignore
cape.png  # Ignorer cape.png (duh!)
*.png     # Ignorer tous les fichiers qui finissent par png juste ici.
images/** # Ignorer tous les fichiers contenus dans Images/.
**/*.png  # Ignorer tous les fichiers .png, ici et dans chaque sous-dossier.
```

## Trucs cools � faire
- Contribuer au wiki quand �a s'y pr�te;
- Ouvrir une Issue quand il y a un probl�me, ou pour des requ�tes;
- Profiter de la vie
