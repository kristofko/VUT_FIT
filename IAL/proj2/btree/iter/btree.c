btree/rec/btree.c                                                                                   0000777 0001750 0001750 00000013577 14140563506 013360  0                                                                                                    ustar   Kuto                            adduser                                                                                                                                                                                                                /*
 * Binárny vyhľadávací strom — rekurzívna varianta
 *
 * S využitím dátových typov zo súboru btree.h a pripravených kostier funkcií
 * implementujte binárny vyhľadávací strom pomocou rekurzie.
 */

#include "../btree.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Inicializácia stromu.
 *
 * Užívateľ musí zaistiť, že incializácia sa nebude opakovane volať nad
 * inicializovaným stromom. V opačnom prípade môže dôjsť k úniku pamäte (memory
 * leak). Keďže neinicializovaný ukazovateľ má nedefinovanú hodnotu, nie je
 * možné toto detegovať vo funkcii.
 */
void bst_init(bst_node_t **tree) {
	*tree = NULL;
}
/*
 * Nájdenie uzlu v strome.
 *
 * V prípade úspechu vráti funkcia hodnotu true a do premennej value zapíše
 * hodnotu daného uzlu. V opačnom prípade funckia vráti hodnotu false a premenná
 * value ostáva nezmenená.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
bool bst_search(bst_node_t *tree, char key, int *value) {
	if (tree != NULL){
		if (key == tree->key){
			*value = tree->value;
			return true;
		}
		return bst_search(tree->left, key, value);
		return bst_search(tree->right, key, value);
	}

	return false;
}

/*
 * Vloženie uzlu do stromu.
 *
 * Pokiaľ uzol so zadaným kľúčom v strome už existuje, nahraďte jeho hodnotu.
 * Inak vložte nový listový uzol.
 *
 * Výsledný strom musí spĺňať podmienku vyhľadávacieho stromu — ľavý podstrom
 * uzlu obsahuje iba menšie kľúče, pravý väčšie.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_insert(bst_node_t **tree, char key, int value) {
	// Insert the node
	if ( *tree == NULL ) {
		(*tree) = malloc(sizeof(bst_node_t));
		if ( (*tree) == NULL ){
			exit(-1);
		}

		(*tree)->key = key;
		(*tree)->value = value;
		(*tree)->right = NULL;
		(*tree)->left = NULL;
		return;
	} 

	if( (*tree)->key == key ){
		(*tree)->value = value;
		return;
	}
	else if ( (*tree)->key > key ) {
		bst_insert( &(*tree)->left, key, value );
	}
	else if ( (*tree)-> key < key ) {
		bst_insert( &(*tree)->right, key, value );
	}
	return;
}

/*
 * Pomocná funkcia ktorá nahradí uzol najpravejším potomkom.
 *
 * Kľúč a hodnota uzlu target budú nahradené kľúčom a hodnotou najpravejšieho
 * uzlu podstromu tree. Najpravejší potomok bude odstránený. Funkcia korektne
 * uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkcia predpokladá že hodnota tree nie je NULL.
 *
 * Táto pomocná funkcia bude využitá pri implementácii funkcie bst_delete.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_replace_by_rightmost(bst_node_t *target, bst_node_t **tree) {
	bst_node_t *temp;
	if ( (*tree)->right == NULL ) {
		temp = *tree;
		target->key = (*tree)->key;
		target->value = (*tree)->value;
		if ( (*tree)->left != NULL ){
			temp = (*tree)->left;
			free(*tree);
			*tree = temp; 
		}
		else {
			free(*tree);
			*tree = NULL;
		}
	}
	else {
		bst_replace_by_rightmost(target, &(*tree)->right);
	}
	return;
}

/*
 * Odstránenie uzlu v strome.
 *
 * Pokiaľ uzol so zadaným kľúčom neexistuje, funkcia nič nerobí.
 * Pokiaľ má odstránený uzol jeden podstrom, zdedí ho otec odstráneného uzla.
 * Pokiaľ má odstránený uzol oba podstromy, je nahradený najpravejším uzlom
 * ľavého podstromu. Najpravejší uzol nemusí byť listom!
 * Funkcia korektne uvoľní všetky alokované zdroje odstráneného uzlu.
 *
 * Funkciu implementujte rekurzívne pomocou bst_replace_by_rightmost a bez
 * použitia vlastných pomocných funkcií.
 */
