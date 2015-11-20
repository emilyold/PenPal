/* ========================================================================== */
/* File: crawler.c - Tiny Search Engine web crawler
 *
 * Author: Emily Old
 * Date: April 2015
 *
 * Input: a url to be crawled, a valid directory, and a depth
 *
 * Command line options: None
 *
 * Output: The url being crawled to standard out. Files containing each url's html
 * in the provided directory. 
 *
 * Special Considerations: Memory leaks exist
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
        printf("Crawler requires exactly 3 arguments\n");
        exit(EXIT_FAILURE);
    }
    
    // make sure the directory provided is valid
    if (stat(argv[2], &path) < 0){
        printf("%s is not a valid directory.\n", argv[2]);
        exit(EXIT_FAILURE);
    }
    
    // make sure the depth provided is a valid number
    depth = strtol(argv[3], &ptr, 10);
    if( strlen(ptr) != 0){
        printf("The depth must be a single integer.\n");
        exit(EXIT_FAILURE);
    }
    // and make sure the depth is in range
    else if (depth > MAX_DEPTH){
        printf("Depth %s is too deep!\n", argv[3]);
        exit(EXIT_FAILURE);
    }

    // make sure the url provided is within the http://old-www.cs.dartmouth.edu/~cs50/tse/wiki/ domain
    if ( strncmp(URL_PREFIX, argv[1], strlen(URL_PREFIX)) != 0 ){
        printf("URL provided is not in the permitted domain. Please choose a URL with the prefix: %s\n", URL_PREFIX);
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
    //int normal;
    
    if ((status = GetWebPage(seed)) == 0 ){
        printf("%s was not able to be processed.\n", argv[1]);
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
        printf("[crawler]: Crawling - %s\n", seed->url);
        while( (pos = GetNextURL(seed->html, pos, baseUrl, &result)) > 0 ){
            if(strncmp(URL_PREFIX, result, strlen(URL_PREFIX)) == 0){
                if (NormalizeURL(result) != 0){
                    if(lookUpURL(ht, result) == 0){
                        WebPage *pg = malloc(sizeof(WebPage));
                        pg->url = result;
                        pg->depth = seed->depth + 1;
                        addToHashTable(ht, pg->url);
                        appendToList(theList, pg);
                    }    
                } 
            }
            
        }
    }
    free(seed->html);
    free(seed->url);
    free(seed);

    // while there are urls to crawl
    ListNode *node; //= malloc(sizeof(ListNode));
    while ( (node = pop(theList)) != NULL ){
        WebPage *pg = node->data;
        //pg = 

        // if the webpage is valid, write it to a file 
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

        // if there is still more crawling to do...
        if( pg->depth < depth ){
            printf("[crawler]: Crawling - %s\n", pg->url);
            // extract urls from the current html and add them to the list
            pos = 0;
            baseUrl = pg->url;
            while( (pos = GetNextURL(pg->html, pos, baseUrl, &result)) > 0 ){
                if(strncmp(URL_PREFIX, result, strlen(URL_PREFIX)) == 0){
                    // only add them to the list if they have not already been looked at
                    if ( NormalizeURL(result) != 0 ){
                        if (lookUpURL(ht, result) == 0){                    
                            WebPage *page = malloc(sizeof(WebPage));
                            page->url = result;
                            page->depth = pg->depth + 1;
                            addToHashTable(ht, page->url);
                            appendToList(theList, page);
                        }
                    }
                }          
            }   
        }
        sleep(.1);
        // free memory
        free(pg->url);
        free(pg->html);
        free(pg);
        free(node);
    }
    
    // free memory
    free(fileName);
    free(theList->head);
    free(theList->tail);
    free(theList);
    free(depthString);
    freeHashTable(ht);
    free(ht);
    // cleanup curl
    curl_global_cleanup();

    return 0;
}