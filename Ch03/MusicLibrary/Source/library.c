#include "../Header/library.h"

#define NUM_CHARS 256 // 2^8 = 256
#define BUFFER_LENGTH 200
#define INDEX_DIR_SIZE 100

Artist *artist_directory[NUM_CHARS];
SNode *index_directory[INDEX_DIR_SIZE];
int song_index = 0;

void insert_song_to_artist(Artist *artist, SNode *s_node);
void print_artist(Artist *artist);
void print_song_header();
void print_song_body(Song *song);
Artist *find_artist(char *name);
SNode *find_s_node_by_index(int index);
SNode *find_s_node_by_title(Artist * artist, char *title);
void save_artist(Artist *artist, FILE *filePtr);
void save_song(Song *song, FILE *filePtr);
void delete_song_from_artist(Artist *artist, SNode *s_node);
void destroy_song(Song *song);
void destroy_artist(Artist *artist);

void initialize(){
    for (int i = 0; i < NUM_CHARS; i++)
        artist_directory[i] = NULL;

    for (int i = 0; i < INDEX_DIR_SIZE; i++)
        index_directory[i] = NULL;
}

Artist *create_artist_instance(char *name){
    Artist * artist = (Artist *)malloc(sizeof(Artist));
    artist->name = name;
    artist->head = NULL;
    artist->tail = NULL;
    artist->next = NULL;

    return artist;
}

Artist *add_artist(char *name){
    Artist * artist = create_artist_instance(name);
    
    Artist *current = artist_directory[(unsigned char) name[0]];
    Artist *previous = NULL;

    while (current != NULL && strcmp(current->name, name) < 0 ){
        previous = current;
        current = current->next;
    }

    if (current == NULL & previous == NULL){ // not found
        artist_directory[(unsigned char) name[0]] = artist;
    } else if (previous == NULL){ // add first
        artist->next = current;
        artist_directory[(unsigned char) name[0]] = artist;
    } else { // add after previous
        previous->next = artist;
        artist->next = current;
    }
    
    return artist;
}

Song *create_song_instance(Artist *artist, char *title, char *filePath){
    Song *song = (Song *)malloc(sizeof(Song));
    song->artist = artist;
    song->title = title;
    song->filePath = filePath;

    song->index = song_index++;

    return song;
}

SNode *create_s_node_instance(Song *song){
    SNode *s_node = (SNode *)malloc(sizeof(SNode));
    s_node->song = song; 
    s_node->prev = NULL;
    s_node->next = NULL;

    return s_node;
}

Artist *find_artist(char *name){
    Artist *current = artist_directory[(unsigned char) name[0]]; // 0 ~ 255

    while (current != NULL && strcmp(current->name, name) < 0) // 이름이 알파벳 순으로 정렬되므로 "strcmp() < 0" 때까지만 돌면 된다.
        current = current->next;

    if (current != NULL && strcmp(current->name, name) == 0)
        return current;
    else 
        return NULL;
}

void insert_song_to_artist(Artist *artist, SNode *s_node){
    SNode *current = artist->head;
    while (current != NULL && strcmp(current->song->title, s_node->song->title) < 0)
        current = current->next;
   

    if (artist->head == NULL){ // empty list
        artist->head = s_node;
        artist->tail = s_node;
    } else if (artist->head == current){ // add first
        s_node->next = artist->head;
        artist->head->prev = s_node;
        artist->head = s_node;
    } else if (current == NULL){ // add last
        s_node->prev = artist->tail;
        artist->tail->next = s_node;
        artist->tail = s_node;
    } else { // add between
        s_node->next = current;
        s_node->prev = current->prev;
        current->prev->next = s_node;
        current->prev = s_node;
    }   
}

void insert_song_to_index(Song *song){
    SNode *s_node = create_s_node_instance(song);

    int index = song->index % INDEX_DIR_SIZE;

    SNode *current = index_directory[index];
    SNode *previous = NULL;
    while (current != NULL && strcmp(current->song->title, song->title) < 0){
        previous = current;
        current = current->next;
    }

    if (previous == NULL){ // add first
        s_node->next = current;
        index_directory[index] = s_node;
    } else { // add between previous and current
        s_node->next = current;
        previous->next = s_node;
    }
}

void add_song(char *name, char *title, char *filePath){
    Artist *artist = find_artist(name);
    if (artist == NULL)
        artist = add_artist(name);
    
    Song *song = create_song_instance(artist, title, filePath); // song knows artist
    SNode *s_node = create_s_node_instance(song);  // s_node knows song
    
    insert_song_to_artist(artist, s_node); 
    insert_song_to_index(song);
}




void print_song_header(){
    printf("+-------+---------------------------+-----------------------------------------------+\n");
    printf("| %-5s | %-25s | %-45s |\n", "IDX", "TITLE", "FILE PATH");
    printf("+-------+---------------------------+-----------------------------------------------+\n");
}

void print_song_body(Song *song){
    printf("| %-5d | %-25s | %-45s |\n", song->index, song->title, song->filePath);
    printf("+-------+---------------------------+-----------------------------------------------+\n");
}


void print_artist(Artist *artist){
    printf("\n[ Artist: %s ]\n", artist->name);
    SNode *current = artist->head;

    print_song_header();
    while (current != NULL){
        print_song_body(current->song);
        current = current->next;
    }
}


