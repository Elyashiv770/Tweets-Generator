
#include "stdio.h"
#include "markov_chain_ex3a.h"
#include "string.h"
#include "ctype.h"
#include <stdlib.h>

#define FILE_PATH_ERROR "Error: incorrect file path"
#define NUM_ARGS_ERROR "Usage: invalid number of arguments"

#define READ_ALL_WORDS -1
#define FOUR_ARGUMENTS 4
#define FIVE_ARGUMENTS 5
#define BASE_TEN 10
#define MAX_WORDS 20
#define MAX_LINE 1000
#define DELIMITERS " \n\t\r"

/**
* Read one line from the file and fill the database
 * @param fp - given pointer to the file
 * @param words_to_read - given integer, the number of word to read
 * @param markov_chain - given pointer to markovchain
 * @param flag - given pointer flag, to know when to stop read words
 * @param written_words - given pointer written_words , to know when to
 * stop read words
 * @param line - given line
 * database.
 * @return 0 in case of success, 1 otherwise
 */
int fill_database_one_line(int words_to_read, MarkovChain
*markov_chain, int *flag,int *written_words, char line[])
{
  char *current_word;
  Node *previous_node_word;
  Node *add_node;

  current_word = strtok (line, DELIMITERS);

  if(current_word == NULL)
  {
    return  0;
  }
  add_node = add_to_database (markov_chain,current_word);
  if(add_node == NULL)
  {
    return 1;
  }
  else
  {
    *written_words = *written_words + 1;
    current_word = strtok (NULL, DELIMITERS);
    previous_node_word = add_node;
  }
  while (current_word != NULL && *flag ==1)
  {
    add_node = add_to_database (markov_chain, current_word);
    if(add_node == NULL)
    {
      return 1;
    }
    if(add_node_to_frequency_list(previous_node_word->data,
                                   add_node->data ) == 1)
    {
      return 1;
    }
    *written_words = *written_words + 1;
    previous_node_word = add_node;
    if(words_to_read != READ_ALL_WORDS)
    {
      if (*written_words == words_to_read)
      {
        *flag = 0;
      }
    }
    current_word = strtok (NULL, DELIMITERS);
  }
  return 0;
}


/**
* Fill database from the given file
 * @param fp - given pointer to the file
 * @param words_to_read - given integer, the number of word to read
 * @param markov_chain - given pointer to markovchain
 * @return 0 in case of success, 1 otherwise
 */
int fill_database(FILE *fp, int words_to_read, MarkovChain *markov_chain)
{

  char line[MAX_LINE];
  int written_words = 0;
  int flag = 1;

// Read line from file
  while (fgets (line, MAX_LINE, fp) != NULL && flag)
  {
    // Read line
    line[strcspn (line, "\n")] = '\0';

    if(fill_database_one_line (words_to_read,markov_chain,&flag,
                               &written_words,line) == 1)
    {
      return 1;
    }

  }
  return 0;
}

/**
* generate tweet
 * @param first_node - given pointer to node of the first word
 * @param max_length - given integer, the number max words in the tweet
 */
void generate_tweet(MarkovNode *first_node, int max_length)
{
  // Variables definition
  int i = 1;
  int flag = 1;
  int length_of_word;

  // Pointers definition
  MarkovNode *current_random;

  current_random = first_node;

  // Print the first random word
  printf ("%s", current_random->data);
  printf (" ");

  // get random nodes as long as the node is not end of sentence or max tweets
  while ( i < max_length && flag == 1)
  {
    // get the next random node
    current_random = get_next_random_node (current_random);
    printf ("%s", current_random->data);
    i++;
    // In case the word is not end of sentence
    length_of_word = strlen (current_random->data);
    if (current_random->data[length_of_word-1] != '.')
    {
      printf (" ");
    }
    // in case is end of sentence
    else
    {
      flag = 0;
    }
  }
}

/**
* print tweets
 * @param markov_chain - given pointer to markovchain
 * @param num_of_tweets - given integer, the number of tweets
 */
