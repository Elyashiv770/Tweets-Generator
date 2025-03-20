#include <stdio.h>
#include "markov_chain_ex3a.h"
#include "string.h"
#include "stdlib.h"

#define IS_NOT_ON_LIST -1

/**
* Check if data_ptr is in database. If so, return the Node wrapping it in
 * the markov_chain, otherwise return NULL.
 * @param markov_chain the chain to look in its database
 * @param data_ptr the data to look for
 * @return Pointer to the Node wrapping given data, NULL if state not in
 * database.
 */
Node* get_node_from_database(MarkovChain *markov_chain, char *data_ptr)
{
  Node *node_in_database = markov_chain->database->first;
  // Check all the nodes in database
  while (node_in_database != NULL)
  {
    if(strcmp(node_in_database->data->data, data_ptr) == 0)
    {
      return node_in_database;
    }
    // In case it is not in first, first will be next
    else
    {
      node_in_database = node_in_database->next;
    }
  }
  // In case the word is not in database at all
  return NULL;
}

/**
* If data_ptr in markov_chain, return it's node. Otherwise, create new
 * node, add to end of markov_chain's database and return it.
 * @param markov_chain the chain to look in its database
 * @param data_ptr the data to look for
 * @return Node wrapping given data_ptr in given chain's database,
 * returns NULL in case of memory allocation failure.
 */
Node* add_to_database(MarkovChain *markov_chain, char *data_ptr)
{
  // Check if the node already exists in the database
  Node *existing_node = get_node_from_database(markov_chain, data_ptr);
  if (existing_node)
  {
    return existing_node;
  }

  // Allocate memory for a new MarkovNode
  MarkovNode *markov_node = malloc(sizeof(MarkovNode));
  if (markov_node == NULL)
  {
    // Allocation failed
    return NULL;
  }

  // Allocate memory for the data (word)
  char *word = malloc(strlen(data_ptr) + 1);
  if (word == NULL)
  {
    // Allocation failed, free the MarkovNode memory
    free(markov_node);
    return NULL;
  }

  // Copy the data into the newly allocated memory
  strcpy(word, data_ptr);

  // Set the data field of the MarkovNode
  markov_node->data = word;

  // Add the MarkovNode to the linked list/database
  if (add(markov_chain->database, markov_node) != 0)
  {
    // If adding to the database failed, free allocated memory
    free(word);
    word = NULL;
    free(markov_node);
    markov_node = NULL;
    return NULL;
  }

  // Initialize the frequency list and other fields for the last node
  markov_chain->database->last->data->frequency_list_size = 0;
  markov_chain->database->last->data->total_of_frequency = 0;
  markov_chain->database->last->data->frequency_list = NULL;

  // Return the last node added to the database
  return markov_chain->database->last;
}


int is_node_in_frequency_list(MarkovNodeFrequency *list_frequency,int
frequency_list_size, char *word)
{
  for (int i = 0; i < frequency_list_size; i++)
  {
    if (strcmp(list_frequency[i].markov_node->data,word) == 0)
    {
      return i;
    }
  }
  return IS_NOT_ON_LIST;

}

/**
 * Add the second markov_node to the frequency list of the first markov_node.
 * If already in list, update it's occurrence frequency value.
 * @param first_node
 * @param second_node
 * @return success/failure: 0 if the process was successful, 1 if in
 * case of allocation error.
 */

