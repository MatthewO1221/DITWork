#include "pl.h"

CNF const operator|(Literal const &op1, Literal const &op2) { return CNF(op1) | CNF(op2); }
CNF const operator|(Literal const &op1, CNF const &op2) { return CNF(op1) | op2; }
CNF const operator&(Literal const &op1, Literal const &op2) { return CNF(op1) & CNF(op2); }
CNF const operator&(Literal const &op1, CNF const &op2) { return CNF(op1) & op2; }
CNF const operator>(Literal const &op1, Literal const &op2) { return CNF(op1) > CNF(op2); }
CNF const operator>(Literal const &op1, CNF const &op2) { return CNF(op1) > op2; }

KnowledgeBase::KnowledgeBase() : clauses() {}
////////////////////////////////////////////////////////////////////////////
KnowledgeBase &KnowledgeBase::operator+=(CNF const &cnf)
{
    for (std::set<Clause>::const_iterator it = cnf.begin(); it != cnf.end(); ++it)
    {
        clauses.insert(*it);
    }
    return *this;
}
////////////////////////////////////////////////////////////////////////
std::set<Clause>::const_iterator KnowledgeBase::begin() const { return clauses.begin(); }
std::set<Clause>::const_iterator KnowledgeBase::end() const { return clauses.end(); }
unsigned KnowledgeBase::size() const { return clauses.size(); }
////////////////////////////////////////////////////////////////////////////
bool KnowledgeBase::ProveByRefutation(CNF const &alpha) const
{
    KnowledgeBase copy = *this;
    CNF negate = ~alpha;
    copy += negate;

    while (true)
    {
        bool resolution_done = false;

        for (std::set<Clause>::const_iterator it1 = copy.begin(); it1 != copy.end(); ++it1)
        {
            for (std::set<Clause>::const_iterator it2 = copy.begin(); it2 != copy.end(); ++it2)
            {
                if (it1 != it2)
                {
                    CNF resolvent = (*it1) | (*it2);

                    if (resolvent.Empty())
                        return true;

                    copy += resolvent;
                    resolution_done = true;
                }
            }
        }

        if (!resolution_done)
        {
            return false;
        }
    }
}
////////////////////////////////////////////////////////////////////////////
std::ostream &operator<<(std::ostream &os, KnowledgeBase const &kb)
{
    unsigned size = kb.clauses.size();
    for (std::set<Clause>::const_iterator it1 = kb.clauses.begin(); it1 != kb.clauses.end(); ++it1)
    {
        os << *it1 << ", ";
    }
    return os;
}
