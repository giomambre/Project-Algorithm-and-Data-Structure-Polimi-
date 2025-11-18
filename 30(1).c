// INIZIATO IL 04/08/2024 11.23


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_LEN 30
#define or ||
#define and &&

unsigned int conta_istanti = 0, tempo_rif = 0, capienza_iniziale = 0, capienza_attuale = 0;
short int sc; // valore per evitare i warnings dello scanf
typedef enum { ATTESA,
               PRONTO } stato;
typedef enum { RED,
               BLACK } color;

typedef struct node_lotto {
    unsigned short int qta;
    int scad;
    struct node_lotto *next;

} node_lotto;

typedef struct node_magazzino {

    char nome[MAX_LEN];
    int qta_tot;
    color c;
    node_lotto *lotti;
    struct node_magazzino *left, *right, *parent;

} node_magazzino;

typedef struct node_ricetta {

    char nome[MAX_LEN]; // nome ricetta
    color c;
    int peso_tot;
    struct node_ricetta *left, *right, *parent;
    struct ingrediente_ricetta *ingredienti;

} node_ricetta;

typedef struct ingrediente_ricetta {
     short   int qta;
    node_magazzino *lotto;
    struct ingrediente_ricetta *next;

} ingrediente_ricetta;

typedef struct {
    node_ricetta *root, *nil;
} tree_r;

typedef struct {
    node_magazzino *root, *nil;
} tree_m;

typedef struct node_ordine {
    node_ricetta *ricetta_ordine;
    unsigned short int  num_dolci;
    int tempo;
      
    struct node_ordine *next;

} node_ordine;

typedef struct ordini_attesa {

    node_ordine *head;
    node_ordine *tail;

} ordini_attesa;

typedef struct ordini_pronti {

    node_ordine *curr;
    struct ordini_pronti *next;

} ordini_pronti;

typedef struct camioncino {
    node_ordine *curr;

    struct camioncino *next;

} camioncino;

ordini_pronti *lista_pronti;
ordini_attesa lista_attesa;
camioncino *camion;
// PROTOTIPI FUNZIONI
tree_r *create_rb_r();
tree_m *create_rb_m();
node_ricetta *ricerca_r(node_ricetta *, char[MAX_LEN]);
node_magazzino *ricerca_m(node_magazzino *, char[MAX_LEN]);
void left_rotate_r(tree_r *, node_ricetta *);
void right_rotate_r(tree_r *, node_ricetta *);
void left_rotate_m(tree_m *, node_magazzino *);
void right_rotate_m(tree_m *, node_magazzino *);
void aggiungi_ricetta(tree_r *, tree_m *);
void rb_insert_r(tree_r *, node_ricetta *);
void rb_insert_m(tree_m *, node_magazzino *);
void rb_insert_fixup_m(tree_m *, node_magazzino *);
void rb_insert_fixup_r(tree_r *, node_ricetta *);
void stampa_albero_r(node_ricetta *);
void stampa_albero_m(node_magazzino *);
void rifornimento(tree_m *);
void inserisci_lotto(node_magazzino *, node_lotto **, char *);
void aggiorna_peso_magazzino(node_magazzino *);
void ordina(tree_r *,tree_m*);
stato assegna_stato(node_ordine *);
void prepara_ordine(node_ordine *);
void init_queues(ordini_attesa *);
void aggiungi_in_attesa(ordini_attesa *, node_ordine *);
void aggiungi_pronto(ordini_pronti **, node_ordine *);
void rimuovi_pronto(ordini_pronti **, node_ordine *, ordini_pronti **);
void rimuovi_attesa(ordini_attesa *, node_ordine *, node_ordine*, node_ordine**);
void controlla_attesa(ordini_attesa *);
void stampa_lista_attesa(ordini_attesa *);
void stampa_lista_pronti(ordini_pronti *);
void ritiro_ordini(ordini_pronti *);
void aggiungi_al_camion(camioncino **, node_ordine *);
void stampa_camioncino(camioncino *);
void svuota_camioncino(camioncino **);
void rimuovi_ricetta(tree_r *, ordini_pronti *, ordini_attesa *);
node_ricetta *min_r(node_ricetta *, node_ricetta *);
node_magazzino *min_m(node_magazzino *);
void trasplant_r(tree_r *, node_ricetta *, node_ricetta *);
void trasplant_m(tree_m *, node_magazzino *, node_magazzino *);
void delete_r(tree_r *, node_ricetta *);
void rb_delete_fixup_r(tree_r *, node_ricetta *);
void delete_m(tree_m *, node_magazzino *);
void rb_delete_fixup_m(tree_m *, node_magazzino *);
void elimina_ingredienti(ingrediente_ricetta** );
void elimina_lotti(node_lotto** );


