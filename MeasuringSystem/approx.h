#if !defined(_BCL_LIB_Approx)
#define      _BCL_LIB_Approx

#include <cmath>

// ���낢��ȋߎ��W���̎Z�o

class simpleApprox
{ // �ߎ��W���Z�o�N���X�̃e���v���[�g
  protected:
    int num;

  public:
    simpleApprox(): num(0) { }
    virtual void add(double x, double y)      // �f�[�^�̒ǉ�
    {
       num++;
    }

    virtual void clear() {num = 0;}           // ���͂��ꂽ�f�[�^���N���A����
    int getN() {return num;}                  // ���͂��ꂽ�f�[�^�̐���Ԃ�

    virtual ~simpleApprox() { }
};

class linearApprox : public simpleApprox
{
  protected:
    double sumXY;
    double sumX;
    double sumY;
    double sumX2;
    double sumY2;

  public:
    linearApprox() { clear(); }
    virtual void add(double x, double y)
    {
      simpleApprox::add(x, y);
      sumXY += x * y;
      sumX  += x;
      sumY  += y;
      sumX2 += x * x;
      sumY2 += y * y;
    }

  void clear() 
  {
    simpleApprox::clear();
    sumXY = 0;
    sumX  = 0;
    sumY  = 0;
    sumX2 = 0;
    sumY2 = 0;
  }

  double getA() { return (num * sumXY - sumX * sumY) / (num * sumX2 - sumX * sumX); }

  double getB() { return (sumX2 * sumY - sumXY * sumX) / (num * sumX2 - sumX * sumX); }

  double getR()
  {
     if (num == 0) return 0;
     double a  = sumXY - (sumX * sumY) / num;
     double b1 = sumX2 - (sumX * sumX) / num;
     double b2 = sumY2 - (sumY * sumY) / num;
     return a / std::sqrt(b1 * b2);
  }

};


#endif

