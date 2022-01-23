/*
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
