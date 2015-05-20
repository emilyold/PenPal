DocumentNode *one = malloc(sizeof(DocumentNode));
one->doc_id = 1;
one->freq = 3;
one->next = malloc(sizeof(DocumentNode));
one->next->doc_id = 2;
one->next->freq = 7;
one->next->next = malloc(sizeof(DocumentNode));
one->next->next->doc_id = 3;
one->next->next->freq = 2;
one->next->next->next = NULL;

DocumentNode *two = malloc(sizeof(DocumentNode));
two->doc_id = 1;
two->freq = 3;
two->next = malloc(sizeof(DocumentNode));
two->next->doc_id = 2;
two->next->freq = 7;
two->next->next = malloc(sizeof(DocumentNode));
two->next->next->doc_id = 3;
two->next->next->freq = 2;
two->next->next->next = NULL;

DocumentNode *combo = ANDUpdate(one, two);
while (combo != NULL){
	printf("doc_id: %d freq: %d\n", combo->doc_id, combo->freq);
	combo = combo->next;
}	
