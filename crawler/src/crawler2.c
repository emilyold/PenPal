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
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
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

    WebPage *seed = malloc(sizeof(WebPage));

    struct stat path;
    long depth;
    char *ptr;

    HashTable *ht;
    List *theList;
    //int currentDepth = 0;
    int docID = 1;
    char *fileName = malloc(sizeof(char)*50);
    char *depthString = malloc(sizeof(char)*5);
   
    //check command line arguments
    if (argc != 4){
        fprintf(stderr, "Crawler requires exactly 3 arguments\n");
        exit(EXIT_FAILURE);
    }
    
    if (stat(argv[2], &path) < 0){
        fprintf(stderr, "%s is not a valid directory.\n", argv[2]);
        exit(EXIT_FAILURE);
    }
    
    depth = strtol(argv[3], &ptr, 10);
    if( strlen(ptr) != 0){
        fprintf(stderr, "The depth must be a single integer.\n");
        exit(EXIT_FAILURE);
    }
    else if (depth > MAX_DEPTH){
        fprintf(stderr, "Depth %s is too deep!\n", argv[3]);
        exit(EXIT_FAILURE);
    }

    if ( strncmp(URL_PREFIX, argv[1], strlen(URL_PREFIX)) != 0 ){
        fprintf(stderr, "URL provided is not in the permitted domain. Please choose a URL with the prefix: %s", URL_PREFIX);
        exit(EXIT_FAILURE);
    }
    

    // init curl
    curl_global_init(CURL_GLOBAL_ALL);

    //initialize data structures
    ht = malloc(sizeof(HashTable));
    theList = malloc(sizeof(List));

    initializeHashTable(ht);
    initializeList(theList);

    
    // setup seed page
    seed->depth = 0;
    

    seed->url = malloc(sizeof(char*) * 1000);
    strcpy(seed->url, argv[1]);

    // get seed webpage
    int status;
    
    if ( (status = GetWebPage(seed)) == 0 ){
        fprintf(stderr, "%s was not able to be processed.", argv[1]);
    }

    // write seed file
    else{  
        sprintf(fileName, "%s/%d", argv[2], docID);
        docID++;
        FILE *fp;
        fp = fopen(fileName, "w");
        fputs(seed->url, fp);
        fputs("\n0\n", fp);
        fputs(seed->html, fp);
        fclose(fp);
    }

    // add seed page to hashtable
    if(lookUpURL(ht, seed->url) == 0){
        addToHashTable(ht, seed->url);
    }
        
    // extract urls from seed page
    int pos = 0;
    char *result;
    char *baseUrl = seed->url;

    if(seed->depth < depth){
        
        
        while( (pos = GetNextURL(seed->html, pos, baseUrl, &result)) > 0 ){
            if(lookUpURL(ht, result) == 0){
                WebPage *pg = malloc(sizeof(WebPage));
                pg->url = result;
                pg->depth = seed->depth + 1;
                addToHashTable(ht, pg->url);
                appendToList(theList, pg);
                printf("added %s\n", pg->url);
                printf("%d\n", pos);
            }    
        }
    }

    // while there are urls to crawl
    ListNode *node = malloc(sizeof(ListNode));
    while ( (node = pop(theList)) != NULL ){
        WebPage *pg = malloc(sizeof(WebPage));
        pg = node->page;
        printf("%s %d %ld\n", pg->url, pg->depth, depth);
        // if(pg->depth > currentDepth){
        //     currentDepth = pg->depth;
        // }
        
        if ( (status = GetWebPage(pg)) != 0 ){
            sprintf(fileName, "%s/%d", argv[2], docID);
            sprintf(depthString, "\n%d\n", pg->depth);
            docID++;
            FILE *fp;
            fp = fopen(fileName, "w");
            fputs(pg->url, fp);
            fputs(depthString, fp);
            fputs(pg->html, fp);
            fclose(fp);
        }

        if( pg->depth < depth ){
            pos = 0;
            baseUrl = pg->url;
            while( (pos = GetNextURL(pg->html, pos, baseUrl, &result)) > 0 ){
                if(strncmp(URL_PREFIX, result, strlen(URL_PREFIX)) == 0 && lookUpURL(ht, result) == 0){
                    WebPage *page = malloc(sizeof(WebPage));
                    page->url = result;
                    page->depth = pg->depth + 1;
                    addToHashTable(ht, result);
                    appendToList(theList, page);
                       
                }
                      
            }    
        }
        sleep(.2);
    }

        // get next url from list

        // get webpage for url

        // write page file

        // extract urls from webpage

    //sleep(INTERVAL_PER_FETCH);

    // cleanup curl
    curl_global_cleanup();

    return 0;
}