void bst_delete(bst_node_t **tree, char key) {
	bst_node_t *temp;
	if ( *tree != NULL){
		if ( (*tree)->key > key ) bst_delete ( &(*tree)->left, key);
		else if ( (*tree)->key < key ) bst_delete ( &(*tree)->right, key);
		else if ( (*tree)->key == key ) {
			if ( (*tree)->left == NULL && (*tree)->right == NULL ){
				// We found leaf, so we can just delete it
				free(*tree);
				(*tree) = NULL;
			}

			else if ( (*tree)->left != NULL && (*tree)->right == NULL ){
				// Only has one subtree
				temp = (*tree)->left;
				free(*tree);
				(*tree) = temp;
			}

			else if ( (*tree)->left == NULL && (*tree)->right != NULL ){
				temp = (*tree)->right;
				free(*tree);
				(*tree) = temp;
			}

			else {
				// Tree has both subtrees
				bst_replace_by_rightmost(*tree, &(*tree)->left);
			}
		}
	}
}

/*
 * Zrušenie celého stromu.
 *
 * Po zrušení sa celý strom bude nachádzať v rovnakom stave ako po
 * inicializácii. Funkcia korektne uvoľní všetky alokované zdroje rušených
 * uzlov.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_dispose(bst_node_t **tree) {
	if (*tree != NULL){
		bst_dispose(&(*tree)->left);
		bst_dispose(&(*tree)->right);
		free(*tree);
	}

	*tree = NULL;
	return;
}

/*
 * Preorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_preorder(bst_node_t *tree) {
	if (tree != NULL){
		bst_print_node(tree);
		bst_preorder(tree->left);
		bst_preorder(tree->right);
	}
	return;
}

/*
 * Inorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_inorder(bst_node_t *tree) {
	if (tree != NULL){
		bst_inorder(tree->left);
		bst_print_node(tree);
		bst_inorder(tree->right);
	}
	return;
}
/*
 * Postorder prechod stromom.
 *
 * Pre aktuálne spracovávaný uzol nad ním zavolajte funkciu bst_print_node.
 *
 * Funkciu implementujte rekurzívne bez použitia vlastných pomocných funkcií.
 */
void bst_postorder(bst_node_t *tree) {
	if (tree != NULL){
		bst_postorder(tree->left);
		bst_postorder(tree->right);
		bst_print_node(tree);
	}
	return;
}
                                                                                                                                 hashtable/hashtable.c                                                                               0000777 0001750 0001750 00000010043 14143734405 014255  0                                                                                                    ustar   Kuto                            adduser                                                                                                                                                                                                                /*
 * Tabuľka s rozptýlenými položkami
 *
 * S využitím dátových typov zo súboru hashtable.h a pripravených kostier
 * funkcií implementujte tabuľku s rozptýlenými položkami s explicitne
 * zreťazenými synonymami.
 *
 * Pri implementácii uvažujte veľkosť tabuľky HT_SIZE.
 */

#include "hashtable.h"
#include <stdlib.h>
#include <string.h>

int HT_SIZE = MAX_HT_SIZE;

/*
 * Rozptyľovacia funkcia ktorá pridelí zadanému kľúču index z intervalu
 * <0,HT_SIZE-1>. Ideálna rozptyľovacia funkcia by mala rozprestrieť kľúče
 * rovnomerne po všetkých indexoch. Zamyslite sa nad kvalitou zvolenej funkcie.
 */
int get_hash(char *key) {
  int result = 1;
  int length = strlen(key);
  for (int i = 0; i < length; i++) {
    result += key[i];
  }
  return (result % HT_SIZE);
}

/*
 * Inicializácia tabuľky — zavolá sa pred prvým použitím tabuľky.
 */
