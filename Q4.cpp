#include <iostream>
using namespace std;
#include <cstdio>

void lowerCase(string &s);
int levenshteinDistance(string s1, string s2); // basically.. it checks how much two strings are alike

struct Word  /*in "dictionary.txt" file, there are words and their meanings in a single line. 
This struct is used to separate them.*/
{
    string spelling;
    string meaning;
    Word(){}
    Word(string line)
    {
        lowerCase(line);
        int i = 0;
        while(i < line.length() && line[i] != ' ')spelling += line[i++];
        i++;
        while(i < line.length() && line[i] == ' ') i++; 
        while(i < line.length()) meaning += line[i++];
    }
};

class Dictionary // basically...an array of "Word" objects
{
    private:
    Word *words;
    int size = 0;
    public:
    Dictionary()
    {
        FILE *file = fopen("dictionary.txt", "r");
        if (!file)
        {
            cerr << "Error opening file\n";
            exit(1);
        }
        char line[10000];
        while (fgets(line, sizeof(line), file)) size++;
        fclose(file);
        file = fopen("dictionary.txt", "r");
        words = new Word[size];
        int i = 0;
        while (fgets(line, sizeof(line), file)) words[i++] = Word(line);
        fclose(file);
    }
    void searchWord(string word)
    {
        lowerCase(word);
        for(int i = 0; i < size; i++)
        {
            if(words[i].spelling == word)
            {
            cout << "The Meaning of " << word<< " is:\n";
            cout<<"\033[1;33m" << words[i].meaning << "\033[0m\n\n";
            return;
            }
        }
            int mostAlikeWordIndex = -1;
            int mostAlikeWordDistance = 2;
        for(int i = 0; i < size; i++)
        {
            
            if(levenshteinDistance(word, words[i].spelling) <= mostAlikeWordDistance)
            {
                mostAlikeWordIndex = i;
                mostAlikeWordDistance = levenshteinDistance(word, words[i].spelling);
            }
        }
        if(mostAlikeWordIndex == -1)
        {
            cout << "Word not found in dictionary\n";
            return;
        }
        cout << "Did you mean " << words[mostAlikeWordIndex].spelling << " ?\n";
        cout << "The Meaning of " << words[mostAlikeWordIndex].spelling << " is:\n";
        cout<<"\033[1;33m" << words[mostAlikeWordIndex].meaning << "\033[0m\n\n"; //coloring the meaning
        return;     
    }
    ~Dictionary()
    {
        delete[] words;
    }
};

class ChatBot
{
    private:
    Dictionary dict;
    public:
    void correctWord(string word)
    {
        lowerCase(word);
        dict.searchWord(word);
    }
};


int main()
{
    ChatBot bot;
    while(true)
    {
        string word;
        cout << "Enter a word: ";
        cin >> word;
        if(word == "exit") break;
        bot.correctWord(word);
    }
    return 0;
}



void lowerCase(string &s)
{
    for(int i = 0; i < s.length(); i++)
        if(s[i] >= 'A' && s[i] <= 'Z')s[i] += 32;
}

int levenshteinDistance(string s1, string s2)
{
    int n = s1.length();
    int m = s2.length();
    int dp[n + 1][m + 1];
    for(int i = 0; i <= n; i++)
    {
        for(int j = 0; j <= m; j++)
        {
            if(i == 0) dp[i][j] = j;
            else if(j == 0) dp[i][j] = i;
            else if(s1[i - 1] == s2[j - 1]) dp[i][j] = dp[i - 1][j - 1];
            else dp[i][j] = 1 + min(dp[i - 1][j - 1], min(dp[i - 1][j], dp[i][j - 1]));
        }
    }
    return dp[n][m];
}

