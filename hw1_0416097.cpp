#include<iostream>
#include<fstream>
#include<string>
#include<cstdio>
#include<cstdlib>
#include<cstring>
using namespace std;

#define SIZE 1000000
char text[SIZE];
int pattern_length=0;
int text_length=0;
char *pattern;
int *pattern_ff;
int position[100000];

void getText(ifstream &fin)//get the text in the input file
{
    int i=0;
    while(fin.get(text[i])){
    	i++;
	}
    text_length = i;

    return;
}

void getPattern(char *line)//get pattern in the text
{
    for(int i=0;line[i]!=' ';i++)
    {
        if(line[i]=='\0')
        	break;
		pattern = new char;
        pattern_ff = new int;
        pattern_length++;
    }
    for(int i=0;i<pattern_length;i++)
        pattern[i] = line[i];
    for(int i=0;i<pattern_length;i++)
        pattern_ff[i] = 0;

    return;
}

void failure_function(char *p) //build the failure_function
{
	pattern_ff[0] = 0;
	int i = 1 , j = 0;
	while(p[i]!=p[j]&&i<pattern_length){
		pattern_ff[i] = 0;
		i++;
	}
	while(i<pattern_length){
		if(p[i]==p[j]){
			pattern_ff[i] = j + 1;
			i++;
			j++;
		}
		else{
			while(p[i]!=p[j]){
				if(!j){
					pattern_ff[i] = j; //j is at the leftmost and p[i] is not equal to p[j]
					i++;
					break;
				}
				else{
					j = pattern_ff[j-1];
					if(p[i]==p[j]){
						pattern_ff[i] = j + 1;
						i++;
						j++;
						break;
					}
				}
			}
		}
	}

	return;
}

void substring_search(ofstream &fout)
{
	for(int i=0;i<100000;i++)
		position[i] = 0 ;
	int count=0; //how many substrings are there
	int frequency = 0; //not include substring
	int pattern_index=0; //index for pattern
	int text_index=pattern_length+1; //index for text
	int word_count=1;// word_count+1 represents this word is the (word_count+1)th word

	while( text_index < text_length )
	{
		if(text[text_index]==' '){
				word_count++;
				text[text_index] = '0';
		}
		if(text[text_index]==pattern[pattern_index]){
			pattern_index++;
			text_index++;
			if(pattern_index==pattern_length){
				if( ( text[text_index]<65 || (text[text_index]>90&&text[text_index]<97) || text[text_index]>122) &&
				    ( text[text_index-pattern_length-1]<65 || (text[text_index-pattern_length-1]>90&&text[text_index-pattern_length-1]<97) ||text[text_index-pattern_length-1]>122)
				  ){
				    frequency++;
			    }
				position[word_count]++;
				count++;
				pattern_index=0;
				text_index -= (pattern_length-1);
			}
		}
		else{
            while( (pattern[pattern_index]!=text[text_index]) || (!pattern_index) ){
                //the first character in pattern does not match the text[text_index] so move text[text_index] to the next
				if(!pattern_index){
                    text_index++;
                    break;
                }
                //use failure_function; move pattern_index back to the last possiblly-matching character  
				//if match, then break the loop (and keep examining whether the pattern is in the text or not)
				//else go through the loop again 
				//until 1. finding the "matching character" or 2. pattern_index = 0 (same as above)
				else{
                    pattern_index = pattern_ff[pattern_index-1];
                    if(pattern[pattern_index]==text[text_index]){
                        break;
                    }
                }
            }
		}
	}

	fout<<frequency+1<<endl;
	fout<<1;
	for(int i=0;count>0;i++)
		while(position[i]>0){
			fout<<i+1;
			position[i]--;
			count--;
		}

	return;
}

int main(void)
{
	ifstream fin;
	ofstream fout;
	string in_file,out_file;
	cin>>in_file>>out_file;
	fin.open(in_file.c_str());
	fout.open(out_file.c_str());

    getText(fin);
    getPattern(text);
	failure_function(pattern);
	substring_search(fout);

    fin.close();
    fout.close();

	return 0;
}
