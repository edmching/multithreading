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
  virtual double eval(double x, double y, double z) = 0;

};

// computes x*fn(x,y,z)
class XFunction : public Function {
  Function& fn;
 public:
  XFunction(Function& fn) : fn(fn){};
  double eval(double x, double y, double z) {
    return x*fn.eval(x,y,z);
  }
};

// computes y*fn(x,y,z)
class YFunction : public Function {
  Function& fn;
 public:
  YFunction(Function& fn) : fn(fn){};
  double eval(double x, double y, double z) {
    return y*fn.eval(x,y,z);
  }
};

// computes z*fn(x,y,z)
class ZFunction : public Function {
  Function& fn;
 public:
  ZFunction(Function& fn) : fn(fn){};
  double eval(double x, double y, double z) {
    return z*fn.eval(x,y,z);
  }
};

class Simple_Density : public Function {
	public:
		double eval(double x, double y, double z) {
			if (x >= 0.0) {
				return 1;
			}
			return 0;
		}
};
// new function for representing density 1
class Density1 : public Function {
 public:
  double eval(double x, double y, double z) {
    double norm2 = x*x+y*y+z*z;
	/*
    if (norm2 > 1) {
      return 0;
    }
	*/
    return std::exp(-norm2);
  }
};

class Density2 : public Function {
	public:
		double eval(double x, double y, double z) {
			return std::abs(x + y + z);
		}
};

class Density3 : public Function {
	public:
		double eval(double x, double y, double z) {
			return (x - 1.0)*(x-1.0) + (y - 2.0)*(y - 2.0) + (z - 3.0)*(z-3.0);
		}

};

double integrate(Function& fn, int nsampler)
{
	double vol = 0;
	double vsphere = 4 / 3 * M_PI;
	Point p1;

	std::default_random_engine rnd;
	std::uniform_real_distribution<double> dist(-1,1);

	for (int i = 0; i < nsampler; ++i)
	{
		p1.x = dist(rnd);
		p1.y = dist(rnd);
		p1.z = dist(rnd);

		if (p1.x*p1.x + p1.y*p1.y + p1.z*p1.z <= 1.0)
			vol += fn.eval(p1.x, p1.y, p1.z);
		else
			nsampler++;
	}
	
	vol *= vsphere/nsampler;

	return vol;
}

double multi_threaded_integrate(Function& fn, int nsampler)
{

}

int main() {
  
  double cx, cy, cz, mass;
  int sample = 10000;

  //Simple_Density Objects
  Simple_Density sd;
  XFunction xsd(sd);
  YFunction ysd(sd);
  ZFunction zsd(sd);

  mass = integrate(sd, sample);
  cx = integrate(xsd, sample) / mass;
  cy = integrate(ysd, sample) / mass;
  cz = integrate(zsd, sample) / mass;
  std::cout << "integral of simple density " << mass << std::endl;
  std::cout << "centre of mass of simple density: (" << cx << "," << cy << "," << cz << ")" << std::endl;


  // Density1 Objects
  Density1 d1;
  XFunction xd1(d1);  // x*d1(x,y,z)
  YFunction yd1(d1);  // y*d1(x,y,z)
  ZFunction zd1(d1);  // z*d1(x,y,z)

  mass = integrate(d1, sample);
  cx = integrate(xd1, sample) / mass;
  cy = integrate(yd1, sample) / mass;
  cz = integrate(zd1, sample) / mass;
  std::cout << "integral of density1: " << mass << std::endl;
  std::cout << "centre of mass of density1: (" << cx << "," << cy << "," << cz << ")" << std::endl;

  //Density2 Objects
  Density2 d2;
  XFunction xd2(d2);
  YFunction yd2(d2);
  ZFunction zd2(d2);

  mass = integrate(d2, sample);
  cx = integrate(xd2, sample) / mass;
  cy = integrate(yd2, sample) / mass;
  cz = integrate(zd2, sample) / mass;
  std::cout << "integral of density 2: " << mass << std::endl;
  std::cout << "centre of mass of density 2: (" << cx << "," << cy << "," << cz << ")" << std::endl;

  //Density3 Objects
  Density3 d3;
  XFunction xd3(d3);
  YFunction yd3(d3);
  ZFunction zd3(d3);

  mass = integrate(d3, sample);
  cx = integrate(xd3, sample) / mass;
  cy = integrate(yd3, sample) / mass;
  cz = integrate(zd3, sample) / mass;
  std::cout << "integral of density 3: " << mass << std::endl;
  std::cout << "centre of mass of density 3: (" << cx << "," << cy << "," << cz << ")" << std::endl;


  //std::cout << "xd1.eval(0.1,0.2,0.3): " << xd1.eval(0.1,0.2,0.3) << std::endl;
  //std::cout << "yd1.eval(0.1,0.2,0.3): " << yd1.eval(0.1,0.2,0.3) << std::endl;
  //std::cout << "zd1.eval(0.1,0.2,0.3): " << zd1.eval(0.1,0.2,0.3) << std::endl;
 

  // YOUR CODE HERE
	

  return 0;
}