#include "../../util/web.h"
#include "../../util/hashtable.h"
#include "../../util/file.h"
#include "../../util/indexer_utils.h"

DocumentNode *CopyListData(DocumentNode *head);

DocumentNode *GetDocList(HashTable *ht, char *word);

DocumentNode *ANDUpdate(DocumentNode *first, DocumentNode *second);

DocumentNode *ORUpdate(DocumentNode *first, DocumentNode *second);

DocumentNode *CreateUnsortedDocList(char *input, HashTable *ht);

void SortDocList(DocumentNode **head);

int ReportInfo(DocumentNode *head, char *directory);

void FreeDocList(DocumentNode *head);