//Projekt 3 - Jednoducha shlukova analyza
//Tom Vykypel - xvykyp02

/**
 * Kostra programu pro 3. projekt IZP 2018/19
 *
 * Jednoducha shlukova analyza: 2D nejblizsi soused.
 * Single linkage
 */
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h> // sqrtf
#include <limits.h> // INT_MAX

/*****************************************************************
 * Ladici makra. Vypnout jejich efekt lze definici makra
 * NDEBUG, napr.:
 *   a) pri prekladu argumentem prekladaci -DNDEBUG
 *   b) v souboru (na radek pred #include <assert.h>
 *      #define NDEBUG
 */
#ifdef NDEBUG
#define debug(s)
#define dfmt(s, ...)
#define dint(i)
#define dfloat(f)
#else

// vypise ladici retezec
#define debug(s) printf("- %s\n", s)

// vypise formatovany ladici vystup - pouziti podobne jako printf
#define dfmt(s, ...) printf(" - "__FILE__":%u: "s"\n",__LINE__,__VA_ARGS__)

// vypise ladici informaci o promenne - pouziti dint(identifikator_promenne)
#define dint(i) printf(" - " __FILE__ ":%u: " #i " = %d\n", __LINE__, i)

// vypise ladici informaci o promenne typu float - pouziti
// dfloat(identifikator_promenne)
#define dfloat(f) printf(" - " __FILE__ ":%u: " #f " = %g\n", __LINE__, f)

#endif

/*****************************************************************
 * Deklarace potrebnych datovych typu:
 *
 * TYTO DEKLARACE NEMENTE
 *
 *   struct obj_t - struktura objektu: identifikator a souradnice
 *   struct cluster_t - shluk objektu:
 *      pocet objektu ve shluku,
 *      kapacita shluku (pocet objektu, pro ktere je rezervovano
 *          misto v poli),
 *      ukazatel na pole shluku.
 */

struct obj_t {
    int id;
    float x;
    float y;
};

struct cluster_t {
    int size;
    int capacity;
    struct obj_t *obj;
};

/*****************************************************************
 * Deklarace potrebnych funkci.
 *
 * PROTOTYPY FUNKCI NEMENTE
 *
 * IMPLEMENTUJTE POUZE FUNKCE NA MISTECH OZNACENYCH 'TODO'
 *
 */

/*
 Inicializace shluku 'c'. Alokuje pamet pro cap objektu (kapacitu).
 Ukazatel NULL u pole objektu znamena kapacitu 0.
*/
void init_cluster(struct cluster_t *c, int cap)
{
    assert(c != NULL);
    assert(cap >= 0);

    // TODO

    c->size = 0;
    if(cap > 0){
        //alokace pameti pro pole objektu
        if((c->obj = malloc(cap * sizeof(struct obj_t)))){
            c->capacity = cap;
            return;
        }
    }
    c->capacity = 0;
    c->obj = 0;
}

/*
 Odstraneni vsech objektu shluku a inicializace na prazdny shluk.
 */
void clear_cluster(struct cluster_t *c)
{
    // TODO

    assert(c);
    //uvolneni pameti pole objektu
    free(c->obj);
    //inicializace na prazdny shluk
    init_cluster(c, 0);
}

/// Chunk of cluster objects. Value recommended for reallocation.
const int CLUSTER_CHUNK = 10;

/*
 Zmena kapacity shluku 'c' na kapacitu 'new_cap'.
 */
struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap)
{
    // TUTO FUNKCI NEMENTE
    assert(c);
    assert(c->capacity >= 0);
    assert(new_cap >= 0);

    if (c->capacity >= new_cap)
        return c;

    size_t size = sizeof(struct obj_t) * new_cap;

    void *arr = realloc(c->obj, size);
    if (arr == NULL)
        return NULL;

    c->obj = (struct obj_t*)arr;
    c->capacity = new_cap;
    return c;
}

/*
 Prida objekt 'obj' na konec shluku 'c'. Rozsiri shluk, pokud se do nej objekt
 nevejde.
 */
void append_cluster(struct cluster_t *c, struct obj_t obj)
{
    // TODO

    assert(c);
    assert(c->size >= 0);
    assert(c->obj);

    if(c->capacity <= c->size){
        //rozsireni kapacity shluku
        if(!resize_cluster(c, c->capacity + CLUSTER_CHUNK)){
            return;
        }
    }
    //pridani objektu 'obj' na konec shluku 'c' a zvyseni poctu objektu
    c->obj[c->size] = obj;
    c->size++;
}

/*
 Seradi objekty ve shluku 'c' vzestupne podle jejich identifikacniho cisla.
 */
void sort_cluster(struct cluster_t *c);

/*
 Do shluku 'c1' prida objekty 'c2'. Shluk 'c1' bude v pripade nutnosti rozsiren.
 Objekty ve shluku 'c1' budou serazeny vzestupne podle identifikacniho cisla.
 Shluk 'c2' bude nezmenen.
 */
