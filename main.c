#include "bencode.h"
#include "stdio.h"

void print_content(bencode_content_t content);

int main(void) {
    char* test_bencode_string = "d3:bar4:spam3:fooi-42ee";
    
    bencode_content_t content = decode(test_bencode_string, 23);

    print_content(content);

    return 0;
}

void print_content(bencode_content_t content)
{
    if (content.content_type == BENCODE_CONTENT_TYPE_DICT || content.content_type == BENCODE_CONTENT_TYPE_LIST)
    {
        printf("BENCODE_CONTENT_TYPE_DICT || BENCODE_CONTENT_TYPE_LIST\n");

        for (int i = 0; i < content.children_count; i++)
        {
            print_content(content.children[i]);
        }
    } 
    else if (content.content_type == BENCODE_CONTENT_TYPE_INT)
    {
        printf("BENCODE_CONTENT_TYPE_INT\n");
        printf("%d\n", content.int_value);
    }
    else if (content.content_type == BENCODE_CONTENT_TYPE_STRING)
    {
        printf("BENCODE_CONTENT_TYPE_STRING\n");

        int len = content.str_value_length;
        for (int i = 0; i < len; i++)
        {
            printf("%c", content.str_value[i]);
        }
        printf("\n");
    }
}