#include <string>
#include <set>

struct Article;

struct Subscriber
{
  std::string ip;
  int port;
  mutable std::set<Article> articles;

  Subscriber(std::string ip, int port);

  //returns true if subscribed article in category of "art"
  bool isSubs(const Article &art) const;

  /*  matching article remove */
  void unSubs(const Article &art) const;

  void print() const;
};

/* Comparator struct used by set */
bool operator<(const Subscriber &lhs, const Subscriber &rhs);