void print_tweets(MarkovChain *markov_chain, int num_of_tweets)
{

  MarkovNode *first_random;

  for (int i = 0; i < num_of_tweets; i++)
  {

    first_random = get_first_random_node (markov_chain);
    printf ("Tweet ");
    printf ("%d",i+1);
    printf (": ");
    generate_tweet (first_random,MAX_WORDS);
    printf ("\n");
  }

}

/**
* crate tweets
 * @param argc - number of arguments
 * @param - strings from cmd
 * @return 0 in case of success, 1 otherwise
 */

int check_arguments(int argc, char *argv[],int *number_of_words_to_read,
                    int *num_of_tweets, int *seed)
{
  if(argc != FOUR_ARGUMENTS && argc != FIVE_ARGUMENTS)
  {
    printf (NUM_ARGS_ERROR);
    return 1;
  }
  else
  {
    *seed = strtol (argv[1],NULL,BASE_TEN);
    *num_of_tweets = strtol (argv[2],NULL,BASE_TEN);
    if(argc == FIVE_ARGUMENTS)
    {
      *number_of_words_to_read = strtol (argv[4],NULL,BASE_TEN);
    }
    else
    {
      *number_of_words_to_read = READ_ALL_WORDS;
    }
    return 0;
  }
}

/**
 * @brief Fills the Markov chain database and prints generated tweets.
 *
 * This function reads words from a file into the Markov chain. If successful,
 * it generates and prints tweets. Otherwise, it prints an error message.
 *
 * @param file Pointer to the file containing input text.
 * @param markov_chain Pointer to the MarkovChain structure.
 * @param num_of_words_to_read Number of words to read from the file.
 * @param num_of_tweets Number of tweets to generate.
 * @return 0 on success, 1 on failure.
 */

int fill_database_and_print(FILE *file,MarkovChain *markov_chain, int
num_of_words_to_read, int num_of_tweets)
{
  if ( fill_database (file, num_of_words_to_read,
                      markov_chain) == 0)
  {
    print_tweets (markov_chain, num_of_tweets);
    free_database (&markov_chain);
  }
  else
  {
    printf (ALLOCATION_ERROR_MASSAGE);
    free_database (&markov_chain);
    return 1;
  }
  return 0;
}

/**
 * @brief Main function to generate tweets using a Markov chain model.
 *
 * This program reads a text file, builds a Markov chain database, and generates
 * random tweets based on the input data. It validates command-line arguments,
 * initializes memory, and handles errors appropriately.
 *
 * @param argc Number of command-line arguments.
 * @param argv Array of command-line arguments.
 *             - argv[1]: Number of words to read from the file.
 *             - argv[2]: Number of tweets to generate.
 *             - argv[3]: File path of the input text.
 *
 * @return EXIT_SUCCESS (0) if the program runs successfully, EXIT_FAILURE (1) on error.
 */
int main(int argc, char *argv[])
{
  // Variable definition
  int num_of_words_to_read = 0;
  int num_of_tweets = 0;
  int seed = 0;

  MarkovChain *markov_chain = malloc (sizeof (*markov_chain));

  // Faild to allocate memory
  if(markov_chain == NULL)
  {
    printf (ALLOCATION_ERROR_MASSAGE);
    EXIT_FAILURE;
  }

  LinkedList *linked_list = malloc (sizeof (*linked_list));

  // Faild to allocate memory
  if(linked_list == NULL)
  {
    printf (ALLOCATION_ERROR_MASSAGE);
    EXIT_FAILURE;
  }
  linked_list->first = NULL;
  linked_list->last=NULL;
  linked_list->size = 0;
  markov_chain->database = linked_list;

  // Check if input is valid
  if(check_arguments (argc,argv,&num_of_words_to_read,&num_of_tweets,&seed)
     == 0)
  {
    srand (seed);
    FILE *file_to_read;
    file_to_read = fopen (argv[3],"r");
    if(file_to_read)
    {

      if(fill_database_and_print (file_to_read,markov_chain,
                                  num_of_words_to_read,num_of_tweets)==0)
      {
        fclose (file_to_read);
        EXIT_SUCCESS;
      }
      else
      {
        EXIT_FAILURE;
      }
    }
    else
    {
      printf (FILE_PATH_ERROR);
      EXIT_FAILURE;
    }
  }
  else
  {
    EXIT_FAILURE;
  }
}


