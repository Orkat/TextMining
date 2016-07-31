Présentation du projet
======================



Questions:
==========

1. Décrivez les choix de design de votre programme

   Nous avions au départ voulu faire un Patricia Trie pour ce projet. Or, nous nous sommes heurtés à des difficultés de structure, notamment en ce qui concerne la longueur des strings. Ainsi, nous sommes revenus sur une implémentation de Trie plus classique.

2. Listez l’ensemble des tests effectués sur votre programme (en plus des units tests)


3. Avez-vous détecté des cas où la correction par distance ne fonctionnait pas (même avec une distance élevée) ?

   Notre implementation nous donne des segfault pour des distances importantes.

4. Quelle est la structure de données que vous avez implémentée dans votre projet, pourquoi ?

   La structure de données utilisée est un compact trie. Les enfants d'un noeud
   sont représentés sous la forme d'une liste chaînée. On utilise une liste
   chaînée pour réduire l'espace utilisé en mémoire, qui serait plus élevé si on
   utilisait un tableau contigu en mémoire de la taille de l'alphabet.

5. Proposez un réglage automatique de la distance pour un programme qui prend juste une chaîne de caractères en entrée, donner le processus d’évaluation ainsi que les résultats

   Un problème survient pour les mots courts. Par exemple, si le mot à corriger
   est "mot", un mot à une distance de 3 pourrait être "lac", qui n'a absolument
   rien à voir et ne devrait pas figurer parmi les propositions.

   Le paramètre de distance maximale doit donc être fonction de la longueur du mot
   à corriger.

   On pourrait choisir `min(2, len(s / 4))`

6. Comment comptez vous améliorer les performances de votre programme

   La parallélisation est tout à fait envisageable, particulièrement dans une
   structure d'arbre où il est possible d'explorer plusieurs branches en
   parallèle.

7. Que manque-t-il à votre correcteur orthographique pour qu’il soit à l’état de l’art ?

   Le corpus utilisé ne contenant que 47 caractères différents, la tâche est
   simplifiée. Le modèle ne gère pas les caractères spécifiques à d'autres langues
   que l'anglais. On pourrait construire un trie avec un set de caractères
   différents pour chaque langue. C'est certainement ce que fait le spell checker
   des navigateurs connus en chargeant chaque dictionnaire de chaque langue dans
   une structure de données séparée.

   Les algorithmes à l'état de l'art sont certainement parallélisables :
   http://arxiv.org/pdf/1204.0184.pdf (Parallel Spell-Checking Algorithm Based on Yahoo! N-Grams Dataset)

   Ces algorithmes sont utilisés pour appliquer une correction à l'output d'un
   système de reconnaissance de la parole. Par exemple, sur les vidéos YouTube,
   des sous-titres sont générés à partir des vidéos. Cette génération des
   sous-titres est couteuse et exécutée en parallèle sur plusieurs serveurs.