int main() {

    tree_r *albero_r = create_rb_r();
    tree_m *albero_m = create_rb_m();

    init_queues(&lista_attesa);
    
    int tempo_rif;
    char comando[20];
    sc = scanf("%d %d", &tempo_rif, &capienza_iniziale);
    capienza_attuale = capienza_iniziale;
    while (scanf("%s", comando) != EOF) {
        if (conta_istanti % tempo_rif == 0 and conta_istanti != 0) {
            ritiro_ordini(lista_pronti);

            if (capienza_attuale == capienza_iniziale)
                puts("camioncino vuoto");
            capienza_attuale = capienza_iniziale;
        }
        if (!strcmp(comando, "aggiungi_ricetta")) {

            aggiungi_ricetta(albero_r, albero_m);

        } else if (!strcmp(comando, "rimuovi_ricetta")) {

            rimuovi_ricetta(albero_r, lista_pronti, &lista_attesa);

        } else if (!strcmp(comando, "rifornimento")) {

            rifornimento(albero_m);

        } else if (!strcmp(comando, "ordine")) {

            ordina(albero_r,albero_m);
        }
        conta_istanti++;
    }
    // stampa_lista_attesa(&queue_attesa);

    // stampa_lista_pronti(&lista_pronti);
    /*  if (conta_istanti % tempo_rif == 0 and conta_istanti != 0) {
         ritiro_ordini(lista_pronti);

         if (capienza_attuale == capienza_iniziale)
             puts("camioncino vuoto");
     } */
    /* puts("ALBERO MAG");
    stampa_albero_m(albero_m->root);

    puts("\nLISTA ATTESA");
    stampa_lista_attesa(&lista_attesa);
    puts("\nLISTA PRONTI");
    stampa_lista_pronti(lista_pronti);
 */
    return 0;
}

tree_r *create_rb_r() {
    tree_r *t = malloc(sizeof(tree_r));
    if (t == NULL) {
        puts("errore malloc");
        exit(1);
    }
    t->nil = malloc(sizeof(node_ricetta));
    t->nil->left = t->nil->right = t->nil->parent = NULL;
    t->nil->c = BLACK;
    t->root = t->nil;

    return t;
}

tree_m *create_rb_m() {
    tree_m *t = malloc(sizeof(tree_m));
    if (t == NULL) {
        puts("errore malloc");
        exit(1);
    }

    t->nil = malloc(sizeof(node_magazzino));
    t->nil->left = t->nil->right = t->nil->parent = NULL;
    t->nil->c = BLACK;
    t->root = t->nil;

    return t;
}

node_ricetta *ricerca_r(node_ricetta *root, char key[MAX_LEN]) {

    if (root == NULL or strcmp(root->nome, key) == 0)
        return root;
    if (strcmp(key, root->nome) < 0)
        return ricerca_r(root->left, key);
    else
        return ricerca_r(root->right, key);
}

node_magazzino *ricerca_m(node_magazzino *root, char key[MAX_LEN]) {

    if (root == NULL or strcmp(root->nome, key) == 0)
        return root;
    if (strcmp(key, root->nome) < 0)
        return ricerca_m(root->left, key);
    else
        return ricerca_m(root->right, key);
}

