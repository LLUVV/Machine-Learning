#include <iostream>
#include <vector>
#include <string>
#include <fstream>
using namespace std;
float theta = 0.2;
float rate = 0.2;
float training_set[90][5];
float testing_set[15][5];
float weight[4] = {0.2};
int perceptron(int pick,bool test)
{
    float pos=theta;
    for(int i=0;i<4;i++)
    {
      if(test)
        pos += weight[i] * testing_set[pick][i];
      else
        pos += weight[i] * training_set[pick][i];
    }
    if(pos > 0) return 1;
    else return 0;
}
void modify(int pick,bool in)
{
    if(in == true)
        for(int i=0;i<4;i++)
        {
            if(training_set[pick][i] == 1)
                weight[i] -= rate;
            if(i == 0)
                theta -= rate;
            //if(i == 0) cout << "Now weight : " << theta << " ";
            //  cout << weight[i] << " ";
        }
    else
        for(int i=0;i<4;i++)
        {
            if(training_set[pick][i] == 1)
                weight[i] += rate;
            if(i == 0)
                theta += rate;
            //if(i == 0) cout << "Now weight : " << theta << " ";
            //    cout << weight[i] << " ";
        }
    //cout << endl;
}
int main(void)
{
    fstream fin;
    char in;
    char space;
    int now = 0;
    float input;
    string classs;
    fin.open("training data.txt",ios::in);
    if(!fin)
      cout << "ERROR!" << endl;
    else
    {
      while(!fin.eof())
      {
        for(int i=0;i<4;i++)
        {
          fin >> input;
          training_set[now][i] = input;
          fin >> space;
        }
        fin >> classs;
        if(classs == "Iris-setosa")
            training_set[now][4] = 0;
        else if(classs == "Iris-versicolor")
            training_set[now][4] = 1;
        else
        {
            training_set[now][4] = 2;
            cout << "ERROR!" << endl;
            system("pause");
        }
        now++;
      }
      fin.close();
    }
        //cout << "OUT" << endl;
        int counte = 0;

        theta = 0.2;
        counte = 0;
        bool incorrect = true;
        //while(incorrect)
        //{
          counte++;
          incorrect = false;
          //cout << "CYCLE : " << counte << endl;
          for(int i=0;i<90;i++)
          {
            int per = perceptron(i,false);
            //cout << "PERCEPTRON : " << per << endl;
            if(per != training_set[i][4])
            {
              incorrect = true;
              if(training_set[i][4] == 0) modify(i,true);
              else modify(i,false);
            }
          }
        //}
        //cout << "Rate : " << rate << endl;
        //cout << "Use How much Time : " << (counte-1)*20 << endl;

// Complete Training classifier

        now = 0;
        fin.open("testing-data.txt",ios::in);
        if(!fin)
          cout << "ERROR! 1" << endl;
        else
        {
          while(!fin.eof())
          {
            for(int i=0;i<4;i++)
            {
              fin >> input;
              testing_set[now][i] = input;
              fin >> space;
              //cout << "Testing eat : " << testing_set[now][i] << endl;
            }
            fin >> classs;
            if(classs == "Iris-setosa")
                testing_set[now][4] = 0;
            else if(classs == "Iris-versicolor")
                testing_set[now][4] = 1;
            else if(classs == "Iris-virginica")
                testing_set[now][4] = 2;
            else
            {
                testing_set[now][4] = 3;
                cout << "ERROR! 2" << endl;
                system("pause");
            }
            now++;
          }
          fin.close();
        }
        int correct = 0;
        for(int i=0;i<10;i++)
        {
            int ans = perceptron(i,true);
            //cout << "Example " << i << "classify as " << ans << endl;
            if(ans == testing_set[i][4])
            {
              cout << i << "  is correct !" << endl;
              correct++;
            }
        }
        cout << "RESULT (correct percentage) : " << correct << " over 10 examples" << endl;
}
