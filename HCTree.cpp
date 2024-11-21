#include "HCTree.hpp"
#include <stack>
#include <queue>
using namespace std;

void HCTree::build (const vector<int>& freqs)
{
    priority_queue<HCNode*, vector<HCNode*>, HCNodePtrComp> pq;

    for (int i = 0;i < freqs.size();i++)        // make forest
    {
        unsigned char c = i;
        if (freqs[i] == 0)
        {
            continue;           // only include nonzero counts
        }
        HCNode* newNode;
        newNode = new HCNode(freqs[i],c);
        newNode->c0 = nullptr;
        newNode->c1 = nullptr;
        pq.push(newNode);
        leaves[i] = newNode;
    }

    while (pq.size() > 1)
    {
        HCNode* top1;
        HCNode* top2;
        HCNode* newNode;

        top1 = pq.top();
        pq.pop();

        top2 = pq.top();
        pq.pop();

        newNode = new HCNode(top1->count+top2->count,top2->symbol);
        newNode->c1 = top1;
        newNode->c0 = top2;

        top1->p = newNode;
        top2->p = newNode;

        pq.push(newNode);
    }

    root = pq.top();


}

void HCTree::encode(unsigned char symbol, FancyOutputStream & out) const
{
    stack<string> s;
    int index = symbol;
    HCNode * nodeTraverser = leaves[index];
    if (nodeTraverser == nullptr)
    {
        return;
    }

    while (nodeTraverser != root)
    {
        HCNode * parent = nodeTraverser->p;
        if (nodeTraverser == parent->c0)
        {
            s.push("0");
        }
        if (nodeTraverser == parent->c1)
        {
            s.push("1");
        }
        nodeTraverser = nodeTraverser->p;
    }

    string encoding = "";
    while (!s.empty())
    {
        encoding += s.top();
        s.pop();
    }
    
    for (int i = 0; i < encoding.length();i++)
    {
        if (encoding[i] == '0')
        {
            out.write_bit(0);
        }
        if (encoding[i] == '1')
        {
            out.write_bit(1);
        }
    }

}

unsigned char HCTree::decode(FancyInputStream & in) const
{
    HCNode * nodeTraverser = root;
    int bit = in.read_bit();
    while (bit != -1)
    {
       if (bit == 0)
       {
          if (nodeTraverser->c0 != nullptr)
          {
            nodeTraverser = nodeTraverser->c0;
          }
       }
       if (bit == 1)
       {
          if (nodeTraverser->c1 != nullptr)
          {
            nodeTraverser = nodeTraverser->c1;
          }
       }
       if (nodeTraverser->c0 == nullptr && nodeTraverser->c1 == nullptr)
       {
          return nodeTraverser->symbol;
       }
       bit = in.read_bit();
    }

    return '0';
}

void clearHelper (HCNode * n)
{
    if (n == nullptr)
    {
        return;
    }

    clearHelper(n->c0);
    clearHelper(n->c1);

    delete n;
}

HCTree::~HCTree ()
{
    clearHelper(root);
    root = nullptr;
}
