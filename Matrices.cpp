#include "Matrices.h"
namespace Matrices
{

Matrix::Matrix(int _rows, int _cols)
{
    rows = _rows; 
    cols = _cols; 
    a.resize(rows);
    for (int i = 0; i < rows; i++)
    {
        a[i].resize(cols); 
    }  
    
}

Matrix operator+(const Matrix &a, const Matrix &b)
{
    Matrix set(a.getRows(), a.getCols()); 

    if (a.getRows() != b.getRows() && a.getCols() != b.getCols())
    {
        throw runtime_error("Error: dimensions must agree");
    }
    else 
    {
        for(int i = 0; i < a.getRows(); i++)
        {
            for(int j = 0; j < a.getCols(); j++)
            {
                set(i,j) = a(i,j) + b(i,j); 
            }
        }
    }
    return set;
}

Matrix operator*(const Matrix &a, const Matrix &b)
{ 
    Matrix c(a.getRows(), b.getCols());
    double sum; 
    if (a.getCols() != b.getRows())
    {
        throw runtime_error("Error: dimensions must agree");
    }
    else 
    {
        for (int i = 0; i < a.getRows(); i++)
        {
            for (int k = 0; k < b.getCols(); k++)
            {
                sum = 0; 
                for (int j = 0; j < a.getCols(); j++)
                {
                    sum += (a(i,j) * b(j,k));
                }
                c(i,k) = sum;
            }
        }
    }
    return c;
}

bool operator==(const Matrix &a, const Matrix &b)
{
    if (a.getRows() != b.getRows() || a.getCols() != b.getCols())
        return false; 
    
    for (int i = 0; i < a.getRows(); i++)
    {
        for (int j = 0; j < a.getCols(); j++)
        {
            if (a(i,j) != b(i,j))
                 return false; 
        }
    }
    
    return true;
}

bool operator!=(const Matrix &a, const Matrix &b)
{
    return (!(a == b));
}

ostream& operator<<(ostream &os, const Matrix &a)
{
     
    for (int i = 0; i < a.getRows(); i++)
    {
        for (int j = 0; j < a.getCols(); j++)
        {
            os << setw(10);
            os << a(i,j) << " ";  
        }
        os << endl;
    }
    // TODO: insert return statement here
    return os; 
}

RotationMatrix::RotationMatrix(double theta) : Matrix(2, 2)  
{
    (*this)(0, 0) = cos(theta);  
    (*this)(0, 1) = -sin(theta);  
    (*this)(1, 0) = sin(theta);   
    (*this)(1, 1) = cos(theta); 
}

ScalingMatrix::ScalingMatrix(double c) : Matrix(2, 2) 
{
    (*this)(0, 0) = c; 
    (*this)(0, 1) = 0;  
    (*this)(1, 0) = 0; 
    (*this)(1, 1) = c;  
}

TranslationMatrix::TranslationMatrix(double xShift, double yShift, int numPoints)
            : Matrix(2, numPoints)  
{
    for (int i = 0; i < numPoints; ++i) 
    {
        (*this)(0, i) = xShift;  
        (*this)(1, i) = yShift; 
    }
}

}
