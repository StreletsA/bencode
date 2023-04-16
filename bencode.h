#define BENCODE_CONTENT_TYPE_DICT 1
#define BENCODE_CONTENT_TYPE_INT 2
#define BENCODE_CONTENT_TYPE_STRING 3
#define BENCODE_CONTENT_TYPE_LIST 4

#define BENCODE_CONTENT_TYPE_DICT_START 'd'
#define BENCODE_CONTENT_TYPE_DICT_END 'e'
#define BENCODE_CONTENT_TYPE_INT_START 'i'
#define BENCODE_CONTENT_TYPE_INT_END 'e'
#define BENCODE_CONTENT_TYPE_LIST_START 'l'
#define BENCODE_CONTENT_TYPE_LIST_END 'e'

#define BENCODE_CONTENT_TYPE_STRING_LEN_DATA_DELIMITER ':'

typedef struct bencode_content_t
{
    short content_type;
    struct bencode_content_t *children;
    int children_count;
    char* str_value;
    int str_value_length;
    int int_value;
} bencode_content_t;

bencode_content_t decode(char* data, int len);