#pragma once

#include <string>
#include <set>

struct Article;

struct Subscriber
{
  std::string ip;
  int port;
  mutable std::set<Article> articles;

  Subscriber(std::string ip, int port);

  /* True if one of subbed articles is a super cat of art */
  bool isSubbed(const Article &art) const;

  /* Removes matching article if found. Nothing if not found */
  void unSub(const Article &art) const;

  void printArts() const;
};

/* Comparator struct used by set */
bool operator<(const Subscriber &lhs, const Subscriber &rhs);