int add_node_to_frequency_list(MarkovNode *first_node
    , MarkovNode *second_node)
{
  int index_in_frequency_list;
  index_in_frequency_list = is_node_in_frequency_list
      (first_node->frequency_list,first_node->frequency_list_size,
       second_node->data);
  // In case the word is already in list
  if (index_in_frequency_list != IS_NOT_ON_LIST)

  {
    // Increase the frequency in 1
    first_node->frequency_list[index_in_frequency_list].frequency+=1;

    // Increase the total frequencies in 1
    first_node->total_of_frequency+=1;
    return  0;
  }

  // In case the word is a new word
  else
  {
    MarkovNodeFrequency *temp =
        realloc (first_node->frequency_list,(first_node->frequency_list_size
        + 1) * (sizeof(MarkovNodeFrequency)));
    // In case failed to add
    if (temp == NULL)
    {
      return 1;
    }
    // In case it allocation went successfully
    else
    {

      // Change the pointer to the array to the new one
      first_node->frequency_list = temp;
      first_node->frequency_list_size+=1;

      // Initialing the last element to be the second node
      first_node->frequency_list[first_node->frequency_list_size-1]
      .markov_node = second_node;

      // Initialing frequency to 1
      first_node->frequency_list[first_node->frequency_list_size-1]
          .frequency = 1;

      // Increase the total frequencies in 1
      first_node->total_of_frequency+=1;
      return 0;
    }
  }
}

/**
 * Free markov_chain and all of it's content from memory
 * @param markov_chain markov_chain to free
 */
void free_database(MarkovChain ** ptr_chain)
{
  Node *node_to_free;
  Node *next_node_to_free;
  node_to_free = (*ptr_chain)->database->first;
  if(node_to_free)
  {
    while (node_to_free != NULL)
    {
      // Keep yhe value of the next node we want to delete
      next_node_to_free = node_to_free->next;
      // Free the word
      if(node_to_free->data->data)
      {
        free (node_to_free->data->data);
        node_to_free->data->data = NULL;
      }
      if(node_to_free->data->frequency_list)
      {
        // Free the frequency list
        free(node_to_free->data->frequency_list);
        node_to_free->data->frequency_list = NULL;
      }
      if(node_to_free->data)
      {
        // Free the markovnode
        free(node_to_free->data);
        node_to_free->data = NULL;
      }
      // Free node
      free (node_to_free);
      node_to_free = NULL;
      // update the node to be the next one
      node_to_free = next_node_to_free;
    }
  }
  // Free the linked list
  if((*ptr_chain)->database)
  {
    free((*ptr_chain)->database);
    (*ptr_chain)->database = NULL;
  }
  if(*ptr_chain)
  {
    // Free the markovchain
    free (*ptr_chain);
    *ptr_chain = NULL;
  }
}

/**
 *
 * @param markov_chain - pointer to database
 * @return Random word
 */

MarkovNode* get_node_by_index(MarkovChain *markov_chain, int index)
{
  Node *temp;
  temp = markov_chain->database->first;


  for(int i = 0; i < index; i++)
  {
    temp = temp->next;
  }
  return temp->data;
}

MarkovNode* get_first_random_node(MarkovChain *markov_chain)
{
  int i = 0;
  int flag = 1;
  int size_of_word = 0;

  MarkovNode  *random_word = NULL;
  while(flag)
  {
    // Get random number
    i = get_random_number (markov_chain->database->size);

    random_word = (get_node_by_index (markov_chain,i));
    size_of_word = strlen (random_word->data);
    if(random_word->data[size_of_word-1] != '.')
    {
      flag = 0;
    }
  }
  return random_word;
}

MarkovNode* get_next_random_node(MarkovNode *cur_markov_node)
{
  int i = 0;
  int max_number = 0;
  int in_range = 0;

  max_number = cur_markov_node->total_of_frequency;

  i = get_random_number (max_number);

  for (int j = 0; j < cur_markov_node->frequency_list_size; j++)
  {
    if(i >= in_range &&
       i < in_range + cur_markov_node->frequency_list[j].frequency)
    {
      return cur_markov_node->frequency_list[j].markov_node;
    }
    else
    {
      in_range = in_range + cur_markov_node->frequency_list[j].frequency;
    }
  }
  return NULL;
}

/**
 * Get random number between 0 and max_number [0, max_number).
 * @param max_number
 * @return Random number
 */
int get_random_number(int max_number)
{
    return rand() % max_number;
}