void left_rotate_r(tree_r *tree, node_ricetta *x) {
    node_ricetta *y = x->right;
    x->right = y->left;

    if (y->left != tree->nil) {
        y->left->parent = x;
    }

    y->parent = x->parent;
    if (x->parent == tree->nil) {
        tree->root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
}

void right_rotate_r(tree_r *tree, node_ricetta *x) {
    node_ricetta *y = x->left;
    x->left = y->right;

    if (y->right != tree->nil) {
        y->right->parent = x;
    }

    y->parent = x->parent;
    if (x->parent == tree->nil) {
        tree->root = y;
    } else if (x == x->parent->right) {
        x->parent->right = y;
    } else {
        x->parent->left = y;
    }
    y->right = x;
    x->parent = y;
}

void left_rotate_m(tree_m *tree, node_magazzino *x) {
    node_magazzino *y = x->right;
    x->right = y->left;

    if (y->left != tree->nil) {
        y->left->parent = x;
    }

    y->parent = x->parent;
    if (x->parent == tree->nil) {
        tree->root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
}

void right_rotate_m(tree_m *tree, node_magazzino *x) {
    node_magazzino *y = x->left;
    x->left = y->right;

    if (y->right != tree->nil) {
        y->right->parent = x;
    }

    y->parent = x->parent;
    if (x->parent == tree->nil) {
        tree->root = y;
    } else if (x == x->parent->right) {
        x->parent->right = y;
    } else {
        x->parent->left = y;
    }
    y->right = x;
    x->parent = y;
}

void aggiungi_ricetta(tree_r *tree, tree_m *tree_magazzino) {
    char nome_ricetta[MAX_LEN], flag = ' ';
    char nome_ingrediente[MAX_LEN];
    int quantita;
    node_ricetta *new_node;
    ingrediente_ricetta *new_ingred, *current;
    node_magazzino *ingre_magazzino;

    sc = scanf("%s", nome_ricetta);

    if (ricerca_r(tree->root, nome_ricetta) != NULL) {
        puts("ignorato");
        while (flag != '\n' and flag != EOF) { // per consumare il resto dell'input
            sc = scanf("%*s %*d");

            flag = getc(stdin);
        }
        return;
    }

    new_node = malloc(sizeof(node_ricetta));
    if (new_node == NULL) {
        puts("errore malloc");
        exit(1);
    }
    strcpy(new_node->nome, nome_ricetta);
    new_node->left = new_node->right = new_node->parent = tree->nil;
    new_node->ingredienti = NULL;
    new_node->peso_tot = 0;
    flag = ' ';
    while (flag != '\n' and flag != EOF) {
        sc = scanf("%s %d", nome_ingrediente, &quantita);
        new_node->peso_tot += quantita;
        current = new_node->ingredienti;
        flag = getc(stdin);
        // printf("%s %d %c\n",nome_ingrediente, quantita, flag);

        new_ingred = malloc(sizeof(ingrediente_ricetta));

        if (new_ingred == NULL) {
            puts("errore malloc");
            exit(1);
        }
        
        new_ingred->qta = quantita;
        new_ingred->next = NULL;

        // aggiunta puntatore al ingredinete al magazzino, se non presente lo creo

        ingre_magazzino = ricerca_m(tree_magazzino->root, nome_ingrediente);
        if (ingre_magazzino == NULL) {

            ingre_magazzino = malloc(sizeof(node_magazzino));
            strcpy(ingre_magazzino->nome, nome_ingrediente);
            ingre_magazzino->qta_tot = 0;
            ingre_magazzino->lotti = NULL;
            ingre_magazzino->left = ingre_magazzino->right = ingre_magazzino->parent = tree_magazzino->nil;
            rb_insert_m(tree_magazzino, ingre_magazzino);
        }
        new_ingred->lotto = ingre_magazzino;

        if (new_node->ingredienti == NULL) {

            new_node->ingredienti = new_ingred;
        } else { // inserimento in testa

            while (current->next != NULL) {

                current = current->next;
            }
            current->next = new_ingred;
        }
    }
    rb_insert_r(tree, new_node);
    puts("aggiunta");
    return;
}

void rb_insert_r(tree_r *tree, node_ricetta *z) {
    node_ricetta *y = tree->nil;
    node_ricetta *x = tree->root;

    while (x != tree->nil) {
        y = x;
        if (strcmp(z->nome, x->nome) < 0)
            x = x->left;
        else
            x = x->right;
    }

    z->parent = y;

    if (y == tree->nil)
        tree->root = z;
    else if (strcmp(z->nome, y->nome) < 0)
        y->left = z;
    else
        y->right = z;

    z->left = tree->nil;
    z->right = tree->nil;
    z->c = RED;

    rb_insert_fixup_r(tree, z);
    return;
}

void rb_insert_m(tree_m *tree, node_magazzino *z) {
    node_magazzino *x = tree->root;
    node_magazzino *y = tree->nil;

    while (x != tree->nil) {
        y = x;
        if (strcmp(z->nome, x->nome) < 0)
            x = x->left;
        else
            x = x->right;
    }

    z->parent = y;

    if (y == tree->nil)
        tree->root = z;
    else if (strcmp(z->nome, y->nome) < 0)
        y->left = z;
    else
        y->right = z;

    z->left = tree->nil;
    z->right = tree->nil;
    z->c = RED;

    rb_insert_fixup_m(tree, z);
    return;
}

void rb_insert_fixup_r(tree_r *tree, node_ricetta *z) {
    while (z->parent->c == RED) {
        if (z->parent == z->parent->parent->left) {
            node_ricetta *y = z->parent->parent->right;
            if (y->c == RED) {
                z->parent->c = BLACK;
                y->c = BLACK;
                z->parent->parent->c = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    left_rotate_r(tree, z);
                }
                z->parent->c = BLACK;
                z->parent->parent->c = RED;
                right_rotate_r(tree, z->parent->parent);
            }
        } else {
            node_ricetta *y = z->parent->parent->left;
            if (y->c == RED) {
                z->parent->c = BLACK;
                y->c = BLACK;
                z->parent->parent->c = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    right_rotate_r(tree, z);
                }
                z->parent->c = BLACK;
                z->parent->parent->c = RED;
                left_rotate_r(tree, z->parent->parent);
            }
        }
    }
    tree->root->c = BLACK;
}