void show(){
    for (int i = 0; i < NUM_CHARS; i++){
        Artist *artist = artist_directory[i];
        while (artist != NULL) {
            print_artist(artist);
            artist = artist->next;
        }
    }
}

int open(FILE *filePtr){
    char buffer[BUFFER_LENGTH];
    char *name = NULL, *title = NULL, *filePath = NULL;

    while (1){
        if (read_line(filePtr, buffer, BUFFER_LENGTH) <= 0)
            break;

        name = strtok(buffer, "#");
        if (name == NULL || strcmp(name, "") == 0){
            name = NULL;
        }else {
            name = strdup(name);
        }

        title = strtok(NULL, "#");
        if (title == NULL || strcmp(title, "") == 0){
            title = NULL;
        }else {
            title = strdup(title);
        }

        filePath = strtok(NULL, "#");
        if (filePath == NULL || strcmp(filePath, "") == 0){
            filePath = NULL;
        }else {
            filePath = strdup(filePath);
        }

        add_song(name, title, filePath);
    }
    return 1;
}

SNode *find_s_node_by_index(int index){
    SNode *current = index_directory[index % INDEX_DIR_SIZE];
    while (current != NULL && current->song->index != index)
        current = current->next;

    return current;
}

SNode *find_s_node_by_title(Artist * artist, char *title){
    SNode *current = artist->head;

    while (current != NULL && strcmp(current->song->title, title) < 0)
        current = current->next;
    
    if (current != NULL && strcmp(current->song->title, title) != 0)
        return NULL;

    return current;
}

void find_song(char *name, char *title){
    Artist *artist = find_artist(name);
    if (artist == NULL){
        printf("No results found\n");
        return;
    }

    if (title == NULL) {
        print_artist(artist);
        return;
    }

    SNode *current = find_s_node_by_title(artist, title);
    
    if (current == NULL){
        printf("No results found\n");
        return;  
    } 

    print_song_header();
    print_song_body(current->song);
}


void play(int index){
     SNode *s_node = find_s_node_by_index(index);
    if (s_node == NULL){
        printf("No results found\n");
        return;
    }

    printf("%s is playing\n", s_node->song->title);
}

void delete(int index){
    SNode *current = index_directory[index % INDEX_DIR_SIZE];
    SNode *previous = NULL;
    while (current != NULL && current->song->index != index){
        previous = current;
        current = current->next;
    }

    if (current == NULL){ // empty list or not exists
        printf("No results found\n");
        return;
    }

    Song *song = current->song;

    // delete s_node of index_directory
    if (previous == NULL){
        index_directory[index % INDEX_DIR_SIZE] = current->next;
    } else {
        previous->next = current->next;
    }
    free(current);

    // delete s_node of artist_directory
    Artist *artist = song->artist;
    SNode *s_node = find_s_node_by_title(artist, song->title);
    if (s_node == NULL){
        printf("Error: Not found s_node\n");
        return;
    }
    delete_song_from_artist(artist, s_node);
    destroy_song(song);

}

void destroy_song(Song *song){
    if (song->title != NULL)
        free(song->title);

    if (song->filePath != NULL)
        free(song->filePath);

    free(song);
}

void delete_song_from_artist(Artist *artist, SNode *s_node){
    SNode *first = artist->head;
    SNode *last = artist->tail;
        
    if (first == s_node && last == s_node){ // only
        destroy_artist(artist);
    } else if (first == s_node){ // delete first
        artist->head = s_node->next;
        artist->head->prev = NULL;
    } else if (last == s_node){ // delete last
        artist->tail = s_node->prev;
        artist->tail->next = NULL;
    } else { // delete between
        s_node->prev->next = s_node->next;
        s_node->next->prev = s_node->prev;
    }
    free(s_node);
}

void destroy_artist(Artist *artist){   
    Artist *current = artist_directory[(unsigned char) artist->name[0]]; // 0 ~ 255
    Artist *previous = NULL;

    while (current != NULL && current->name != artist->name){
        previous = current;
        current = current->next;
    }

    if (current == NULL){ // empty list or not exists
        printf("Error: Not found artist\n");
        return;
    }

    // delete artist of artist_directory
    if (previous == NULL){
        artist_directory[(unsigned char) artist->name[0]] = current->next;
    } else {
        previous->next = current->next;
    }

    if (artist->name != NULL)
        free(artist->name);

    free(artist);
}

void save(FILE *filePtr){
    if (song_index <= 0){
        printf("No data available.\n");
        return;
    }

    for (int i = 0; i < NUM_CHARS; i++){
        Artist *artist = artist_directory[i];
        while (artist != NULL) {
            save_artist(artist, filePtr);
            artist = artist->next;
        }
    }
}

void save_artist(Artist *artist, FILE *filePtr){
    SNode *current = artist->head;

    while (current != NULL){
        save_song(current->song, filePtr);
        current = current->next;
    }
}

void save_song(Song *song, FILE *filePtr){
    if (song->artist != NULL)
        fprintf(filePtr, "%s#", song->artist->name);
    else
        fprintf(filePtr, "#");

    if (song->title != NULL && strlen(song->title) > 0 && song->title[0] != '\0')
        fprintf(filePtr, "%s#", song->title);
    else
        fprintf(filePtr, "#");
    
    if (song->filePath != NULL && strlen(song->filePath) > 0 && song->filePath[0] != '\0')
        fprintf(filePtr, "%s#\n", song->filePath);
    else
        fprintf(filePtr, "#\n");
}

