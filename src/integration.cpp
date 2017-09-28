#define _USE_MATH_DEFINES

#include <iostream>
#include <random>
#include <thread>
#include <cmath>   // for exp, abs

// three-dimensional point
struct Point {
  double x,y,z;
};

// virtual base class for functions
class Function {
 public:
  virtual double operator(double x, double y, double z) = 0;
};

// computes x*fn(x,y,z)
class XFunction : public Function {
  Function& fn;
 public:
  XFunction(Function& fn) : fn(fn){};
  double operator()(double x, double y, double z) {
    return x*fn(x,y,z);
  }
};

// computes y*fn(x,y,z)
class YFunction : public Function {
  Function& fn;
 public:
  YFunction(Function& fn) : fn(fn){};
  double operator()(double x, double y, double z) {
    return y*fn(x,y,z);
  }
};

// computes z*fn(x,y,z)
class ZFunction : public Function {
  Function& fn;
 public:
  ZFunction(Function& fn) : fn(fn){};
  double operator()(double x, double y, double z) {
    return z*fn(x,y,z);
  }
};

double integrate(Function& fn, int nsampler)
{
	double vol;
	double vsphere = 4 / 3 * M_PI;
	int hit = 0;
	Point p1;

	

	std::default_random_engine rnd;
	std::uniform_real_distribution<double> dist(-1,1);

	for (int i = 0; i < nsampler; ++i)
	{
		p1.x = dist(rnd);
		p1.y = dist(rnd);
		p1.z = dist(rnd);

		if (p1.x*p1.x + p1.y*p1.y + p1.z*p1.z <= 1.0)
		{
			hit++;
			vol = vsphere*fn.operator(p1.x,p1.y,p1.z);
		}

	}

}

int main() {

  // YOUR CODE HERE
	

  return 0;
}