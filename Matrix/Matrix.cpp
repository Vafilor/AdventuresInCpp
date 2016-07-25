//Matrix.cpp
//By Andrey Melnikov

#include "Matrix.h"
#include <stdexcept>
#include <iostream> //For debugging
#include <string>
using namespace std; //For debugging

Matrix::Matrix()
{
	this->entries = NULL;
	this->rows = 0;
	this->columns = 0;
}

Matrix::Matrix(unsigned int rows, unsigned int columns)
{
	if(rows == 0 || columns == 0)
	{
		throw invalid_argument("rows or columns is 0");
	}

    double** data = new double*[rows];
    
    for(int i = 0; i < rows; i++) 
    {
        data[i] = new double[columns]; 
        
        for(int j = 0; j < columns; j++) 
        {
            data[i][j] = 0.0;
        }
    }
    
    this->entries = data;
    this->rows = rows;
    this->columns = columns;
}

Matrix::Matrix(const Matrix& that)
{
	this->rows = that.rows;
	this->columns = that.columns;

	this->entries = new double*[this->rows];
	
	for(int i = 0; i < this->rows; i++)
	{
		this->entries[i] = new double[this->columns];

		for(int j = 0; j < this->columns; j++)
		{
			this->entries[i][j] = that.entries[i][j];
		}
	}
}

Matrix::Matrix(Matrix&& that)
{
	this->entries = that.entries;
	this->rows = that.rows;
	this->columns = that.columns;
	
	that.entries = nullptr;
}

Matrix::~Matrix()
{
	this->freeEntriesMemory();
}

void Matrix::freeEntriesMemory()
{
	if(this->entries == nullptr)
	{
		return;
	}

	for(int i = 0; i < this->rows; i++) 
    {	
        delete[] this->entries[i];
    }

	if(this->rows > 0 && this->columns > 0) 
	{
		delete[] this->entries;
	}
}

Matrix Matrix::operator+(const Matrix& that) const
{
	if(this->rows != that.rows || this->columns != that.columns)
	{
		throw invalid_argument("Matrix summation not defined for matrices");
	}

    return Matrix(this->rows, this->columns, [&](unsigned int i, unsigned int j){ return entries[i][j] + that.entries[i][j]; });
}

Matrix Matrix::operator*(const Matrix& that) const
{
	if(this->columns != that.rows)
	{
		throw invalid_argument("Matrix Multiplication not defined for incoming matrix");
	}	

    int newRows = this->getRows();
    int newColumns = that.columns;

    Matrix product( newRows, newColumns );
    
    double newValue = 0.0;
    
    for(int i = 0; i < newRows; i++) 
    {
        for(int j = 0; j < newColumns; j++)
        {
            for(int k = 0; k < this->getColumns(); k++)
            {
                newValue += this->entries[i][k] * that.entries[k][j];
            }
            
            product.entries[i][j] = newValue;
            
            newValue = 0.0;
        }    
    }
    
    return product;
}

Matrix& Matrix::operator=(const Matrix& that) 
{
	this->freeEntriesMemory();

	this->rows = that.rows;
	this->columns = that.columns;

	this->entries = new double*[this->rows];
	
	for(int i = 0; i < this->rows; i++)
	{
		this->entries[i] = new double[this->columns];

		for(int j = 0; j < this->columns; j++)
		{
			this->entries[i][j] = that.entries[i][j];	
		}
	}
	
	return *this;
}

Matrix& Matrix::operator=(Matrix&& that)
{
	this->freeEntriesMemory();

	this->entries = that.entries;
	this->rows = that.rows;
	this->columns = that.columns;
	
	that.entries = nullptr;
	
	return *this;
}

Matrix operator*(const Matrix& that, double scalar)
{
	return Matrix(that.rows, that.columns, [&](unsigned int i, unsigned int j) { return that.entries[i][j] * scalar; });
}

bool operator==(const Matrix & a, const Matrix& b)
{
	if(a.rows != b.rows || a.columns != b.columns)
	{
		return false;
	}

	for(int i = 0; i < a.rows; i++)
	{
		for(int j = 0; j < a.columns; j++)
		{
			if(a.entries[i][j] != b.entries[i][j])
			{
				return false;
			}
		}
	}

	return true;
}

std::ostream & operator<<(std::ostream& output, const Matrix& matrix)
{
	for(int i = 0; i < matrix.rows; i++)
	{
		for(int j = 0; j < matrix.columns; j++)
		{
			output << matrix.entries[i][j] << " ";
		}

		output << endl;
	}
	return output;
}

const Matrix& Matrix::operator*=(double scalar)
{
	for(int i = 0; i < this->rows; i++)
	{
		for(int j = 0; j < this->columns; j++)
		{
			this->entries[i][j] *= scalar;
		}
	}
	
	return *this;
}

const Matrix& Matrix::operator+=(const Matrix& that)
{
	if(this->rows != that.rows || this->columns != that.columns)
	{
		throw invalid_argument("Matrix += not defined with incoming matrix");
	}

	for(int i = 0; i < this->rows; i++)
	{
		for(int j = 0; j < this->columns; j++)
		{
			this->entries[i][j] += that.entries[i][j];
		}
	}
	
	return *this;
}

Matrix Matrix::transpose()
{
	return Matrix(this->columns, this->rows, [&](unsigned int i, unsigned int j) { return entries[j][i]; });
}

Matrix Matrix::multiplyEntries(const Matrix& that) const
{
	if(this->rows != that.rows || this->columns != that.columns)
	{
		throw invalid_argument("multiplyEntry - matrices of incompatible size");
	}

	return Matrix(that.rows, that.columns, [&](unsigned int i, unsigned int j) { return entries[i][j] * that.entries[i][j]; });
}

Matrix Matrix::operator-(const Matrix& that) const
{
	return (*this) + (-that);
}

double& Matrix::operator()(unsigned int row, unsigned int column)
{
	if(row >= this->rows || column >= this->columns) 
	{
		throw invalid_argument("Access out of bounds");
	}

	return this->entries[row][column];
}

double Matrix::operator()(unsigned int row, unsigned int column) const
{
	if(row >= this->rows || column >= this->columns) 
	{
		throw invalid_argument("Access out of bounds");
	}

	return this->entries[row][column];
}

unsigned int Matrix::getRows() const 
{	
	return this->rows;
}

unsigned int Matrix::getColumns() const 
{
	return this->columns;
}

Matrix Matrix::operator-() const 
{ 
	return (*this) * -1.0; 
}

const Matrix& Matrix::operator/=(double scalar) 
{
	return (*this) *= 1.0/scalar; 
}

const Matrix& Matrix::operator-=(const Matrix& that)
{
	return (*this) += -that; 
}

Matrix operator*(double scalar, const Matrix& that)
{
	return that * scalar;
}