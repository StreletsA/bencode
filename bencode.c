#include "bencode.h"

#include <malloc.h>
#include <stdbool.h>

const char digits[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

typedef struct content_interval
{
    int start_index;
    int end_index;
} content_interval_t;

void find_interval(content_interval_t* interval, char* data, int start, int end);
int short convert_to_digit(char c);
int simple_pow(int num, int p);
int parse_unsigned_int_from_interval(char* data, content_interval_t interval);
content_interval_t find_str_len_interval(char* data, int start, int end);
bool is_digit(char ch);
bencode_content_t _decode(bencode_content_t content, content_interval_t root_interval, char* data, int len);
void add_children(bencode_content_t *content, bencode_content_t children);

bencode_content_t decode(char* data, int len) 
{
    bencode_content_t content;
    content.content_type = BENCODE_CONTENT_TYPE_DICT;
    content.children_count = 0;
    bencode_content_t* children = malloc(0);
    content.children = children;

    content_interval_t root_interval;
    root_interval.start_index = 0;
    root_interval.end_index = 0;
    find_interval(&root_interval, data, 0, len);

    return _decode(content, root_interval, data, len);
}

bencode_content_t _decode(bencode_content_t content, content_interval_t root_interval, char* data, int len)
{
    for (int i = root_interval.start_index + 1; i < root_interval.end_index - 1; i++) 
    {
        char cur = data[i];

        if (is_digit(cur))
        {
            content_interval_t str_len_interval = find_str_len_interval(data, i, len);
            int str_len = parse_unsigned_int_from_interval(data, str_len_interval);
            int str_start_index = str_len_interval.end_index + 1;
            char* str = malloc(str_len);

            for (int j = str_start_index; j < str_start_index + str_len; j++)
            {
                str[j - str_start_index] = data[j];
            }

            bencode_content_t str_content;
            str_content.content_type = BENCODE_CONTENT_TYPE_STRING;
            str_content.str_value = str;
            str_content.str_value_length = str_len;

            add_children(&content, str_content);

            i = str_start_index + str_len - 1;
        } 
        else if (cur == BENCODE_CONTENT_TYPE_INT_START)
        {
            int int_start_index = i + 1;
            int int_end_index = int_start_index;
            bool negative = data[int_start_index] == '-';
            if (negative)
            {
                int_start_index++;
            }

            for (int j = int_start_index; j < len; j++) {
                if (data[j] == 'e')
                {
                    int_end_index = j;
                    break;
                }
            }

            content_interval_t int_interval;
            int_interval.start_index = int_start_index;
            int_interval.end_index = int_end_index;

            int int_value = parse_unsigned_int_from_interval(data, int_interval);
            int_value = negative ? -int_value : int_value;

            bencode_content_t int_content;
            int_content.content_type = BENCODE_CONTENT_TYPE_INT;
            int_content.int_value = int_value;

            add_children(&content, int_content);

            i = int_interval.end_index;
        }
        else if (cur == BENCODE_CONTENT_TYPE_LIST_START)
        {
            bencode_content_t list_content;
            list_content.content_type = BENCODE_CONTENT_TYPE_LIST;
            list_content.children_count = 0;
            bencode_content_t* children = malloc(0);
            list_content.children = children;

            content_interval_t list_interval;
            find_interval(&list_interval, data, i, len);

            list_content = _decode(list_content, list_interval, data, len);

            add_children(&content, list_content);

            i = list_interval.end_index - 1;
        }
        else if (cur == BENCODE_CONTENT_TYPE_DICT_START)
        {
            bencode_content_t dict_content;
            dict_content.content_type = BENCODE_CONTENT_TYPE_DICT;
            dict_content.children_count = 0;
            bencode_content_t* children = malloc(0);
            dict_content.children = children;

            content_interval_t dict_interval;
            find_interval(&dict_interval, data, i, len);

            dict_content = _decode(dict_content, dict_interval, data, len);

            add_children(&content, dict_content);

            i = dict_interval.end_index - 1;
        }
    }

    return content;
}

void add_children(bencode_content_t *content, bencode_content_t children)
{
    int count = content->children_count + 1;
bencode_content_t* new_children = (bencode_content_t*) realloc(content->children, count * sizeof(bencode_content_t));

if (new_children == NULL) {
    printf("ALARM!");
} else {
    content->children = new_children;
    content->children_count = count;
    content->children[count - 1] = children;
}
}

bool is_digit(char ch) 
{
    for (int i = 0; i < 10; i++)
    {
        if (ch == digits[i]) return true;
    }

    return false;
}

content_interval_t find_str_len_interval(char* data, int start, int end)
{
    content_interval_t interval;
    interval.start_index = start;

    int cur_index = start;

    while (data[cur_index] != BENCODE_CONTENT_TYPE_STRING_LEN_DATA_DELIMITER)
    {
        cur_index++;
    }

    interval.end_index = cur_index;

    return interval;
}

int parse_unsigned_int_from_interval(char* data, content_interval_t interval)
{
    int digits_count = interval.end_index - interval.start_index;
    int c = simple_pow(10, digits_count - 1);
    int res = 0;

    for (int i = interval.start_index; i < interval.end_index; i++) 
    {
        short digit = convert_to_digit(data[i]);
        res = res + (digit * c);
        c /= 10;
    }

    return res;
}

int simple_pow(int num, int p) 
{
    if (p == 0)
    {
        return 1;
    }

    int res = num;

    while (p > 1) {
        res *= num;
        p--;
    }

    return num;
}

int short convert_to_digit(char c) 
{
    switch (c)
    {
    case '1':
        return 1;
        break;
    case '2':
        return 2;
        break;
    case '3':
        return 3;
        break;
    case '4':
        return 4;
        break;
    case '5':
        return 5;
        break;
    case '6':
        return 6;
        break;
    case '7':
        return 7;
        break;
    case '8':
        return 8;
        break;
    case '9':
        return 9;
        break;
    
    default:
        return 0;
        break;
    }
}

void find_interval(content_interval_t* interval, char* data, int start, int end)
{
    int start_index = -1;
    int end_index = start_index;
    int start_char_count = 0;

    bool is_int = false;
            
    for (int i = start; i < end; i++)
    {
        char ch = data[i];

        if (ch == 'e') 
        {
            start_char_count--;

            if (is_int)
            {
                is_int = false;
            }
        }
        else if (ch == BENCODE_CONTENT_TYPE_INT_START)
        {
            if (start_index == -1) 
            {
                start_index = i;
            }
            start_char_count++;
            is_int = true;
        }
        else if ((ch == BENCODE_CONTENT_TYPE_DICT_START 
                    || ch == BENCODE_CONTENT_TYPE_LIST_START))
        {
            if (start_index == -1) 
            {
                start_index = i;
            }
            start_char_count++;
        }

        if (start_char_count == 0)
        {
            end_index = i + 1;
            break;
        }

        if (is_digit(ch) && !is_int)
        {
            content_interval_t str_len_interval = find_str_len_interval(data, i, end);
            int str_len = parse_unsigned_int_from_interval(data, str_len_interval);

            i = str_len_interval.end_index + str_len;
        }
    }

    interval->start_index = start_index;
    interval->end_index = end_index;
}