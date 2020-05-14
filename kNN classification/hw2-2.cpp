#include<vector>
#include<iostream>
#include<fstream>
#include<string>
#include<math.h>
using namespace std;

vector <vector<float>> train(180);
vector <vector<float>>::iterator it;
int which = 0;
int examine(vector<float> in,int k)
{
    int s = 0;
    float distance = 0;
    float minn[k][2] = {1000000};
    int now = 0;
    vector<int> vote(3,0);
    for(auto it=train[s];s<which-1;s++,it=train[s])
    {
      distance = 0;
      if((s>=0 && s<=29) || (s>=50&&s<=79) || (s>=100&&s<=129))
      {

          for(int j=0;j<4;j++)
          {
            //cout << "ATTRI  " << abs(it[j]-in[j]) << endl;
            distance += abs(it[j] - in[j])*abs(it[j]-in[j]);
          }

          //cout << "DISTANCE : " << distance << endl;
          if(now < k)
          {
            //vote[0] = *it[4];
            minn[now][0] = it[4];
            minn[now][1] = distance;
            now++;
          }
          else
          {
            for(int i=0;i<k;i++)
            {
                if(distance < minn[i][1])
                {
                    minn[i][0] = it[4];
                    minn[i][1] = distance;
                    break;
                }
            }
          }
        }
    }
    int maxx = 0;
    int answer;

    for(int i=0;i<k;i++)
        vote[minn[i][0]-1]++;
    for(int i=0;i<3;i++)
    {
        //cout << "VOTE " << i << "  " << vote[i] << endl;
        if(vote[i] > maxx)
        {
            answer = i;
            maxx = vote[i];
        }
    }
    /*
    if(answer == 0)
        return "Iris-setosa";
    else if(answer == 1)
        return "Iris-versicolor";
    else if(answer == 2)
        return "Iris-virginica";*/
    return answer+1;
}
int main(){
    fstream fin;
    float input;
    char space;
    string classs;

    fin.open("iris.data.txt",ios::in);
    if(!fin)
      cout << "ERROR!" << endl;
    else
    {
      while(!fin.eof())
      {
        for(int i=0;i<4;i++)
        {
          fin >> input;
          train[which].push_back(input);
          //cout << "INPUT " << input << endl;
          fin >> space;
        }
        fin >> classs;
        if(classs == "Iris-setosa")
            train[which].push_back(1);
        else if(classs == "Iris-versicolor")
            train[which].push_back(2);
        else if(classs == "Iris-virginica")
            train[which].push_back(3);
        else
        {
            train[which].push_back(0);
            cout << "ERROR!" << endl;
        }
        //cout << "CLASS " << classs << endl;
        //fin >> space;
        which++;
      }
      fin.close();
    }
    int s=0;
    vector<float> x;
    float correct=0;
    float falses=0;
    for(auto it=train[s];s<which-1;s++,it=train[s])
    {
        if(!((s>=0 && s<=29) || (s>=50&&s<=79) || (s>=100&&s<=129)))
        {
            for(int j=0;j<4;j++)
            {
                x.push_back(train[s][j]);
            }
            int out = examine(x,1);
            if(train[s][4] == out)
                correct++;
            else
                falses++;
            //cout << s << "   " << examine(x,1) << endl;
        }
        x.clear();
    }
    cout << "Correct : " << correct << endl;
    cout << "False : " << falses << endl;
    float sss = correct/60;
    cout << "Correct Percentage : " << sss*100 << "%" << endl;
    int ss=0;
    /*for(it=train.begin();ss<which-1;it++,ss++)
    {
      cout << ss << "  ";
      for(auto it2=it->begin();it2!=it->end();it2++)
        cout << *it2 << " ";
      cout << endl;
    }*/



    system("pause");
    return 0;
}