void rb_insert_fixup_m(tree_m *tree, node_magazzino *z) {
    node_magazzino *y;
    while (z->parent->c == RED) {
        if (z->parent == z->parent->parent->left) {
            y = z->parent->parent->right;
            if (y->c == RED) {
                z->parent->c = BLACK;
                y->c = BLACK;
                z->parent->parent->c = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->right) {
                    z = z->parent;
                    left_rotate_m(tree, z);
                }
                z->parent->c = BLACK;
                z->parent->parent->c = RED;
                right_rotate_m(tree, z->parent->parent);
            }
        } else {
            y = z->parent->parent->left;
            if (y->c == RED) {
                z->parent->c = BLACK;
                y->c = BLACK;
                z->parent->parent->c = RED;
                z = z->parent->parent;
            } else {
                if (z == z->parent->left) {
                    z = z->parent;
                    right_rotate_m(tree, z);
                }
                z->parent->c = BLACK;
                z->parent->parent->c = RED;
                left_rotate_m(tree, z->parent->parent);
            }
        }
    }
    tree->root->c = BLACK;
}

void stampa_albero_r(node_ricetta *t) { // stampa IN ORDER!

    if (t != NULL) {
        stampa_albero_r(t->left);
        if (t->c == RED) {
            puts("Red");
        } else
            puts("Black");
        puts(t->nome);
        printf("peso tot :  %d\n", t->peso_tot);

        while (t->ingredienti != NULL) {

            printf("%s : %d\t  qta : disp : %d\n", t->ingredienti->lotto->nome , t->ingredienti->qta, t->ingredienti->lotto->qta_tot);
            t->ingredienti = t->ingredienti->next;
        }

        stampa_albero_r(t->right);
    }
}

void stampa_albero_m(node_magazzino *t) { // stampa IN ORDER!

    if (t != NULL) {
        stampa_albero_m(t->left);
        if (t->c == RED) {
            puts("Red");
        } else
            puts("Black");
        puts(t->nome);
        printf("qta tot :  %d\n", t->qta_tot);

        while (t->lotti != NULL) {

            printf("scad : %d , qta : %d\n", t->lotti->scad, t->lotti->qta);
            t->lotti = t->lotti->next;
        }

        stampa_albero_m(t->right);
    }
}

void rifornimento(tree_m *tree) {
    char nome_lotto[MAX_LEN], flag = ' ';

    node_magazzino *new_node;
    while (flag != '\n' and flag != EOF) {

        sc = scanf("%s", nome_lotto);

        new_node = ricerca_m(tree->root, nome_lotto);
        if (new_node == NULL) {
            new_node = malloc(sizeof(node_magazzino));
            strcpy(new_node->nome, nome_lotto);
            new_node->left = new_node->right = new_node->parent = tree->nil;
            new_node->qta_tot = 0;
            new_node->lotti = NULL;
            inserisci_lotto(new_node, &(new_node->lotti), &flag);
            rb_insert_m(tree, new_node);
        } else {

            inserisci_lotto(new_node, &(new_node->lotti), &flag);
        }
    }

    controlla_attesa(&lista_attesa);
    puts("rifornito");
}

void inserisci_lotto(node_magazzino *ingr_magazzino, node_lotto **lotto_head, char *flag) { // inserisci in ordine crescente in base alla scandenza
    node_lotto *new_node, *curr;
    int scad, qta;
    sc = scanf("%d %d", &qta, &scad);
    *flag = getc(stdin);
    if (scad <= conta_istanti or qta == 0)
        return;

    ingr_magazzino->qta_tot += qta;
    if (*lotto_head == NULL) {
        new_node = malloc(sizeof(node_lotto));
        new_node->qta = qta;
        new_node->scad = scad;
        new_node->next = NULL;
        *lotto_head = new_node;
        return;
    }
    if ((*lotto_head)->scad == scad) {
        (*lotto_head)->qta += qta;
        return;
    }
    if ((*lotto_head)->scad > scad) {
        new_node = malloc(sizeof(node_lotto));
        new_node->qta = qta;
        new_node->scad = scad;
        new_node->next = *lotto_head;
        *lotto_head = new_node;
        return;
    }

    new_node = malloc(sizeof(node_lotto));
    new_node->next = NULL,
    new_node->qta = qta;
    new_node->scad = scad;
    curr = *lotto_head;
    while (curr->next != NULL && curr->next->scad <= new_node->scad) {

        if (curr->next->scad == new_node->scad) {
            curr->next->qta += qta;
            return;
        }
        curr = curr->next;
    }
    new_node->next = curr->next;
    curr->next = new_node;
    return;
}

