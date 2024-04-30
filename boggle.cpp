#ifndef RECCHECK
#include <iostream>
#include <vector>
#include <string>
#include <set>
#include <random>
#include <iomanip>
#include <fstream>
#include <exception>
#endif

#include "boggle.h"

std::vector<std::vector<char> > genBoard(unsigned int n, int seed)
{
	//random number generator
	std::mt19937 r(seed);

	//scrabble letter frequencies
	//A-9, B-2, C-2, D-4, E-12, F-2, G-3, H-2, I-9, J-1, K-1, L-4, M-2, 
	//N-6, O-8, P-2, Q-1, R-6, S-4, T-6, U-4, V-2, W-2, X-1, Y-2, Z-1
	int freq[26] = {9,2,2,4,12,2,3,2,9,1,1,4,2,6,8,2,1,6,4,6,4,2,2,1,2,1};
	std::vector<char> letters;
	for(char c='A'; c<='Z';c++)
	{
		for(int i=0;i<freq[c-'A'];i++)
		{
			letters.push_back(c);
		}
	}
	std::vector<std::vector<char> > board(n);
	for(unsigned int i=0;i<n;i++)
	{
		board[i].resize(n);
		for(unsigned  int j=0;j<n;j++)
		{
			board[i][j] = letters[(r() % letters.size())];
		}
	}
	return board;
}

void printBoard(const std::vector<std::vector<char> >& board)
{
	unsigned int n = board.size();
	for(unsigned int i=0;i<n;i++)
	{
		for(unsigned int j=0;j<n;j++)
		{
			std::cout << std::setw(2) << board[i][j];
		}
		std::cout << std::endl;
	}
}

std::pair<std::set<std::string>, std::set<std::string> > parseDict(std::string fname)
{
	std::ifstream dictfs(fname.c_str());
	if(dictfs.fail())
	{
		throw std::invalid_argument("unable to open dictionary file");
	} 
	std::set<std::string> dict;
	std::set<std::string> prefix;
	std::string word;
	while(dictfs >> word)
	{
		dict.insert(word);
		for(unsigned int i=word.size()-1;i>=1;i--)
		{
			prefix.insert(word.substr(0,i));
		}
	}
	prefix.insert("");
	return make_pair(dict, prefix);
}

std::set<std::string> boggle(const std::set<std::string>& dict, const std::set<std::string>& prefix, const std::vector<std::vector<char> >& board)
{
	std::set<std::string> result;
	for(unsigned int i=0;i<board.size();i++)
	{
		for(unsigned int j=0;j<board.size();j++)
		{
			boggleHelper(dict, prefix, board, "", result, i, j, 0, 1);
			boggleHelper(dict, prefix, board, "", result, i, j, 1, 0);
			boggleHelper(dict, prefix, board, "", result, i, j, 1, 1);
		}
	}
	
	return result;
}

bool boggleHelper(const std::set<std::string>& dict, const std::set<std::string>& prefix, const std::vector<std::vector<char> >& board, 
								   std::string word, std::set<std::string>& result, unsigned int r, unsigned int c, int dr, int dc)
{
//add your solution here!
    
    // true means there could be a longer word out there
    // false means there are no viable words beyond this point

    
    // case 1: the word has reached the end of the board
    // there cannot be any more viable words beyond here
    if (r >= board.size() || c >= board.size()) {

      // case 1.1: this word at the end of the grid is valid
      if (dict.find(word) != dict.end()) {
        //std::cout << "case 1.1 ";
        //std::cout << "appending word to result: " << word << std::endl;
        result.insert(word);
      }
      // case 1.2: if not, do not add word to results

      return false;
    }
    
    word.push_back(board[r][c]);

    // case 2: the word is not part of the prefix set
    if (prefix.count(word) == 0) {
      // case 2.1: the word can still be a valid standalone word
      // if it is, it is guaranteed that that is the longest word possible      
      if (dict.count(word) == 1) {
        //std::cout << "case 2.1 ";
        //std::cout << "appending word to result: " << word << std::endl;
        result.insert(word);
        return true;
      }
      // case 2.2: it is not a valid standalone word
      
      // regardless, we stop searching after this
      return false;
    }
    
    // case 3: the word is part of the prefix set
    else {

      // case 3.1: the word is a valid standalone word
      if (!boggleHelper(dict, prefix, board, word, result, r + dr, c + dc, dr, dc)) {
        if (dict.count(word) == 1) {
          //std::cout << "case 3.1 ";
          //std::cout << "appending word to result: " << word << std::endl;
          result.insert(word);
          return true;
        }
      }
      // case 3.2: the word is a prefix, but not valid
      // there may be a longer word out there
      else {
        return true;
      } 
      
    }
}


