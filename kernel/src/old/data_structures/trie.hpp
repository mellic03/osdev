#pragma once


namespace idk
{
    struct TrieNode;
    class Trie;
}


struct idk::TrieNode
{
    TrieNode *children[26];
};

class idk::Trie
{

};