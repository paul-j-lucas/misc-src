/*
**      PJL Library
**      src/hash_table.c
**
**      Copyright (C) 2025-2026  Paul J. Lucas
**
**      This program is free software: you can redistribute it and/or modify
**      it under the terms of the GNU General Public License as published by
**      the Free Software Foundation, either version 3 of the License, or
**      (at your option) any later version.
**
**      This program is distributed in the hope that it will be useful,
**      but WITHOUT ANY WARRANTY; without even the implied warranty of
**      MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**      GNU General Public License for more details.
**
**      You should have received a copy of the GNU General Public License
**      along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

// local
#include "hash_table.h"

// standard
#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#define ARRAY_SIZE(ARRAY)         (sizeof((ARRAY)) / sizeof((ARRAY)[0]))

////////// local constants ////////////////////////////////////////////////////

static unsigned const HT_PRIME[] = {
       53,      97,     193,      389,      769,
     1543,    3079,    6151,    12289,    24593,
    49157,   98317,  196613,   393241,   786433,
  1572869, 3145739, 6291469, 12582917, 25165843
};

////////// local functions ////////////////////////////////////////////////////

/**
 * Grows a hash table.
 *
 * @param ht The hash table to grow.
 */
static void ht_grow( hash_table_t *ht ) {
  assert( ht != NULL );

  unsigned const new_n_buckets = HT_PRIME[ ++ht->prime_idx ];
  ht_entry_t *const new_buckets = calloc( new_n_buckets, sizeof(ht_entry_t) );

  for ( unsigned b = 0; b < new_n_buckets; ++b ) {
    ht_hash_val_t const hash = ht->buckets[b].hash;
    for ( ht_entry_t *entry = ht->buckets[b].next, *next;
          entry != NULL; entry = next ) {
      ht_entry_t *const new_head = &new_buckets[ hash % new_n_buckets ];

      next = entry->next;
      entry->next = new_head->next;
      entry->prev = new_head;

      if ( new_head->next != NULL )
        new_head->next->prev = entry;
      new_head->next = entry;
      new_head->hash = hash;
    } // for
  } // for

  free( ht->buckets );
  ht->buckets = new_buckets;
}

////////// extern functions ///////////////////////////////////////////////////

void ht_cleanup( hash_table_t *ht, ht_free_fn_t free_fn ) {
  if ( ht == NULL )
    return;

  for ( unsigned b = 0; b < HT_PRIME[ ht->prime_idx ]; ++b ) {
    for ( ht_entry_t *entry = ht->buckets[b].next, *next;
          entry != NULL; entry = next ) {
      if ( free_fn != NULL )
        (*free_fn)( entry->data );
      next = entry->next;
      free( entry );
    }
  } // for

  free( ht->buckets );
  *ht = (hash_table_t){ 0 };
}

void ht_delete( hash_table_t *ht, ht_entry_t *entry ) {
  assert( ht != NULL );
  assert( entry != NULL );

  entry->prev->next = entry->next;
  if ( entry->next != NULL )
    entry->next->prev = entry->prev;
  free( entry );
  --ht->size;
}

ht_entry_t* ht_find( hash_table_t *ht, void const *key ) {
  assert( ht != NULL );
  assert( key != NULL );

  unsigned const b = (*ht->hash_fn)( key ) % HT_PRIME[ ht->prime_idx ];
  for ( ht_entry_t *entry = ht->buckets[b].next; entry != NULL;
        entry = entry->next ) {
    if ( (*ht->cmp_fn)( key, entry->data ) == 0 )
      return entry;
  } // for

  return NULL;
}

void ht_init( hash_table_t *ht, double max_lf, unsigned est_size,
              ht_cmp_fn_t cmp_fn, ht_hash_fn_t hash_fn ) {
  assert( ht != NULL );
  assert( max_lf > 0.0 );
  assert( cmp_fn != NULL );
  assert( hash_fn != NULL );

  unsigned prime_idx = 0;
  for ( ; prime_idx < ARRAY_SIZE( HT_PRIME ); ++prime_idx ) {
    if ( HT_PRIME[ prime_idx ] * max_lf >= est_size )
      break;
  } // for

  *ht = (hash_table_t){
    .buckets = calloc( HT_PRIME[ prime_idx ], sizeof(ht_entry_t) ),
    .cmp_fn = cmp_fn,
    .hash_fn = hash_fn,
    .max_lf = max_lf,
    .prime_idx = prime_idx
  };
}

ht_insert_rv_t ht_insert( hash_table_t *ht, void const *key,
                          size_t data_size ) {
  ht_hash_val_t const hash = (*ht->hash_fn)( key );
  unsigned const n_buckets = HT_PRIME[ ht->prime_idx ];
  unsigned const b = hash % n_buckets;
  ht_entry_t *const head = &ht->buckets[b], *entry;

  for ( entry = head->next; entry != NULL; entry = entry->next ) {
    if ( (*ht->cmp_fn)( key, entry->data ) == 0 )
      return (ht_insert_rv_t){ entry, .inserted = false };
  } // for

  entry = malloc( sizeof(ht_entry_t) + data_size );
  *entry = (ht_entry_t){ .next = head->next, .prev = head };
  if ( head->next != NULL )
    head->next->prev = entry;
  *head = (ht_entry_t){ .next = entry, .hash = hash };

  double const lf = ++ht->size / (double)n_buckets;
  if ( lf >= ht->max_lf )
    ht_grow( ht );

  return (ht_insert_rv_t){ entry, .inserted = true };
}

void ht_iter_init( ht_iter_t *it, hash_table_t *ht ) {
  assert( it != NULL );
  assert( ht != NULL );

  *it = (ht_iter_t){
    .ht = ht,
    .bucket_idx = (unsigned)-1,
    .n_buckets = HT_PRIME[ ht->prime_idx ]
  };
}

ht_entry_t* ht_next( ht_iter_t *it ) {
  assert( it != NULL );

  for (;;) {
    assert( it->n_buckets == HT_PRIME[it->ht->prime_idx] );
    if ( it->next != NULL ) {
      ht_entry_t *const entry = it->next;
      it->next = it->next->next;
      return entry;
    }
    if ( ++it->bucket_idx == it->n_buckets )
      return NULL;
    it->next = it->ht->buckets[ it->bucket_idx ].next;
  } // for
}

///////////////////////////////////////////////////////////////////////////////

/** @} */

/// @cond DOXYGEN_IGNORE

extern inline bool ht_empty( hash_table_t const* );

/// @endcond

/* vim:set et sw=2 ts=2: */
