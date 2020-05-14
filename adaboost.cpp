#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <math.h>
using namespace std;
float training_set[90][6]; // 0~3 attr 4 class 5 weight
float testing_set[15][5];
float random;
int take[9][10];
float theta = 0;
float rate = 0.2;
float weight[9] = {0.2};
int result[90];
int peresult [9];
/**
  round : the classifier you want to use
  pick : the index you want to pick in testing/training set
  return -> true : classify correct / false : classify wrong
  test -> testing or training
**/
int examine(int pick,int round,bool test)
{
    float distance;
    float min[2] = {100000000};
    for(int i=0;i<10;i++)
    {
      distance = 0;

        for(int j=0;j<4;j++)
        {
          if(!test)
            distance += abs(training_set[pick][j] - training_set[take[round][i]][j])*abs(training_set[pick][j] - training_set[take[round][i]][j]);
          else
            distance += abs(testing_set[pick][j] - training_set[take[round][i]][j])*abs(testing_set[pick][j] - training_set[take[round][i]][j]);
        }

        if(min[0] > distance)
        {
          min[0] = distance;
          min[1] = i;
        }

    }
    return training_set[take[round][int(min[1])]][4];

}
/**
  test : the index that you want to test in training_set
  return -> 0 true / 1 false
**/
bool perexamine(int test)
{
    float pos = theta;

    for(int i=0;i<9;i++)
    {
      peresult[i] = examine(test,i,false);
      if(peresult[i] == 1) pos += weight[i];

    }

    if(pos > 0 && training_set[test][4] == 1) return true;
    else if(pos <= 0 && training_set[test][4] == 0) return true;

    else return false;
}
int finalexamine(int test)
{
  float pos = 0;
  float neg = 0;
  for(int i=0;i<9;i++)
  {
    peresult[i] = examine(test,i,true);
    if(peresult[i] == 1) pos += weight[i];
    else neg += weight[i];
  }
  if(pos > neg) return 1;
  else return 0;

}
bool perceptron(int test)
{
    if(perexamine(test)) return true;
    for(int i=0;i<9;i++)
    {
      if(peresult[i] == 1)
        weight[i] = (training_set[test][4] == 1) ?  weight[i] + rate : weight[i] - rate;
      if(i == 0)
        theta = (training_set[test][4] == 1) ? theta + rate : theta - rate;
    }
    return false;
}
int main(void)
{
    fstream fin;
    char in;
    char space;
    int now = 0;
    float input;
    string classs;
/***
    MAKE CLASSIFIER
***/
    /*** random initial weight ***/
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
        training_set[now][5] = float(1)/float(90);
        now++;
      }
      fin.close();
    }
    srand(time(NULL));
/***
    MAKE SUBCLASSIFIER
***/
    for(int k=0;k<9;k++)
    {
      for(int i=0;i<10;i++)
      {
        random = ((float) rand() / (RAND_MAX));
        int j = 0;
        float sum = training_set[j][5];
        while(1)
        {
          if(random < sum || j == 89)
          {
            take[k][i] = j;
            break;
          }
          else
            sum += training_set[++j][5];
        }
      }
/***
    Finish one SUBCLASSIFIER , Changing probability of subset choosing
***/
      float sum = 0;
      float beta;
      for(int i=0;i<90;i++)
      {
        result[i] = examine(i,k,false);
        if(result[i] != training_set[i][4])
          sum += training_set[i][5];
      }
      beta = sum/(1-sum);
      sum = 0;
      if(beta != 0)
      {
        for(int i=0;i<90;i++)
        {
          if(result[i] == training_set[i][4])
            training_set[i][5] *= beta;
        }
        for(int i=0;i<90;i++)
        {
          sum += training_set[i][5];
        }
        for(int i=0;i<90;i++)
        {
          training_set[i][5] /= sum;
        }
      }
    }
/***
    MAKE MASTER CLASSIFIER
***/
    bool incorrect = true;
    int counter = 0;
    while(incorrect)
    {
      incorrect = false;
      for(int i=0;i<90;i++)
        if(!perceptron(i)) incorrect = true;
      counter++;
    }


/***
    CLASSIFIER COMPLETED
***/
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
        int ans = finalexamine(i);

        if(ans == testing_set[i][4])
        {
          cout << i << "  is correct !" << endl;
          correct++;
        }
    }
    cout << "RESULT (correct percentage) : " << correct << " over 10 examples" << endl;

}
