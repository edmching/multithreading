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

	std::default_random_engine rnd(
		std::chrono::system_clock::now().time_since_epoch().count());
	std::uniform_real_distribution<double> dist(-1,1);

	for (int i = 0; i < nsampler; ++i)
	{
		p1.x = dist(rnd);
		p1.y = dist(rnd);
		p1.z = dist(rnd);

		if (p1.x*p1.x + p1.y*p1.y + p1.z*p1.z <= 1.0)
			vol += fn.eval(p1.x, p1.y, p1.z);

	}
	
	vol *= vsphere/nsampler;

	return vol;
}
void integrate_hits(Function& fn, std::vector<double>& hits, int idx, int nsamples)
{
	Point p1;

	std::default_random_engine rnd(
		std::chrono::system_clock::now().time_since_epoch().count());
	std::uniform_real_distribution<double> dist(-1, 1);

	for (int i = 0; i < nsamples; ++i)
	{
		p1.x = dist(rnd);
		p1.y = dist(rnd);
		p1.z = dist(rnd);

		if (p1.x*p1.x + p1.y*p1.y + p1.z*p1.z <= 1)
			hits[idx] += fn.eval(p1.x, p1.y, p1.z);

	}
}

double multithreaded_integrate(Function& fn, int nsamples)
{
	//number of available cores
	int nthreads = std::thread::hardware_concurrency();
	
	std::vector<double> hits(nthreads, 0);

	std::vector<std::thread> threads;
	int msamples = 0;
	for (int i = 0; i < nthreads-1; ++i) {
		threads.push_back(
			std::thread(integrate_hits,std::ref(fn),std::ref(hits), i, nsamples/nthreads)
		);
		msamples += nsamples/nthreads;
	}

	threads.push_back(
		std::thread(integrate_hits,std::ref(fn), std::ref(hits), nthreads - 1, nsamples - msamples)
	);

	for (int i = 0; i < nthreads; ++i) {
		threads[i].join();
	}

	double vol = 0;
	double vsphere = 4 / 3 * M_PI;
	for (int i = 0; i < nthreads; ++i) {
		vol += hits[i];
	}

	vol *= vsphere /nsamples;

	return vol;

}

int main() {
  
  double cx, cy, cz, mass;
  int msample = 1000000;
  int sample = 1000000;

  //Simple_Density Objects
  Simple_Density msd;
  XFunction xmsd(msd);
  YFunction ymsd(msd);
  ZFunction zmsd(msd);

  mass = multithreaded_integrate(msd, msample);
  cx = multithreaded_integrate(xmsd, msample) / mass;
  cy = multithreaded_integrate(ymsd, msample) / mass;
  cz = multithreaded_integrate(zmsd, msample) / mass;
  std::cout << "integral of simple density " << mass << std::endl;
  std::cout << "centre of mass of simple density: (" << cx << "," << cy << "," << cz << ")" << std::endl;


  // Density1 Objects
  Density1 d1;
  XFunction xd1(d1);  // x*d1(x,y,z)
  YFunction yd1(d1);  // y*d1(x,y,z)
  ZFunction zd1(d1);  // z*d1(x,y,z)

  mass = multithreaded_integrate(d1, sample);
  cx = multithreaded_integrate(xd1, sample) / mass;
  cy = multithreaded_integrate(yd1, sample) / mass;
  cz = multithreaded_integrate(zd1, sample) / mass;
  std::cout << "integral of density1: " << mass << std::endl;
  std::cout << "centre of mass of density1: (" << cx << "," << cy << "," << cz << ")" << std::endl;

  //Density2 Objects
  Density2 d2;
  XFunction xd2(d2);
  YFunction yd2(d2);
  ZFunction zd2(d2);

  mass = multithreaded_integrate(d2, sample);
  cx = multithreaded_integrate(xd2, sample) / mass;
  cy = multithreaded_integrate(yd2, sample) / mass;
  cz = multithreaded_integrate(zd2, sample) / mass;
  std::cout << "integral of density 2: " << mass << std::endl;
  std::cout << "centre of mass of density 2: (" << cx << "," << cy << "," << cz << ")" << std::endl;

  //Density3 Objects
  Density3 d3;
  XFunction xd3(d3);
  YFunction yd3(d3);
  ZFunction zd3(d3);

  mass = multithreaded_integrate(d3, sample);
  cx = multithreaded_integrate(xd3, sample) / mass;
  cy = multithreaded_integrate(yd3, sample) / mass;
  cz = multithreaded_integrate(zd3, sample) / mass;
  std::cout << "integral of density 3: " << mass << std::endl;
  std::cout << "centre of mass of density 3: (" << cx << "," << cy << "," << cz << ")" << std::endl;


  /*
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
*/

  return 0;
}