void aggiorna_peso_magazzino(node_magazzino *da_aggiornare) { // funziona che rimuove i lotti scaduti e aggiorna la disponibilià dell ingr
    node_lotto *curr = da_aggiornare->lotti, *da_cancellare;
    
    while (curr != NULL and (curr->scad <= conta_istanti or curr->qta == 0)) {

        da_aggiornare->qta_tot -= curr->qta;
        // printf("%d\n",curr->qta);
        da_cancellare = curr;
        da_aggiornare->lotti = curr->next;
        curr = curr->next;
        free(da_cancellare);
    }
    
    return;
}

void ordina(tree_r *tree,tree_m *tm) {
    int num_dolci;
    char nome_dolce[MAX_LEN];
    ingrediente_ricetta *ingredienti;
    node_ricetta *tmp;
    sc = scanf("%s %d", nome_dolce, &num_dolci);
    tmp = ricerca_r(tree->root, nome_dolce);
    if (tmp == NULL) {
        puts("rifiutato");
        return;
    }
    node_ordine *new_ordine = malloc(sizeof(node_ordine));
    new_ordine->tempo = conta_istanti;

    new_ordine->ricetta_ordine = tmp;

    new_ordine->num_dolci = num_dolci;
    new_ordine->next = NULL;
    ingredienti = new_ordine->ricetta_ordine->ingredienti;
    while (ingredienti != NULL) {
        aggiorna_peso_magazzino(ingredienti->lotto);
     //  if(ingredienti->lotto->qta_tot == 0) elimina_lotti(&ingredienti->lotto->lotti);
        ingredienti = ingredienti->next;
    }

    if (assegna_stato(new_ordine) == ATTESA) {

        aggiungi_in_attesa(&lista_attesa, new_ordine);
    } else {

        prepara_ordine(new_ordine);
        aggiungi_pronto(&lista_pronti, new_ordine);
    }
    puts("accettato");
    return;
}

stato assegna_stato(node_ordine *new_ordine) {

    ingrediente_ricetta *ingredienti = new_ordine->ricetta_ordine->ingredienti;
    node_magazzino *lotti_ingredienti;

    while (ingredienti != NULL) {

        lotti_ingredienti = ingredienti->lotto;
        aggiorna_peso_magazzino(lotti_ingredienti);
        // printf("%d\n",lotti_ingredienti->qta_tot);
        if (lotti_ingredienti->qta_tot < (ingredienti->qta * new_ordine->num_dolci)) {

            return ATTESA;
        }

        ingredienti = ingredienti->next;
    }

    return PRONTO;
}

void prepara_ordine(node_ordine *ordine) {

    ingrediente_ricetta *ingredienti = ordine->ricetta_ordine->ingredienti;
    node_lotto *lotti_ingredienti;
    node_magazzino *magazz;
    int qta_necessaria = 0;
    while (ingredienti != NULL) {

        qta_necessaria = ingredienti->qta * ordine->num_dolci;
        lotti_ingredienti = ingredienti->lotto->lotti;
        magazz = ingredienti->lotto;
        magazz->qta_tot -= qta_necessaria;
        while (lotti_ingredienti != NULL and qta_necessaria != 0) {

            if (qta_necessaria > lotti_ingredienti->qta) {

                qta_necessaria -= lotti_ingredienti->qta;
                lotti_ingredienti->qta = 0;
            } else {

                lotti_ingredienti->qta -= qta_necessaria;

                qta_necessaria = 0;
            }
            lotti_ingredienti = lotti_ingredienti->next;
        }
        ingredienti = ingredienti->next;
    }

    return;
}

void init_queues(ordini_attesa *q1) {

    q1->head = NULL;
    q1->tail = NULL;

    return;
}

void aggiungi_in_attesa(ordini_attesa *q, node_ordine *da_aggiungere) {

    if (q->tail != NULL) {
        q->tail->next = da_aggiungere;
    }
    q->tail = da_aggiungere;
    if (q->head == NULL) {
        q->head = q->tail = da_aggiungere;
    }
    return;
}