void merge_clusters(struct cluster_t *c1, struct cluster_t *c2)
{
    assert(c1 != NULL);
    assert(c2 != NULL);

    // TODO

    int i;
    //pridani objektu ze shluku 'c2' do shluku 'c1'
    for(i = 0; i < c2->size; i++){
        append_cluster(c1, c2->obj[i]);
    }
    //serazeni objektu ve shluku 'c1' po pridani objektu shluku 'c2'
    sort_cluster(c1);
}

/**********************************************************************/
/* Prace s polem shluku */

/*
 Odstrani shluk z pole shluku 'carr'. Pole shluku obsahuje 'narr' polozek
 (shluku). Shluk pro odstraneni se nachazi na indexu 'idx'. Funkce vraci novy
 pocet shluku v poli.
*/
int remove_cluster(struct cluster_t *carr, int narr, int idx)
{
    assert(idx < narr);
    assert(narr > 0);

    // TODO
    //nova velikost bude o 1 mensi kvuli odstraneni pozadovaneho shluku z pole 'carr'
    int novy_narr = narr - 1, i;
    //uvolneni pameti pro shluk 'idx'
    clear_cluster(&carr[idx]);
    //posunuti shluku v poli 'carr' od 'idx' doleva
    for(i = idx; i < novy_narr; i++){
        carr[i] = carr[i + 1];
    }
    return novy_narr;
}

/*
 Pocita Euklidovskou vzdalenost mezi dvema objekty.
 */
float obj_distance(struct obj_t *o1, struct obj_t *o2)
{
    assert(o1 != NULL);
    assert(o2 != NULL);

    // TODO

    return sqrtf(powf(o1->x - o2->x, 2.0) + powf(o1->y - o2->y, 2.0));
}

/*
 Pocita vzdalenost dvou shluku.
*/
float cluster_distance(struct cluster_t *c1, struct cluster_t *c2)
{
    assert(c1 != NULL);
    assert(c1->size > 0);
    assert(c1->obj);
    assert(c2 != NULL);
    assert(c2->size > 0);
    assert(c2->obj);

    // TODO

    int i, j;
    float vzdalenost, min_vzdalenost = 1500.0;
    for(i = 0; i < c1->size; i++){
        for(j = 0; j < c2->size; j++){
            //vypocet vzdalenosti
            vzdalenost = obj_distance(&c1->obj[i], &c2->obj[j]);
            //hledani minimalni vzdalenosti
            if(vzdalenost < min_vzdalenost){
                min_vzdalenost = vzdalenost;
            }
        }
    }
    return min_vzdalenost;
}

/*
 Funkce najde dva nejblizsi shluky. V poli shluku 'carr' o velikosti 'narr'
 hleda dva nejblizsi shluky. Nalezene shluky identifikuje jejich indexy v poli
 'carr'. Funkce nalezene shluky (indexy do pole 'carr') uklada do pameti na
 adresu 'c1' resp. 'c2'.
*/
void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2)
{
    assert(narr > 0);
    assert(carr);
    assert(c1);
    assert(c2);

    // TODO

    //pokud je v poli pouze jeden shluk
    if (narr == 1){
		*c1 = *c2 = 0;
		return;
	}
    int i, j;
    float vzdalenost, min_vzdalenost = 1500.0;
	for (i = 0; i < narr - 1; i++) {
		for (j = i + 1; j < narr; j++){
            //pocitani vzdalenosti
			vzdalenost = cluster_distance(&carr[i], &carr[j]);
			//hledani minimalni vzdalenosti
            if (vzdalenost < min_vzdalenost){
                min_vzdalenost = vzdalenost;
				*c1 = i;
				*c2 = j;
			}
		}
	}
}

// pomocna funkce pro razeni shluku
static int obj_sort_compar(const void *a, const void *b)
{
    // TUTO FUNKCI NEMENTE
    const struct obj_t *o1 = (const struct obj_t *)a;
    const struct obj_t *o2 = (const struct obj_t *)b;
    if (o1->id < o2->id) return -1;
    if (o1->id > o2->id) return 1;
    return 0;
}

/*
 Razeni objektu ve shluku vzestupne podle jejich identifikatoru.
*/
void sort_cluster(struct cluster_t *c)
{
    // TUTO FUNKCI NEMENTE
    qsort(c->obj, c->size, sizeof(struct obj_t), &obj_sort_compar);
}

/*
 Tisk shluku 'c' na stdout.
*/
void print_cluster(struct cluster_t *c)
{
    // TUTO FUNKCI NEMENTE
    for (int i = 0; i < c->size; i++)
    {
        if (i) putchar(' ');
        printf("%d[%g,%g]", c->obj[i].id, c->obj[i].x, c->obj[i].y);
    }
    putchar('\n');
}

