/*
*Dylan Carlson
*Machine Learning
*Assignment 6
*4/2018
*/

#include "mat.h"
#include <iostream>

void buildKDTree(Matrix*, int, int, int); //Turns the labeled matrix into a KDTree (in a matrix format)


using namespace std;

int main(){
  ////////MAIN DECLARATIONS//////
  char **label; //contains labels of read matrix
  Matrix labeledMatrix; //read Matrix
  Matrix unlabeledMatrix; //unlabeled Matrix



  ///////MATRIX INPUT FROM STDIN////////
  label = labeledMatrix.readLabeledRow();
  unlabeledMatrix.read();
  labeledMatrix.print();

  ////BUILD KDTREE///
  Matrix kdTree = new Matrix(labeledMatrix);
  buildKDTree(&kdTree, 1, 0, labeledMatrix.numRows()-1);

  kdTree.print();


}
/*
* Turns the labeled matrix into a KDTree (in a matrix format)
* Destroys original matrix
*/
void buildKDTree(Matrix *data, int featureNum, int minR, int maxR){
  if(featureNum >= data->numCols()){
    cout << "FEATURES ARE OUT" << endl;
    return;
  }

  //Sort on specific feature
  data->sortRowsByCol(featureNum, minR, maxR);

  //Increase feature number
  featureNum++;

  //Base Cases
  int range = maxR-minR;

  if(range == 1){
    return; //do nothing
  }

  else if(range == 2){
    return; //do nothing
  }

  else{
    buildKDTree(data, featureNum, minR, maxR/2);
    buildKDTree(data, featureNum, (maxR/2)+1, maxR);
  }





}