void aggiungi_pronto(ordini_pronti **pronti_head, node_ordine *da_aggiungere) {
    ordini_pronti *new_node, *curr;
    if ((*pronti_head) == NULL) {
        new_node = malloc(sizeof(ordini_pronti));
        new_node->curr = da_aggiungere;
        new_node->next = NULL;
        *pronti_head = new_node;

        return;
    }

    if (da_aggiungere->tempo < (*pronti_head)->curr->tempo) {
        new_node = malloc(sizeof(ordini_pronti));
        new_node->curr = da_aggiungere;
        new_node->next = *pronti_head;
        *pronti_head = new_node;
        return;
    }

    new_node = malloc(sizeof(ordini_pronti));
    new_node->curr = da_aggiungere;
    new_node->next = NULL;
    curr = *pronti_head;
    while (curr->next != NULL && curr->next->curr->tempo < new_node->curr->tempo) {

        curr = curr->next;
    }

    new_node->next = curr->next;
    curr->next = new_node;
    return;
}

void rimuovi_pronto(ordini_pronti **q_head, node_ordine *da_rimuovere, ordini_pronti **cont) {
    ordini_pronti *tmp, *prev;
    tmp = (*q_head);
    prev = (*q_head);
    if (tmp->curr == da_rimuovere) {
        (*q_head) = (*q_head)->next;
        *cont = *q_head;
        free(tmp);
        return;
    }

    while (tmp->curr != da_rimuovere and tmp != NULL) {
        prev = tmp;
        tmp = tmp->next;
    }
    if (tmp == NULL)
        return;
    prev->next = tmp->next;
    *cont = tmp->next;
    free(tmp);
}

void controlla_attesa(ordini_attesa *q) {
    node_ordine *curr= q->head,*prev = NULL;

    if (curr == NULL)
        return;
    
    while (curr != NULL) {
       
        if (assegna_stato(curr) == PRONTO) {
            aggiungi_pronto(&lista_pronti, curr);
            prepara_ordine(curr);
            rimuovi_attesa(&lista_attesa, prev,curr,&curr);
            
        } else {
            prev = curr;
            if(curr!=NULL)
            curr = curr->next;
        }
    }

    return;
}

void rimuovi_attesa(ordini_attesa *q, node_ordine *prev,node_ordine *da_rimuovere,node_ordine **cont) {


    if (q->head == NULL or q->tail ==NULL) {
        q->tail = q->head =NULL;
        *cont = NULL;
        return;
    }
    
    if (q->head == da_rimuovere) {
       node_ordine * tmp = q->head->next;
       q->head->next = NULL;
       q->head = tmp;
       
       if (q->head == NULL or q->tail ==NULL) {
        q->tail = q->head =NULL;
    }
        *cont =  q->head;
        
        return;
    }
    
    if(q->tail == da_rimuovere){
        da_rimuovere->next = NULL;
        prev->next = NULL;
        q->tail = prev;
    }else{
    prev->next = da_rimuovere->next;
    da_rimuovere->next = NULL;
    }
    if (q->head == NULL or q->tail ==NULL) {
        q->tail = q->head =NULL;
        *cont = NULL;
        return;

    }
    *cont =  prev->next;
    return;
}

/* void rimuovi_attesa(ordini_attesa **q_head, node_ordine *da_rimuovere, ordini_attesa **cont) {
    ordini_attesa *tmp, *prev;

    tmp = (*q_head);
    prev = (*q_head);
    if (tmp->curr == da_rimuovere) {
        (*q_head) = (*q_head)->next;
        *cont = *q_head;
        free(tmp);
        return;
    }

    while (tmp->curr != da_rimuovere and tmp != NULL) {
        prev = tmp;
        tmp = tmp->next;
    }
    if (tmp == NULL)
        return;
    prev->next = tmp->next;
    *cont = tmp->next;
    free(tmp);
} */

void stampa_lista_attesa(ordini_attesa *q) {

    node_ordine *tmp = q->head;

    while (tmp != NULL) {

        printf("%d , %s , %d\n", tmp->tempo, tmp->ricetta_ordine->nome, tmp->num_dolci);
        tmp = tmp->next;
    }
    return;
}
void stampa_lista_pronti(ordini_pronti *q) {

    while (q != NULL) {

        printf("%d , %s , %d\n", q->curr->tempo, q->curr->ricetta_ordine->nome, q->curr->num_dolci);
        q = q->next;
    }

    return;
}

