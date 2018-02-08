#include "subscribe.h"
#include "article.h"
#include <tuple>

using std::string;
using std::set;
using std::tie;

bool operator<(const Subscriber &lhs, const Subscriber &rhs)
{
  return tie(lhs.ip, lhs.port) < tie(rhs.ip, rhs.port);
}

Subscriber::Subscriber(string ip, int port)
{
  this->ip = ip;
  this->port = port;
}

bool Subscriber::isSubbed(const Article &art) const
{
  if (articles.find(art) != articles.end())
    {
      return true;
    }

  auto cats = art.getSuperCats();
  for (int i = 0; i < cats.size(); i++)
    {
      if (articles.find(Article(cats[i])) != articles.end())
	{
	  return true;
	}
    }

  return false;
}

void Subscriber::unSub(const Article &art) const
{
  auto art_it = articles.find(art);
  if (art_it != articles.end())
    {
      articles.erase(art_it);
    }
}

void Subscriber::printArts() const
{
  for (auto it = articles.begin(); it != articles.end(); ++it)
    {
      (*it).print();
    }
}
