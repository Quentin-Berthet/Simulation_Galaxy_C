---
# author:
# - Orestis Malaspinas
title: Travail pratique de programmation séquentielle
autoSectionLabels: false
autoEqnLabels: true
eqnPrefix:
  - "l'éq."
  - "les éqs."
chapters: true
numberSections: false
chaptersDepth: 1
sectionsDepth: 3
lang: fr
documentclass: article
papersize: A4
cref: false
urlcolor: blue
---
\newcommand{\dd}{\mathrm{d}}
\newcommand{\real}{\mathbb{R}}
\newcommand{\integer}{\mathbb{Z}}
\renewcommand{\natural}{\mathbb{N}}
\newcommand{\ua}{\boldsymbol{a}}
\newcommand{\randone}{\mathcal{R}}
\newcommand{\ub}{\boldsymbol{b}}
\newcommand{\uu}{\boldsymbol{u}}
\newcommand{\uc}{\boldsymbol{c}}
\newcommand{\ux}{\boldsymbol{x}}
\newcommand{\ug}{\boldsymbol{g}}
\newcommand{\uj}{\boldsymbol{j}}
\newcommand{\vectwo}[2]{\begin{pmatrix}#1 \\ #2 \end{pmatrix}}

# Simulation de galaxie

## Le problème à $n$-corps

En physique le problème à $n$-corps consiste à prédire le mouvement d'un groupe de $n$ objets
célestes interagissant les uns avec les autres uniquement via l'attraction gravitationnelle.
Lorsque $n>2$, ce problème est trop complexe pour être résolu analytiquement et nécessite
d'être simulé par ordinateur. Dans ce travail pratique, nous allons
simuler un tel ensemble d'étoiles en deux dimensions (et non en trois comme dans la vraie vie).

Pour ce faire, on effectue un certain nombre de simplifications et d'approximations.
On considère $n$ étoiles de masse $\{m_i\}_{i=0}^{n-1}$ qui sont repérées par leurs vecteurs
position respectifs $\{\vec r_i\}_{i=0}^{n-1}$. Dans un espace à deux dimensions, $\vec r_i$ s'écrit
$$
\vec r_i=\vectwo{r_{ix}}{r_{iy}}.
$$
Entre chacune des paires d'étoiles s'applique la loi de la gravitation de Newton.
La force exercée par la masse $m_j$ sur la masse $m_i$ est donnée par
$$
\vec F_{ij} = \frac{Gm_im_j(\vec r_j-\vec r_i)}{||\vec r_j-\vec r_i||^3},
$$
où $G=6.67\cdot 10^{-11}\mathrm{m}^3\cdot \mathrm{kg}^{-2}\cdot \mathrm{s}^{-2}$ est la constante de gravitation universelle, et $r_{ij}=||\vec r_j-\vec r_i||$ est la norme du vecteur $\vec r_j-\vec r_i$ qui se calcule comme
$$
r_{ij}=||\vec r_j-\vec r_i||=\sqrt{(r_{jx}-r_{ix})^2+(r_{jy}-r_{iy})^2}.
$$
Cette grandeur est également la distance entre les étoiles $i$ et $j$.

La force résultante s'appliquant sur $m_i$ est donc donnée par la somme sur toutes
les autres étoiles
$$
\vec F_i=\sum_{j=0,j\neq i}^n \frac{Gm_im_j(\vec r_j-\vec r_i)}{||\vec r_j-\vec r_i||^3}.
$$
La deuxième loi de Newton nous dit que le mouvement de chacune des étoiles sera donné
par la solution de l'équation différentielle suivante
$$
m_i \vec r_i''(t)=\vec F_i(t),\quad i=0,...,n-1,
$$
où $\vec r_i''(t)$ représente l'accélération de la masse $m_i$ au temps $t$ (la deuxième dérivée de sa position). Afin de résoudre cette équation différentielle, on a également besoin de conditions initiales (l'état initial de notre ensemble d'étoiles). Pour ce faire, il faut spécifier une position et une vitesse au temps $t=0$ pour chacune des particules
$$
\vec r_i(t=0)=\vec R_i,\quad \vec r_i'(t=0)=\vec v_i.
$$

## Approximation numérique

Afin de simuler ce système, on approxime l'équation du mouvement
$$
\vec r_i''(t)=\frac{\vec F_i(t)}{m_i}\equiv\vec a_i(t),\quad i=0,...,n-1.
$$
Pour ce faire, on *discrétise* le temps par pas de $\delta t$. Le temps plutôt
que de s'écouler de façon continue, s'écoule de façon discrète
$$
t_i=i\cdot \delta t,\quad i\in\natural.
$$
On commence à $t_0=0$ et on progresse dans le temps par pas de $\delta t$.
On aura donc la position de chacune des étoiles qui sera donnée par une suite de positions
$$
\vec r_i(t_0),\ \vec r_i(t_1),\ \vec r_i(t_2), ...
$$
On cherche une approximation de cette suite de positions.
Cette approximation se fera itérativement à l'aide de la formule de Verlet[^1]
$$
\vec r(t+\delta t)=2\vec r(t)-\vec r(t-\delta t)+\delta t^2\vec a(t).
$$
On voit dans cette équation que pour calculer $\vec r_i(t+\delta t)$ on a besoin de $\vec r_i(t)$
et de $\vec r_i(t-\delta t)$, ainsi que de l'accélération $\vec a_i(t)=F_i(t)/m_i$.
En particulier, on doit donc avoir $\vec r_i(0)$ et $\vec r_i(\delta t)$ qui sont spécifiés
comme conditions initiales pour pouvoir calculer $\vec r_i(2\delta t)$ et ainsi les positions suivantes.On donnera explicitement la position initiale des étoiles, $\vec r_i(0)$, ainsi que
leurs vitesses initiales, $\vec v_i$, et on déduira $\vec r_i(\delta t)=r(0)+\delta t\vec v_i$.

Dans ce travail, on considérera $\delta t=10^{10}$ (il sera noté `dt` dans le code plus tard).

Une autre approximation très importante pour éviter certains problèmes, est une petite modification de la force.
On constate que dans la formulation standard la force tend vers l'infini quand
$\vec r_i\rightarrow \vec r_j$, car dans ce cas
$$
r_{ij}^3\rightarrow 0,
$$
(rappelez vous que $r_{ij}=||\vec r_i-\vec r_j||$).
Pour éviter les problèmes on remplace $r_{ij}$ par $r_{ij}'=r_{ij}+\varepsilon$, où $\varepsilon>0$.
De cette façon la force ne tendra jamais vers l'infini. La force aura donc la forme
$$
\vec F_{ij} = \frac{Gm_im_j(\vec r_j-\vec r_i)}{(r_{ij} + \varepsilon)^3}.
$$
La valeur de $\varepsilon=3\cdot 10^4\mathrm{m}$.

## L'algorithme de Barnes--Hut

Une des difficultés principales pour simuler l'évolution d'une galaxie est le calcul de la force
agissant sur chacune des étoiles. Chaque étoile subissant l'attraction gravitationnelle
des $n-1$ autres étoiles de la galaxie, on a que la complexité algorithmique
de la mise à jour des force est de $n^2$ ce qui est trop coûteux. 

L'algorithme de Barnes-Hut[^2] est une approximation permettant de faire baisser dramatiquement
la complexité algorithmique de ce genre de simulations: elle devient $n\cdot\log(n)$. L'idée générale
de l'algorithme est de simplifier le problème en approximant le calcul de la force.

Considérons la situation de la @fig:barnes_hut1, où une étoile isolée, $m_0$, est éloignée d'un groupe d'étoiles, $\{m_i\}_{i=1}^{n-1}$. Une bonne approximation de la force agissant sur l'étoile isolée est de considérer le groupe d'étoiles comme une unique "super-étoile" dont la masse, $m_{se}$, est la masse totale des étoiles du groupe et sa position, $\vec r_{se}$, est le centre de masse du groupe
$$
m_{se}=\sum_{i=1}^{n-1}m_i,\quad \vec r_{se}=\frac{1}{m_{se}}\sum_{i=1}^{n-1}m_i\vec r_i.
$$

![L'étoile $m_0$ est isolée du reste du groupe. On considère que celle-ci ne subit pas l'influence de toutes les étoiles individuelles (en noir), mais uniquement de la super étoile (en rouge) donc la masse est $m_{se}$ et la position $\vec r_{se}$. La force entre $m_0$ et le groupe est donc calculée grâce à $m_0$, $m_{se}$, et $\vec r_{0,se}$.](figs/barnes_hut.svg){#fig:barnes_hut1 width="50%"}

Pour utiliser cette approximation de façon efficace, nous subdivisons notre
domaine de simulation en deux dimensions en sous-domaines rectangulaires
à l'aide d'une structure de quad-tree. Le quad-tree stockera une étoile sur chacune de ces feuilles
et une "super-étoile" sur chacun de ses nœuds internes.
Chaque nœud de notre quad-tree contient deux types d'étoiles:
une étoile normale et une super étoile.
L'insertion des étoiles dans le quad-tree se fait récursivement,
à l'aide du pseudo-code suivant:

```language-c
void insert_star(node *n, star *s) {
    si (s est dans le box de n et n est alloué) {
        si (n est une feuille) {
            si (est une feuille vide) {
                mettre s dans n
            } sinon {
                subdiviser le nœud n
                pour chaque enfant de n {
                    insert_star(enfant de n, n->s)
                }

                pour chaque enfant de n {
                    insert_star(enfant de n, s)
                }

                n ne contient plus d'étoile
            } 
        } sinon {
            incrémenter la masse de la super étoile
            incrémenter la valeur du centre de masse

            pour chaque enfant de n {
                insert_star(enfant de n, s)
            }
        }
    }
}
```

En résumé:

- Si l'étoile à rajouter, `s` n'est pas dans le domaine du nœud on fait rien.
- Sinon on a trois cas de figure:

    1. Le nœud est une feuille vide: on rajoute `s` dans la feuille.
    2. Le nœud est une feuille non-vide. On subdivise le nœud en quatre et on rajoute l'étoile déjà présente dans le nœud dans un enfant. Puis, on isère `s` dans un enfant. Finalement, on "efface" l'étoile déjà présente dans le nœud (qui a été déplacée dans un enfant).
    3. Le nœud n'est pas une feuille, on met à jour la super étoile et on insère l'étoile dans un nœud enfant.

A l'aide de cette structure on peut à présent calculer récursivement la mise à jour de l'accélération
sur chaque étoile, `s`, en fonction de son éloignement à chaque nœud du quad-tree, `n`, et d'un
paramètre `double theta`. Cette fonction aura la signature suivante:

```language-c
void update_acceleration_from_node(const node *const n,
                                   star *s, double theta);
```

Cette fonction fera la chose suivante:

1. Si le nœud est une feuille non-vide et que l'étoile n'est pas dans le sous-domaine du nœud, on met à jour l'accélération entre l'étoile et l'étoile contenue dans le nœud.
2. Sinon, si `n` est assez éloigné de `s` (si la taille du sous-domaine de `n` divisée par la distance entre la position de la super étoile de `n` et `s` est plus petite que `theta`) on met à jour l'accélération de `s` à l'aide de la super étoile de `n`.
3. Sinon, pour tous les enfants de `n`, on rappelle

    ```language-c

    update_acceleration_from_node(n, s, theta);
    ```


## Condition initiale

Comme pour toute simulation qui dépend du temps, il est nécessaire de spécifier une condition initiale du système. Ici, on va supposer que toutes les étoiles sauf une ont une masse aléatoire,
$m_i=m_\mathrm{min}+m_i'$, $m_\mathrm{min}=10^{20}\mathrm{kg}$, $i>0$, avec $m_i'$ qui est donné par
$$
m_i'=\randone(10)\cdot m_\mathrm{solaire},
$$
où $\randone(10)$ est un nombre aléatoire entre 0 et 10, et avec $m_\mathrm{solaire}=1.98892\cdot 10^{30}\mathrm{kg}$. Chacune de ces masses aura une position $\vec r_i$ qui sera donnée par
$$
\vec r_i=R\cdot \left(\frac{\log(1 - \randone(1))}{1.8}\right)\vectwo{0.5 - \randone(1)}{0.5 - \randone(1)},
$$ 
où $R=10^{18}\mathrm{m}$, $\randone(1)$ est un nombre aléatoire entre zéro et un.
Une seule étoile aura une masse beaucoup plus grande, $m_0=10^6\cdot m_{\mathrm{solaire}}$, et sera exactement au centre de la
galaxie, donc en $\vec r_0=\vectwo{0}{0}$. La vitesse initiale de l'étoile centrale est de $\vec 0$.
Les autres étoiles ont une vitesse donnée par
$$
\vec v_i = \sqrt{\frac{G (m_i + m_0)}{||\vec r_i||}}\vectwo{-\sin(\phi)}{\cos{\phi}},
$$
où $\phi=\arctan(r_{iy}/r_{ix})$. Pour se simplifier la vie utiliser la fonction `atan2()`{.language-c} de `C`.

# Travail à réaliser

Pour toutes ces sections écrivez dans la mesure du possible des tests
pour vous assurer que tout fonctionne bien. La gestion de la mémoire
est particulièrement ardue dans ce travail pratique. Utilisez donc au maximum
les sanitizers, ainsi que les outils `valgrind` et `gdb` afin de vérifier
que vous n'avez pas de fuites mémoire, de doubles libérations, de pointeurs pendouillants, ...

## Une librairie de vecteurs

Écrire une librairie de vecteurs à deux dimensions de `double`.

```{.language-c}
typedef struct __vec {
    double x, y;
} vec;
```

Écrire des fonctions qui permettent de manipuler les vecteurs:

- Créer et initialiser un vecteur:

    ```language-c

    vec *new_vec(double x, double y);
    ```
- Additionner deux vecteurs

    ```language-c

    vec *add_vec(const vec *const v1, const vec *const v2);
    ```
- Soustraire deux vecteurs

    ```language-c

    vec *sub_vec(const vec *const v1, const vec *const v2);
    ```
- Multiplier un vecteur par un scalaire

    ```language-c

    vec *mul_vec(const vec *const v1, const vec *const v2);
    ```
- Calculer la norme d'un vecteur

    ```language-c

    double norm(const vec *const v1);
    ```
- Calculer la distance entre deux vecteurs

    ```language-c

    double distance(const vec *const v1, const vec *const v12);
    ```
- Afficher un vecteur (ça vous aidera pour le débogage)

    ```language-c

    void print_vec(const vec *const v);
    ```

## Une librairie de `box`

Afin de gérer le domaine de simulation et ses sous-domaines, il faut créer une librairie de boîtes, `box`, défini par:

```language-c
typedef struct __box {
    double x0, x1, y0, y1;
} box;
```

contient `x0`, `x1`, `y0`, et `y1`, les dimensions du domaine
de simulation. Pour simplifier, le centre du domaine de simulation sera toujours situé en $\vec r_0=\vec 0$.

Cette librairie devra implémenter au moins les fonctions suivantes:

- Création d'une nouvelle `box`:

    ```language-c

    box new_box(double x0, double x1, double y0, double y1);
    ```

- Division d'une `box` en quatre parties égales

    ```language-c
    
    box *divide_in_four(box b);
    ```

- Déterminer si une position est à l'intérieur de la `box`

    ```language-c

    bool is_inside(box b, vec v);
    ```

- Déterminer la taille maximale d'un des côtés de la `box`

    ```language-c

    double compute_length(box b);
    ```

- Affiche la `box` (utile pour le débogage)

    ```language-c

    void print_box(box b);
    ```

## Une librairie d'étoiles

Écrire une librairie permettant de gérer la création d'étoiles,
ainsi que leur déplacement. Cette structure aura la forme suivante

```language-c
typedef struct __star {
    vec pos_t, pos_t_dt, acc;
    double mass;
} star;
```

où `pos_t` est la position actuelle de l'étoile (équivalent à $\vec r_i(t)$), `pos_t_dt` (équivalent à $\vec r_i(t-\delta t)$) sa position au temps précédent,  `acc` l'accélération de l'étoiles, et `mass` sa masse.

Cette librairie devrait au moins contenir les fonctions suivantes:

- Création d'une nouvelle étoile à la position `pos_t`, vitesse `vel`, accélération `acc`, masse `mass` et `dt` la discrétisation temporelle:

    ```language-c

    star *new_star_vel(vec pos, vec vel, vec acc, double mass, 
                       double dt);
    ```

    On initialisera `pos_t_dt` à l'aide de la relation (en pseudo-code)
    
    ```language-c

      pos_t_dt = pos_t - dt * vel
    ```

- Remise à zéro de l'accélération d'une étoile:

    ```language-c

    void reset_acceleration(star *s);
    ```
- Mise à jour de l'accélération d'une étoile, `s`, à cause de l'attraction gravitationnelle d'une autre étoile, `s2`:

    ```language-c

    void update_acceleration(star *s, const star *const s2);
    ```
- Mise à jour de la position d'une étoile, avec `dt` la discrétisation temporelle:

    ```language-c
    
    void update_position(star *s, double dt);

    ```

    En pseudo-code cette mise à jour est de la forme:
    
    ```language-c

    pos_t = 2*pos_t - pos_t_dt + acc * dt * dt
    ```

- Affichage des champs d'une étoile pour aider au débogage:

    ```language-c

    void print_star(const star *const s);
    ```

## La galaxie

Vos programme devra contenir une `struct galaxy`{.language-c}

```language-c
typedef struct __galaxy {
    int num_bodies;
    star *stars;
    box b;
} galaxy;
```

où `stars` est un tableau avec `num_bodies` étoiles de la galaxie et `box`{.language-c} qui contiendra les coordonnées du domaine de simulation.

La galaxie doit avoir au moins les fonctions suivantes:

- L'allocation et initialisation de la galaxie:

    ```language-c

    galaxy *create_and_init_galaxy(int num_bodies, box b,
                                   double dt);
    ```

    avec `dt`  la discrétisation temporelle.

- La remise à zéro de toutes les accélérations des étoiles de la galaxie:

    ```language-c

    void reset_accelerations(galaxy *g);
    ```

- La mise à jour des positions de toutes les étoiles de la galaxie:

    ```language-c

    void update_positions(galaxy *g, double dt);

    ```

    avec `dt`  la discrétisation temporelle.

- La libération de la mémoire de la galaxie:

    ```language-c

    void free_galaxy(galaxy *g);
    ```

- Lorsqu'une étoile, `s`, sort de la `box` de la galaxie, celle-ci doit être `s` doit effacée du tableau d'étoiles contenu dans la galaxie et sa mémoire libérée:

    ```language-c

    void resize_galaxy(galaxy *g);
    ```

## Le quad tree

La structure du quad tree

```language-c
typedef struct __quad_tree {
    node *root;
} quad_tree;
```

contenant la racine de type `node` qui est une structure définie comme

```language-c
typedef struct __node {
    struct __node *children[4];
    box b;
    star *s;
    star *super_s;
    bool is_empty;
} node;
```

Le quad tree doit avoir au moins trois fonctions dont les signatures sont:

- Création du `quad_tree`:

    ```language-c

    quad_tree *create_quad_tree_from_galaxy(const galaxy *const g); 
    ```
- Libération de la mémoire du `quad_tree`

    ```language-c

    void free_quad_tree(quad_tree *t);
    ```

- Mise à jour de l'accélération d'une étoile:

    ```language-c

    void update_acceleration_on_star(const node *const n, star *s,
                                     double theta);
    ```

- Mise à jour de l'accélération de toutes les étoiles:

    ```language-c

    void update_accelerations_of_all_stars(const node *const n, 
                                           galaxy *g, double theta);
    ```


## Structure du programme

La fonction `main` s’occupe seulement de gérer les arguments de la ligne de commande, initialiser les structures de données, les itérations temporelles et les entrées/sorties (clavier et affichage). Elle sera structurée comme suit (en pseudo-c, il manque les arguments aux fonctions):

```language-c
int main() {
    gestion_de_la_ligne_de_commande();
    allocation_mémoire_et_initialisation_de_la_galaxie();
    itérations_temporelles {
        liberation_des_etoiles_qui_sortent_du_domaine();
        affichage();
        mise_a_zero_des_accelerations();
        creation_quad_tree();
        mise_a_jour_des_accelerations_via_le_quad_tree();
        mise_a_jour_des_positions();
        destruction_du_quad_tree();
    }
    destruction_galaxie();
}
```

## Cahier des charges

- Le programme à développer sera nommé `galaxy` et sa syntaxe est la suivante :
``` {.changelog}
galaxy <num_stars> <theta>
    - num_stars est un entier représentant le nombre d'étoiles
      dans la galaxie et theta la valeur définissant la distance 
      minimale entre les nœuds et les feuilles du quad_tree.
Exemple : ./galaxy 1000 0.5
```
- Un `makefile` devra être présent à la racine et aura comme première règle la compilation de votre programme produisant un exécutable nommé `galaxy`.
- L’affichage doit être réalisé avec la librairie `gfx` fournie (voir la section suivante).
- Le programme se terminera, proprement, une fois la touche d’échappement (`ESCAPE`) pressée.
- Aucune variable globale n’est autorisée. À noter qu’il est permis d’utiliser des constantes globales (déclarées via la directive `#define` ou le mot-clé `const`).

## Informations utiles

**Attention:** Votre programme doit se compiler et s'exécuter sur une machine Linux.

### Affichage et entrée/sorties

Dans le dépôt `git` de ce travail pratique, dans le répertoire `lib`, se trouve une librairie d'affichage et
de gestion du clavier très sommaire basée sur la librairie `SDL2`. Il y a également
un exemple d'utilisation de cette librairie, `noise.c`, qui affiche des niveaux de gris des positions aléatoires qui vous sera utile pour
afficher votre galaxie et permettre à l'utilisateur de quitter votre programme en appuyant sur la touche `ESCAPE`. Si vous utilisez votre machine personnelle, il faut installer la librairie `SDL2`. Pour les distributions Debian utiliser:

```
sudo apt-get install libsdl2-2.0 libsdl2-dev
```

Pour les distributions ArchLinux

```
sudo pacman -S sdl2
```

### Options de compilation

Nous vous conseillons de compiler votre code avec gcc et les options de compilation suivantes :

```
-g -std=c11 -Wall -Wextra -fsanitize=address -fsanitize=leak
-fsanitize=undefined
```

Afin de pouvoir utiliser les fonctions d'affichage et les fonctions mathématiques
il faut également effectuer l'édition des liens avec les options

```
-lm -lSDL2
```

# Travail à rendre

- Ce travail sera réalisé individuellement.
- Vous devez utiliser **git et gitedu.hesge.ch**. Vous devrez "forker" ce repository et suivre à la lettre la procédure contenue dans le `README.md`, sous peine de pénalités. Il est exigé qu'au minimum une version (donc un commit) soit réalisée par séance de TP, et ce jusqu'au rendu du travail.
- Le rendu du travail est fixé au **dimanche 16 juin 2019 à 23h30** (la version antérieure ou égale à cette date sera récupérée). Vous devez mettre le lien vers votre dépôt `git`
sur le wiki de `cyberlearn` qui se trouve dans la section *Simulation de galaxie*.
- Suite au rendu, vous devrez effectuer une présentation orale de votre travail le **mardi 18 juin 2019**. La note sera une combinaison du code rendu et de la présentation.

[^1]: Voir <https://fr.wikipedia.org/wiki/Int%C3%A9gration_de_Verlet> ou <https://en.wikipedia.org/wiki/Verlet_integration> pour plus de détails.
[^2]: Voir <https://en.wikipedia.org/wiki/Barnes%E2%80%93Hut_simulation>.