void ritiro_ordini(ordini_pronti *q1) {

    if (q1 == NULL)
        return;

    if (q1->curr->ricetta_ordine->peso_tot * q1->curr->num_dolci > capienza_iniziale)
        exit(1);

    while (q1 != NULL) {

        if (q1->curr->ricetta_ordine->peso_tot * q1->curr->num_dolci <= capienza_attuale) {

            capienza_attuale -= q1->curr->ricetta_ordine->peso_tot * q1->curr->num_dolci;
            aggiungi_al_camion(&camion, q1->curr);
            rimuovi_pronto(&lista_pronti, q1->curr, &q1);

        } else {
            stampa_camioncino(camion);
            svuota_camioncino(&camion);

            return;
        }
    }

    stampa_camioncino(camion);
    svuota_camioncino(&camion);
}

void aggiungi_al_camion(camioncino **head, node_ordine *da_aggiungere) {
    camioncino *new_node;
    new_node = malloc(sizeof(camioncino));
    new_node->curr = da_aggiungere;
    new_node->next = NULL;

    if (*head == NULL) {
        *head = new_node;
        return;
    }
    camioncino *tmp = *head;
    int peso_new_node = new_node->curr->ricetta_ordine->peso_tot*new_node->curr->num_dolci;
    if (tmp->curr->ricetta_ordine->peso_tot * tmp->curr->num_dolci < peso_new_node) {
        new_node->next = tmp;
        *head = new_node;
        return;
    }
    while (tmp->next != NULL && ((tmp->next->curr->ricetta_ordine->peso_tot * tmp->next->curr->num_dolci) >= peso_new_node)) {

        tmp = tmp->next;
    }
    new_node->next = tmp->next;
    tmp->next = new_node;
    return;
}
void stampa_camioncino(camioncino *camion) {

    while (camion != NULL) {

        printf("%d %s %d\n", camion->curr->tempo, camion->curr->ricetta_ordine->nome, camion->curr->num_dolci);
        camion = camion->next;
    }
}

void svuota_camioncino(camioncino **camion) {

    camioncino *curr = *camion;
    camioncino *next = NULL;

    while (curr != NULL) {

        next = curr->next;
        free(curr->curr);
        free(curr);
        curr = next;
    }
    *camion = NULL;
}

void rimuovi_ricetta(tree_r *tree, ordini_pronti *q1, ordini_attesa *q2) {
    char nome[MAX_LEN];
    node_ricetta *trovata;
    node_ordine *tmp;
    sc = scanf("%s", nome);
    trovata = ricerca_r(tree->root, nome);
    if (trovata == NULL) {
        puts("non presente");
        return;
    }

    while (q1 != NULL) {

        if (!strcmp(q1->curr->ricetta_ordine->nome, nome)) {
            puts("ordini in sospeso");
            return;
        }
        q1 = q1->next;
    }

    tmp = q2->head;

    while (tmp != NULL) {

        if (!strcmp(tmp->ricetta_ordine->nome, nome)) {
            puts("ordini in sospeso");
            return;
        }
        tmp = tmp->next;
    }

    delete_r(tree, trovata);

    puts("rimossa");
    return;
}

node_ricetta *min_r(node_ricetta *x, node_ricetta *nil) {

    while (x->left != nil) {
        x = x->left;
    }
    return x;
}
node_magazzino *min_m(node_magazzino *x) {

    while (x->left != NULL) {
        x = x->left;
    }
    return x;
}

