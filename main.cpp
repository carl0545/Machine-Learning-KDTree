/*
*Dylan Carlson
*Machine Learning
*Assignment 6
*4/2018
*/

#include "mat.h"
#include <iostream>
#include <cmath>

void buildKDTree(Matrix*, int, int, int); //Turns the labeled matrix into a KDTree (in a matrix format)
void nearestAux(Matrix *tree,   // the kdtree matrix
                Matrix *item,   // the row
                int rowstart,   // beginning row of region to search (subtree)
                int rowend,     // ending row of region to search
                int c,          // column or feature to compare
                double &best,   // the distance to nearest point found so far
                int &bestex);   // the row of the nearest point found so far
void labeledOutput(char**, Matrix); //Outputs the labeled matrix in assignment specs
double distFix(Matrix, Matrix); //distance fixer
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

  //iterate passing each row of item matrix through thing

  labeledOutput(label, kdTree);


  for(int i = 0; i < unlabeledMatrix.numRows(); i++){
    Matrix subItem;
    double bestAns;
    bestAns = 100000000.0;
    int bestExam;
    bestExam = 10000;
    subItem = new Matrix(unlabeledMatrix.subMatrix(i, 0, 1, unlabeledMatrix.numCols()));
    cout << endl << endl;
    cout << "SOLVE:        ";
    for(int c = 0; c < subItem.numCols(); c++){
      cout << subItem.get(0,c) << "   ";
    }
    cout << endl;

    nearestAux(&kdTree, &subItem, 0, kdTree.numRows()-1, 1, bestAns, bestExam);
    cout << "ans: ";
    for(int c = 1; c < kdTree.numCols(); c++){
      cout << kdTree.get(bestExam, c) << "  ";
    }
    cout <<  sqrt(bestExam) << "  ";
    cout << label[bestExam] << endl;


  }


}
//Distance fixer make sure a is item and b is parent
double distFix(Matrix a, Matrix b){
  double sum;

  sum = 0;
  for (int r=0; r<a.numRows(); r++) {
      for (int c=0; c<a.numCols(); c++) {
          double tmp;

          tmp = a.get(r,c) - b.get(r,c+1);
          sum += tmp * tmp;
      }
  }

  return sum;


}

/*
* Turns the labeled matrix into a KDTree (in a matrix format)
* Destroys original matrix
*/
void buildKDTree(Matrix *data, int featureNum, int minR, int maxR){
  if(featureNum >= data->numCols()){
    featureNum = 1;
  }

  //Sort on specific feature
  data->sortRowsByCol(featureNum, minR, maxR);

  //Increase feature number
  featureNum++;

  //Base Cases
  int range = maxR-minR;

  if(range <= 1){
    return; //do nothing
  }

  else{
    int newMaxR = minR + range/2;
    buildKDTree(data, featureNum, minR, newMaxR-1); //left side
    buildKDTree(data, featureNum, newMaxR+1, maxR); //right side
  }

}

void nearestAux(Matrix *tree, Matrix *item, int rowstart, int rowend, int c, double &best, int &bestex) {
  //item will always be a 1xc matrix
  cout << "RANGE: " << rowstart << " to " << rowend << endl;

  Matrix item_n = new Matrix(item);
  int range = rowend - rowstart;

  /*
  if(c >= tree->numCols()){ //WRAP Around
    c = 1;
  }
  */

  if(range <= 0){ //leaf
    Matrix compute = new Matrix(tree->subMatrix(rowstart, 0, 1, tree->numCols()));
    double m_dist = distFix(item_n, compute);
    if(m_dist < best){
      cout << "BESTLEAF:   " << sqrt(m_dist) << " " << rowstart << endl;
      best = m_dist;
      bestex = rowstart;
      return;
    }
    else{
      return;
    }
  }

  else { //parent
    int median = rowstart + range/2;

    double comparison = tree->get(median, c);


    Matrix parent = new Matrix(tree->subMatrix(median, 0, 1, tree->numCols()));

    if(comparison > item->get(0,c-1)){ //which side to split
      //left/upper side

      int newRange = median - rowstart - 1;
      int otherRange = rowend - median;

      if(newRange >= 0){ //split on first children

        nearestAux(tree, item, rowstart, newRange, c+1, best, bestex);

      //  if(abs(item->get(0,c-1) - parent.get(0,c)) > best){
          return;
      //  }
      }

      if(distFix(item_n, parent) < best){ //try parent
        best = distFix(item_n, parent);
        cout << "BESTPARENT(2): " << sqrt(best) << endl;
        bestex = median;
        if(best == 0){
          return;
        }
      }

      if(otherRange >= 0){ //Try other child
        double d = 100000;
        int e = 1000;
        nearestAux(tree, item, otherRange, rowend, c+1, d, e);
        if(d < best){
          best = d;
          bestex = e;
        }

      }


    }
    else{
      //right/lower side

      int newRange = rowend - median;
      int otherRange = median - rowstart - 1;

      if(newRange >= 0){ //second child
        nearestAux(tree, item, newRange, rowend, c+1, best, bestex);
      //  if(abs(item->get(0,c-1) - parent.get(0,c))){
          return;
      //  }
      }

      if(distFix(item_n, parent) < best){
        best = distFix(item_n, parent);
        bestex = median;
        cout << "BESTPARENT1: " << sqrt(best) << bestex << endl;
        if(best == 0){
          return;
        }
      }

      if(otherRange >= 0){
        double d = 100000;
        int e = 10000;
        nearestAux(tree, item, rowstart, otherRange+1, c+1, d, e);
        if(d < best){
          best = d;
          bestex = e;
        }
      }



    }



  }

return;

}


void labeledOutput(char **label, Matrix output){

  cout << "KDTree version of matrix(size of kd tree: " << output.numRows() << " X " << output.numCols() << ")" << endl;

  for(int r = 0; r < output.numRows(); r++){
    cout << label[(int)output.get(r,0)] << " ";
    for(int c = 1; c < output.numCols(); c++){
      cout << output.get(r,c) << " ";
    }
    cout << endl;
  }

}
