#ifndef TSEUTIL_H
#define TSEUTIL_H

#define MAX_HASH_SLOT 1000

typedef struct HashTableNode {
    void *data;                               // url previously seen
    struct HashTableNode *next;              // pointer to next node
} HashTableNode;

typedef struct HashTable {
    HashTableNode *table[MAX_HASH_SLOT];     // actual hashtable
} HashTable;

typedef struct DocumentNode {
  struct DocumentNode *next;         // pointer to the next member of the list.
  int doc_id;                        // document identifier
  int freq;                          // number of occurrences of the word
} DocumentNode;

typedef struct WordNode {
  struct WordNode *next;            // pointer to the next word (for collisions)
  char *word;                       // the word
  DocumentNode *page;               // pointer to the first element of the page list.
} WordNode;


/*
 * GetNextWord - returns the next word from doc[pos] into word
 * @doc: pointer to an html document
 * @pos: current position in the document
 * @word: a pointer to a c-style string, used to pass the word back out
 *
 * Returns the current position searched so far in doc; otherwise, returns < 0;
 * The doc should be a valid character buffer. The pos argument should be 0 on the
 * initial call. The word argument should be a NULL pointer. On successful parse
 * of the html in doc, word will contain a newly allocated character buffer;
 * may be NULL on failed return. The caller is responsible for free'ing this
 * memory.
 *
 * Usage example: (retrieve all words in a page)
 * int pos = 0;
 * char *word;
 * char *doc = "<ruh>Vox clamantis <roh> 3.0 < 5.0 /> in deserto.<raggy>";
 *
 * while((pos = GetNextWord(doc, pos, &word)) > 0) {
 *     // do something with word
 *     free(word);
 * }
 */
int GetNextWord(const char *doc, int pos, char **word);

/*
 * NormalizeWord - lowercases all the alphabetic characters in word
 * @word: the character buffer to normalize
 *
 * Word is modified in-place, with all uppercase letters lowered.
 *
 * Usage example:
 * char *str = "HELLO WORLD!";
 * NormalizeWord(str);
 * // str should now be "hello world!"
 */
void NormalizeWord(char *word);

/*
 * jenkins_hash - Bob Jenkins' one_at_a_time hash function
 * @str: char buffer to hash
 * @mod: desired hash modulus
 *
 * Returns hash(str) % mod. Depends on str being null terminated.
 * Implementation details can be found at:
 *     http://www.burtleburtle.net/bob/hash/doobs.html
 */
unsigned long JenkinsHash(const char *str, unsigned long mod);

/*
* initializeHashTable
* @ht: HashTable struct that has been allocated
* 
* Returns void
*/
void initializeHashTable(HashTable *ht);

/*
 * addToHashTable
 * @ht: HashTable struct that has been allocated
 * @data: data to be stored in the hash table
 * @hashKey: char buffer that determines where the data is hashed to
 *
 * Returns void
 */
void addToHashTable(HashTable *ht, void *data, const char *hashKey);

/*
 * @ht: HashTable struct that has been allocated
 * @hashKey: char buffer that needs to be looked up
 *
 * Returns NULL if nothing is hashed to the corresponding spot
 * Otherwise returns the HashTableNode that is hashed to that spot
 */
HashTableNode *lookUp(HashTable *ht, const char *hashKey);


/*
 * IsDir - determine if path is a directory
 * @path: path to check
 *
 * Returns non-zero if path is a directory; otherwise, 0.
 *
 * Usage example:
 * if(IsDir(".")) {
 *     // "." is a directory
 * }
 */
int IsDir(const char *path);

/*
 * IsFile - determine if path is a file
 * @path: path to check
 *
 * Returns non-zero if path is a file; otherwise, 0.
 *
 * Usage example:
 * if(IsFile("/etc/passwd")) {
 *     // "/etc/passwd" is a file
 * }
 */
int IsFile(const char *path);

/*
 * GetFilenamesInDir - populates a list of filenames found in the directory dir
 * @dir: directory path to search
 * @filenames: a pointer to an array of c-style strings
 *
 * Returns the number of filenames found; otherwise, a negative number indicates
 * failure. The filenames array should not be preallocated, but will be created
 * by this function. It is the caller's responsibility to free the filenames and
 * the array containing them.
 *
 * Usage example:
 * char **filenames = NULL;
 * int num_files = 0;
 *
 * num_files = GetFilenamesInDir(".", &filenames);
 * if(num_files < 0) {
 *     // failed to get any filenames (various reasons)
 * } else {
 *     for(int i = 0; i < num_files; i++) {
 *         printf("File: %s\n", filenames[i]);
 *         free(filenames[i]);
 *     }
 *     free(filenames);
 * }
 */
int GetFilenamesInDir(const char *dir, char ***filenames);

#endif // TSEUTIL_H
