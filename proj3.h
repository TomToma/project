/**
 * Kostra hlavickoveho souboru 3. projekt IZP 2018/19
 * a pro dokumentaci Javadoc.
 */

/**
* @mainpage Projekt 3 - Jednoducha shlukova analyza
* @link proj3.h @endlink
* 
* @file proj3.h
* @brief Projekt 3 - Jednoducha shlukova analyza
* @author Tom Vykypel - xvykyp02
* @date Prosinec 2018
* @version 1.0
*/

/**
* @brief Struktura pro objekt s urcitymi souradnicemi
*/

struct obj_t {
    /**identifikacni cislo*/
    int id;
    /**souradnice x*/
    float x;
    /**souradnice y*/
    float y;
};

/**
* @brief Struktura pro shluk objektu
*/

struct cluster_t {
    /**pocet objektu ve shluku*/
    int size;
    /**kapacita shluku*/
    int capacity;
    /**pole objektu*/
    struct obj_t *obj;
};

/**
 * @defgroup clusters Prace se shluky
 * @{
 */

/**
 * @brief Inicializace shluku 'c'. Alokuje pamet pro cap objektu (kapacitu).
 * Ukazatel NULL u pole objektu znamena kapacitu 0.
 * 
 * @post
 * Shluk 'c' bude mit alokovanou pamet pro 'cap' objektu a bude inicializovan
 *
 * @param c shluk pro inicializaci
 * @param cap pozadovana kapacita shluku
 */
void init_cluster(struct cluster_t *c, int cap);

/**
 * @brief Odstraneni vsech objektu shluku a inicializace na prazdny shluk.
 *
 * @post
 * Alokovana pamet pro vsechny objektu shluku 'c' bude uvolnena.
 *
 * @param c shluk, ktery ma byt odstranen
 */

void clear_cluster(struct cluster_t *c);

/**
 * Chunk of cluster objects. Value recommended for reallocation.
 */
 
extern const int CLUSTER_CHUNK;

/**
 * @brief Zmena kapacity shluku 'c' na kapacitu 'new_cap'.
 * 
 * @pre
 * Kapacita shluku 'c' musi byt vetsi nebo rovna 0.
 *
 * @post
 * Kapacita shluku 'c' bude zmenena na 'new_cap', pokud nastane chyba pri alokaci, bude kapacita NULL.
 *
 * @param c shluk, kteremu se zmeni kapacita
 * @param new_cap nova kapacita
 *
 * @return shluk 'c' s novou kapacitou
 */

struct cluster_t *resize_cluster(struct cluster_t *c, int new_cap);

/**
 * @brief Prida objekt 'obj' na konec shluku 'c'. Rozsiri shluk, pokud se do nej objekt nevejde.
 * 
 * @post
 * Objekt bude pridan na konec shluku, pokud se objekt do shluku nevejde, bude rozsiren o CLUSTER_CHUNK.
 *
 * @param c shluk, kam se ma objekt pridat
 * @param obj objekt, ktery bude do shluku 'c' pridan
 */

void append_cluster(struct cluster_t *c, struct obj_t obj);

/**
 * @brief Do shluku 'c1' prida objekty 'c2'. Shluk 'c1' bude v pripade nutnosti rozsiren.
 * Objekty ve shluku 'c1' budou serazeny vzestupne podle identifikacniho cisla.
 * Shluk 'c2' bude nezmenen.
 *
 * @post
 * Shluk 'c1' bude rozsiren o objekty shluku 'c2'.
 * Objekty ve shluku 'c1' budou serazeny vzestupne podle jejich identifikacniho cisla.
 *
 * @param c1 shluk, do ktereho budou pridany objektu ze shluku 'c2'
 * @param c2 shluk, jehoz objekty budou pridany do shluku 'c1'
 */

void merge_clusters(struct cluster_t *c1, struct cluster_t *c2);

/**
 * @brief Odstrani shluk z pole shluku 'carr'.
 *
 * @post Z pole shluku 'carr' odstrani shluk na indexu 'idx'
 *
 * @param carr pole shluku
 * @param narr pocet shluku v poli
 * @param idx index shluku pro odstraneni
 *
 * @return novy pocet shluku v poli (pocet shluku bude o jedno mensi)
 */

int remove_cluster(struct cluster_t *carr, int narr, int idx);

/**
 * @brief Pocita Euklidovskou vzdalenost mezi dvema objekty.
 *
 * @param o1 objekt 1
 * @param o2 objekt 2
 * 
 * @return Euklidovskou vzdalenost mezi objekty 'o1' a 'o2'
 */

float obj_distance(struct obj_t *o1, struct obj_t *o2);

/**
 * @brief Pocita vzdalenost dvou shluku
 *
 * @pre
 * pocet objektu ve shluku 'c1' a 'c2' bude vetsi nez 0
 *
 * @param c1 prvni shluk
 * @param c2 druhy shluk
 *
 * @return vzdalenost shluku 'c2' a 'c2'
 */

float cluster_distance(struct cluster_t *c1, struct cluster_t *c2);

/**
 * @brief Funkce najde dva nejblizsi shluky v poli shluku 'carr'.
 * Indexy nalezenych shluku v poli shluku 'carr' uklada do pameti na adresu 'c1' a 'c2'.
 *
 * @post
 * Indexy dvou nejblizsich shluku budou ulozeny do pameti na adrese 'c1' a 'c2'.
 *
 * @param carr pole shluku
 * @param narr pocet shluku v poli
 * @param c1 index jednoho z nalezenych shluku
 * @param c2 index druheho z nalezenuch shluku
 */

void find_neighbours(struct cluster_t *carr, int narr, int *c1, int *c2);

/**
 * @brief Razeni objektu ve shluku vzestupne podle jejich identifikatoru.
 *
 * @post
 * Objekty ve shluku 'c' budou vzestupne serazeny podle jejich identifikatoru
 *
 * @param c shluk pro serazeni
 */

void sort_cluster(struct cluster_t *c);

/**
 * @brief Tisk shluku na stdout
 *
 * @post
 * Objekty shluku 'c' budou vypsany na stdout
 *
 * @param c shluk pro tisk
 */

void print_cluster(struct cluster_t *c);

/**
 * @brief Ze souboru 'filename' nacte objekty. Pro kazdy objekt vytvori shluk a ulozi
 * jej do pole shluku. Alokuje prostor pro pole vsech shluku a ukazatel na prvni
 * polozku pole (ukazatel na prvni shluk v alokovanem poli) ulozi do pameti,
 * kam se odkazuje parametr 'arr'. Funkce vraci pocet nactenych objektu (shluku).
 *
 * @pre
 * Bude existovat soubor pro cteni 'filename' a data v souboru budou ve spravnem formatu
 *
 * @post
 * Pro kazdy nacteny objekt vytvori shluk, tyto shluky budou ulozeny v poli shluku 'arr',
 * pro ktere alokuje pamet podle poctu shluku uvedenych ve vstupnim souboru
 *
 * @filename nazev souboru pro nacteni objektu
 * @param arr ukazatel na pole shluku nactenych ze souboru
 * 
 * @return pocet nactenych shluku, v pripade chyby bude vracet -1
 */

int load_clusters(char *filename, struct cluster_t **arr)

/**
 * @brief Tisk pole shluku na stdout
 *
 * @post
 * Objekty vsech shluku v poli shluku 'carr' se vypisi na stdout
 *
 * @param carr pole shluku pro tisk
 * @param narr pocet shluku v poli
 */
void print_clusters(struct cluster_t *carr, int narr);

/**
 * @}
 */