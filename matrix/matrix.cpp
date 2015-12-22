#include <Arduino.h>
#include "matrix.h"

Matrix::Matrix(int din, int cs, int clk)
{
  _din = din;
  _cs = cs;
  _clk = clk;
  pinMode(_din, OUTPUT);
  pinMode(_cs, OUTPUT);
  pinMode(_clk, OUTPUT);
}

void Matrix::putData(char addr, char data)
{
  putChipData(0, addr, data);
}

void Matrix::putChipData(int chip, char addr, char data)
{
  digitalWrite(_cs, LOW);
  putByte(addr);
  putByte(data);
  for (int i = 0; i < chip; i++)
  {
    putByte(Matrix::reg_noop);
    putByte(0x00);
  }
  digitalWrite(_cs, HIGH);
}

void Matrix::putByte(char b)
{
  // write byte MSB first 8-1
  for (int i = 7; i >= 0; i--)
  {
    digitalWrite(_clk, LOW);
    if ((b >> i) & 0x01)
    {
      digitalWrite(_din, HIGH);
    }
    else
    {
      digitalWrite(_din, LOW);
    }
    digitalWrite(_clk, HIGH);
  }
}

void Matrix::setScanLimit(char b)
{
  putData(Matrix::reg_scanLimit, b);
}
void Matrix::setDecodeMode(char b)
{
  putData(Matrix::reg_decodeMode, b);
}
void Matrix::setShutdownMode(char b)
{
  putData(Matrix::reg_shutdown, b);
}
void Matrix::setTestMode(char b)
{
  putData(Matrix::reg_test, b);
}
void Matrix::setIntensity(char b)
{
  putData(Matrix::reg_intensity, b);
}