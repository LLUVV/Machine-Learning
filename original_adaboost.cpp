#include <iostream>
#include <string>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <math.h>
using namespace std;
float training_set[90][6];
float testing_set[15][5];
float random;
int take[9][10];
float theta = 0.2;
float rate = 0.2;
//float weight[9] = {float(1) / float(9)};
float alpha[9];
bool result[90];
float res[9];
/**
  round,pick : the index you want to pick in take array (take[round][pick])
  k : knn's k
  train : a bool to see if this call is to train subclassifier(true) or master classifier(false)
  return -> 0 true / 1 false
  test -> testing or training
**/
int examine(int pick,int round,bool test)
{
    float distance;
    float min[2] = {100000000};
    for(int i=0;i<10;i++)
    {
      distance = 0;
      //if(pick != take[round][i] || test)
      //{
        for(int j=0;j<4;j++)
        {
          //cout << " ATT " << pick << "  " << take[round][i] << "  " << abs(training_set[pick][j] - training_set[take[round][i]][j]) << endl;
          if(!test)
            distance += abs(training_set[pick][j] - training_set[take[round][i]][j])*abs(training_set[pick][j] - training_set[take[round][i]][j]);
          else
            distance += abs(testing_set[pick][j] - training_set[take[round][i]][j])*abs(testing_set[pick][j] - training_set[take[round][i]][j]);
        }

        //if(test && round == 0) cout << "Example " << pick << "  DISTANCE : " << distance << endl;
        if(min[0] > distance)
        {
          min[0] = distance;
          min[1] = i;
        }
      //}
    }
    return training_set[take[round][int(min[1])]][4];
    //if(test && testing_set[pick][4] == training_set[int(min[1])][4]) return true;
    //else if(!test && training_set[pick][4] == training_set[int(min[1])][4]) return true;
    //else return false;
}
int finalexamine(int test)
{
  float pos = 0;
  float neg = 0;
  for(int i=0;i<9;i++)
  {
    result[i] = examine(test,i,true);
    if(result[i] == 1) pos += alpha[i];
    else neg += alpha[i];
    //cout << "GREAT !! " << i << endl;
    //cout << "Classifier " << i << "classify example " << test << " as " << result[i] << endl;
  }
  if(pos > neg) return 1;
  else return 0;
}
/**
  test : the index that you want to test in training_set
  return -> 0 true / 1 false
**/
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
// MAKE SUBCLASSIFIER
    for(int k=0;k<9;k++)
    {
      for(int i=0;i<10;i++)
      {
        random = ((float) rand() / (RAND_MAX));
        //cout << "RANDOM " << random << endl;
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
        //cout << "TAKE : " << j << endl;
      }
      //cout << "OK 1 " << k << endl;
      float sum = 0;
      float beta;
      for(int i=0;i<90;i++)
      {
        result[i] = examine(i,k,false);
        if(result[i] != training_set[i][4])
          sum += training_set[i][5];
      }
      beta = sqrt(sum/(1-sum));
      alpha[k] = log((1-sum)/sum)/2;
      //cout << "OK 2 " << k << endl;
      sum = 0;
      if(beta != 0)
      {
        for(int i=0;i<90;i++)
        {
          if(result[i] == training_set[i][4])
            training_set[i][5] *= beta;
          else
            training_set[i][5] /= beta;
        }
          //cout << "OK"<< endl;
        for(int i=0;i<90;i++)
        {
          sum += training_set[i][5];
        }
        for(int i=0;i<90;i++)
        {
          training_set[i][5] /= sum;
        }
      }
      //cout << "OK 3 " << k << endl;
    }
// MAKE MASTER CLASSIFIER
    //cout << "THERE" << endl;
  //  float alpha;
  //  bool incorrect = true;
    /*while(incorrect)
    {
      incorrect = false;
      for(int i=0;i<90;i++)
      {
        float sum = 0;
        float fault = 0;
        for(int j=0;j<9;j++)
        {
          res[i] = examine(i,j,false);
          if(res[i] != training_set[i][4]) fault += weight[j];
          //cout << j << endl;
        }
        if(fault != 0) incorrect = true;
        alpha = log((1-fault/9)/fault*9)/2;
        for(int j=0;j<9;j++)
        {
          weight[j] = weight[j] * exp(-1*alpha*training_set[i][4]*res[i]);
          sum += weight[j];
        }
        for(int j=0;j<9;j++)
        {
          weight[j] /= sum;
        }
      }
    }*/

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
        int ans = finalexamine(i);
        //cout << "Example " << i << "classify as " << ans << endl;
        if(ans == testing_set[i][4])
        {
          cout << i << "  is correct !" << endl;
          correct++;
        }
    }
    cout << "RESULT (correct percentage) : " << correct << " over 10 examples" << endl;

}