/*
 Ze souboru 'filename' nacte objekty. Pro kazdy objekt vytvori shluk a ulozi
 jej do pole shluku. Alokuje prostor pro pole vsech shluku a ukazatel na prvni
 polozku pole (ukazatel na prvni shluk v alokovanem poli) ulozi do pameti,
 kam se odkazuje parametr 'arr'. Funkce vraci pocet nactenych objektu (shluku).
 V pripade nejake chyby uklada do pameti, kam se odkazuje 'arr', hodnotu NULL.
*/
int load_clusters(char *filename, struct cluster_t **arr)
{
    assert(arr != NULL);
    struct obj_t objekt;
    // TODO

    //otevreni souboru s objekty
    FILE *file = fopen(filename, "r");
    //kontrola zda se dany soubor da otevrit nebo zda existuje
    if(!file){
        printf("\nSoubor %s se nepodarilo otevrit.\n", filename);
        return 0;
    }
    const int MAX_delka_radku = 1024;
    int pocet_shluku, i;
    char radek[MAX_delka_radku], *p; 
    fgets(radek, MAX_delka_radku, file);
    //Zjisteni poctu shluku, +6 je zde proto, aby bylo prevedeno cislo az za slovem count=
    pocet_shluku = strtol(radek + 6, &p, 10);
    if(pocet_shluku <= 0){
        printf("\nPocet shluku musi byt vetsi nez 0.\n");
        fclose(file);
        return 0;
    }
    //alokovani pameti pro shluky
    *arr = malloc(pocet_shluku * sizeof(struct cluster_t));
    if(!(*arr)){
        printf("\nChyba pri alokovani pameti\n");
        fclose(file);
        return 0;
    }
    for (i = 0; i < pocet_shluku; i++){
        //inicializace pole shluku
        init_cluster(&(*arr)[i], CLUSTER_CHUNK);
        //kontrolovani zda v souboru s daty je ID, X a Y cisla jednotlivych shluku
        if(fscanf(file, "%d %f %f\n", &objekt.id, &objekt.x, &objekt.y) != 3){
            printf("\nV souboru s daty chybi bud ID, X nebo Y.\n");
            return -1;
        }
        //kontrolovani, zda cisla X a Y neprekracuji dane rozmezi
        if(objekt.x < 0 || objekt.x > 1000 || objekt.y < 0 || objekt.y > 1000){
            printf("\nCislo X nebo Y se nenachazi v rozmezi 0 az 1000.\n");
            return -1;
        }
        append_cluster(&(*arr)[i], objekt);
    }
    fclose(file);
    return pocet_shluku;
}

/*
 Tisk pole shluku. Parametr 'carr' je ukazatel na prvni polozku (shluk).
 Tiskne se prvnich 'narr' shluku.
*/
void print_clusters(struct cluster_t *carr, int narr)
{
    printf("Clusters:\n");
    for (int i = 0; i < narr; i++)
    {
        printf("cluster %d: ", i);
        print_cluster(&carr[i]);
    }
}

int main(int argc, char *argv[])
{
    // TODO

    struct cluster_t *clusters;
    int i = load_clusters(argv[1], &clusters);
    int j, pocet_shluku, c1, c2;
    //pokud v load clusteru nastala chyba, uvolni se misto v pameti
    if(i == -1){
        for(j = 0; j < i; j++){
        clear_cluster((&clusters[j]));
        }
        free(clusters);
        return 0;
    }
    //kontrola argumentu
    if(argc != 2 && argc != 3){
        printf("\nArgumenty byly spatne zadany.\n");
        printf("\nPro spusteni programu napiste: ./proj3 SOUBOR [N]   SOUBOR = jmeno souboru se vstupnimi daty  [N] = volitelny argument definujici cilovy pocet shluku N > 0.");
        return 1;
    }
    //kontrola argumentu, pokud chybi argument [N], nastavi hodnotu na 1
    else if(argc == 2){
        pocet_shluku = 1;
    }
    else{
        char *p = NULL;
        pocet_shluku = strtol(argv[2], &p, 10);
        //kontrola, zda se na miste argumentu [N] nenachazi znak
        if(*p){
            printf("\nArgument N musi byt cislo, ale argument obsahuje: %s.\n", p);
            printf("\nPro spusteni programu napiste: ./proj3 SOUBOR [N]   SOUBOR = jmeno souboru se vstupnimi daty  [N] = volitelny argument definujici cilovy pocet shluku N > 0.\n");
            return 1;
        }
        //kontrola, zda pocet shluku v souboru neni mensi, nez argument [N]
        else if(pocet_shluku > i){
            printf("\nArgument N je vetsi, nez pocet shluku v souboru: %s\n", argv[1]);
            return -1;
        }
        //kontrola, zda argument [N] neni mensi nez 0
        else if(pocet_shluku <= 0){
            printf("\nArgument N musi byt cislo vetsi nez 0\n");
            printf("\nPro spusteni programu napiste: ./proj3 SOUBOR [N]   SOUBOR = jmeno souboru se vstupnimi daty  [N] = volitelny argument definujici cilovy pocet shluku N > 0\n.");
            return 1;
        }
    }
    //pokud bude argument [N] mensi, nez pocet shluku v souboru s daty, vykonaji si nasledujici funkce
    while(pocet_shluku < i){
        find_neighbours(clusters, i, &c1, &c2);
        merge_clusters(&(clusters[c1]), &(clusters[c2]));
        i = remove_cluster(clusters, i, c2);
    }
    print_clusters(clusters, pocet_shluku);
    for(j = 0; j < i; j++){
        clear_cluster((&clusters[j]));
    }
    free(clusters);
    return 0;
}