void trasplant_r(tree_r *t, node_ricetta *u, node_ricetta *v) {
    if (u->parent == t->nil) {
        t->root = v;
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }
    v->parent = u->parent;
}
void trasplant_m(tree_m *t, node_magazzino *u, node_magazzino *v) {
    if (u->parent == t->nil) {
        t->root = v;
    } else if (u == u->parent->left) {
        u->parent->left = v;
    } else {
        u->parent->right = v;
    }
    v->parent = u->parent;
}
void delete_r(tree_r *t, node_ricetta *z) {

    node_ricetta *y = z;
    node_ricetta *x;
    color y_original_color = y->c;
    
    if (z->left == t->nil) {
        x = z->right;
        trasplant_r(t, z, z->right);
    } else if (z->right == t->nil) {
        x = z->left;
        trasplant_r(t, z, z->left);
    } else {
        y = min_r(z->right, t->nil);
        y_original_color = y->c;
        x = y->right;
        if (y != z->right) {
            trasplant_r(t, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        } else {
            x->parent = y;
        }
        trasplant_r(t, z, y);
        y->left = z->left;
        y->left->parent = y;
        y->c = z->c;
    }
    if (y_original_color == BLACK) {
        rb_delete_fixup_r(t, x);
    }
    elimina_ingredienti(&(z->ingredienti));
    free(z);
}

void rb_delete_fixup_r(tree_r *t, node_ricetta *x) {
    node_ricetta *w;
    while (x != t->root and x->c == BLACK) {

        if (x == x->parent->left) {
            w = x->parent->right;
            if (w->c == RED) {

                w->c = BLACK;
                x->parent->c = RED;
                left_rotate_r(t, x->parent);
                w = x->parent->right;
            }
            if (w->left->c == BLACK and w->right->c == BLACK) {
                w->c = RED;
                x = x->parent;
            } else {
                if (w->right->c == BLACK) {
                    w->left->c = BLACK;
                    w->c = RED;
                    right_rotate_r(t, w);
                    w = x->parent->right;
                }
                w->c = x->parent->c;
                x->parent->c = BLACK;
                w->right->c = BLACK;
                left_rotate_r(t, x->parent);
                x = t->root;
            }

        } else {
            w = x->parent->left;
            if (w->c == RED) {

                w->c = BLACK;
                x->parent->c = RED;
                right_rotate_r(t, x->parent);
                w = x->parent->left;
            }
            if (w->right->c == BLACK and w->left->c == BLACK) {
                w->c = RED;
                x = x->parent;
            } else {
                if (w->left->c == BLACK) {
                    w->right->c = BLACK;
                    w->c = RED;
                    left_rotate_r(t, w);
                    w = x->parent->left;
                }
                w->c = x->parent->c;
                x->parent->c = BLACK;
                w->left->c = BLACK;
                right_rotate_r(t, x->parent);
                x = t->root;
            }
        }
    }
    x->c = BLACK;
}

void delete_m(tree_m *t, node_magazzino *z) {

    node_magazzino *y = z, *x;
    color y_original_color = y->c;

    if (z->left == t->nil) {
        x = z->right;
        trasplant_m(t, z, z->right);
    } else if (z->right == t->nil) {
        x = z->left;
        trasplant_m(t, z, z->left);
    } else {
        y = min_m(z->right);
        y_original_color = y->c;
        x = y->right;
        if (y != z->right) {
            trasplant_m(t, y, y->right);
            y->right = z->right;
            y->right->parent = y;
        } else
            x->parent = y;
        trasplant_m(t, z, y);
        y->left = z->left;
        y->left->parent = y;
        y->c = z->c;
    }
    if (y_original_color == BLACK) {
        rb_delete_fixup_m(t, x);
    }
    elimina_lotti(&(z->lotti));
    free(z);
}

void rb_delete_fixup_m(tree_m *t, node_magazzino *x) {
    node_magazzino *w;
    while (x != t->root and x->c == BLACK) {

        if (x == x->parent->left) {
            w = x->parent->right;
            if (w->c == RED) {

                w->c = BLACK;
                x->parent->c = RED;
                left_rotate_m(t, x->parent);
                w = x->parent->right;
            }
            if (w->left->c == BLACK and w->right->c == BLACK) {
                w->c = RED;
                x = x->parent;
            } else {
                if (w->right->c == BLACK) {
                    w->left->c = BLACK;
                    w->c = RED;
                    right_rotate_m(t, w);
                    w = x->parent->right;
                }
                w->c = x->parent->c;
                x->parent->c = BLACK;
                w->right->c = BLACK;
                left_rotate_m(t, x->parent);
                x = t->root;
            }

        } else {
            w = x->parent->right;
            if (w->c == RED) {

                w->c = BLACK;
                x->parent->c = RED;
                right_rotate_m(t, x->parent);
                w = x->parent->right;
            }
            if (w->left->c == BLACK and w->right->c == BLACK) {
                w->c = RED;
                x = x->parent;
            } else {
                if (w->right->c == BLACK) {
                    w->left->c = BLACK;
                    w->c = RED;
                    left_rotate_m(t, w);
                    w = x->parent->right;
                }
                w->c = x->parent->c;
                x->parent->c = BLACK;
                w->right->c = BLACK;
                right_rotate_m(t, x->parent);
                x = t->root;
            }
        }
    }
    x->c = BLACK;
}

void elimina_ingredienti(ingrediente_ricetta** ingr){

ingrediente_ricetta *curr = *ingr;
    ingrediente_ricetta *next = NULL;

    while (curr != NULL) {

        next = curr->next;
        free(curr);
        curr = next;
    }
    *ingr = NULL;
    return;
}
void elimina_lotti(node_lotto** lot){

node_lotto *curr = *lot;
    node_lotto *next = NULL;

    while (curr != NULL) {

        next = curr->next;
        free(curr);
        curr = next;
    }
    *lot = NULL;
    return;
}
