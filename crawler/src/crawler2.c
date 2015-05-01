/* ========================================================================== */
/* File: crawler.c - Tiny Search Engine web crawler
 *
 * Author: Emily Old
 * Date: April 2015
 *
 * Input:
 *
 * Command line options:
 *
 * Output:
 *
 * Error Conditions:
 *
 * Special Considerations:
 *
 */
/* ========================================================================== */
// ---------------- Open Issues

// ---------------- System includes e.g., <stdio.h>
#include <stdio.h>                           // printf

#include <curl/curl.h>                       // curl functionality
#include <string.h>
// ---------------- Local includes  e.g., "file.h"
#include "common.h"                          // common functionality
#include "web.h"                             // curl and html functionality
#include "list.h"                            // webpage list functionality
#include "hashtable.h"                       // hashtable functionality
#include "utils.h"                           // utility stuffs

// ---------------- Constant definitions

// ---------------- Macro definitions

// ---------------- Structures/Types

// ---------------- Private variables

// ---------------- Private prototypes

/* ========================================================================== */

int main(int argc, char* argv[]){

    //WebPage *seed = malloc(sizeof(WebPage));

    printf("Get cooking cs50!\n"
        "Translate the pseudo code. Data structures are in crawler.h\n"
        "Good luck\n");
    
    //check command line arguments
    if (argc != 4){
        fprintf(stderr, "Crawler requires exactly 3 arguments");
        exit(EXIT_FAILURE);
    }


    // hashtable check
    HashTable *ht = malloc (sizeof(HashTable));
    initializeHashTable(ht);
    addToHashTable(ht, argv[1]);
    addToHashTable(ht, argv[1]);
    
    printf("%d", returnVal);
    unsigned long hash = JenkinsHash(argv[1], 5);
    printf("%s", ht->table[hash]->url);
   
    
    // init curl
    curl_global_init(CURL_GLOBAL_ALL);
    
    seed->depth = 0;

    seed->url = malloc(sizeof(char*) * 1000);
    strcpy(seed->url, argv[1]);
    int status;
   
    if ( (status = GetWebPage(seed)) == 0 ){
        fprintf(stderr, "%s was not able to be processed.", argv[1]);
    }
    else{
        printf("Good to go.");
    }
    
    
    // setup seed page

    


    

    // get seed webpage

    // write seed file

    // add seed page to hashtable

    // extract urls from seed page

    // while there are urls to crawl
        // get next url from list

        // get webpage for url

        // write page file

        // extract urls from webpage

    //sleep(INTERVAL_PER_FETCH);

    // cleanup curl
    curl_global_cleanup();

    return 0;
}
