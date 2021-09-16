#pragma once

#include <stdexcept>
#include <string>
#include <list>
#include <vector>

class TextBlock;
class Block;
class Page;
class Renderer;

// thrown if we get into an unexpected state
class ParseException : public std::exception
{
private:
  std::string message;

public:
  ParseException(int index)
  {
    message = "Parse error at index " + std::to_string(index);
  }
  const char *what() const throw()
  {
    return message.c_str();
  }
};

// a very stupid xhtml parser - it will probably work for very simple cases
// but will probably fail for complex ones
class RubbishHtmlParser
{
private:
  const char *m_html;
  int m_length;

  std::list<Block *> blocks;
  TextBlock *currentTextBlock = nullptr;
  std::vector<Page *> pages;

public:
  RubbishHtmlParser(const char *html, int length);
  ~RubbishHtmlParser();
  // is there any more whitespace we should consider?
  bool isWhiteSpace(char c)
  {
    return (c == ' ' || c == '\r' || c == '\n');
  }
  // we've got a limited set of tags that we treat as block tags
  // block tags contain a paragraph of text
  bool isBlockTag(const char *html, int index, int length);
  bool isClosingBlockTag(const char *html, int index, int length);
  // move past an html tag - basically move forward until we hit '>'
  int skipTag(const char *html, int index, int length);
  // move past anything that should be considered part of a work
  int skipAlphaNum(const char *html, int index, int length);
  bool isImageTag(const char *html, int index, int length);
  // start a new text block if needed
  void startNewTextBlock();
  // skip past any white space characters
  int skipWhiteSpace(const char *html, int index, int length);
  // find the location of an attribute within a tag
  bool findAttribute(const char *html, int index, int length, const char *name, int *src_start, int *src_end);
  // a closing tag will have a '/' character immediately following the '<'
  bool isClosingTag(const char *html, int index, int length);
  // self closing tags have a / before we hit the '>'
  // this assumes that we have already eliminated the possibility of a closing tag
  bool isSelfClosing(const char *html, int index, int length);
  void parse(const char *html, int index, int length);
  void layout(Renderer *renderer, Epub *epub);
  int get_page_count()
  {
    return pages.size();
  }
  void render_page(int page_index, Renderer *renderer);
};
