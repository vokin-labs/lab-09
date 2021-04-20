//
// Created by alesandr on 19.04.2021.
//

#include "Parser.h"

bool not_repeated_img(const string& obj, const std::vector<string>& vec){
  for (const auto& i : vec) {
    if (obj == i) {
      return false;
    }
  }
  return true;
}

bool not_repeated_link(const string& obj, const std::deque<string>& vec){
  for (const auto& i : vec) {
    if (obj == i) {
      return false;
    }
  }
  return true;
}

void Parser::search_for_links(GumboNode* node, const Html& html) {
  if (node->type != GUMBO_NODE_ELEMENT) {
    return;
  }
  GumboAttribute* href;
  if (node->v.element.tag == GUMBO_TAG_A &&
      (href = gumbo_get_attribute(&node->v.element.attributes, "href"))) {
    string link = (string{href->value}.find('/') == 0)
                  ? string{html.protocol + "://" + html.host +
                           string{href->value}}
                  : href->value;
    if (all_links.empty()){
      all_links.push_back(link);
    } else if (not_repeated_link(link, all_links) && link[0] == 'h' && (link != string{html.protocol + "://" + html.host})) {
      all_links.push_back(link);
    }
  }

  GumboVector* children = &node->v.element.children;
  for (unsigned int i = 0; i < children->length; ++i) {
    search_for_links(static_cast<GumboNode*>(children->data[i]), html);
  }
}
void Parser::find_links(const Html& html) {
  GumboOutput* output = gumbo_parse(html.html.c_str());
  search_for_links(output->root, html);
  gumbo_destroy_output(&kGumboDefaultOptions, output);
}

void Parser::search_for_img(GumboNode* node, const Html& html) {
  if (node->type != GUMBO_NODE_ELEMENT) {
    return;
  }
  GumboAttribute* img;
  if ((node->v.element.tag == GUMBO_TAG_IMG || node->v.element.tag == GUMBO_TAG_IMAGE) &&
      (img = gumbo_get_attribute(&node->v.element.attributes, "src")) && string{img->value} != "") {
    string image = (string{img->value}.find('/') == 0)
                   ? string{html.protocol + "://" + html.host +
                            string{img->value}}
                   : img->value;
    if (ref_all_img.empty()){
      ref_all_img.push_back(image);
    } else if (not_repeated_img(image, ref_all_img)) {
      ref_all_img.push_back(image);
    }
  }
  GumboVector* children = &node->v.element.children;
  for (unsigned int i = 0; i < children->length; ++i) {
    search_for_img(static_cast<GumboNode*>(children->data[i]), html);
  }
}

void Parser::find_img(const Html& html) {
  GumboOutput* output = gumbo_parse(html.html.c_str());
  search_for_img(output->root, html);
  gumbo_destroy_output(&kGumboDefaultOptions, output);
}

std::deque<string> Parser::parse(const Html& html_, bool flag) {
  auto res = parsers.enqueue([&](const Html& html){
    if(!flag)
      find_links(html);
    find_img(html);
    return all_links;
  }, html_);
  return res.get();
}