void ht_init(ht_table_t *table) {

	if (table == NULL) {
		exit(-1);					// Check if we can work with the table
	}
	for (int i = 0; i < HT_SIZE; i++){
		(*table)[i] = NULL;			// Set all items to NULL
	}
	return ;
	
}

/*
 * Vyhľadanie prvku v tabuľke.
 *
 * V prípade úspechu vráti ukazovateľ na nájdený prvok; v opačnom prípade vráti
 * hodnotu NULL.
 */
ht_item_t *ht_search(ht_table_t *table, char *key) {
  int hash = get_hash(key);
  ht_item_t *item_ptr = (*table)[hash];
  while (item_ptr != NULL){
    if (!strcmp(item_ptr->key, key)){	// Check for colliding hashes
      return item_ptr;
    }

    item_ptr = item_ptr->next;
  }
  
  return NULL;
}

/*
 * Vloženie nového prvku do tabuľky.
 *
 * Pokiaľ prvok s daným kľúčom už v tabuľke existuje, nahraďte jeho hodnotu.
 *
 * Pri implementácii využite funkciu ht_search. Pri vkladaní prvku do zoznamu
 * synonym zvoľte najefektívnejšiu možnosť a vložte prvok na začiatok zoznamu.
 */
void ht_insert(ht_table_t *table, char *key, float value) {
	int hash = get_hash(key);
	ht_item_t *item_ptr = ht_search(table, key);
	if ( item_ptr == NULL ){
		// We didnt fint the item, so we are inserting it 
		item_ptr = malloc (sizeof(ht_item_t));
		if (item_ptr == NULL){
			exit(-1);
		}
		item_ptr->key = key;
		item_ptr->value = value;
		if ( (*table)[hash] == NULL ){
			item_ptr->next = NULL;
		}
		else {
			item_ptr->next = (*table)[hash];
		}
		(*table)[hash] = item_ptr;
	}

	else {
		// Item was found, so we update its value
		item_ptr->value = value;
	}
	return;
}

/*
 * Získanie hodnoty z tabuľky.
 *
 * V prípade úspechu vráti funkcia ukazovateľ na hodnotu prvku, v opačnom
 * prípade hodnotu NULL.
 *
 * Pri implementácii využite funkciu ht_search.
 */
float *ht_get(ht_table_t *table, char *key) {
  ht_item_t *item_ptr = ht_search(table, key);
  if (item_ptr == NULL){
	  // Item wasnt found
	  return NULL;
  }
  else {
	  // Found the item and returning pointer to its value
	  return &(item_ptr->value);
  }

}

/*
 * Zmazanie prvku z tabuľky.
 *
 * Funkcia korektne uvoľní všetky alokované zdroje priradené k danému prvku.
 * Pokiaľ prvok neexistuje, nerobte nič.
 *
 * Pri implementácii NEVYUŽÍVAJTE funkciu ht_search.
 */
void ht_delete(ht_table_t *table, char *key) {
	int hash = get_hash(key);
	ht_item_t *item_ptr = (*table)[hash];
	ht_item_t *previous_ptr = (*table)[hash];			// Pointer to the previous item
	while (item_ptr != NULL) {
		if (!strcmp(item_ptr->key, key)){
			previous_ptr->next = item_ptr->next;
			free(item_ptr);
			break;
		}
		previous_ptr = item_ptr;					// Bind the previous item to the 2nd next item
		item_ptr = item_ptr->next;
	}

	return;

}

/*
 * Zmazanie všetkých prvkov z tabuľky.
 *
 * Funkcia korektne uvoľní všetky alokované zdroje a uvedie tabuľku do stavu po
 * inicializácii.
 */
void ht_delete_all(ht_table_t *table) {
	ht_item_t *item_ptr = NULL;
	ht_item_t *next_item  = NULL;  
	for (int i = 0; i < HT_SIZE; i++){   // Looping through table
		item_ptr = (*table)[i];
		while (item_ptr != NULL){
			next_item = item_ptr->next;  // Dont lose the pointer to next item
			free(item_ptr);
			item_ptr = next_item;
		} // while
	} // for

	ht_init(table);

	return;
}
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                             