#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int good_char(char ch) {
  if ((ch >= 'a' && ch <= 'z') ||
      (ch >= 'A' && ch <= 'Z') ||
      (ch >= '0' && ch <= '9') ||
      ch == ' ' ||
      ch == ',' ||
      ch == '$' ||
      ch == '.') return 1;
  return 0;
}

char** read_text(size_t* sentences_count) {
  char* sentences[256];
  *sentences_count = 0;
  size_t bfSize = 1024,
         sentenceSize = 0,
         lfCount = 0;
  char buffer[bfSize];
  char curChar = '$';
  while (lfCount < 2) {
    curChar = getc(stdin);
    if (curChar == '\n') {
      ++lfCount;
    } else {
      lfCount = 0;
    }
    if (good_char(curChar)) {
      buffer[sentenceSize++] = curChar;
      if (curChar == '.') {
        buffer[sentenceSize - 1] = '\0';
        if (sentenceSize > 0) {
          char* new_sentence = malloc(sentenceSize * sizeof(char));
          strcpy(new_sentence, buffer);
          sentences[(*sentences_count)++] = new_sentence;
        }
        sentenceSize = 0;
      }
    }
  }
  buffer[sentenceSize] = '\0';
  if (sentenceSize > 0) {
    char* new_sentence = malloc(sentenceSize * sizeof(char));
    strcpy(new_sentence, buffer);
    sentences[(*sentences_count)++] = new_sentence;
  }
  char** sentences_result = malloc(*sentences_count * sizeof(char*));
  for (size_t i = 0; i < *sentences_count; ++i) {
    sentences_result[i] = sentences[i];
  }
  return sentences_result;
}

char* handle_special_character(char* sentence, size_t size) {
  char* result;
  size_t left_dollar = 0, right_dollar = 0;
  while (sentence[left_dollar] != '$' && left_dollar < size) {
    ++left_dollar;
  }
  if (left_dollar == size) {
    left_dollar = 0;
  }
  for (size_t i = 0; i < size; ++i) {
    if (sentence[i] == '$') {
      right_dollar = i;
    }
  }
  if (right_dollar == left_dollar) {
    if (sentence[left_dollar] == '$') {
      result = malloc(size * sizeof(char));
      for (size_t i = 0; i < left_dollar; ++i) {
        result[i] = sentence[i];
      }
      for (size_t i = left_dollar + 1; i <= size; ++i) {
        result[i - 1] = sentence[i];
      }
      free(sentence);
    } else {
      result = sentence;
    }
  } else {
    result = malloc((right_dollar - left_dollar) * sizeof(char));
    for (size_t i = left_dollar + 1; i < right_dollar; ++i) {
      result[i - left_dollar - 1] = sentence[i];
    }
    free(sentence);
  }
  return result;
}

